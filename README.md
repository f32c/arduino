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

    windows cache: .../Arduino15/staging/packages (use Find files)
    linux cache: ~/.arduino15/staging/packages

Delete packages directory, then remove/install from boards manager.

To upload some bitstreams OpenOCD may complain about
libusb drivers. libusb is common on linux. 
To install libusb on windows, easiest way is with
Zadig http://zadig.akeo.ie Options->List All Devices
Select usb serial device, select driver WinUSB and click install.

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

Adafruit SSD1306 displays (both SPI and I2C)

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

# What partially works (WIP)

SDRAM works for data with d-cache. SDRAM works also for 
instructions without i-cache (slow).
Work is currently in progress to fix i-cache and 
make full functional bus arbiter for SDRAM.

VGA and HDMI outputs work but only as proof of concept
because BRAM content is displayed which is very small,
few KB typical. When multiport synchronous RAM bus access
from VGA/HDMI to SDRAM is ready then some better HDMI 
picture might be seen.

# What works only on ULX2S (with 1MB SRAM)

1MB SRAM using both 8-bit ports works at 81.25 MHz
CPU clock, widely tested.

TV Framebuffer with composite video output displaying SRAM content

PCM sound (depends on DMA).
PCM outputs PWM for headphones and FM for
reception on 87-108 MHz radio.

Text-to-Speech library TTS. (depends on PCM)
TTS library could be converted to use a simple 
tongenerator then it will not depend on PCM)

DCF77 transmitter (depends on PCM)
A proof of concept to adjust longwave RF clocks.

# What doesn't work yet

Analog inputs (cheap FPGA used to not to have ADC, 
please contribute some code for new hardware...)

Hardware I2C slave is not planned soon, as f32c is 
expected to be an I2C master in most cases.

There's SoftwareWire I2C master which works
because F32C is fast enough and has high
resolution 32-bit timer.
