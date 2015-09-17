/******************************************************************************
 * @file    flash_memory.h 
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
#ifndef FLASH_MEMORY_H_
#define FLASH_MEMORY_H_

#include <stdint.h>

/**
 * @class FlashMemory
 * @brief TODO_one_line_description_of_class.
 *
 * TODO_longer_description_of_class_meant_for_users._
 * 
 */
class FlashMemory
{
public:
	int8_t read(int);
	int8_t write(int, uint8_t);
};

extern FlashMemory FlashMem;

#endif /* FLASH_MEMORY_H_ */
