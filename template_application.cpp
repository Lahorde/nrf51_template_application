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
#include "pinout.h"

/**************************************************************************
 * Manifest Constants
 **************************************************************************/
const char* as8_bleName = "template_app";
static const unsigned int LOOP_PERIOD_MS = 200;
static const int8_t UNKNOWN_RSSI = 128;

/**************************************************************************
 * Local Functions
 **************************************************************************/
void sendLedStatus(bool arg_b_on);

/**************************************************************************
 * Type Definitions
 **************************************************************************/
class LedActivator : public EventListener{
	void processEvent(uint8_t eventCode, int eventParam);
};

class BtListener : public IBleTransceiverListener{
	void onDataReceived(uint8_t arg_u8_dataLength, uint8_t arg_au8_data[]);
	void onConnection(void);
	void onDisconnection(void);
	void onRSSIChange(int8_t arg_s8_rssi);
};
/**************************************************************************
 * Variables
 **************************************************************************/
static Button button1(BUTTON_1_INPUT_PIN, 1, true);
static LedActivator ledActivator;
static BtListener btListener;
static int8_t rssi = UNKNOWN_RSSI;
/**************************************************************************
 * Macros
 **************************************************************************/

/**************************************************************************
 * Global Functions
 **************************************************************************/

void application_setup(void){
	BLETransceiver::Error loc_error;

	Serial.begin(9600);
	LOG_INIT(LOG_LEVEL);
	LOG_INFO_LN("\nStarting application ...");
	LOG_INFO_LN("min remaining stack = %l", MemoryWatcher::getMinRemainingStack());
	LOG_INFO_LN("min remaining heap = %l", MemoryWatcher::getMinRemainingHeap());
	LOG_INFO_LN("remaining stack = %l", MemoryWatcher::getRemainingStack());
	MemoryWatcher::paintStackNow();

	/** Use event manager - instantiate it now */
	 EventManager::getInstance();

	 /** Add led */
	pinMode(LED_1_OUTPUT_PIN, OUTPUT);

	BLETransceiver::getInstance()->addListener(&btListener);
	BLETransceiver::getInstance()->init(as8_bleName);
	loc_error = BLETransceiver::getInstance()->advertise();
	if(loc_error != BLETransceiver::NO_ERROR){
		LOG_ERROR("Error %d when launching advertisement", loc_error);
	}
	LOG_INFO_LN("Setup finished");

	EventManager::getInstance()->addListener(TEST_EVENT, &ledActivator);
	EventManager::getInstance()->enableListener(TEST_EVENT, &ledActivator, true);

	EventManager::getInstance()->addListener(BUTTON_SHORT_PRESS, &ledActivator);
	EventManager::getInstance()->enableListener(BUTTON_SHORT_PRESS, &ledActivator, true);

	EventManager::getInstance()->addListener(BUTTON_MEDIUM_PRESS, &ledActivator);
	EventManager::getInstance()->enableListener(BUTTON_MEDIUM_PRESS, &ledActivator, true);
}

void sendLedStatus(bool arg_b_on)
{
	uint8_t loc_u8_dataSize = sizeof(bool);
	BLETransceiver::Error loc_error = BLETransceiver::getInstance()->send(loc_u8_dataSize, (uint8_t*) &arg_b_on);
	if(loc_error != BLETransceiver::NO_ERROR){
		LOG_INFO_LN("Led status could not be sent, error : %d ", loc_error);
	}
}

void LedActivator::processEvent(uint8_t eventCode, int eventParam){
	static bool ledOn = false;
	BLETransceiver::Error loc_error;

	if(eventCode == TEST_EVENT)
		LOG_INFO_LN("event = %d - value = %l", eventCode, eventParam);
	else if(eventCode == BUTTON_SHORT_PRESS)
	{
		ledOn = !ledOn;
		digitalWrite(LED_1_OUTPUT_PIN, ledOn);
		if(BLETransceiver::getInstance()->isConnected())
		{
			sendLedStatus(ledOn);
		}
	}
	else if(eventCode == BUTTON_MEDIUM_PRESS)
	{
		LOG_INFO_LN("Button medium press");
		if(BLETransceiver::getInstance()->isConnected())
		{
			LOG_INFO_LN("disconnect peripheral");
			loc_error = BLETransceiver::getInstance()->disconnect();
			if(loc_error != BLETransceiver::NO_ERROR){
				LOG_ERROR("Error %d when disconnection peripheral", loc_error);
				return;
			}
		}
		else
		{
			if(BLETransceiver::getInstance()->isAdvertising())
			{
				LOG_INFO_LN("stop advertisement");
				loc_error = BLETransceiver::getInstance()->stopAdvertisement();
				if(loc_error != BLETransceiver::NO_ERROR){
					LOG_ERROR("Error %d when stopping advertisement", loc_error);
					return;
				}
			}
			else
			{
				LOG_INFO_LN("start advertisement");
				loc_error = BLETransceiver::getInstance()->advertise();
				if(loc_error != BLETransceiver::NO_ERROR){
					LOG_ERROR("Error %d when launching advertisement", loc_error);
				}
			}
		}
	}
	else
	{
		LOG_INFO_LN("Event %d not handled", eventCode);
	}
}

void BtListener::onDataReceived(uint8_t arg_u8_dataLength, uint8_t arg_au8_data[])
{
	LOG_INFO_LN("data received : ");
	for(uint8_t loc_u8_Index = 0; loc_u8_Index < arg_u8_dataLength; loc_u8_Index++)
	{
		LOG_INFO_LN("%x", arg_au8_data[loc_u8_Index]);
	}
}

void BtListener::onConnection(void)
{
	LOG_INFO_LN("peripheral connected");
	BLETransceiver::getInstance()->startRSSIMeasure();
}

void BtListener::onDisconnection(void)
{
	LOG_INFO_LN("peripheral disconnected");
	BLETransceiver::getInstance()->stopRSSIMeasure();
	rssi = UNKNOWN_RSSI;
}

void BtListener::onRSSIChange(int8_t arg_s8_rssi)
{
	rssi = arg_s8_rssi;
}

/**
 * Called in application context
 */
void application_loop(void){
	static uint32_t loc_u32_counter = 0;
	loc_u32_counter++;

	if(loc_u32_counter*LOOP_PERIOD_MS > 1000){
		LOG_INFO_LN("Current silicium temp = %d C", nrf51_status_getSiliconTemp());
		if(rssi != UNKNOWN_RSSI)
			LOG_INFO_LN("RSSI = %d dbm", rssi);
		loc_u32_counter = 0;
	}
    MemoryWatcher::checkRAMHistory();
    MemoryWatcher::paintStackNow();
    EventManager::applicationTick(LOOP_PERIOD_MS);
}
