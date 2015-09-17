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
#define UNAVAILABLE_GPIOTE_CHANNEL  (0xFFU)
#define UNAVAILABLE_PPI_CHANNEL     (0xFFU)
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
static const uint8_t NB_PPI_APP_CHANNELS_SD_ENABLED = 8;
static const uint8_t NB_PPI_APP_CHANNELS_SD_DISABLED = 16;

/**************************************************************************
 * Macros
 **************************************************************************/

/**************************************************************************
 * Global Functions
 **************************************************************************/
uint8_t gpioteChannelFind();

void gpioteChannelSet(uint8_t channel);

void gpioteChannelClean(uint8_t channel);

/**
 * Find a free ppi channel among application available channels
 * @param exclude_channel
 * @return channel or UNAVAILABLE_PPI_CHANNEL
 */
uint8_t findFreePPIChannel(uint8_t exclude_channel);

/**
 * Wire and enables a PPI channel between given event and given task.
 * @param channel_num
 * @param evt_endpoint event end point
 * @param task_endpoint task end point
 * @return 0 if successful
 */
uint32_t wirePPIChannel(uint8_t channel_num, const volatile void * evt_endpoint, const volatile void * task_endpoint);

/**
 * Disable given ppi channel wire. After this call, channel becomes free.
 * @param channel_num
 * @return
 */
uint32_t freePPIChannel(uint8_t channel_num);

void ppiOffFromGPIO(uint32_t pin);

#ifdef __cplusplus
}
#endif


#endif /* SYSTEM_INTER_PERIPH_COM_H_ */
