//=========================================================================
//  CPARSIMSEGMENT.CC - part of
//
//                  OMNeT++/OMNEST
//           Discrete System Simulation in C++
//
//  Author: Andras Varga, 2003
//          Dept. of Electrical and Computer Systems Engineering,
//          Monash University, Melbourne, Australia
//
//=========================================================================

/*--------------------------------------------------------------*
  Copyright (C) 2003-2008 Andras Varga
  Copyright (C) 2006-2008 OpenSim Ltd.

  This file is distributed WITHOUT ANY WARRANTY. See the file
  `license' for details on this and other legal matters.
*--------------------------------------------------------------*/

#include <stdlib.h>
#include <stdio.h>
#include "cmessage.h"
#include "errmsg.h"
#include "cplaceholdermod.h"
#include "cproxygate.h"
#include "cparsimpartition.h"
#include "ccommbuffer.h"
#include "cparsimcomm.h"
#include "cparsimsynchr.h"
#include "creceivedexception.h"
#include "messagetags.h"
#include "cenvir.h"
#include "cconfiguration.h"
#include "globals.h"
#include "cconfigoption.h"
#include "regmacros.h"

USING_NAMESPACE

Register_Class(cParsimPartition);

Register_GlobalConfigOption(CFGID_PARSIM_DEBUG, "parsim-debug", CFG_BOOL, "true", "With parallel-simulation=true: turns on printing of log messages from the parallel simulation code.");

cParsimPartition::cParsimPartition()
{
    sim = NULL;
    comm = NULL;
    synch = NULL;
    debug = ev.getConfig()->getAsBool(CFGID_PARSIM_DEBUG);
}

cParsimPartition::~cParsimPartition()
{
}

void cParsimPartition::setContext(cSimulation *simul, cParsimCommunications *commlayer, cParsimSynchronizer *sync)
{
    sim = simul;
    comm = commlayer;
    synch = sync;
}

void cParsimPartition::startRun()
{
    connectRemoteGates();
}

void cParsimPartition::endRun()
{
}

void cParsimPartition::shutdown()
{
    if (!comm) return; // if initialization failed

    cException e("Process has shut down");
    broadcastException(e);

    comm->shutdown();
}


void cParsimPartition::connectRemoteGates()
{
    cCommBuffer *buffer = comm->createCommBuffer();

    //
    // Step 1: broadcast what input gates we have that have to be connected
    //
    ev << "connecting remote gates: step 1 - broadcasting input gates...\n";
    for (int modId=0; modId<=sim->getLastModuleId(); modId++)
    {
        cPlaceholderModule *mod = dynamic_cast<cPlaceholderModule *>(sim->getModule(modId));
        if (mod)
        {
            for (cModule::GateIterator i(mod); !i.end(); i++)
            {
                cGate *g = i();
                // if this is a normal output gate which leads to a simple module,
                // send the input gate where it is connected.
                if (g->getType()==cGate::OUTPUT && g->getNextGate() &&
                    g->getPathEndGate()->getOwnerModule()->isSimple())
                {
                    cGate *ing = g->getNextGate();
                    // pack gate "address" here
                    buffer->pack(ing->getOwnerModule()->getId());
                    buffer->pack(ing->getId());
                    // pack gate name
                    buffer->pack(ing->getOwnerModule()->getFullPath().c_str());
                    buffer->pack(ing->getName());
                    buffer->pack(ing->getIndex());
                }
            }
        }
    }
    buffer->pack(-1); // "the end"
    comm->broadcast(buffer, TAG_SETUP_LINKS);

    //
    // Step 2: collect info broadcast by others, and use it to fill in output cProxyGates
    //
    ev << "connecting remote gates: step 2 - collecting broadcasts sent by others...\n";
    for (int i=0; i<comm->getNumPartitions()-1; i++)
    {
        // receive:
        int tag, remoteProcId;
        // note: *must* filter for TAG_SETUP_LINKS here, to prevent race conditions
        if (!comm->receiveBlocking(TAG_SETUP_LINKS, buffer, tag, remoteProcId))
            throw cRuntimeError("connectRemoteGates() interrupted by user");
        ASSERT(tag==TAG_SETUP_LINKS);

        ev << "  processing msg from procId=" << remoteProcId << "...\n";

        // process what we got:
        while(true)
        {
            int remoteModId;
            int remoteGateId;
            char *moduleFullPath;
            char *gateName;
            int gateIndex;

            // unpack a gate -- modId==-1 indicates end of packet
            buffer->unpack(remoteModId);
            if (remoteModId==-1)
                break;
            buffer->unpack(remoteGateId);
            buffer->unpack(moduleFullPath);
            buffer->unpack(gateName);
            buffer->unpack(gateIndex);

            // find corresponding output gate (if we have it) and set remote
            // gate address to the received one
            cModule *m = sim->getModuleByPath(moduleFullPath);
            cGate *g = m ? m->gate(gateName,gateIndex) : NULL;
            cProxyGate *pg = dynamic_cast<cProxyGate *>(g);

            ev << "    gate: " << moduleFullPath << "." << gateName << "["  << gateIndex << "] - ";
            if (!pg)
                ev << "not here\n";
            else
                ev << "points to (procId=" << remoteProcId << " moduleId=" << remoteModId << " gateId=" << remoteGateId << ")\n";

            if (pg)
            {
                pg->setPartition(this);
                pg->setRemoteGate(remoteProcId,remoteModId,remoteGateId);
            }

            delete [] moduleFullPath;
            delete [] gateName;
        }
        buffer->assertBufferEmpty();
    }
    ev << "  done.\n";
    comm->recycleCommBuffer(buffer);
}

