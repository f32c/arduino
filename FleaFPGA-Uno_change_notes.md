# *** f32c FleaFPGA-Uno change notes ***

Here are some notes regarding changes to the FPGArduino "core" files.

### hardware\fpga\f32c\boards.txt:

* Removed OpenOCD from JTAG menu name (it can be in menu option, but not all boards use this)
* Added preliminary FleaFPGA-Uno support
	- no JTAG yet

### hardware\fpga\f32c\system\include\dev\io.h:

* Added IO_GPIO_INPUT register (for getting input from GPIO vs IO_GPIO_DATA)
	- modeled after AVR ATmega328, where reading DATA reads output register (not GPIO input), writing DATA 1 when input enables pull-up, and writing INPUT 1 toggles corresponding DATA.

### hardware\fpga\f32c\platform.txt:

* Removed -fpermissive from C compile options (not legal for C) [Warning fix]

### hardware\fpga\f32c\cores\f32c\UARTClass.cpp:
### hardware\fpga\f32c\cores\f32c\UARTClass.h:

* Significant changes to support multiple UARTs.  I did this via a templated class (aka C++ magic).  It is possible to do this via pointers too (and perhaps smaller code size).
* This also brings in variant.h file which instantiates the proper UARTs for the board.
	- I suspect this could also help a future interrupt handler know what ports to look for...

### hardware\fpga\f32c\cores\f32c\wiring.c:

* Removed CPU "DELAY" stuff and implemented delay via micros() primitive. :-) [Couldn't resist]

### hardware\fpga\f32c\cores\f32c\wiring_analog.c:

* Added optioanl support for analogRead (on the FleaFPGA-Uno or other boards with proper hardware).  Uses definition of I/O address IO_ADC_A0 to determine if the feature is present in the current variant.  Also uses some values defined in variant.h to scale the ADC value to match Arduino expected value (0 to 1023).

### hardware\fpga\f32c\cores\f32c\wiring_digital.c:

* added optional support for INPUT_PULLUP in pinMode (boards without pull-up support will harmlessly set a register that gets ignored).

### hardware\fpga\f32c\cores\f32c\wiring_pulse.cpp:

* warning fix for unused parameters pin1 and pin2 (I compile with "maximum warnings" enabled).

## Added libraries

### libraries\f32c_PS2Keyboard added

* Simple demo of PS/2 port usage (on f32c with PS/2 low-level port support).
	- Based on Arduino PS/2 library
	- Extended to support control-keys
	- Decodes most special function keys
	- Detects keyboard present or not
	- Resets keyboard and sets max repeat rate upon init
	- Does not yet support setting caps-lock LED etc. (but HDL has support).
	- International keyboard support disabled currently (but is "mostly" supported)
		- currently only returns 8-bits so special function keys take too many codes

### libraries\f32c_VGATextConsole added

* Supports text console access to VGA text-mode in f32c
	- including scrolling and control character support.
	- use "vga" similar to as you would (e.g.) Arduino "Serial"
	- supports screen windowing
	- 2-color or 16-color mode (16-color requires 8K BRAM)
* 8 or 16 high font (8 high line-doubled), 128 or 256 characters
* text cursor
* optional palette
* optional monochrome bitmap
* Simple demo that draws RLE HaD logo (and echoes PS/2 keyboard)



	