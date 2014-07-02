/** \file buffer implementation
*/
#ifndef RING_BUFFER_H
#define RING_BUFFER_H

#include <stdint.h>

#include "synchronization.h"
//Return code when an element got successfully to the buffer without overwriting existing elements
#define ELEMENT_ADDED_SUCCESSFULLY 1
//Return code when an element was overwritten when attempted to add to the buffer (because the buffer was full)
#define ELEMENT_ADDED_OVERWRITE -1

/** \brief This is a ring buffer implementation.
 *template parameter1: T, the type we are storing
 *template parameter2: the maximum size of the stack
 *
 *The implementation approach used here is to keep track of the position
 *in the array of the first and last elements and also to keep track of
 *how many elements are stored at any given point in time.
 */
template<
	typename T,
	uint8_t BUFFER_SIZE
>
class ring_buffer{
static_assert(BUFFER_SIZE > 1,"A circular buffer of size < 2 doesn't make any sense, use a different data type instead.");

	public:
		/** The type we are storing */
		typedef T value_type;

		/** The reference type we are storing */
		typedef T& reference_type;

		/** The reference type we are storing */
		typedef T* pointer_type;

		/** The size type */
		typedef uint8_t size_type;

		/** The array indexing type */
		typedef uint8_t index_t;

		/** construct an empty circular ring buffer */
		ring_buffer():
			m_storage(),
			m_size(0),
			m_first_element(0),
			m_last_element(0),
			m_first_ptr(m_storage),
			m_last_ptr(m_storage),
			m_buffer_end_ptr(m_storage+BUFFER_SIZE)
		{}

		/** Emulate array access semantics.
		@param[in] element number
		*/
		T& operator[](index_t idx){
			return m_storage[(m_first_element + idx) % BUFFER_SIZE];
		}

		/** Emulate array access semantics - const version
		@param[in] element number
		*/
		T const& operator[](index_t idx) const{
			return m_storage[(m_first_element + idx) % BUFFER_SIZE];
		}

		/** Returns a reference to the first element in the buffer.
		Note this has a precondition that the buffer has to not be empty.*/
		T& front(){
			return m_storage[m_first_element];
		}

		/** Returns a reference to the first element in the buffer - const version
		Note this has a precondition that the buffer has to not be empty.*/
		T const& front() const{
			return m_storage[m_first_element];
		}

		/** Returns a reference to the last element in the buffer
		Note this has a precondition that the buffer has to not be empty.*/
		T& back(){
			return m_storage[m_last_element];
		}

		/** Returns a reference to the last element in the buffer - const version
		Note this has a precondition that the buffer has to not be empty.*/
		T const& back() const{
			return m_storage[m_last_element];
		}

		/** The maximum number of elements that the container can store. */
		size_type capacity(){
			return BUFFER_SIZE;
		}

		/** The number of elements currently stored in the buffer*/
		size_type size(){
			return m_size;
		}

		/** Returns true if the buffer is full and false otherwise */
		bool full(){
			return size() == BUFFER_SIZE;
		}

		/** Check if the container is empty. */
		bool empty(){
			return size() == 0;
		}
		/** Remove all elements from the buffer */
		void clear(){
			synchronized{
				m_size = 0;
				m_first_element = 0;
				m_last_element = 0;
				m_first_ptr = m_storage;
				m_last_ptr = m_storage;
				m_buffer_end_ptr = m_storage + BUFFER_SIZE;
			}
		}

		/** Put an element into the buffer at the back
		@param[in] the element we are placing into the buffer
		*/
		int push_back(T data){
			synchronized{
				if(!full()){
					(*m_last_ptr) = data;
					this->m_last_element = (m_last_element + 1) % BUFFER_SIZE;
					increment(this->m_last_ptr);
					++m_size;
					synchronized_return(ELEMENT_ADDED_SUCCESSFULLY);
				}else{//If the buffer is full we are overwriting elements
					(*m_last_ptr) = data;
					this->m_first_element = (m_first_element + 1) % BUFFER_SIZE;
					this->m_last_element = (m_last_element + 1) % BUFFER_SIZE;
					increment(this->m_first_ptr);
					increment(this->m_last_ptr);
					synchronized_return(ELEMENT_ADDED_OVERWRITE);
				}
			}
			return 1;
		}

