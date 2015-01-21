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

#ifdef __cplusplus
extern "C" {
#endif
/**
 * Get silicon temperature in Celsius
 * @return
 */
int32_t nrf51_status_getSiliconTemp(void);

#ifdef __cplusplus
}
#endif

#endif /* SYSTEM_NRF51_STATUS_H_ */
