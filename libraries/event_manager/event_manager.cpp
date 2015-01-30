/******************************************************************************
 * @file    event_manager.cpp 
 * @author  Rémi Pincent - INRIA
 * @date    29 janv. 2015   
 *
 * @brief application event_manager wrapped around nRF51 application scheduler.
 * Gives event broadcasting feature to wrapped scheduler.
 * Event queue handled by application scheduler.
 * 
 * Project : event_manager
 * Contact:  Rémi Pincent - remi.pincent@inria.fr
 * 
 * Revision History:
 * TODO_revision history
 *****************************************************************************/

/**************************************************************************
 * Include Files
 **************************************************************************/
#include <memory_watcher.h>
#include <stddef.h>
#include "EventManager.h"
extern "C"{
#include "app_scheduler.h"
}
#include "application_config.h"
#include "delay.h"
#include "app_util.h"
#include "app_timer.h"

/**************************************************************************
 * Manifest Constants
 **************************************************************************/

/**************************************************************************
 * Type Definitions
 **************************************************************************/
typedef struct{
	uint8_t u8_eventCode;
	int32_t s32_eventParam;
}SEvent;

/**************************************************************************
 * Variables
 **************************************************************************/

/**************************************************************************
 * Macros
 **************************************************************************/

#if EVENTMANAGER_DEBUG
#define EVTMGR_DEBUG_PRINT( x )		Serial.print( x );
#define EVTMGR_DEBUG_PRINTLN( x )	Serial.println( x );
#define EVTMGR_DEBUG_PRINT_PTR( x )	Serial.print( reinterpret_cast<unsigned long>( x ), HEX );
#define EVTMGR_DEBUG_PRINTLN_PTR( x )	Serial.println( reinterpret_cast<unsigned long>( x ), HEX );
#else
#define EVTMGR_DEBUG_PRINT( x )
#define EVTMGR_DEBUG_PRINTLN( x )
#define EVTMGR_DEBUG_PRINT_PTR( x )
#define EVTMGR_DEBUG_PRINTLN_PTR( x )
#endif

EventManager EventManager::_my_instance;

EventManager* EventManager::getInstance()
{
	/** do not perform dynamic allocation */
	return &_my_instance;
}


EventManager::EventManager( SafetyMode safety )
{
	APP_SCHED_INIT(SCHED_MAX_EVENT_DATA_SIZE, SCHEDULER_QUEUE_SIZE);
}

bool EventManager::queueEvent( uint8_t eventCode, int eventParam, EventPriority pri )
{
	SEvent loc_s_event = {eventCode, eventParam};
	uint32_t loc_u32_err_code = NRF_SUCCESS;

	/** Event prio not handled */

#if EVENTMANAGER_DEBUG
    if ( !mInterruptSafeMode )
    {
        EVTMGR_DEBUG_PRINT( "queueEvent() enter " )
        EVTMGR_DEBUG_PRINT( eventCode )
        EVTMGR_DEBUG_PRINT( ", " )
        EVTMGR_DEBUG_PRINTLN( eventParam )
    }
#endif

    if(!getInstance()->mListeners.hasActiveListeners(eventCode))
    {
    	/** Do not push an event without any listeners */
    	return false;
    }
    loc_u32_err_code = app_sched_event_put(&loc_s_event, sizeof(loc_s_event), appEventHandler);
	APP_ERROR_CHECK(loc_u32_err_code);

	if(loc_u32_err_code == NRF_SUCCESS)
	{
		return true;
	}
	else
	{
		return false;
	}
}


int EventManager::processEvent()
{
	/** process all events */
	app_sched_execute();
    return 1;
}

/**
 * Not extremely precise
 * TODO handle microsec
 * @param ms
 */
