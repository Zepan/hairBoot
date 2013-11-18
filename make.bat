iccstm8 bootloader.c -e -Oh --code_model  small --data_model medium --vregs 16
ilinkstm8 --config_def _CSTACK_SIZE=0x100 --config_def _HEAP_SIZE=0x100 --config lnkstm8s103f3.icf bootloader.o -o bootloader.out
ielftool --bin bootloader.out bootloader.bin
pause