/******************************************************************************
 * @file    template_application.cpp
 * @author  Rémi Pincent - INRIA
 * @date    14 janv. 2015   
 *
 * @brief TODO_one_line_description_of_file
 * 
 * Project : wimu
 * Contact:  Rémi Pincent - remi.pincent@inria.fr
 * 
 * Revision History:
 * TODO_revision history
 *****************************************************************************/

/**************************************************************************
 * Include Files
 **************************************************************************/
#include "Arduino.h"
#include "ble_transceiver.h"
#include "logger.h"
#include "nrf51_status.h"
#include "assert.h"
#include "memory_watcher.h"
#include "EventManager.h"
#include "events.h"
#include "button.h"

/**************************************************************************
 * Manifest Constants
 **************************************************************************/
const char* as8_bleName = "template_app";

/**************************************************************************
 * Type Definitions
 **************************************************************************/
class Test : public EventListener{
	void processEvent(uint8_t eventCode, int eventParam){
		if(eventCode == TEST_EVENT)
			LOG_INFO_LN("event = %d - value = %l", eventCode, eventParam);
		else
			LOG_INFO_LN("event = %d" , eventCode);
	}
};

class TimerTest : public TimerListener{
	void timerElapsed(void)
	{
		LOG_INFO_LN("timer elapsed = %l" , millis());
		//EventManager::getInstance()->queueEvent(TEST_EVENT, millis());
	}
};
/**************************************************************************
 * Variables
 **************************************************************************/
//static BLETransceiver                   bleTransceiver(as8_bleName);
static Button button1(2, 1, true);
static TimerTest timerTest;
static Timer timer(&timerTest);
static Test testEvent;
static const unsigned int LOOP_PERIOD_MS = 200;
/**************************************************************************
 * Macros
 **************************************************************************/

/**************************************************************************
 * Local Functions
 **************************************************************************/

/**************************************************************************
 * Global Functions
 **************************************************************************/

volatile int value = 0;

void it(void){
	value = (value+50)%250;
	analogWrite(3, value);
}

void application_setup(void){
	Serial.begin(9600);
	LOG_INIT(LOG_LEVEL);
	LOG_INFO_LN("\nStarting application ...");
	LOG_INFO_LN("min remaining stack = %l", MemoryWatcher::getMinRemainingStack());
	LOG_INFO_LN("min remaining heap = %l", MemoryWatcher::getMinRemainingHeap());
	LOG_INFO_LN("remaining stack = %l", MemoryWatcher::getRemainingStack());
	MemoryWatcher::paintStackNow();

	/** Use event manager - instantiate it now */
	 EventManager::getInstance();

	//timer.periodicNotify(50);
	pinMode(3, OUTPUT);

	//appliquer correctif pour RISING
	//attachInterrupt(2, it, CHANGE);

	//bleTransceiver.init();
	//bleTransceiver.advertise();
	extern const uint32_t FREE_MEM_PATTERN;
	LOG_INFO_LN("stack size %x", FREE_MEM_PATTERN);
	LOG_INFO_LN("Setup finished");

	EventManager::getInstance()->addListener(TEST_EVENT, &testEvent);
	EventManager::getInstance()->enableListener(TEST_EVENT, &testEvent, true);

	EventManager::getInstance()->addListener(BUTTON_PRESSED, &testEvent);
	EventManager::getInstance()->enableListener(BUTTON_PRESSED, &testEvent, true);

	EventManager::getInstance()->addListener(BUTTON_SHORT_PRESS, &testEvent);
	EventManager::getInstance()->enableListener(BUTTON_SHORT_PRESS, &testEvent, true);

	EventManager::getInstance()->addListener(BUTTON_RELEASED, &testEvent);
	EventManager::getInstance()->enableListener(BUTTON_RELEASED, &testEvent, true);
}



/**
 * Called in application context
 */
void application_loop(void){
	LOG_INFO_LN("min remaining heap = %l", MemoryWatcher::getMinRemainingHeap());
	LOG_INFO_LN(" remaining heap = %l", MemoryWatcher::getRemainingHeap());
	LOG_INFO_LN(" remaining ram = %l", MemoryWatcher::getRemainingRAM());
    LOG_INFO_LN("Current silicium temp = %d C", nrf51_status_getSiliconTemp());
    LOG_INFO_LN("min remaining stack = %l", MemoryWatcher::getMinRemainingStack());
    MemoryWatcher::paintStackNow();

    EventManager::applicationTick(LOOP_PERIOD_MS);
}
