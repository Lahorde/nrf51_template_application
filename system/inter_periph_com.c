/******************************************************************************
 * @file    inter_periph_com.c 
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

/**************************************************************************
 * Include Files
 **************************************************************************/
#include "inter_periph_com.h"
#include "interrupt_controller.h"
#include "app_error.h"
#include "nrf51.h"
#include "nrf_sdm.h"
#include "nrf_gpiote.h"

/**************************************************************************
 * Manifest Constants
 **************************************************************************/

/**************************************************************************
 * Type Definitions
 **************************************************************************/

/**************************************************************************
 * Variables
 **************************************************************************/
uint16_t PPI_Channels_Occupied[4][2] = {{UNAVAILABLE_PPI_CHANNEL, UNAVAILABLE_PPI_CHANNEL},
		{UNAVAILABLE_PPI_CHANNEL, UNAVAILABLE_PPI_CHANNEL},
		{UNAVAILABLE_PPI_CHANNEL, UNAVAILABLE_PPI_CHANNEL},
		{UNAVAILABLE_PPI_CHANNEL, UNAVAILABLE_PPI_CHANNEL}}; 	//Save PPI channel number, each GPIOTE channel takes up two PPI channels
uint8_t GPIOTE_Channels_Occupied[4]  = {UNAVAILABLE_GPIOTE_CHANNEL,
		UNAVAILABLE_GPIOTE_CHANNEL,
		UNAVAILABLE_GPIOTE_CHANNEL,
		UNAVAILABLE_GPIOTE_CHANNEL}; 			  				//GPIOTE channel Status, 1--have occupied, 255--not occupied
uint8_t GPIOTE_Channel_for_Analog[3] = {UNAVAILABLE_GPIOTE_CHANNEL,
		UNAVAILABLE_GPIOTE_CHANNEL,
		UNAVAILABLE_GPIOTE_CHANNEL};				  				//Save the Channel number used by PWM,
uint8_t Timer1_Occupied_Pin[3] = {255, 255, 255}; 				      				//the pin which used for analogWrite

/**************************************************************************
 * Macros
 **************************************************************************/

/**************************************************************************
 * Local Functions
 **************************************************************************/
/** Enable given ppi channel
 * @param channel_num
 */
void enablePPIChannel(uint8_t channel_num);

/**************************************************************************
 * Global Functions
 **************************************************************************/

/**********************************************************************
name :
function : find free GPIOTE channel
**********************************************************************/
uint8_t gpioteChannelFind()
{
	uint8_t _index;

	for(_index=0; _index<4; _index++)
	{
		if( GPIOTE_Channels_Occupied[_index] == UNAVAILABLE_GPIOTE_CHANNEL)
		{
			return _index;
		}
	}

	return UNAVAILABLE_GPIOTE_CHANNEL;
}

void gpioteChannelSet(uint8_t channel)
{
	GPIOTE_Channels_Occupied[channel] = 1;
}

void gpioteChannelClean(uint8_t channel)
{
	nrf_gpiote_unconfig(channel);
	GPIOTE_Channels_Occupied[channel] = UNAVAILABLE_GPIOTE_CHANNEL;
}

uint8_t findFreePPIChannel(uint8_t exclude_channel)
{
	uint32_t err_code = NRF_SUCCESS, chen;
	uint8_t  softdevice_enabled;
	uint8_t channelIndex = 0;
	/** Refer S110 Sift Device Specification 10.4 */
	uint8_t nbAppPPIChannels = 0;

	err_code = sd_softdevice_is_enabled(&softdevice_enabled);
	APP_ERROR_CHECK(err_code);
	if(softdevice_enabled == 0)
	{
		chen = NRF_PPI->CHEN;
		nbAppPPIChannels = NB_PPI_APP_CHANNELS_SD_DISABLED;
	}
	else
	{
		err_code = sd_ppi_channel_enable_get(&chen);
		APP_ERROR_CHECK(err_code);
		nbAppPPIChannels = NB_PPI_APP_CHANNELS_SD_ENABLED;
	}
	for (; channelIndex < nbAppPPIChannels; channelIndex++)
	{
		if(! ( chen & (1 << channelIndex) ) )
		{
			if (channelIndex != exclude_channel)
			{
				return channelIndex;
			}
		}
	}
	return UNAVAILABLE_PPI_CHANNEL;
}

uint32_t wirePPIChannel(uint8_t channel_num, const volatile void * evt_endpoint, const volatile void * task_endpoint)
{
	uint32_t err_code = NRF_SUCCESS;
	uint8_t  softdevice_enabled;

	APP_ERROR_CHECK_BOOL(channel_num != UNAVAILABLE_PPI_CHANNEL
			&& channel_num < NB_PPI_APP_CHANNELS_SD_DISABLED);

	err_code = sd_softdevice_is_enabled(&softdevice_enabled);
	APP_ERROR_CHECK(err_code);
	if (softdevice_enabled == 0 )
	{
		NRF_PPI->CH[channel_num].EEP	=	(uint32_t)evt_endpoint;
		NRF_PPI->CH[channel_num].TEP  =	(uint32_t)task_endpoint;
		enablePPIChannel(channel_num);
	}
	else
	{
		err_code = sd_ppi_channel_assign(channel_num, evt_endpoint, task_endpoint);
		APP_ERROR_CHECK(err_code);
		enablePPIChannel(channel_num);
	}
	return 0;
}