void cParsimPartition::processOutgoingMessage(cMessage *msg, int procId, int moduleId, int gateId, void *data)
{
    if (debug) ev << "sending message '" << msg->getFullName() << "' (for T=" <<
                 msg->getArrivalTime() << " to procId=" << procId << "\n";

    synch->processOutgoingMessage(msg, procId, moduleId, gateId, data);
}

void cParsimPartition::processReceivedBuffer(cCommBuffer *buffer, int tag, int sourceProcId)
{
    opp_string errmsg;
    switch (tag)
    {
        case TAG_TERMINATIONEXCEPTION:
            buffer->unpack(errmsg);
            throw cReceivedTerminationException(sourceProcId, errmsg.c_str());
        case TAG_EXCEPTION:
            buffer->unpack(errmsg);
            throw cReceivedException(sourceProcId, errmsg.c_str());
        default:
            throw cRuntimeError("cParsimPartition::processReceivedBuffer(): unexpected tag %d "
                                 "from procId %d", tag, sourceProcId);
    }
    buffer->assertBufferEmpty();
}

void cParsimPartition::processReceivedMessage(cMessage *msg, int destModuleId, int destGateId, int sourceProcId)
{
    msg->setSrcProcId(sourceProcId);
    cModule *mod = sim->getModule(destModuleId);
    if (!mod)
        throw cRuntimeError("parallel simulation error: destination module id=%d for message \"%s\""
                             "from partition %d does not exist (any longer)",
                             destModuleId, msg->getName(), sourceProcId);
    cGate *g = mod->gate(destGateId);
    if (!g)
        throw cRuntimeError("parallel simulation error: destination gate %d of module id=%d "
                             "for message \"%s\" from partition %d does not exist",
                             destGateId, destModuleId, msg->getName(), sourceProcId);

    // do our best to set the source gate (the gate of a cPlaceholderModule)
    cGate *srcg = g->getPathStartGate();
    msg->setSentFrom(srcg->getOwnerModule(), srcg->getId(), msg->getSendingTime());

    // deliver it to the "destination" gate of the connection -- the channel
    // has already been simulated in the originating partition.
    g->deliver(msg, msg->getArrivalTime());
    ev.messageSent_OBSOLETE(msg); //FIXME change to the newer callback methods! messageSendHop() etc
}

void cParsimPartition::broadcastTerminationException(cTerminationException& e)
{
    // send TAG_TERMINATIONEXCEPTION to all partitions
    cCommBuffer *buffer = comm->createCommBuffer();
    buffer->pack(e.what());
    try
    {
        comm->broadcast(buffer, TAG_TERMINATIONEXCEPTION);
    }
    catch (std::exception&)
    {
        // swallow exceptions -- here we're not interested in them
    }
    comm->recycleCommBuffer(buffer);
}

void cParsimPartition::broadcastException(std::exception& e)
{
    // send TAG_EXCEPTION to all partitions
    cCommBuffer *buffer = comm->createCommBuffer();
    buffer->pack(e.what());
    try
    {
        comm->broadcast(buffer, TAG_EXCEPTION);
    }
    catch (std::exception&)
    {
        // swallow any exceptions -- here we're not interested in them
    }
    comm->recycleCommBuffer(buffer);
}


