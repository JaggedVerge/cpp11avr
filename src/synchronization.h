#ifndef SYNCRONIZATION_H
#define SYNCRONIZATION_H


#ifndef UNIT_TESTS //When compiling unit tests on x86 arch we don't have access to this header/ AVR specific functions
#include <avr/interrupt.h>

/**
 * Disable interrupts and return flags.
 * @return processor flags.
 */
inline uint8_t lock(){
	uint8_t processor_flags = SREG;
	cli();
	return (processor_flags);
}

/**
 * Restore processor flags and possible enable of interrupts.
 * @param[in] key processor flags.
 */
inline void unlock(uint8_t processor_flags)
{
	SREG = processor_flags;
}

#endif


/**
 * Syntactic sugar for a synchronized block. Used in the form:
 * @code
 * synchronized {
 *   ...
 *   synchronized_return(expr);
 *   ...
 *   synchronized_goto(label);
 *   ...
 * }
 * label:
 * @endcode
 * Interrupts are disabled in the block allowing secure update.
 */
#ifndef UNIT_TESTS
	#define synchronized							\
		for (uint8_t __key = lock(), i = 1; i != 0; i--, unlock(__key))
	#define synchronized_return(expr)					\
		return (unlock(__key), expr)
	#define synchronized_goto(label)					\
		do { unlock(__key); goto label; } while (0)
#else //If we are compiling unit tests for x86 then we have to change these definitions to not depend on AVR functionality
	#define synchronized					\
		if(true)
	#define synchronized_return(expr)		\
		return (expr)
	#define synchronized_goto(label)		\
		do { goto label; } while (0)
#endif

#endif //SYNCRONIZATION_H