void EventManager::applicationTick(unsigned long ms)
{
	unsigned int  loc_u16_temp_time, loc_u16_temp_sauv = 0;
	unsigned long loc_u32_curr_count = 0;
	static const uint16_t loc_u16_warn_duration = 2000;
	static const uint16_t loc_u16_wait_duration = loc_u16_warn_duration + 8000;

	if(ms == 0)
	{
		EVTMGR_DEBUG_PRINT(F("0 duration given for delay\n"));
		return;
	}

	while (loc_u32_curr_count < ms) {
		loc_u16_temp_sauv = (uint16_t)micros();
		getInstance()->processEvent();

		/** Overflow in case of very long task duration > 2^16 -1 */
		loc_u16_temp_time = (uint16_t)micros() -  loc_u16_temp_sauv;
		if(loc_u16_temp_time > loc_u16_warn_duration)
		{
			EVTMGR_DEBUG_PRINT(F("long processing %uµs\n"));
			EVTMGR_DEBUG_PRINTLN(loc_u16_temp_time);
		}

		MemoryWatcher::checkRAMHistory();
		/** paint stack in case of free / malloc */
		MemoryWatcher::paintStackNow();
		loc_u16_temp_time = (uint16_t)micros() -  loc_u16_temp_sauv;

		/** send tick event */
	    if(getInstance()->mListeners.hasActiveListeners(EVENT_TICK))
	    {
	    	/** some listeners listen on event tick - continue notification */
	    	getInstance()->mListeners.sendEvent( EVENT_TICK, 0 );
	    }

		if(loc_u16_temp_time > loc_u16_wait_duration)
		{
			/** Continue iteration without waiting */
			loc_u32_curr_count += loc_u16_temp_time/1000;
		}
		else
		{
			/** TODO for now delay - to be changed - decisive for low power modes */
			delay((loc_u16_wait_duration - loc_u16_temp_time)/1000);
			loc_u32_curr_count += loc_u16_wait_duration/1000;
		}
	}
}

int EventManager::processAllEvents()
{
	/** process all events */
	app_sched_execute();
    return 1;
}



/********************************************************************/



EventManager::ListenerList::ListenerList() :
mNumListeners( 0 )
{
}


bool EventManager::ListenerList::addListener( uint8_t eventCode, EventListener* const listener )
{
    EVTMGR_DEBUG_PRINT( "addListener() enter " )
    EVTMGR_DEBUG_PRINT( eventCode )
    EVTMGR_DEBUG_PRINT( ", " )
    EVTMGR_DEBUG_PRINTLN_PTR( listener )

    // Argument check
    if ( !listener )
    {
    	EVTMGR_DEBUG_PRINTLN(F("invalid listener given"));
        return false;
    }

    // Check for full dispatch table
    if ( isFull() )
    {
        EVTMGR_DEBUG_PRINTLN( "addListener() list full" )
        return false;
    }

    mListeners[ mNumListeners ].listener = listener;
    mListeners[ mNumListeners ].eventCode = eventCode;
    mListeners[ mNumListeners ].enabled 	= true;
    mNumListeners++;

    EVTMGR_DEBUG_PRINTLN( "addListener() listener added" )

    return true;
}


bool EventManager::ListenerList::removeListener( uint8_t eventCode, EventListener* const listener )
{
    EVTMGR_DEBUG_PRINT( "removeListener() enter " )
    EVTMGR_DEBUG_PRINT( eventCode )
    EVTMGR_DEBUG_PRINT( ", " )
    EVTMGR_DEBUG_PRINTLN_PTR( listener )

    if ( mNumListeners == 0 )
    {
        EVTMGR_DEBUG_PRINTLN( "removeListener() no listeners" )
        return false;
    }

    int k = searchListeners( eventCode, listener );
    if ( k < 0 )
    {
        EVTMGR_DEBUG_PRINTLN( "removeListener() not found" )
        return false;
    }

    for ( int i = k; i < mNumListeners - 1; i++ )
    {
        mListeners[ i ].listener  = mListeners[ i + 1 ].listener;
        mListeners[ i ].eventCode = mListeners[ i + 1 ].eventCode;
        mListeners[ i ].enabled   = mListeners[ i + 1 ].enabled;
        mNumListeners--;
    }

    EVTMGR_DEBUG_PRINTLN( "removeListener() removed" )

    return true;
}


int EventManager::ListenerList::removeListener( EventListener* const listener )
{
    EVTMGR_DEBUG_PRINT( "removeListener() enter " )
    EVTMGR_DEBUG_PRINTLN_PTR( listener )

    if ( mNumListeners == 0 )
    {
        EVTMGR_DEBUG_PRINTLN( "removeListener() no listeners" )
        return 0;
    }

    int removed = 0;
    while ( int k = searchListeners( listener ) >= 0 )
    {
        for ( int i = k; i < mNumListeners - 1; i++ )
        {
            mListeners[ i ].listener  = mListeners[ i + 1 ].listener;
            mListeners[ i ].eventCode = mListeners[ i + 1 ].eventCode;
            mListeners[ i ].enabled   = mListeners[ i + 1 ].enabled;
            mNumListeners--;
            removed++;
        }
    }

    EVTMGR_DEBUG_PRINT( "removeListener() removed " )
    EVTMGR_DEBUG_PRINTLN( removed )

    return removed;
}


