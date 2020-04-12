/***************************************************************************
 * file:        groups.h
 *
 * author:      Marc Loebbers & Daniel Willkomm
 *
 * copyright:   (C) 2004 Telecommunication Networks Group (TKN) at
 *              Technische Universitaet Berlin, Germany.
 *
 *              This program is free software; you can redistribute it
 *              and/or modify it under the terms of the GNU General Public
 *              License as published by the Free Software Foundation; either
 *              version 2 of the License, or (at your option) any later
 *              version.
 *              For further information see file COPYING
 *              in the top level directory
 ***************************************************************************
 * part of:     framework implementation developed by tkn
 * description: group definitions for doxygen
 ***************************************************************************
 * changelog:   $Revision: 370 $
 *              last modified:   $Date: 2006-07-06 16:46:04 +0200 (Thu, 06 Jul 2006) $
 *              by:              $Author: willkomm $
 ***************************************************************************/

/**
 * @defgroup applLayer applLayer - application layer modules
 */

/**
 * @defgroup netwLayer netwLayer - network layer modules
 */

/**
 * @defgroup nic nic - network interfaces
 */

/**
 * @defgroup macLayer macLayer - MAC layer modules
 * @ingroup nic
 */

/**
 * @defgroup phyLayer phyLayer - physical layer modules
 * @ingroup nic
 *
 * \image html BasePhyLayer.png "BasePhyLayer class graph"
 *
 * The BasePhyLayer class itself serves more or less only as container
 * and glue between several subclasses. They can be described well by their
 * task during the receiving process of an AirFrame:
 *
 * \image html RecvDetails.png "Receiving process details"
 *
 * On reception of an new AirFrame BasePhyLayer registers it at first with
 * its ChannelInfo member which keeps track of all currently active AirFrames on
 * the channel.
 *
 * Then the signal contained in the AirFrame is handed to every AnalogueModel
 * member of this phy layer. They calculate and add the attenuation effect of
 * the channel on the signal during its transmission.
 * A special AnalogueModel which every phy layer has is the AnalogueModel of the
 * Radio member. It simulates the effect of the state of the radio (SLEEP, RX, TX) on
 * the signal by defining an attenuation of 0% for the time the Radio is
 * receiving and 100% for the other times (which means the signal couldn't be
 * received at all).
 *
 * After that the AirFrame is handed the first time to the Decider which has
 * to decide if the AirFrame can and should be received or not. It also has to decide
 * if the Signal was received correctly or if there where errors. To do this
 * it can tell the phy layer to hand the AirFrame to it at arbitrary other
 * points in time during the transmission.
 * The Decider also has to tell the phy layer when a received packet should
 * be sent up to the upper layer.
 *
 * At the end of the transmission BasePhyLayer tells ChannelInfo that the
 * AirFrame has ended.
 */

/**
 * @defgroup analogueModels analogueModels - AnalogueModel implementations
 * @ingroup phyLayer
 */

/**
 * @defgroup decider decider - decider modules
 * @ingroup phyLayer
 */

/**
 * @defgroup mobility mobility - modules handling the mobility of the hosts
 */

/**
 * @defgroup utils utils  -  utiltity classes
 */


/**
 * @defgroup base base - base MiXiM modules
 *
 * The following graph shows how most of the base modules are working together.
 *
 * \image html MiximBasics.png
 *
 * BaseUtility is responsible for broadcasting information (BBItems) between the different modules
 * which have to implement the ImNotifiable interface.
 *
 * BaseMobility is responsible for position and movement of a host and provides this information
 * as Move for other modules by using BaseUtility.
 *
 * BaseConnectionManager is responsible for keeping track of the hosts which are
 * in range of every other host. To do this it gets the position of the hosts by
 * listening for Move changes at BaseUtility.
 *
 * At last BasePhyLayer uses the ChannelAccess interface provided by BaseConnectionManager
 * to send its packets to every other host in range.
 *
 */

/**
 * @defgroup baseModules baseModules - base module classes of MiXiM
 * @ingroup base
 */

/**
 * @defgroup blackboard BaseUtility - blackboard related stuff
 * @ingroup base
 *
 * Besides keeping the current position of its host it is mainly responsible for providing a black
 * board like interface on which other modules of a host can publish and subscribe information.
 *
 * \image html BaseUtility.png
 *
 * To publish an information on the "black board" a module has to express that information
 * as a subclass of BBitem.
 *
 * To subscribe for information a module has to implement the ImNotifiable interface.
 */

/**
 * @defgroup connectionManager connectionManager - channel and connection related classes
 * @ingroup base
 *
 * BaseConnectionManager keeps track of the hosts (or better NICs) in range of another host
 * (or better NIC). This information is needed by the BasePhyLayer when it sends an AirFrame to
 * the channel.
 * To get this information BasePhyLayer subclasses from ChannelAccess.
 *
 * ChannelAccess provides access for BasePhyLayer to BaseConnectionManager and provides
 * the current position of a NIC for BaseConnectionManager.
 *
 * BaseConnectionManager itself keeps a list of NicEntry objects for every NIC in the simulation.
 * The NicEntry further has a pointer to its NIC as well as a list of pointers to the NicEntries
 * of the NICs which are in range of this NicEntries NIC.
 *
 * The information of NICs in range of each other is updated by BaseConnectionManager every time a host
 * moves.
 *
 * ConnectionManager subclasses from BaseConenctionManager and decides if two NICs are in range of
 * each other by calculating the maximum transmission distance given a certain sensitivity,
 * transmission power and a number of path loss coefficients.
 *
 * \image html ConnectionManager.png
 *
 */

/**
 * @defgroup baseUtils baseUtils - utitities for base MiXiM
 * @ingroup base
 */

/**
 * @defgroup mapping mapping - classes representing mathematical mappings
 *
 * The following class graph shows an overview of the most important classes used by Mappings.
 *
 * \image html MappingDetail.png
 *
 * There a two types of mapping interfaces, ConstMapping and Mapping. ConstMapping can represent any
 * mathematical mapping but the the course of the mapping can't be set or changed arbitrary.
 * This interface is meant to be used for implementations which define the course of the
 * mapping by a concrete formula.
 * The Mapping interface extends the ConstMapping interface by a setValue() method
 * which can set and change the course of the represented mapping arbitrary. This interface is meant
 * to be used for generic mapping implementations which uses methods like interpolation to be able
 * to represent any arbitrary course of a mapping.
 *
 * Every ConstMapping (and therefore every Mapping) has a DimensionSet member which defines the domain
 * of the represented mapping. The DimensionSet class itself is an actual set of Dimension objects.
 * Each Dimension object represents a certain dimension like time or frequency.
 *
 * Further ConstMapping and Mapping define their own Iterator interfaces ConstMappingIterator and
 * MappingIterator which define the interface for iterators over a mapping implementation.
 *
 * The last important class is the Argument. It defines a position in a mappings domain by
 * mapping a number of Dimensions to the value inside that Dimension.
 */

/**
 * @defgroup mappingDetails mappingDetails - mostly internal mapping classes
 * @ingroup mapping
 */

/**
 * @defgroup examples examples - Classes for the examples of MiXiM
 */

/**
 * @defgroup exampleAM AnalogueModels example - Classes from the AnalogueModels example
 * @ingroup examples
 */

/**
 * @defgroup exampleCSMA CSMAMac example - Classes from the CSMAMac example
 * @ingroup examples
 */

/**
 * @defgroup power Power consumption - Classes using/defining power consumption
 */
