/******************************************************************************
 * @file    transceiver_listener.h 
 * @author  Rémi Pincent - INRIA
 * @date    3 févr. 2015   
 *
 * @brief TODO_one_line_description_of_file
 * 
 * Project : transceiver
 * Contact:  Rémi Pincent - remi.pincent@inria.fr
 * 
 * Revision History:
 * TODO_revision history
 *****************************************************************************/
#ifndef ITRANSCEIVER_LISTENER_H_
#define ITRANSCEIVER_LISTENER_H_

/**
 * @class TransceiverListener
 * @brief TODO_one_line_description_of_class.
 *
 * TODO_longer_description_of_class_meant_for_users._
 * 
 */
class ITransceiverListener
{
public:
	virtual ~ITransceiverListener(){};
	virtual void onDataReceived(uint8_t arg_u8_dataLength, uint8_t arg_au8_data[]) = 0;
};

#endif /* ITRANSCEIVER_LISTENER_H_ */
