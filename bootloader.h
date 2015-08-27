/********************************************************************************
 * 文件名  ：bootloader.h
 * 描述    ：bootloader 头文件    
 * 作者    ：泽畔无材  zepanwucai@gmail.com
 *修改时间 ：2013-11-18
**********************************************************************************/
#ifndef __BOOTLOADER_H
#define __BOOTLOADER_H

#include "stm8s.h"
#include <stdio.h>

//device information
#define INIT_PAGE 0x08
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
#define BOOT_LOCK_UBC 0xaa

//uart parameter
#define SYSCLK 16000000
#define BAUDRATE    115200
#define BaudRate_Mantissa       ((uint32_t)SYSCLK / (BAUDRATE << 4))
#define BaudRate_Mantissa100    (((uint32_t)SYSCLK * 100) / (BAUDRATE << 4))

//reg definition
#define FLASH_RASS_KEY1 ((uint8_t)0x56) /*!< First RASS key */
#define FLASH_RASS_KEY2 ((uint8_t)0xAE) /*!< Second RASS key */
typedef enum { UART1_FLAG_TXE   = (uint16_t)0x0080, /*!< Transmit Data Register Empty flag */
               UART1_FLAG_TC    = (uint16_t)0x0040, /*!< Transmission Complete flag */
               UART1_FLAG_RXNE  = (uint16_t)0x0020, /*!< Read Data Register Not Empty flag */
               UART1_FLAG_IDLE  = (uint16_t)0x0010, /*!< Idle line detected flag */
               UART1_FLAG_OR    = (uint16_t)0x0008, /*!< OverRun error flag */
               UART1_FLAG_NF    = (uint16_t)0x0004, /*!< Noise error flag */
               UART1_FLAG_FE    = (uint16_t)0x0002, /*!< Framing Error flag */
               UART1_FLAG_PE    = (uint16_t)0x0001, /*!< Parity Error flag */
               UART1_FLAG_LBDF  = (uint16_t)0x0210, /*!< Line Break Detection Flag */
               UART1_FLAG_SBK   = (uint16_t)0x0101  /*!< Send Break characters Flag */
             } UART1_Flag_TypeDef;
typedef enum {
    FLASH_MEMTYPE_PROG      = (uint8_t)0xFD, /*!< Program memory */
    FLASH_MEMTYPE_DATA      = (uint8_t)0xF7  /*!< Data EEPROM memory */
} FLASH_MemType_TypeDef;

void Set_HSI(void);
void InitUART1(u32 baud);
void UART1_SendB(u8 ch);
u8 UART1_RcvB(void);
IN_RAM(void FLASH_ProgBlock(uint8_t * addr, uint8_t *Buffer));


#endif
