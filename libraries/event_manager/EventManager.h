/*
 * EventManager.h
 *
 *
 * An event handling system
 *
 * Author: igormt@alumni.caltech.edu
 * Copyright (c) 2013 Igor Mikolic-Torreira
 * Author: igormt@alumni.caltech.edu
 * Copyright (c) 2013 Igor Mikolic-Torreira
 * 
 * Inspired by and adapted from the
 * Arduino Event System library by
 * Author: mromani@ottotecnica.com
 * Copyright (c) 2010 OTTOTECNICA Italy
 *
 * This library is free software; you can redistribute it
 * and/or modify it under the terms of the GNU Lesser
 * General Public License as published by the Free Software
 * Foundation; either version 2.1 of the License, or (at
 * your option) any later version.
 *
 * This library is distributed in the hope that it will
 * be useful, but WITHOUT ANY WARRANTY; without even the
 * implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE.  See the GNU Lesser General Public
 * License for more details.
 *
 * You should have received a copy of the GNU Lesser
 * General Public License along with this library; if not,
 * write to the Free Software Foundation, Inc.,
 * 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
 *
 */


#ifndef EventManager_h
#define EventManager_h

#include "EventListener.h"
#include <stdint.h>

// Size of the listener list.  Adjust as appropriate for your application.
// Requires a total of sizeof(*f())+sizeof(int)+sizeof(bool) uint8_ts of RAM for each unit of size
#ifndef EVENTMANAGER_LISTENER_LIST_SIZE
#define EVENTMANAGER_LISTENER_LIST_SIZE		8
#endif

class EventManager
{

public:

    // EventManager can be instantiated in either as interrupt safe or
    // non-interrupt safe configuration.  The default is interrupt safe, but
    // these constants can be used to explicitly set the configuration.
    // If you queue events from an interrupt handler, you must instantiate
    // the EventManager in interrupt safe mode.
    enum SafetyMode { kNotInterruptSafe, kInterruptSafe };

    // EventManager recognizes two kinds of events.  By default, events are
    // are queued as low priority, but these constants can be used to explicitly
    // set the priority when queueing events
    //
    // NOTE high priority events are always handled before any low priority events.
    enum EventPriority { kHighPriority, kLowPriority };

    /**
     * Event manager has a special event called EVENT_TICK
     * Listeners registered to this event will be notified each
     * time an event manager tick occurs.
     * This event must have been defined in your applications
     * events.h
     */
    static const uint8_t EVENT_TICK = -1;

    //Singleton design pattern
    // Return EventManagerInstance, it operates in interrupt safe mode
    static EventManager* getInstance();

    // Create an event manager
    // By default, it operates in interrupt safe mode, allowing you to queue events from interrupt handlers
    EventManager( SafetyMode safety = kInterruptSafe );

    // Add a listener
    // Returns true if the listener is successfully installed, false otherwise (e.g. the dispatch table is full)
    bool addListener( uint8_t eventCode, EventListener*  const listener );

    // Remove (event, listener) pair (all occurrences)
    // Other listeners with the same function or event code will not be affected
    bool removeListener( uint8_t eventCode, EventListener* const listener );

    // Remove all occurrences of a listener
    // Removes this listener regardless of the event code; returns number removed
    // Useful when one listener handles many different events
    int removeListener( EventListener* listener );

    // Enable or disable a listener
    // Return true if the listener was successfully enabled or disabled, false if the listener was not found
    bool enableListener( uint8_t eventCode, EventListener* const listener, bool enable );

    // Returns the current enabled/disabled state of the (eventCode, listener) combo
    bool isListenerEnabled( uint8_t eventCode, EventListener* const listener );

    // The default listener is a callback function that is called when an event with no listener is processed
    // These functions set, clear, and enable/disable the default listener
    bool setDefaultListener( EventListener* const listener );
    void removeDefaultListener();
    void enableDefaultListener( bool enable );

    // Is the ListenerList empty?
    bool isListenerListEmpty();

    // Is the ListenerList full?
    bool isListenerListFull();

    // Returns true if no events are in the queue
    bool isEventQueueEmpty( EventPriority pri = kLowPriority );

    // Returns true if no more events can be inserted into the queue
    bool isEventQueueFull( EventPriority pri = kLowPriority );

