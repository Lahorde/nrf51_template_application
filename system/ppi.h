#ifndef _PPI_H_
#define _PPI_H_

#include <stdint.h>

/**
 * Methods to interact with PPI :
 * The Programmable Peripheral Interconnect (PPI) enables different peripherals to interact autonomously
 * with each other using tasks and events without use of the CPU. The PPI provides a mechanism to
 * automatically trigger a task in one peripheral as a result of an event occurring in another. A task is connected
 * to an event through a PPI channel
 */
void ppi_channel_assign(uint8_t channel_num, const volatile void * evt_endpoint, const volatile void * task_endpoint);
void ppi_channel_unassign(uint8_t channel_num);

#endif //_PPI_H_
