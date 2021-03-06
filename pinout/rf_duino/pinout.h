/******************************************************************************
 * @file    pinout.h
 * @author  Rémi Pincent - INRIA
 * @date    20 nov. 2014
 *
 * @brief Pin out for application used on RFDuino target
 * http://rfduino.com/product/rfd22301-rfduino-ble-smt/index.html
 *
 * Project : nrf51_template_application
 * Contact:  Rémi Pincent - remi.pincent@inria.fr
 *
 * Revision History:
 * TODO_revision history
 *****************************************************************************/

#ifndef PINOUT_H_
#define PINOUT_H_

/*********************************
 * UART
 *********************************/
/** RFDuino Rx */
static const uint8_t UART_RX_PIN = 0;
/** RFDuino Tx */
static const uint8_t UART_TX_PIN = 1;

/*********************************
 * Buttons
 *********************************/
/** RFDuino GPIO 2 */
static const uint8_t BUTTON_1_INPUT_PIN = 3;

/*********************************
 * LED
 *********************************/
/** RFDuino GPIO 3 */
static const uint8_t LED_1_OUTPUT_PIN = 4;

#endif /* PINOUT_H_ */