    // Actual number of events in queue
    int getNumEventsInQueue( EventPriority pri = kLowPriority );

    // tries to insert an event into the queue;
    // returns true if successful, false if the
    // queue if full and the event cannot be inserted
    bool queueEvent( uint8_t eventCode, int eventParam, EventPriority pri = kLowPriority );

    // this must be called regularly (usually by calling it inside the loop() function)
    int processEvent();


    static void appEventHandler(void * p_event_data, uint16_t event_size);

    // this function can be called to process ALL events in the queue
    // WARNING:  if interrupts are adding events as fast as they are being processed
    // this function might never return.  YOU HAVE BEEN WARNED.
    int processAllEvents();

    /**
     * Application tick - must be called to schedule application
     * after given duration
     * @param ms
     * @return
     */
    static void applicationTick(unsigned long ms);

private:

    // ListenerList class used internally by EventManager
    class ListenerList
    {

    public:

        // Create an event manager
        ListenerList();

        // Add a listener
        // Returns true if the listener is successfully installed, false otherwise (e.g. the dispatch table is full)
        bool addListener( uint8_t eventCode, EventListener* const listener );

        // Remove event listener pair (all occurrences)
        // Other listeners with the same function or eventCode will not be affected
        bool removeListener( uint8_t eventCode, EventListener* const listener );

        // Remove all occurrances of a listener
        // Removes this listener regardless of the eventCode; returns number removed
        int removeListener( EventListener* const listener );

        // Enable or disable a listener
        // Return true if the listener was successfully enabled or disabled, false if the listener was not found
        bool enableListener( uint8_t eventCode, EventListener* const listener, bool enable );

        bool isListenerEnabled( uint8_t eventCode, EventListener* const listener );

        // Is the ListenerList empty?
        bool isEmpty();

        // Is the ListenerList full?
        bool isFull();

        // Send an event to the listeners; returns number of listeners that handled the event
        int sendEvent( uint8_t eventCode, int param );

        //checks wether given event has active listener
        bool hasActiveListeners( uint8_t eventCode );

    private:
        // Maximum number of event/listener entries
        // Can be changed to save memory or allow more events to be dispatched
        static const int kMaxListeners = EVENTMANAGER_LISTENER_LIST_SIZE;

        // Actual number of event listeners
        int mNumListeners;

        // Listener structure and corresponding array
        struct ListenerItem
        {
            EventListener* listener;		// The listener
            uint8_t		   eventCode;		// The event code
            bool		   enabled;			// Each listener can be enabled or disabled
        };
        ListenerItem mListeners[ kMaxListeners ];

        // get the current number of entries in the dispatch table
        int getNumEntries();

        // returns the array index of the specified listener or -1 if no such event/function couple is found
        int searchListeners( uint8_t eventCode, EventListener* const listener);
        int searchListeners( EventListener* const listener );
        int searchEventCode( uint8_t eventCode );

        };
        //Singleton design pattern
        static EventManager _my_instance;
        ListenerList		mListeners;
};

//*********  INLINES   EventManager::  ***********

inline bool EventManager::addListener( uint8_t eventCode, EventListener* const listener )
{
    return mListeners.addListener( eventCode, listener );
}

inline bool EventManager::removeListener( uint8_t eventCode, EventListener* const listener )
{
    return mListeners.removeListener( eventCode, listener );
}

inline int EventManager::removeListener( EventListener* const listener )
{
    return mListeners.removeListener( listener );
}

inline bool EventManager::enableListener( uint8_t eventCode, EventListener* const listener, bool enable )
{
    return mListeners.enableListener( eventCode, listener, enable );
}

inline bool EventManager::isListenerEnabled( uint8_t eventCode, EventListener* const listener )
{
    return mListeners.isListenerEnabled( eventCode, listener );
}

inline bool EventManager::isListenerListEmpty()
{
    return mListeners.isEmpty();
}

inline bool EventManager::isListenerListFull()
{
    return mListeners.isFull();
}

//*********  INLINES   EventManager::ListenerList::  ***********

inline bool EventManager::ListenerList::isEmpty()
{
    return (mNumListeners == 0);
}

inline bool EventManager::ListenerList::isFull()
{
    return (mNumListeners == kMaxListeners);
}

inline int EventManager::ListenerList::getNumEntries()
{
    return mNumListeners;
}


#endif
