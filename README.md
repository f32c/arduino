# arduino

Arduino support for fpga-based 32-bit mips and risc-v softcores.

Copy or symlink this to existing arduino directory 
(versions 1.6.0 and newer), close arduino IDE, open
it again and new based boards, progammers
and examples should appear under pull down menus
tools->boards->...new mips and riscv boards..
tools->programmers->...ujprog openocd etc...
file->examples->f32c->...RCswitch..Adafruid GFX..

We had to do small modifications of original examples
for AVR-specific or hardware-dependent stuff in order 
to get them running out of the box on f32c.
