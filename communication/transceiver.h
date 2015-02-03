/******************************************************************************
 * @file    transceiver.h 
 * @author  Rémi Pincent - INRIA
 * @date    26 févr. 2014   
 *
 * @brief transceiver description. Sends and receives data.
 * 
 * Project : transceiver
 * Contact:  Rémi Pincent - remi.pincent@inria.fr
 * 
 * Revision History:
 * TODO_revision history
 *****************************************************************************/

#ifndef TRANSCEIVER_H_
#define TRANSCEIVER_H_

#include <stdint.h>
#include <transceiver_listener.h>

/**
 * @class ITransceiver
 * @brief Transceiver interface.
 * 
 */
class ITransceiver {
/** Error Codes */
public:
	typedef int8_t Error;
	static const int8_t NO_ERROR = 0;
	static const int8_t GENERAL_ERROR = 1;
	static const int8_t LAST_ERROR_CODE = GENERAL_ERROR;

public:
	virtual ~ITransceiver() {};

	/**
	 * Connect transceiver
	 */
	virtual Error connect(void) = 0;

	/**
	 * returns true if connected
	 * @return
	 */
	virtual bool isConnected(void) = 0;

	/**
	 * Disconnect transceiver
	 */
	virtual Error disconnect(void) = 0;

	/**
	 * Send given bytes
	 * @param arg_u16_nb_bytes number of bytes to send
	 * @param arg_au8_bytes_send bytes to send, updated to number of bytes sent
	 * @return number of bytes sent
	 */
	virtual Error send(uint8_t& arg_u8_nb_bytes, uint8_t arg_au8_bytesSend[]) = 0;

	/**
	 * Receive given number of bytes.
	 * @param arg_u16_nb_bytes number of received bytes to read, updated to number of bytes
	 * successfully read
	 * @param arg_au8_bytes_read bytes received
	 * @return number of bytes received
	 */
	virtual Error receive(uint8_t& arg_u8_nb_bytes, uint8_t arg_au8_bytesRead[]) = 0;
};

#endif /* TRANSCEIVER_H_ */
