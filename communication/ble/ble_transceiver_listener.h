/******************************************************************************
 * @file    ble_transceiver_listener.h 
 * @author  Rémi Pincent - INRIA
 * @date    15 janv. 2015   
 *
 * @brief TODO_one_line_description_of_file
 * 
 * Project : nrf51_template_application
 * Contact:  Rémi Pincent - remi.pincent@inria.fr
 * 
 * Revision History:
 * TODO_revision history
 *****************************************************************************/
#ifndef COMMUNICATION_BLE_BLE_TRANSCEIVER_LISTENER_H_
#define COMMUNICATION_BLE_BLE_TRANSCEIVER_LISTENER_H_

class IBleTransceiverListener{
	virtual ~IBleTransceiverListener() = 0;
	virtual void onConnection(void) = 0;
	virtual void onDisconnection(void) = 0;
	virtual void onRSSIChange(void) = 0;
};



#endif /* COMMUNICATION_BLE_BLE_TRANSCEIVER_LISTENER_H_ */
