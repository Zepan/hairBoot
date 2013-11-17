/********************************************************************************
 * 文件名  ：bootloader.c
 * 描述    ：bootloader     
 * 作者    ：泽畔无材  QQ:715805855
 *修改时间 ：2013-10-21
**********************************************************************************/

#include "bootloader.h"

int main(void)
{
    u16 tryCnt = 65535;
    u8 ch, page;
    u8 i = 10;  //0.5s
    u8 buf[BLOCK_BYTES];
    u8 verify;
    u8* addr;
    //set hsi = 16M,复位后默认hsi
    CLK->CKDIVR &= (uint8_t)(~CLK_CKDIVR_HSIDIV); //清空(00)即不分频
    while(!(CLK->ICKR & 0x02));   //等待内部时钟稳定
    //set baudrate = 115200, 复位后默认开启uart，格式默认，只需设置波特率及使能收发
    UART1->BRR2 |= (uint8_t)((uint8_t)(((BaudRate_Mantissa100 - (BaudRate_Mantissa * 100)) << 4) / 100) & (uint8_t)0x0F) |\
                   (uint8_t)((BaudRate_Mantissa >> 4) & (uint8_t)0xF0);  
    UART1->BRR1 |= (uint8_t)BaudRate_Mantissa; 
    UART1->CR2 |= (uint8_t)(UART1_CR2_TEN | UART1_CR2_REN);  //使能收发    
    //unlock flash,解锁flash
    FLASH->PUKR = FLASH_RASS_KEY1;
    FLASH->PUKR = FLASH_RASS_KEY2;
    //bootloader通信过程
    while(i)    
    {
        if(UART1->SR & (u8)UART1_FLAG_RXNE)    //wait for head 
        {
            ch = (uint8_t)UART1->DR;    
            if(ch == BOOT_HEAD) break;
        }
        tryCnt--;
        if(tryCnt == 0) i--;
    }
    
    if(i == 0)
    {    //goto app
        goto goApp;
    }
    else
    {
        UART1_SendB(0xa0|INIT_PAGE);    
        while(1)
        {
            ch = UART1_RcvB();
            switch(ch)
            {
            case BOOT_GO:
                goApp:
                FLASH->IAPSR &= FLASH_MEMTYPE_PROG; //锁住flash
                //goto app
                asm("JP $8240");
                break;
            case BOOT_WRITE:
                page = UART1_RcvB();
                addr = (u8*)(FLASH_START + (page << BLOCK_SHIFT));
                verify = 0;
                for(i = 0; i < BLOCK_BYTES; i++)   
                {
                    buf[i] = UART1_RcvB();
                    verify += buf[i];
                }
                if(verify == UART1_RcvB())  //通信校验成功
                {
                    FLASH_ProgBlock(addr, buf);
                    //verify flash 
                    for(i = 0; i < BLOCK_BYTES; i++)   
                    {
                        verify -= addr[i];
                    }
                    if(verify == 0)  //写入校验成功
                    {
                        UART1_SendB(BOOT_OK);
                        break;
                    }
                    //else,写入校验失败，可能是flash损坏
                }
            default: //上面校验失败的情况也会到这里来
                UART1_SendB(BOOT_ERR);
                break;
            }
        }
    }
}

void UART1_SendB(u8 ch)
{
    while (!(UART1->SR & (u8)UART1_FLAG_TXE));
    UART1->DR = ch;    
}

u8 UART1_RcvB(void)
{
     while(!(UART1->SR & (u8)UART1_FLAG_RXNE));
     return ((uint8_t)UART1->DR);
}

//addr must at begin of block
IN_RAM(void FLASH_ProgBlock(uint8_t * addr, uint8_t *Buffer))
{
    u8 i;
    /* Standard programming mode */ /*No need in standard mode */
    FLASH->CR2 |= FLASH_CR2_PRG;
    FLASH->NCR2 &= (uint8_t)(~FLASH_NCR2_NPRG);
    /* Copy data bytes from RAM to FLASH memory */
    for (i = 0; i < FLASH_BLOCK_SIZE; i++)
    {
        *((PointerAttr uint8_t*) (uint16_t)addr + i) = ((uint8_t)(Buffer[i]));    
    }
}
