#include <stdlib.h>

#include "libcpp_avr_extension.h"

/** __cxa_pure_virtual is called when a pure-virtual function
is called. Note that this is only ever invoked in an error condition.
Such an error can be caused if a call to a virtual function
is made before the object is fully constructed.
Because we cannot handle this case gracefully we have to abort the
program. Note that the call to abort() here could be substituted
for some other error reporting mechanism when used in your project,
abort() is just a default.
*/
void __cxa_pure_virtual(void){
	abort();
}

