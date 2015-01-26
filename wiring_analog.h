

#ifndef _WIRING_ANALOG_
#define _WIRING_ANALOG_

#ifdef __cplusplus
extern "C" {
#endif


#define ADC_RESOLUTION        10
#define PWM_RESOLUTION        16

#define WRITE_CURRENT_RESOLUTION    8
#define READ_CURRENT_RESOLUTION		10


extern void analogWrite( uint32_t ulPin, uint32_t ulValue );
extern uint32_t analogRead( uint32_t ulPin );

extern void analogReference( uint32_t type );
extern void analogInpselType( uint32_t type);
//extern void analogExtReference( uint32_t type );
extern void analogReadResolution( uint8_t resolution);
extern void analogWriteResolution( uint8_t resolution );

#ifdef __cplusplus
}
#endif

#endif 
