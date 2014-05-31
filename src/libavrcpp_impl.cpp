#include <stdlib.h>

#include "libcpp_avr_extension.h"

/** This is only ever invoked in an error condition.
Such an error can be caused by if a call to a virtual function
is made before the object is fully constructed.
Because we cannot handle this case gracefully we have to abort the
program.
*/
void __cxa_pure_virtual(void){
	abort();
}

