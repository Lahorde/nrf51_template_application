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

class IBleTransceiverListener : public ITransceiverListener{
public:
	virtual ~IBleTransceiverListener(){};
	virtual void onDataReceived(uint8_t arg_u8_dataLength, uint8_t arg_au8_data[]) = 0;
	virtual void onConnection(void) = 0;
	virtual void onDisconnection(void) = 0;
	virtual void onRSSIChange(int8_t arg_s8_rssi) = 0;
};



#endif /* COMMUNICATION_BLE_BLE_TRANSCEIVER_LISTENER_H_ */
