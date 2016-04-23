# FPGArduino

Arduino core extensions for FPGA-based 32-bit MIPS and RISC-V 
soft CPU cores (f32c).

We have preliminary Boards Manager support.
In File->Preferences->Additional Boards Manager URLs enter:

    http://www.nxlab.fer.hr/fpgarduino/package_f32c_core_index.json

Select pull down menu Tools->Board->Board Manager
and instal FPGArduino (cca 100MB).

Upgrading

Before upgrading to new version, boards manager cache has to be
manually removed by deleting hidden directory 'packages'.
If install is attempted with old cache, it will fail with CRC error.

    windows cache: %appdata%/Arduino15/staging/packages (use Find files)
    linux cache: ~/.arduino15/staging/packages
    apple cache: ~/Library/Arduino15/staging/packages

Windows: %appdata%/Arduino15 (open File Explorer, enter %appdata% 
into the Address Bar and press Enter)

Delete packages directory, then remove/install from boards manager.

To upload some bitstreams OpenOCD may complain about
libusb drivers. libusb is common on linux.
To install libusb on windows, easiest way is with
Zadig http://zadig.akeo.ie Options->List All Devices
Select usb serial device, select driver WinUSB and click install.
Zadig is not needed for ftdi based boards with own uploader like 
ULX2S or FleaFPGA. Zadig can have some issues with ftdi drivers for 
windows 10.

Manual install: Copy or symlink this to existing arduino directory 
(versions 1.6.0 and newer), close the Arduino IDE, open
it again and new FPGA boards, progammers
and examples should appear under pull down menus:

tools->boards->...new mips and riscv boards..

tools->programmers->...ujprog openocd etc...

file->examples->f32c->...

file->examples->...RCswitch...Adafruit GFX/SSD1306..

We had to do small modifications of original examples
for AVR-specific or hardware-dependent stuff in order 
to get them running out of the box on f32c.

Source tree arduino/hardware/fpga contains complete 
arduino core for unified MIPS and RISC-V 
achitecture support. Only this directory is needed to be
copied to your existing arduino-1.6.x tree. Core can work
alone without libraries and examples.

The core comes without GCC for MIPS/RISC-V and tools to
upload binary and FPGA bitstreams (ujprog, openocd, JTAGs).

Precompiled binaries for linux and windows with 
GCC MIPS and RISC-V can be fetched and installed
separately, see http://www.nxlab.fer.hr/fpgarduino for 
further instructions.

There are some short scripts and patches to
build GCC from source in 
https://github.com/f32c/f32c/tree/master/src/compiler

# What should work

Blink led :)

Serial (over usb-ttl adapter, some boards need it external)

Timer (millis(), micros() - 32-bit CPU core clock counter, glitch-free, good for realtime)

GPIO (digitalWrite(), digitalRead())

Interrupts (MIPS only, attachInterrupt() gpio rising/falling edge, timer)

PWM (analogWrite(), analogWriteResolution(), analogWriteFrequency())

Fade (PWM) works on 2 output pins (LEDs).

Software SPI (bitbang, Adafruit OLED library)

Hardware SPI (SD card library)

Software I2C in master mode (SoftwareWire library)

OLED displays SSD1306 compatible (Adafruit SSD1306 library, SPI and I2C)

PID (Proportional-Integral-Derivative controller,
fast response, hardware math accellerated,
tested on high speed DC motors with encoders)

433.92 MHz transmitter (RCswitch library, 
Home automation, Remote relays, 
Garage doors).

FM RDS transmitter 87-108 MHz (RDS message displayed on
radio, but PCM sound supported only on ULX2S)

RHT11 Temperature/Humitidy sensors have been reported
to work.

SRAM in 8-bit mode (FleaFPGA Uno) and 16-bit mode (ULX2S)

SDRAM in 16-bit mode (Altera DE0 nano and Scarab MiniSpartan6+)

# What works only on some supported boards

VGA/HDMI/DVI bitmap needs large RAM (SRAM or SDRAM
ULX2S, FleaFPGA Uno, Scarab Minisprtan6+, Altera DE0 nano).
However some test picture and reduced bitmap can be produced 
with BRAM-only boards. It can run asynchronous from CPU
clock. A new textmode is implemented to FleaFPGA and 
under construction for other boards. 

TV Framebuffer with composite video output displaying SRAM content
tested only on ULX2S, should work on any other capable
of 81.25 MHz CPU and 325 MHz pixel clock. Composite video
currently must run synchronous with CPU clock.

DMA works only on boards that support SRAM or SDRAM.

PCM sound depends on DMA.
PCM outputs PWM for headphones and FM for
reception on 87-108 MHz radio.

Text-to-Speech library TTS depends on PCM.
TTS library could be converted to use a simple 
tongenerator then it will not depend on PCM)

DCF77 transmitter depends on PCM.
A proof of concept to adjust longwave RF clocks.

Analog inputs work on FleaFPGA Uno board thanx
for contribution.

Pullup digital input control works on FleaFPGA Uno
board (it would work on any board but 2 hardware
pins must be dedicated for 1 GPIO pin, one of them
having pullup resistor)

# What doesn't work yet

Hardware I2C slave is not planned soon, as f32c is 
expected to be an I2C master in most cases.

There's SoftwareWire I2C master which works
because F32C is fast enough and has high
resolution 32-bit timer.

DDR memory is currently not supported. 
This includes all types, DDR, DDR2, DDR3.
Support for it should be possible because
FPGA vendors are providing some proprietary
solutions for a range of selected memory chips.
