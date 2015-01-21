### Description

Template application over nrf51 SDK using Eclipse IDE.
Just write your code in template_application.cpp

### Prerequisities

#### nRF51 SDK 
 * Provided when purchasing nRF51 development kit
 * An alternative consist in using Nordic nRF51 mbed source files http://developer.mbed.org/platforms/Nordic-nRF51822/

### JLink GDB debugger
 * GDB server can be found here https://www.segger.com/jlink-software.html

#### Eclipse
Eclipse CDT with GNU ARM plugin http://sourceforge.net/projects/gnuarmeclipse/
 * in "Project Properties" -> "Resource" -> "Linked resources" : NRF51_SDK_PATH pointing to nrf51822 folder of Nordic nR51 SDK
 * in "Project Properties" -> "C/C++ build" -> "environment" :
    * NRF51_SDK_PATH pointing to nrf51822 folder of Nordic nR51 SDK
    * JLINK_DEBUGGER_PATH pointing to JLinkGDBServer folder
    
#### Others
Softdevice flashed on target



### TODO 
Add softdevice library