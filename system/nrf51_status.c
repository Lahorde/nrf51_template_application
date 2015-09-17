/******************************************************************************
 * @file    nrf51_status.c 
 * @author  Rémi Pincent - INRIA
 * @date    19 nov. 2014   
 *
 * @brief Get nRF51 status such as silicon temperature, battery level
 * 
 * Project : wimu
 * Contact:  Rémi Pincent - remi.pincent@inria.fr
 * 
 * Revision History:
 * TODO_revision history
 *****************************************************************************/

/**************************************************************************
 * Include Files
 **************************************************************************/
#include "nrf51_status.h"
#include "app_error.h"
#include "app_util.h"
#include "nrf_temp.h"
#include "wiring_analog.h"
#include "wiring_constants.h"
/**************************************************************************
 * Manifest Constants
 **************************************************************************/

/**************************************************************************
 * Type Definitions
 **************************************************************************/

/**************************************************************************
 * Variables
 **************************************************************************/

/**************************************************************************
 * Macros
 **************************************************************************/

/**************************************************************************
 * Local Functions
 **************************************************************************/

/**************************************************************************
 * Global Functions
 **************************************************************************/

int32_t nrf51_status_getSiliconTemp(void)
{
    // This function contains workaround for PAN_028 rev2.0A anomalies 28, 29,30 and 31.
    int32_t volatile temp;

    nrf_temp_init();

    NRF_TEMP->TASKS_START = 1; /** Start the temperature measurement. */

    /* Busy wait while temperature measurement is not finished, you can skip waiting if you enable interrupt for DATARDY event and read the result in the interrupt. */
    /*lint -e{845} // A zero has been given as right argument to operator '|'" */
    while (NRF_TEMP->EVENTS_DATARDY == 0)
    {
    	// Do nothing.
    }
    NRF_TEMP->EVENTS_DATARDY = 0;

    /**@note Workaround for PAN_028 rev2.0A anomaly 29 - TEMP: Stop task clears the TEMP register. */
    temp = (nrf_temp_read()/4);

    /**@note Workaround for PAN_028 rev2.0A anomaly 30 - TEMP: Temp module analog front end does not power down when DATARDY event occurs. */
    NRF_TEMP->TASKS_STOP = 1; /** Stop the temperature measurement. */

    return temp;
}

uint16_t nrf51_status_getBatteryPercent(EBattType arg_e_battType)
{
	uint16_t loc_u16_battLevel = 100;

	if(arg_e_battType == CR2032)
	{
		/** Set VDD as analog input */
		analogInpselType(INPSEL_VDD_1_3_PS);
		/** Set a known reference voltage @ 1.2V */
		analogReference(REFSEL_VBG);

		/** After these calls ADC references must be restored */

		/** Pin selection disabled as INPSEL set to VDD/3 */
		uint16_t loc_f32_vddVoltage = 1000*analogRead(0)*3.6 / 1023;
		loc_u16_battLevel = battery_level_in_percent(loc_f32_vddVoltage);
	}
	else
	{
		APP_ERROR_CHECK_BOOL(false);
	}

	return loc_u16_battLevel;
}
