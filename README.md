hairBoot
========

A tiny (<0.5KB) bootloader for STM8S  low-density family 

Usage
-----
1. Flash hairBoot to stm8s: use STVP and ST-LINK， burn the bin/hex/out to stm8s
2. Build a app project, get the app bin. 
   Don't forget change app's icf: `define region NearFuncCode = [from 0x8200 to 0x9FFF]`
3. Download app bin to stm8s: 
   write a PC program cooperate with hairBoot or 
   just use stm8 launcher~ (search in chrome apps)

Build
-----
1. Install IAR for stm8, add `xxx\IAR Systems\Embedded workbench 6.5\stm8\bin` to Environment var Path
2. Run make.bat to get bin, run out2hex to get hex
3. fix all interrupt vectors except the vecter 0.
   change vector n to 0x82008200 + 4*n, eg vector 1: 82008204
4. finished! you get the bin/hex/out now~

Related
-------
ST-Node: A library and application demos for ST-Node(tiny stm8s moudle)

https://github.com/Zepan/ST-Node.git