bool EventManager::ListenerList::enableListener( uint8_t eventCode, EventListener* const listener, bool enable )
{
    EVTMGR_DEBUG_PRINT( "enableListener() enter " )
    EVTMGR_DEBUG_PRINT( eventCode )
    EVTMGR_DEBUG_PRINT( ", " )
    EVTMGR_DEBUG_PRINT_PTR( listener )
    EVTMGR_DEBUG_PRINT( ", " )
    EVTMGR_DEBUG_PRINTLN( enable )

    if ( mNumListeners == 0 )
    {
        EVTMGR_DEBUG_PRINTLN( "enableListener() no listeners" )
        return false;
    }

    int k = searchListeners( eventCode, listener );
    if ( k < 0 )
    {
        EVTMGR_DEBUG_PRINTLN( "enableListener() not found fail" )
        return false;
    }

    mListeners[ k ].enabled = enable;

    EVTMGR_DEBUG_PRINTLN( "enableListener() success" )
    return true;
}


bool EventManager::ListenerList::isListenerEnabled( uint8_t eventCode, EventListener* const listener )
{
    if ( mNumListeners == 0 )
    {
        return false;
    }

    int k = searchListeners( eventCode, listener );
    if ( k < 0 )
    {
        return false;
    }

    return mListeners[ k ].enabled;
}


int EventManager::ListenerList::sendEvent( uint8_t eventCode, int param )
{
    EVTMGR_DEBUG_PRINT( "sendEvent() enter " )
    EVTMGR_DEBUG_PRINT( eventCode )
    EVTMGR_DEBUG_PRINT( ", " )
    EVTMGR_DEBUG_PRINTLN( param )

    int handlerCount = 0;
    for ( int i = 0; i < mNumListeners; i++ )
    {
        if ( ( mListeners[ i ].listener != 0 ) && ( mListeners[ i ].eventCode == eventCode ) && mListeners[ i ].enabled )
        {
            handlerCount++;
            (*mListeners[ i ].listener).processEvent(eventCode, param);
        }
    }

    EVTMGR_DEBUG_PRINT( "sendEvent() sent to " )
    EVTMGR_DEBUG_PRINTLN( handlerCount )

    if ( !handlerCount )
    {
    	EVTMGR_DEBUG_PRINTLN( "NO handler registered => event not used" )
    }

    return handlerCount;
}

bool EventManager::ListenerList::hasActiveListeners( uint8_t eventCode )
{
    for ( int i = 0; i < mNumListeners; i++ )
    {
        if ( mListeners[i].eventCode == eventCode && mListeners[i].enabled)
        {
            return true;
        }
    }

    return false;
}

int EventManager::ListenerList::searchListeners( uint8_t eventCode, EventListener* const listener )
{
    for ( int i = 0; i < mNumListeners; i++ )
    {
        if ( ( mListeners[i].eventCode == eventCode ) && ( mListeners[i].listener == listener ) )
        {
            return i;
        }
    }

    return -1;
}


int EventManager::ListenerList::searchListeners( EventListener* const listener )
{
    for ( int i = 0; i < mNumListeners; i++ )
    {
        if ( mListeners[i].listener == listener )
        {
            return i;
        }
    }

    return -1;
}


int EventManager::ListenerList::searchEventCode( uint8_t eventCode )
{
    for ( int i = 0; i < mNumListeners; i++ )
    {
        if ( mListeners[i].eventCode == eventCode )
        {
            return i;
        }
    }

    return -1;
}

void EventManager::appEventHandler(void * p_event_data, uint16_t event_size)
{
	SEvent* loc_ps_event = (SEvent*) p_event_data;
	APP_ERROR_CHECK_BOOL(event_size == sizeof(SEvent));
	getInstance()->mListeners.sendEvent( loc_ps_event->u8_eventCode, loc_ps_event->s32_eventParam );
}
/**************************************************************************
 * Local Functions Definitions
 **************************************************************************/

