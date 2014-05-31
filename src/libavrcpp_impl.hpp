/** The AVR standard library misses some important functionality that's required
for certain c++ constructs. This aims to provide those.*/

#ifndef LIBCPP_AVR_EXTENSION
#define LIBCPP_AVR_EXTENSION

/** This is invoked when a pure virtual designated function is called.
This must be defined in order to specify a function as pure virtual without
getting errors from the linker.
Note that such a call is indicative of an error condition. */
extern "C" void __cxa_pure_virtual(void);

#endif //LIBCPP_AVR_EXTENSION
