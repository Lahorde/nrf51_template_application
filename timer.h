/******************************************************************************
 * @file    timer.h 
 * @author  Rémi Pincent - INRIA
 * @date    27 juin 2014   
 *
 * @brief simple timer
 * 
 * Project : ContainerNode
 * Contact:  Rémi Pincent - remi.pincent@inria.fr
 * 
 * Revision History:
 * TODO_revision history
 *****************************************************************************/

#ifndef TIMER_H_
#define TIMER_H_

/**
 * @class Timer
 * @brief simple timer notifying its listener when it elapses
 * EVENTS : must have been defined in events.h
 *   _EMITTED EVENTS :
 *   _SUBSCRIBED EVENTS :
 */
#include "timer_listener.h"
extern "C"{
	#include "app_timer.h"
}
#include <EventListener.h>
#include <stdint.h>

class Timer
{
private :
	TimerListener* _p_timerListener;
	app_timer_id_t _timerId;
	bool _b_periodicTimer;
	bool _isActive;

public:
	Timer(TimerListener* arg_p_timer_listener);
	~Timer();

	/**
	 * Notify listener after given duration
	 * @param arg_u32_ms duration in ms
	 */
	void notifyAfter(uint32_t arg_u32_ms);

	/**
	 * Notifies its listener all arg_u16_ms ms
	 * @param arg_u32_ms duration in ms
	 */
	void periodicNotify(uint32_t arg_u32_ms);

	/**
	 * Returns true if timer still counting
	 * @return
	 */
	bool isActive(void);

	/**
	 * cancel on going timer
	 */
	void cancel(void);
private :

	/**
	 * Initialize timer
	 */
	void init(void);

	/**
	 * start timer
	 */
	void start(uint32_t arg_u32_ms);

	/**
	 * Timer elapsed - timer instance given as parameter
	 * @param eventCode
	 */
	static void timerElapsed(Timer * arg_p_timer);
};

#endif /* TIMER_H_ */
