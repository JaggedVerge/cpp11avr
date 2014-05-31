avrcpp
======
This project aims to create a c++ wrapper for some AVR chipsets.

The core aim is to make a high quality and high performance library.

The emphasis is on being able to use the chips in the Arduino hardware without
needing to use the Arduino IDE. Unlike the Arduino project this project is not
really aimed at beginners.

requirements
~~~~~~~~~~~~
To compile you need to have the avr-gcc toolchain available.
It needs to be a fairly recent version of gcc in order to support the c++11
constructs that are found in the code.

If you are on linux this means you need to get:
avr-gcc avr-libc binutils-avr

On windows you can get AVR studio
