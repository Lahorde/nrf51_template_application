/******************************************************************************
 * @file    timer_listener.h 
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

#ifndef TIMER_LISTENER_H_
#define TIMER_LISTENER_H_

/**
 * @class TimerListener
 * @brief TODO_one_line_description_of_class.
 *
 * TODO_longer_description_of_class_meant_for_users._
 * 
 */
class TimerListener
{
public :
	virtual ~TimerListener(void){};
	virtual void timerElapsed(void) = 0;
};

#endif /* TIMER_LISTENER_H_ */
