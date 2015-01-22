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
void application_setup(void){
	Serial.begin(9600);
	LOG_INIT(LOG_LEVEL);

	LOG_INFO_LN("Starting application ...");

	pinMode(4, INPUT);

	bleTransceiver.init();
	bleTransceiver.advertise();
}

/**
 * Called in application context
 */
void application_loop(void){
    LOG_INFO_LN("Current silicium temp = %d C", nrf51_status_getSiliconTemp());
    delay(500);
}
