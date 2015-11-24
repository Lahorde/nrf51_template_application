/******************************************************************************
 * @file    ring_buffer.tpp
 * @author  Rémi Pincent - INRIA
 * @date    22 sept. 2015   
 *
 * @brief ring buffer implementation - uses virtual mirror
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
 
#include <assert.h>
#include <cstring>
#include <cstdlib>

template <class T>
RingBuffer<T>::RingBuffer(uint16_t arg_u16_bufferSize) :
		_u16_bufferSize(arg_u16_bufferSize),
		_u16_end(0),
		_u16_start(0),
		_b_start(0),
		_b_end(0){
	_a_buffer = (T*)calloc(arg_u16_bufferSize, sizeof(T));
	assert(_a_buffer != NULL);
}

template <class T>
RingBuffer<T>::~RingBuffer() {
	if(_a_buffer != NULL)
		free(_a_buffer);
	_b_start = 0;
	_b_end = 0;
	_u16_end = 0;
	_u16_start = 0;
}

template <class T>
bool RingBuffer<T>::isFull(void)
{
	return ((_u16_end == _u16_start) && (_b_start != _b_end));
}

template <class T>
uint16_t RingBuffer<T>::getRemainingSpace(void)
{
	if(isFull()){
		return 0;
	}

	/** +1 for _u16_end == _u16_start */
	return ((_u16_bufferSize -1 - (_u16_end - _u16_start)) % _u16_bufferSize) + 1;
}

template <class T>
bool RingBuffer<T>::elementsAvailable(void)
{
	return !((_u16_end == _u16_start) && (_b_start == _b_end));
}

template <class T>
typename RingBuffer<T>::EError RingBuffer<T>::pushElement(T arg_val)
{
	/** NO overwrite */
	if(isFull()){
		return BUFFER_FULL;
	}

	_a_buffer[_u16_end] = arg_val;
	incrPtr(_u16_end, _b_end);
	return NO_ERROR;
}

template <class T>
typename RingBuffer<T>::EError RingBuffer<T>::pushElements(uint16_t& arg_u16_nbElems, T arg_a_val[])
{
	assert(arg_u16_nbElems <=  _u16_bufferSize);
	const uint16_t loc_cu16_nbElemsToPush = arg_u16_nbElems;
	for(uint32_t i = 0; i < loc_cu16_nbElemsToPush; i++)
	{
		if(pushElement(arg_a_val[i]) < 0)
		{
			return BUFFER_FULL;
		}
		else{
			arg_u16_nbElems++;
		}
	}
	return NO_ERROR;
}

template <class T>
typename RingBuffer<T>::EError RingBuffer<T>::popElement(T& arg_val)
{
	if(!elementsAvailable())
	{
		return BUFFER_EMTPY;
	}

	/** FIFO */
	arg_val = _a_buffer[_u16_start];
	incrPtr(_u16_start, _b_start);

	return NO_ERROR;
}

template <class T>
typename RingBuffer<T>::EError RingBuffer<T>::popElements(uint16_t& arg_u16_nbElems, T arg_a_val[])
{
	const uint16_t loc_cu16_nbElemsToPop = arg_u16_nbElems;
	for(uint16_t i = 0; i < loc_cu16_nbElemsToPop; i++)
	{
		if(popElement(arg_a_val[i]) < 0)
		{
			return BUFFER_EMTPY;
		}
		else{
			arg_u16_nbElems++;
		}
	}
	return NO_ERROR;
}

template <class T>
void RingBuffer<T>::incrPtr(uint16_t& arg_u_16_ptr, bool& b_msb)
{
	arg_u_16_ptr += 1;
	if(arg_u_16_ptr >= _u16_bufferSize)
	{
		b_msb ^= 1;
		arg_u_16_ptr = 0;
	}
}
