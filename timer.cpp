/******************************************************************************
 * @file    timer.cpp 
 * @author  Rémi Pincent - INRIA
 * @date    27 juin 2014   
 *
 * @brief TODO_one_line_description_of_file
 * 
 * Project : ContainerNode
 * Contact:  Rémi Pincent - remi.pincent@inria.fr
 * 
 * Revision History:
 * TODO_revision history
 *****************************************************************************/

#include "timer.h"
#include "application_config.h"
#include <EventManager.h>
#include <logger.h>

#define INACTIVE_TIMER APP_TIMER_MAX_TIMERS

Timer::Timer(TimerListener* arg_p_timer_listener) :
_p_timerListener(arg_p_timer_listener),
_timerId(INACTIVE_TIMER),
_b_periodicTimer(false),
_isActive(false)
{
	/** before using this class - timer pool must be initialized with
	 * APP_TIMER_INIT()
	 */
}

Timer::~Timer()
{
	if(_isActive)
	{
		cancel();
	}
}

void Timer::notifyAfter(uint32_t arg_u32_ms)
{
	LOG_VERBOSE(F("notify after %ums\n"), arg_u32_ms);
	_b_periodicTimer = false;
	start(arg_u32_ms);
}

void Timer::periodicNotify(uint32_t arg_u32_ms)
{
	LOG_VERBOSE(F("periodic notify of %ums\n"), arg_u32_ms);
	_b_periodicTimer = true;
	start(arg_u32_ms);
}

bool Timer::isActive(void)
{
	return _isActive;
}

void Timer::cancel(void)
{
	LOG_VERBOSE(F("cancel timer\n"));
	uint32_t err_code;
	_isActive = false;
	APP_ERROR_CHECK_BOOL(_timerId != INACTIVE_TIMER);
	err_code = app_timer_stop(_timerId);
	APP_ERROR_CHECK(err_code);
}

void Timer::start(uint32_t arg_u32_ms)
{
	uint32_t err_code;
	init();
	_isActive = true;
	err_code = app_timer_start(_timerId, APP_TIMER_TICKS(arg_u32_ms, 0), this);
	APP_ERROR_CHECK(err_code);
}

void Timer::init(void)
{
	uint32_t err_code;
	if(_timerId != INACTIVE_TIMER)
	{
		/** nothing do - timer already initialized */
	}
	else
	{
		err_code = app_timer_create(&_timerId,
						 APP_TIMER_MODE_REPEATED,
						 (app_timer_timeout_handler_t)timerElapsed);
		APP_ERROR_CHECK(err_code);
	}
}

void Timer::timerElapsed(Timer * arg_p_timer)
{
	if(!arg_p_timer->_b_periodicTimer){
		arg_p_timer->cancel();
	}
	arg_p_timer->_p_timerListener->timerElapsed();
}
