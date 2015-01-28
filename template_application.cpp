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

/**************************************************************************
 * Manifest Constants
 **************************************************************************/
const char* as8_bleName = "template_app";
/**************************************************************************
 * Type Definitions
 **************************************************************************/

/**************************************************************************
 * Variables
 **************************************************************************/
static BLETransceiver                   bleTransceiver(as8_bleName);

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
#include "stdio.h"
void application_setup(void){
	Serial.begin(9600);
	LOG_INIT(LOG_LEVEL);
	LOG_INFO_LN("\nStarting application ...");
	LOG_INFO_LN("min remaining stack = %l", MemoryWatcher::getMinRemainingStack());
	LOG_INFO_LN("min remaining heap = %l", MemoryWatcher::getMinRemainingHeap());
	LOG_INFO_LN("remaining stack = %l", MemoryWatcher::getRemainingStack());
	MemoryWatcher::paintStackNow();

	pinMode(2, INPUT_PULLUP);
	pinMode(3, OUTPUT);

	//appliquer correctif pour RISING
	attachInterrupt(2, it, CHANGE);

	bleTransceiver.init();
	bleTransceiver.advertise();
	extern const uint32_t FREE_MEM_PATTERN;
	LOG_INFO_LN("stack size %x", FREE_MEM_PATTERN);
	LOG_INFO_LN("Setup finished");
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
    byte* test = (byte*) 0x20003fd3;
    Serial.println(*test);


    delay(1000);
}
