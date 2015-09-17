/******************************************************************************
 * @file    nrf51_status.h 
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
#ifndef SYSTEM_NRF51_STATUS_H_
#define SYSTEM_NRF51_STATUS_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
	CR2032,
}EBattType;

/**
 * Get silicon temperature in Celsius
 * @return
 */
int32_t nrf51_status_getSiliconTemp(void);

/**
 * Get battery percent
 * @return
 */
uint16_t nrf51_status_getBatteryPercent(EBattType arg_e_battType);

#ifdef __cplusplus
}
#endif

#endif /* SYSTEM_NRF51_STATUS_H_ */