		/** Pop an element off the end.
		Requires the container to be non-empty to work properly*/
		T pop_back(){
			synchronized{
				this->m_last_element = (this->m_last_element - 1) % BUFFER_SIZE;
				pointer_type old_ptr_to_last = this->m_last_ptr;
				decrement(this->m_last_ptr);
				--m_size;
				synchronized_return(*m_last_ptr);
			}
		}

		/** Put an element into the buffer at the front
		@param[in] the element we are placing into the buffer
		*/
		int push_front(T data){
			synchronized{
				if(!full()){
					decrement(this->m_first_ptr);
					*(m_first_ptr) = data;
					this->m_first_element = (m_first_element - 1) % BUFFER_SIZE;
					++m_size;
					synchronized_return(ELEMENT_ADDED_SUCCESSFULLY);
				}else{//If the buffer is full we are overwriting elements
					this->m_first_element = (m_first_element - 1) % BUFFER_SIZE;
					this->m_last_element = (m_last_element - 1) % BUFFER_SIZE;
					decrement(this->m_first_ptr);
					*(m_first_ptr) = data;
					decrement(this->m_last_ptr);
					synchronized_return(ELEMENT_ADDED_OVERWRITE);
				}
			}
		}

		/** Pop an element from the front.
		Requires the container to be non-empty to work properly*/
		T pop_front(){
			synchronized{
				pointer_type old_ptr_to_first = this->m_first_ptr;
				this->m_first_element = (this->m_first_element + 1) % BUFFER_SIZE;
				increment(this->m_first_ptr);
				--m_size;
				synchronized_return(*old_ptr_to_first);
			}
		}

		/** Get the oldest element out of the buffer */
		inline T pop_oldest(){
			return this->pop_front();
		}

		/** Get the newest element out of the buffer */
		inline T pop_newest(){
			return this->pop_back();
		}

		/** Remove a number elements from the front of the buffer
		@param[in] the number of elements to remove from the buffer
		*/
		inline void erase_front(size_type n){
			if(n >= this->size()){//erase the entire buffer
				this->clear();
			}else{
				while(n > 0){
					increment(this->m_first_ptr);
					--m_size;
					--n;
				}
			}
		}

	private:
		//Helper methods
		/** Helper function to increment a pointer to the internal storage in a circular buffer. */
		template <typename Pointer>
		void increment(Pointer& p){
			if (++p == m_buffer_end_ptr){
				//If pointer points to the end of the underlying storage
				//incrementing now points to the beginning of the storage array.
				p = m_storage;
			}
		}

		/** Helper function to decrement a pointer to the internal storage in a circular buffer.*/
		template <typename Pointer>
		void decrement(Pointer& p){
			if (p == m_storage){
				//If pointer points to the beginning of the underlying storage
				//decrementing now points to the end of the storage array.
				p = m_buffer_end_ptr;
			}
			--p;
		}


	private:
		/** the underlying buffer storage */
		T m_storage[BUFFER_SIZE];

		/** The number of elements currently stored in this container */
		size_type m_size;

		/** The index of the first element in the buffer*/
		index_t m_first_element;

		/** The index of the last element in the buffer*/
		index_t m_last_element;

		/** pointer to the first element */
		pointer_type m_first_ptr;

		/** pointer to one past the last element */
		pointer_type m_last_ptr;

		/** A pointer to the end of the underlying storage buffer */
		pointer_type m_buffer_end_ptr;
};

#endif //RING_BUFFER_H
