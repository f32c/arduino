# FPGArduino

Arduino extensions for FPGA-based 32-bit MIPS and RISC-V 
soft CPU cores (f32c).

Copy or symlink this to existing arduino directory 
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

This repository doesn't contain required compiler toolchains
for mips or risc-v, so they have to be fetched and installed
separately, see http://www.nxlab.fer.hr/fpgarduino for further
instructions.

# What should work

Blink led :)

Serial (over usb-ttl adapter, some boards need it external)

Timer (millis(); 32-bit CPU core clock counter, glitch-free, good for realtime)

GPIO (digitalWrite(), digitalRead())

Interrupts (attachInterrupt() gpio rising/falling edge, timer)

PWM (analogWrite(), analogWriteResolution(), analogWriteFrequency())

Fade should almost always work (may fade bug be with you :-)

# What doesn't yet work

Analog inputs (cheap FPGA used to not to have ADC, 
please contribute some code for new hardware...)
