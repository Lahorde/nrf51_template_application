#include "ppi.h"
#include "nrf51.h"
#include "softdevice_handler.h"

extern int BLE_used;

void ppi_channel_assign(uint8_t channel_num, const volatile void * evt_endpoint, const volatile void * task_endpoint)
{
    if (BLE_used == 0) {
        NRF_PPI->CH[channel_num].EEP = (uint32_t) evt_endpoint;
        NRF_PPI->CH[channel_num].TEP = (uint32_t) task_endpoint;

        NRF_PPI->CHEN |= (1UL) << ((unsigned long) channel_num);  
    } else if (BLE_used == 1) {
        sd_ppi_channel_assign(channel_num, evt_endpoint, task_endpoint);
        sd_ppi_channel_enable_set(((1UL) << ((unsigned long) channel_num)));
    }
}

void ppi_channel_unassign(uint8_t channel_num)
{
    if (BLE_used == 0) {
        NRF_PPI->CHEN &= (0UL) << ((unsigned long) channel_num);
    } else if (BLE_used == 1) {
        sd_ppi_channel_enable_clr(((1UL) << ((unsigned long) channel_num)));
    }    
}
