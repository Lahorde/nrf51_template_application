/*
 Copyright (c) 2013 OpenSourceRF.com.  All right reserved.

 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 2.1 of the License, or (at your option) any later version.

 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 See the GNU Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public
 License along with this library; if not, write to the Free Software
 Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

/*
  Copyright (c) 2011 Arduino.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

/*
 * Here should go some sort of liscense maybe? This file is a modified
 * arduino source file.  It defines the board variant being used
*/

#ifndef _VARIANT_RFDUINO_
#define _VARIANT_RFDUINO_

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
#define RFDUINO

/** Name of the board */
#define VARIANT_NAME        "RFduino"

/** Master clock frequency  (used in TWI and DAC) */
#define VARIANT_MCK         16000000UL

// Number of pins
#define PINS_COUNT           (7u)
#define INVALID_PIN          (PINS_COUNT)

/*
 * SPI Interfaces
 */
#define SPI_INTERFACES_COUNT 2

#define SPI_INTERFACE        NRF_SPI0
#define PIN_SPI_SS           (6u)
#define PIN_SPI_MOSI         (5u)
#define PIN_SPI_MISO         (3u)
#define PIN_SPI_SCK          (4u)

#define SS    PIN_SPI_SS
#define MOSI  PIN_SPI_MOSI
#define MISO  PIN_SPI_MISO
#define SCK   PIN_SPI_SCK

/*
 * Wire Interfaces
 */
#define PIN_WIRE_SDA         (6u)
#define PIN_WIRE_SCL         (5u)
#define WIRE_INTERFACE       NRF_TWI1

/*
 * Analog pins
 */
#define ADC_RESOLUTION        10

/*
 * UART/USART Interfaces
 */
#define UART_DEFAULT_RX_PIN		0
#define UART_DEFAULT_TX_PIN		1

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
 * Global Functions
 **************************************************************************/
/**
 * Convert given PIN to be compatible with Arduino pins
 * @param pin
 * @return
 */
uint32_t arduinoToVariantPin(uint32_t pin);

#ifdef __cplusplus
}
#endif

#endif /* _VARIANT_RFDUINO_ */
