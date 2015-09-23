/******************************************************************************
 * @file    ac_ble_transceiver.h
 * @author  Rémi Pincent - INRIA
 * @date    21 sept. 2015
 *
 * @brief Always Connected BT LE transceiver. When a connection drops. It
 * tries to reconnect it. Rx buffer provided.
 *
 * Project : nrf51_template_application
 * Contact:  Rémi Pincent - remi.pincent@inria.fr
 *
 * Revision History:
 * TODO_revision history
 */

#ifndef COMMUNICATION_AC_BLE_TRANSCEIVER_H_
#define COMMUNICATION_AC_BLE_TRANSCEIVER_H_

#include "ble_transceiver.h"
#include "ring_buffer.h"

class ACBLETransceiver : public BLETransceiver{
private:
	static const uint8_t RX_BUFFER_SIZE;
	RingBuffer<uint8_t> _rxBuffer;

public:
	ACBLETransceiver(void);

	~ACBLETransceiver();

	/**
	 * From BLETransceiver
	 * Connect transceiver
	 */
	ITransceiver::Error connect(void);

	/**
	 * From BLETransceiver
	 * Receive given number of bytes.
	 * @param arg_u16_nb_bytes number of received bytes to read, updated to number of bytes
	 * successfully read
	 * @param arg_au8_bytes_read bytes received
	 * @return number of bytes received
	 */
	Error receive(uint8_t& arg_u8_nb_bytes, uint8_t arg_au8_bytesRead[]);

private:
	/**
	 * From BLETransceiver BLETransceiver
	 * @brief Function for handling the Application's BLE Stack events.
	 *
	 * @param[in]   p_ble_evt   Bluetooth stack event.
	 */
	void onBleEvt(ble_evt_t * p_ble_evt);

	/**
	 * From BLETransceiver BLETransceiver
	 * @brief    Function for handling the data from the Nordic UART Service.
	 *
	 * @details  This function will process the data received from the Nordic UART BLE Service and send
	 *           it to the UART module.
	 */
	/**@snippet [Handling the data received over BLE] */
	void dataReceived(uint8_t * p_data, uint16_t length);
};

#endif /* COMMUNICATION_AC_BLE_TRANSCEIVER_H_ */
