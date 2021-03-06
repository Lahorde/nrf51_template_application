### Description

Template application over nrf51 SDK using Eclipse IDE.
Just write your code in template_application.cpp

### Prerequisities

#### nRF51 SDK 
 * Provided when purchasing nRF51 development kit
 * An alternative consist in using Nordic nRF51 mbed source files http://developer.mbed.org/platforms/Nordic-nRF51822/

##### Modifications
 * in path_to_nrf51_sdk/nrf51822/Source/app_common/app_common.c
 
 ```
 //TO add at beginning
 volatile uint64_t rtc1_overflow_count=0;
 
 //replace method body
 /**@brief Function for handling the RTC1 interrupt.
 *
 * @details Checks for timeouts, and executes timeout handlers for expired timers.
 */
void RTC1_IRQHandler(void)
{
	if( NRF_RTC1->EVENTS_OVRFLW != 0)
	{
		rtc1_overflow_count += 0x1000000LLU;
		rtc1_overflow_count &= 0xffffffffffLLU;
		NRF_RTC1->EVENTS_OVRFLW = 0;
	}
	if( NRF_RTC1->EVENTS_COMPARE[0] !=0 )
	{
		// Clear all events (also unexpected ones)
		NRF_RTC1->EVENTS_COMPARE[0] = 0;
		NRF_RTC1->EVENTS_COMPARE[1] = 0;
		NRF_RTC1->EVENTS_COMPARE[2] = 0;
		NRF_RTC1->EVENTS_COMPARE[3] = 0;
		NRF_RTC1->EVENTS_TICK       = 0;
		// Check for expired timers
		timer_timeouts_check();
	}
}
```

### JLink GDB debugger
 * GDB server can be found here https://www.segger.com/jlink-software.html
 * GOT some disconnections on linux
 
### STLink - openocd
 * details here : https://github.com/RIOT-OS/RIOT/wiki/Board:-yunjia-nrf51822
 * for code uploading refer ./scripts/openocd/open_ocd_upload_application.sh - http://openocd.org/doc/html/Flash-Programming.html
 * to debug : http://openocd.org/doc/html/GDB-and-OpenOCD.html
   * in eclipse : http://gnuarmeclipse.github.io/debug/openocd/#create-the-debugger-configuration

More info here : https://www.besttechnology.co.jp/modules/knowledge/gate.php/openocd.pdf?way=attach&_noumb&refer=OpenOCD&openfile=openocd.pdf

#### Eclipse
Eclipse CDT with GNU ARM plugin http://sourceforge.net/projects/gnuarmeclipse/
 * in "Project Properties" -> "Resource" -> "Linked resources" : NRF51_SDK_PATH pointing to nrf51822 folder of Nordic nR51 SDK
 * in "Project Properties" -> "C/C++ build" -> "environment" :
    * NRF51_SDK_PATH pointing to nrf51822 folder of Nordic nR51 SDK
    * JLINK_DEBUGGER_PATH pointing to JLinkGDBServer folder
    
#### Others

 * Softdevice must be flashed on target (for details refer https://github.com/mrquincle/bluenet )
 * modify ./CMakeBuild.config according to your setup 

```
cd scripts
./softdevice.sh upload
```

#### Application configuration
 * Event queue

### TODO 
Add softdevice library to build
Use nano lib
printf= > trop de stack occupé
new KO
script pour stlink
button en subtree
ajout ITransceiver
gérer reconnection

### refs
http://stackoverflow.com/questions/14588091/allocating-c-classes-in-static-memory-with-gcc
