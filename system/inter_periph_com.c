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
uint16_t PPI_Channels_Occupied[4][2] = {{255, 255}, {255, 255}, {255, 255}, {255, 255}}; 	//Save PPI channel number, each GPIOTE channel takes up two PPI channels
uint8_t GPIOTE_Channels_Occupied[4]  = {UNAVAILABLE_GPIOTE_CHANNEL, UNAVAILABLE_GPIOTE_CHANNEL, UNAVAILABLE_GPIOTE_CHANNEL, UNAVAILABLE_GPIOTE_CHANNEL}; 			  				//GPIOTE channel Status, 1--have occupied, 255--not occupied
uint8_t GPIOTE_Channel_for_Analog[3] = {UNAVAILABLE_GPIOTE_CHANNEL, UNAVAILABLE_GPIOTE_CHANNEL, UNAVAILABLE_GPIOTE_CHANNEL};				  				//Save the Channel number used by PWM,
uint8_t Timer1_Occupied_Pin[3] = {255, 255, 255}; 				      				//the pin which used for analogWrite

/**************************************************************************
 * Macros
 **************************************************************************/

/**************************************************************************
 * Local Functions
 **************************************************************************/

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
/**********************************************************************
name :
function :
**********************************************************************/
void gpioteChannelSet(uint8_t channel)
{
	GPIOTE_Channels_Occupied[channel] = 1;
}
/**********************************************************************
name :
function :
**********************************************************************/
void gpioteChannelClean(uint8_t channel)
{
	nrf_gpiote_unconfig(channel);
	GPIOTE_Channels_Occupied[channel] = UNAVAILABLE_GPIOTE_CHANNEL;
}

/**********************************************************************
name :
function : find free PPI channel
**********************************************************************/
int find_free_PPI_channel(int exclude_channel)
{
	uint32_t err_code = NRF_SUCCESS, chen;
	uint8_t  softdevice_enabled;
	int start = 0;
	int end = 8;
	int i;

	err_code = sd_softdevice_is_enabled(&softdevice_enabled);
	APP_ERROR_CHECK(err_code);
	if(softdevice_enabled == 0)
	{
		chen = NRF_PPI->CHEN;
	}
	else
	{
		err_code = sd_ppi_channel_enable_get(&chen);
		APP_ERROR_CHECK(err_code);
	}
	for (i = start; i < end; i++)
	{
		//if (! (NRF_PPI->CHEN & (1 << i)))
		if(! ( chen & (1 << i) ) )
		{
			if (i != exclude_channel)
			{
				return i;
			}
		}
	}
	return 255;
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
