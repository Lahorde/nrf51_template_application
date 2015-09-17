/******************************************************************************
 * @file    inter_periph_com.h 
 * @author  Rémi Pincent - INRIA
 * @date    22 janv. 2015   
 *
 * @brief TODO_one_line_description_of_file
 * 
 * Project : nrf51_template_application
 * Contact:  Rémi Pincent - remi.pincent@inria.fr
 * 
 * Revision History:
 * TODO_revision history
 *****************************************************************************/
#ifndef SYSTEM_INTER_PERIPH_COM_H_
#define SYSTEM_INTER_PERIPH_COM_H_

/**************************************************************************
 * Include Files
 **************************************************************************/
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************
 * Manifest Constants
 **************************************************************************/
#define UNAVAILABLE_GPIOTE_CHANNEL  (0xFFu)

/**************************************************************************
 * Type Definitions
 **************************************************************************/

/**************************************************************************
 * Variables
 **************************************************************************/
extern uint16_t PPI_Channels_Occupied[4][2]; 	//Save PPI channel number, each GPIOTE channel takes up two PPI channels
extern uint8_t GPIOTE_Channels_Occupied[4]; 			  				//GPIOTE channel Status, 1--have occupied, 255--not occupied
extern uint8_t GPIOTE_Channel_for_Analog[3];				  				//Save the Channel number used by PWM,
extern uint8_t Timer1_Occupied_Pin[3]; 				      				//the pin which used for analogWrite
/**************************************************************************
 * Macros
 **************************************************************************/

/**************************************************************************
 * Global Functions
 **************************************************************************/
uint8_t gpioteChannelFind();

void gpioteChannelSet(uint8_t channel);

void gpioteChannelClean(uint8_t channel);

int find_free_PPI_channel(int exclude_channel);

void ppiOffFromGPIO(uint32_t pin);

#ifdef __cplusplus
}
#endif


#endif /* SYSTEM_INTER_PERIPH_COM_H_ */
