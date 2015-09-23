/******************************************************************************
 * @file    ac_ble_transceiver.cpp
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
#include <ac_ble_transceiver.h>
#include <logger.h>

const uint8_t ACBLETransceiver::RX_BUFFER_SIZE = 70;

ACBLETransceiver::ACBLETransceiver(void) :BLETransceiver(),
	_rxBuffer(RX_BUFFER_SIZE)
{

}

ACBLETransceiver::~ACBLETransceiver() {
}


ITransceiver::Error ACBLETransceiver::connect(void)
{
	BLETransceiver::Error loc_e_error = NO_ERROR;
	loc_e_error = BLETransceiver::connect();

	if(loc_e_error != NO_ERROR)
		return loc_e_error;

	/** start advertising */
	return advertise();
}

ITransceiver::Error ACBLETransceiver::receive(uint8_t& arg_u8_nb_bytes, uint8_t arg_au8_bytesRead[])
{
	uint16_t loc_u16_nbBytesRead = arg_u8_nb_bytes;
	if(_rxBuffer.elementsAvailable())
	{
		if(_rxBuffer.popElements(loc_u16_nbBytesRead, arg_au8_bytesRead) == NO_ERROR)
		{
			return NO_ERROR;
		}
		else
		{
			return GENERAL_ERROR;
		}
	}
	else
	{
		return RX_BUFFER_EMPTY;
	}
}

void ACBLETransceiver::onBleEvt(ble_evt_t * p_ble_evt)
{
	BLETransceiver::Error loc_e_error = NO_ERROR;

	BLETransceiver::onBleEvt(p_ble_evt);
	switch (p_ble_evt->header.evt_id)
	{
		case BLE_GAP_EVT_DISCONNECTED:
			/** try to reconnect */
			loc_e_error = advertise();
			if(loc_e_error != NO_ERROR)
				LOG_ERROR("Cannot advertise on disconnection - errod id = %d", loc_e_error);
			break;

		default:
			break;
	}
}

void ACBLETransceiver::dataReceived(uint8_t * p_data, uint16_t length)
{
	typename RingBuffer<uint8_t>::EError loc_err = _rxBuffer.pushElements(length, p_data);
	if(loc_err != NO_ERROR)
	{
		LOG_ERROR("Cannot push elements to ring buffer");
	}
	ACBLETransceiver::dataReceived(p_data, length);
}


