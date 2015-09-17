/******************************************************************************
 * @file    flash_memory.cpp 
 * @author  Rémi Pincent - INRIA
 * @date    17 sept. 2015   
 *
 * @brief TODO_one_line_description_of_file
 * 
 * Project : nrf51_template_application
 * Contact:  Rémi Pincent - remi.pincent@inria.fr
 * 
 * Revision History:
 * TODO_revision history
 *****************************************************************************/

#include <flash_memory.h>
extern "C" {
#include "ble_flash.h"
#include "app_error.h"
}

/** refer linker script */
/** end rom */
extern uint32_t __etext;

// divide the address by 1024
#define  PAGE_FROM_ADDRESS(address)  ((uint8_t)((uint32_t)address >> 10))

// multiple the page by 1024
#define  ADDRESS_OF_PAGE(page)  ((uint32_t*)(page << 10))

int8_t FlashMemory::read(int address)
{
	if((uint32_t*) address <= &__etext)
	{
		/** writing in application code */
		return -1;
	}
	return (*(uint32_t*)address) & 0xFF;
}

int8_t FlashMemory::write(int address, uint8_t value)
{
	uint32_t err_code = NRF_SUCCESS;

	if((uint32_t*) address <= &__etext)
	{
		/** writing in application code */
		return -1;
	}

	err_code = ble_flash_word_write((uint32_t*)address, (uint32_t) value);
	APP_ERROR_CHECK(err_code);
	return sizeof(value);
}

FlashMemory FlashMem;
