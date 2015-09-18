/******************************************************************************
 * @file    persistent_object.h
 * @author  Rémi Pincent - INRIA
 * @date    14 sept 2015
 *
 * @brief Template functions to write anything on persistent memory
 * 
 * Project : smart_weight_scale
 * Contact:  Rémi Pincent - remi.pincent@inria.fr
 * 
 * Revision History:
 * TODO_revision history
 *****************************************************************************/

#ifndef PERSISTENT_OBJECT_H_
#define PERSISTENT_OBJECT_H_

#include <stdint.h>
#include <stdio.h>
#include "flash_memory.h"

/**
 * Write given object to persistent memory
 * @param arg_as8_location persistent memory location, e.g "rom1", "/file_1"
 * @param arg_s32_address persistent memory address "0x1234" relative to memory location
 * @param arg_object object to write
 * @return number of bytes written
 */
template <class T> int writePersistentObject(const char* arg_as8_location, int32_t arg_s32_address, const T& arg_object)
{
    int i,j;
    int8_t ret = 0;
    uint32_t remaining = 0;

    for (i = 0; i < (int) sizeof(arg_object)/4; i++)
    {
    	ret = FlashMem.writeLong(arg_s32_address + i*sizeof(int32_t), *((const int32_t*)&arg_object + i));
    	if(ret <=0){
    		return i;
    	}
    }

    if(sizeof(arg_object)%4 > 0)
    {
		for(j = sizeof(arg_object)%4 - 1; j >= 0; j--)
		{
			/** little endian */
			remaining =  (*(uint8_t*)((const int8_t*)&arg_object + i*sizeof(int32_t) + j) & 0xFF) | (remaining << 8);
		}

		ret = FlashMem.writeLong(arg_s32_address + i*sizeof(int32_t), remaining);
		if(ret <= 0){
			return i*sizeof(int32_t);
		}
    }
    return sizeof(arg_object);
}

/**
 * Write given object to persistent memory
 * @param arg_as8_location persistent memory location, e.g "rom1", "/file_1"
 * @param arg_s32_address persistent memory address "0x1234" relative to memory location
 * @param arg_object object read
 * @return number of bytes read
 */
template <class T> int readPersistentObject(const char* arg_as8_location, int32_t arg_s32_address, T& arg_object)
{
    unsigned int i, j;
    int8_t ret = 0;
    int32_t readValue = 0;

    for (i = 0; i < sizeof(arg_object)/4; i++)
    {
    	ret = FlashMem.readLong(arg_s32_address + i*sizeof(int32_t), readValue);

    	if(ret <=0){
    		return i*sizeof(int32_t);
    	}
    	*((int32_t*)&arg_object + i) = readValue;
    }

    if(sizeof(arg_object)%4 > 0)
    {
    	ret = FlashMem.readLong(arg_s32_address + i*sizeof(int32_t), readValue);

    	if(ret <=0){
    		return i*sizeof(int32_t);
    	}
        for(j = 0; j < sizeof(arg_object)%4; j++)
        {
        	*((int8_t*)&arg_object + i*sizeof(int32_t) + j) = (readValue >> 8*j) & 0xFF;
        }
    }
    return sizeof(arg_object);
}

#endif /* PERSISTENT_OBJECT_H_ */