void enablePPIChannel(uint8_t channel_num)
{
	uint32_t err_code = NRF_SUCCESS;
	uint8_t  softdevice_enabled;

	APP_ERROR_CHECK_BOOL(channel_num != UNAVAILABLE_PPI_CHANNEL
			&& channel_num < NB_PPI_APP_CHANNELS_SD_DISABLED);

	err_code = sd_softdevice_is_enabled(&softdevice_enabled);
	APP_ERROR_CHECK(err_code);
	if (softdevice_enabled == 0)
	{
		NRF_PPI->CHEN |= (1 << channel_num);
	}
	else
	{
		err_code = sd_ppi_channel_enable_set(1 << channel_num);
		APP_ERROR_CHECK(err_code);
	}
}

uint32_t freePPIChannel(uint8_t channel_num)
{
	uint32_t err_code = NRF_SUCCESS;
	uint8_t  softdevice_enabled;

	APP_ERROR_CHECK_BOOL(channel_num != UNAVAILABLE_PPI_CHANNEL
			&& channel_num < NB_PPI_APP_CHANNELS_SD_DISABLED);

	err_code = sd_softdevice_is_enabled(&softdevice_enabled);
	APP_ERROR_CHECK(err_code);
	if (softdevice_enabled == 0)
	{
		NRF_PPI->CHEN &= ~(1 << channel_num);
	}
	else
	{
		err_code = sd_ppi_channel_enable_clr(1 << channel_num);
		APP_ERROR_CHECK(err_code);
	}
}

/**********************************************************************
name :
function : disconnect PPI
**********************************************************************/
void ppiOffFromGPIO(uint32_t pin)
{
	uint8_t channel;
	uint32_t err_code = NRF_SUCCESS;
	uint8_t  softdevice_enabled;

	err_code = sd_softdevice_is_enabled(&softdevice_enabled);
	APP_ERROR_CHECK(err_code);

	if(Timer1_Occupied_Pin[0] == pin)
	{
		channel = GPIOTE_Channel_for_Analog[0];

		if(softdevice_enabled == 0)
		{
			NRF_PPI->CHEN &= ~( 1 << PPI_Channels_Occupied[channel][0] );
			NRF_PPI->CHEN &= ~( 1 << PPI_Channels_Occupied[channel][1] );
		}
		else
		{
			err_code = sd_ppi_channel_enable_clr( 1 << PPI_Channels_Occupied[channel][0] );
			APP_ERROR_CHECK(err_code);
			err_code = sd_ppi_channel_enable_clr( 1 << PPI_Channels_Occupied[channel][1] );
			APP_ERROR_CHECK(err_code);
		}
		PPI_Channels_Occupied[channel][0] = 255;
		PPI_Channels_Occupied[channel][1] = 255;
		Timer1_Occupied_Pin[0] = 255;

		gpioteChannelClean(channel);
		GPIOTE_Channel_for_Analog[0] = 255;
	}
	else if(Timer1_Occupied_Pin[1] == pin)
	{

		channel = GPIOTE_Channel_for_Analog[1];
		if(softdevice_enabled == 0)
		{
			NRF_PPI->CHEN &= ~( 1 << PPI_Channels_Occupied[channel][0] );
			NRF_PPI->CHEN &= ~( 1 << PPI_Channels_Occupied[channel][1] );
		}
		else
		{
			err_code = sd_ppi_channel_enable_clr( 1 << PPI_Channels_Occupied[channel][0] );
			APP_ERROR_CHECK(err_code);
			err_code = sd_ppi_channel_enable_clr( 1 << PPI_Channels_Occupied[channel][1] );
			APP_ERROR_CHECK(err_code);
		}
		PPI_Channels_Occupied[channel][0] = 255;
		PPI_Channels_Occupied[channel][1] = 255;
		Timer1_Occupied_Pin[1] = 255;
		gpioteChannelClean(channel);
		GPIOTE_Channel_for_Analog[1] = 255;

	}
	else if(Timer1_Occupied_Pin[2] == pin)
	{
		channel = GPIOTE_Channel_for_Analog[2];
		if(softdevice_enabled == 0)
		{
			NRF_PPI->CHEN &= ~( 1 << PPI_Channels_Occupied[channel][0] );
			NRF_PPI->CHEN &= ~( 1 << PPI_Channels_Occupied[channel][1] );
		}
		else
		{
			err_code = sd_ppi_channel_enable_clr( 1 << PPI_Channels_Occupied[channel][0] );
			APP_ERROR_CHECK(err_code);
			err_code = sd_ppi_channel_enable_clr( 1 << PPI_Channels_Occupied[channel][1] );
			APP_ERROR_CHECK(err_code);
		}
		PPI_Channels_Occupied[channel][0] = 255;
		PPI_Channels_Occupied[channel][1] = 255;
		Timer1_Occupied_Pin[2] = 255;

		gpioteChannelClean(channel);
		GPIOTE_Channel_for_Analog[2] = 255;
	}

	//if all PWM stop, disable TIMER1
	if( Timer1_Occupied_Pin[0] == 255 && Timer1_Occupied_Pin[1] == 255 && Timer1_Occupied_Pin[2] == 255 )
	{
		NRF_TIMER1->TASKS_STOP = 1;
		NRF_TIMER1->INTENCLR = (TIMER_INTENCLR_COMPARE3_Disabled << TIMER_INTENCLR_COMPARE3_Pos);
		NVIC_DisableIRQ(TIMER1_IRQn);
		IntController_unlinkInterrupt(TIMER1_IRQn);
	}

}
