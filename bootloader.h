/********************************************************************************
 * 文件名  ：bootloader.h
 * 描述    ：bootloader 头文件    
 * 作者    ：泽畔无材  QQ:715805855
 *修改时间 ：2013-10-21
**********************************************************************************/
#ifndef __BOOTLOADER_H
#define __BOOTLOADER_H

#include "stm8s.h"
#include <stdio.h>

//device information
#define  BLOCK_BYTES          64
#define	 BLOCK_SHIFT		  6
#define  FLASH_START          0x008000
#define  FLASH_END            0x009FFF

//cmd code
#define BOOT_OK	0xa0
#define BOOT_ERR 0xa1

#define BOOT_HEAD 0xa5
#define BOOT_READ 0xa6
#define BOOT_WRITE 0xa7
#define BOOT_VERIFY 0xa8
#define BOOT_GO 0xa9

#define INIT_PAGE 0x09

//uart parameter
#define BAUDRATE    115200
#define BaudRate_Mantissa       ((uint32_t)SYSCLK / (BAUDRATE << 4))
#define BaudRate_Mantissa100    (((uint32_t)SYSCLK * 100) / (BAUDRATE << 4))

typedef unsigned char     uint8_t;
typedef unsigned short    uint16_t;
typedef unsigned long     uint32_t;

typedef uint32_t  u32;
typedef uint16_t u16;
typedef uint8_t  u8;

void Set_HSI(void);
void InitUART1(u32 baud);
void UART1_SendB(u8 ch);
u8 UART1_RcvB(void);
IN_RAM(void FLASH_ProgBlock(uint8_t * addr, uint8_t *Buffer));


#endif
