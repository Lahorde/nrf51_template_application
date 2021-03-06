/******************************************************************************
 * @file    events.h 
 * @author  Rémi Pincent - INRIA
 * @date    29 jan 2015
 *
 * @brief TODO_one_line_description_of_file
 * 
 * Project : nrf51_template_application
 * Contact:  Rémi Pincent - remi.pincent@inria.fr
 * 
 * Revision History:
 * TODO_revision history
 *****************************************************************************/

#ifndef EVENTS_H_
#define EVENTS_H_

#include <stdint.h>
#include <limits.h>

typedef enum __attribute__ ((__packed__)) {
/************************************************
 * Test events
 ************************************************/
	TEST_EVENT = 0,

/************************************************
 * Sensor events
 ************************************************/

/************************************************
 * INTERACTION events
 ************************************************/
	/** buttons */
	BUTTON_PRESSED,
	BUTTON_RELEASED,
	BUTTON_PRESSED_INT,
	BUTTON_RELEASED_INT,
	BUTTON_SHORT_PRESS,
	BUTTON_MEDIUM_PRESS,
	BUTTON_LONG_PRESS,
	BUTTON_LONG_LONG_PRESS,
/************************************************
 * PERCEPTION events
 ************************************************/

/************************************************
 * State events
 ************************************************/

}EEvents;
#endif /* EVENTS_H_ */
