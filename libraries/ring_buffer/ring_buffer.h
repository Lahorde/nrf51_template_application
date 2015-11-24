/******************************************************************************
 * @file    ring_buffer.h
 * @author  Rémi Pincent - INRIA
 * @date    22 sept. 2015   
 *
 * @brief Ring buffer declaration - static allocation
 * 
 * Project : ring_buffer
 * Contact:  Rémi Pincent - remi.pincent@inria.fr
 * 
 * LICENSE :
 * AltSoftSerial (c) by Rémi Pincent
 * AltSoftSerial is licensed under a
 * Creative Commons Attribution-NonCommercial 3.0 Unported License.
 *
 * You should have received a copy of the license along with this
 * work.  If not, see <http://creativecommons.org/licenses/by-nc/3.0/>.
 * *****************************************************************************/

#ifndef RING_BUFFER_H_
#define RING_BUFFER_H_

#include <stdint.h>

template <class T>
class RingBuffer{
public:
	typedef enum{
		NO_ERROR = 0,
		BUFFER_FULL = -1,
		BUFFER_EMTPY = -2
	}EError;
private:
	uint16_t _u16_bufferSize;
	T* _a_buffer;
	uint16_t _u16_end;
	uint16_t _u16_start;
	bool _b_start;
	bool _b_end;
public:
	RingBuffer(void);
	RingBuffer(uint16_t arg_u16_bufferSize);
	~RingBuffer();

	bool isFull(void);
	uint16_t getRemainingSpace(void);
	bool elementsAvailable(void);
	EError pushElement(T arg_val);
	EError pushElements(uint16_t& arg_u6_nbElems, T arg_a_val[]);
	EError popElement(T& arg_val);
	EError popElements(uint16_t& arg_u16_nbElems, T arg_a_val[]);

private:
	void incrPtr(uint16_t& arg_u_16_ptr, bool& b_msb);
};

/** template definitions */
#include "ring_buffer.tpp"

#endif /* RING_BUFFER_H_ */
