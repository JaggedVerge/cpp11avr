/** \file stack buffer implementation
*/
#ifndef STACK_BUFFER_H
#define STACK_BUFFER_H

/** \brief This is a stack buffer implementation. 
 *
 *template parameter1: T, the type we are storing
 *template parameter2: the maximum size of the stack
 */
template<
	typename T,
	uint8_t BUFFER_SIZE
>
class stack_buffer{
	public:
		/** The type we are storing */
		typedef T value_type;

		/** The size type */
		typedef uint8_t size_type;

		/** The array indexing type */
		typedef uint8_t index_t;

		/** constructor */
		stack_buffer():
			m_buffer(),
			m_stack_top(0)
		{ }

		/** Put data into the buffer, return true if successful
		@param[in] the element we are placing into the buffer
		*/
		bool push(T data){
			if(m_stack_top == BUFFER_SIZE){
				//Underlying array is full
				return false;
			}
			this->m_buffer[m_stack_top] = data;
			this->m_stack_top++;
			return true;
		}

		/** Pop one element out of the buffer */
		T pop(){
			T ret_val = m_buffer[m_stack_top-1];
			this->m_stack_top--;
			return ret_val;
		}

		/** Checks if the stack is empty */
		bool empty(){
			return (this->m_stack_top == 0);
		}

		/** Checks if the stack is empty - const version*/
		bool empty() const{
			return (this->m_stack_top == 0);
		}

		/** Returns a reference to the top element */
		T& top(){
			return m_buffer[m_stack_top-1];
		}

		/** Returns a reference to the top element - const version*/
		T const& top() const{
			return m_buffer[m_stack_top-1];
		}

		/** The number of elements currently stored in the buffer*/
		size_type size(){
			return m_stack_top;
		}

		/** The number of elements currently stored in the buffer - const version*/
		size_type size() const{
			return m_stack_top;
		}

		/** Returns true if the buffer is full and false otherwise */
		bool full(){
			return (this->m_stack_top == BUFFER_SIZE);
		}

		/** Returns true if the buffer is full and false otherwise - const version*/
		bool full() const{
			return (this->m_stack_top == BUFFER_SIZE);
		}

	private:
		/** the underlying buffer storage */
		T m_buffer[BUFFER_SIZE];

		/** The index of the first free memory address in the buffer */
		index_t m_stack_top;
};

#endif //STACK_BUFFER_H
