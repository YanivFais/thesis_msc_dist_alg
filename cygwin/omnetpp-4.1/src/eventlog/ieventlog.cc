//=========================================================================
//  IEVENTLOG.CC - part of
//                  OMNeT++/OMNEST
//           Discrete System Simulation in C++
//
//  Author: Levente Meszaros
//
//=========================================================================

/*--------------------------------------------------------------*
  Copyright (C) 2006-2008 OpenSim Ltd.

  This file is distributed WITHOUT ANY WARRANTY. See the file
  `license' for details on this and other legal matters.
*--------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include "ieventlog.h"

USING_NAMESPACE


IEventLog::IEventLog()
{
    lastNeighbourEventNumber = -1;
    lastNeighbourEvent = NULL;
}

void IEventLog::synchronize(FileReader::FileChangedState change)
{
    if (change != FileReader::UNCHANGED) {
        lastNeighbourEventNumber = -1;
        lastNeighbourEvent = NULL;
    }
}

void IEventLog::printEvents(FILE *file, eventnumber_t fromEventNumber, eventnumber_t toEventNumber, bool outputEventLogMessages)
{
    IEvent *event = fromEventNumber == -1 ? getFirstEvent() : getFirstEventNotBeforeEventNumber(fromEventNumber);

    while (event != NULL && (toEventNumber == -1 || event->getEventNumber() <= toEventNumber))
    {
        event->print(file, outputEventLogMessages);
        event = event->getNextEvent();
    }
}

void IEventLog::print(FILE *file, eventnumber_t fromEventNumber, eventnumber_t toEventNumber, bool outputInitializationEntries, bool outputEventLogMessages)
{
    if (outputInitializationEntries)
        printInitializationLogEntries(file);

    printEvents(file, fromEventNumber, toEventNumber, outputEventLogMessages);
}

IEvent *IEventLog::getNeighbourEvent(IEvent *event, eventnumber_t distance)
{
    Assert(event);
    eventnumber_t neighbourEventNumber = event->getEventNumber() + distance;

    if (lastNeighbourEvent && lastNeighbourEventNumber != -1 && abs64(neighbourEventNumber - lastNeighbourEventNumber) < abs64(distance))
        return getNeighbourEvent(lastNeighbourEvent, neighbourEventNumber - lastNeighbourEventNumber);

    while (event != NULL && distance != 0)
    {
        if (distance > 0) {
            distance--;
            event = event->getNextEvent();
        }
        else {
            distance++;
            event = event->getPreviousEvent();
        }
    }

    lastNeighbourEventNumber = neighbourEventNumber;
    lastNeighbourEvent = (IEvent *)event;

    return lastNeighbourEvent;
}

double IEventLog::getApproximatePercentageForEventNumber(eventnumber_t eventNumber)
{
    IEvent *firstEvent = getFirstEvent();
    IEvent *lastEvent = getLastEvent();
    IEvent *event = getEventForEventNumber(eventNumber);

    if (firstEvent == NULL)
        return 0.0;
    else if (event == NULL)
        return 0.5;
    else if (firstEvent == lastEvent)
        return 1.0;
    else {
        file_offset_t beginOffset = firstEvent->getBeginOffset();
        file_offset_t endOffset = lastEvent->getBeginOffset();

        double percentage = (double)(event->getBeginOffset() - beginOffset) / (endOffset - beginOffset);
        Assert(0.0 <= percentage && percentage <= 1.0);
        return percentage;
    }
}
