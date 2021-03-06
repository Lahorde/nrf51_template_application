/******************************************************************************
 * @file    template_application.cpp
 * @author  Rémi Pincent - INRIA
 * @date    14 janv. 2015   
 *
 * @brief Template application over nrf51.
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
#include "ac_ble_transceiver.h"
#include "logger.h"
#include "nrf51_status.h"
#include "assert.h"
#include "memory_watcher.h"
#include "EventManager.h"
#include "events.h"
#include "button.h"
#include "pinout.h"
#include "flash_memory.h"
#include "persistent_object.h"
#include "AltSoftSerial.h"


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
class LedActivator : public EventListener, public TimerListener{
	void processEvent(uint8_t eventCode, int eventParam);
	void timerElapsed(void);
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
Timer ledTimer(&ledActivator);
static BtListener btListener;
static int8_t rssi = UNKNOWN_RSSI;
static bool ledOn = false;
BLETransceiver bleTransceiver;
/**************************************************************************
 * Macros
 **************************************************************************/

/**************************************************************************
 * Global Functions
 **************************************************************************/

void application_setup(void){
	BLETransceiver::Error loc_error;



	/** Transceiver must be initialized before other application peripherals */
	 bleTransceiver.init(as8_bleName);
	 AltSoftSerial::SoftSerial.begin(9600, 6);
	LOG_INIT_STREAM(LOG_LEVEL, &AltSoftSerial::SoftSerial);

	LOG_INFO_LN("\nStarting application ...");
	LOG_INFO_LN("min remaining stack = %l", MemoryWatcher::getMinRemainingStack());
	LOG_INFO_LN("min remaining heap = %l", MemoryWatcher::getMinRemainingHeap());
	LOG_INFO_LN("remaining stack = %l", MemoryWatcher::getRemainingStack());
	MemoryWatcher::paintStackNow();


	/** Use event manager - instantiate it now */
	 EventManager::getInstance();

	 /** Add led */
	pinMode(LED_1_OUTPUT_PIN, OUTPUT);

	bleTransceiver.registerListener(&btListener);
	loc_error = bleTransceiver.advertise();
	if(loc_error != BLETransceiver::NO_ERROR){
		LOG_ERROR("Error %d when launching advertisement", loc_error);
	}
	else
	{
		ledTimer.periodicNotify(200);
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
	BLETransceiver::Error loc_error = bleTransceiver.send(loc_u8_dataSize, (uint8_t*) &arg_b_on);
	if(loc_error != BLETransceiver::NO_ERROR){
		LOG_INFO_LN("Led status could not be sent, error : %d ", loc_error);
	}
}

void LedActivator::processEvent(uint8_t eventCode, int eventParam){
	BLETransceiver::Error loc_error;

	if(eventCode == TEST_EVENT)
		LOG_INFO_LN("event = %d - value = %l", eventCode, eventParam);
	else if(eventCode == BUTTON_SHORT_PRESS)
	{
		ledOn = !ledOn;
		digitalWrite(LED_1_OUTPUT_PIN, ledOn);
		if(bleTransceiver.isConnected())
		{
			sendLedStatus(ledOn);
		}
	}
	else if(eventCode == BUTTON_MEDIUM_PRESS)
	{
		LOG_INFO_LN("Button medium press");
		if(bleTransceiver.isConnected())
		{
			LOG_INFO_LN("disconnect peripheral");
			loc_error = bleTransceiver.disconnect();
			if(loc_error != BLETransceiver::NO_ERROR){
				LOG_ERROR("Error %d when disconnection peripheral", loc_error);
				return;
			}
		}
		else
		{
			if(bleTransceiver.isAdvertising())
			{
				LOG_INFO_LN("stop advertisement");
				loc_error = bleTransceiver.stopAdvertisement();
				if(loc_error != BLETransceiver::NO_ERROR){
					LOG_ERROR("Error %d when stopping advertisement", loc_error);
					return;
				}
			}
			else
			{
				LOG_INFO_LN("start advertisement");
				loc_error = bleTransceiver.advertise();
				if(loc_error != BLETransceiver::NO_ERROR){
					LOG_ERROR("Error %d when launching advertisement", loc_error);
				}
				else
				{
					ledTimer.periodicNotify(200);
				}
			}
		}
	}
	else
	{
		LOG_INFO_LN("Event %d not handled", eventCode);
	}
}

void LedActivator::timerElapsed(void)
{
	if(bleTransceiver.isAdvertising()){
		/** blink led */
		ledOn = !ledOn;
		digitalWrite(LED_1_OUTPUT_PIN, ledOn);
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
	ledTimer.cancel();
	ledOn = 0;
	digitalWrite(LED_1_OUTPUT_PIN, ledOn);
	bleTransceiver.startRSSIMeasure();
}

void BtListener::onDisconnection(void)
{
	BLETransceiver::Error loc_error;

	LOG_INFO_LN("peripheral disconnected");
	ledOn = 0;
	digitalWrite(LED_1_OUTPUT_PIN, ledOn);
	bleTransceiver.stopRSSIMeasure();
	rssi = UNKNOWN_RSSI;

	loc_error = bleTransceiver.advertise();
	if(loc_error != BLETransceiver::NO_ERROR){
		LOG_ERROR("Error %d when launching advertisement", loc_error);
	}
	else
	{
		ledTimer.periodicNotify(200);
	}
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
