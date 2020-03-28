#ifndef _EXT_FLASH_H
#define _EXT_FLASH_H

#include "main.h"

#define READ 0x03              // SPI Flash opcode: Read up up to 25MHz
#define ERASE_SECTOR 0x20      // SPI Flash opcode: sector erase
#define ERASE_BLOCK 0x52       // SPI Flash opcode: block erase
#define ERASE_ALL 0x60         // SPI Flash opcode: Entire chip erase
#define WRITE 0x02             // SPI Flash opcode: Write one byte
//#define WRITE_BYTE_STREAM 0xAF // SPI Flash opcode: Write continuous stream of bytes (AAI mode); available on SST25VF010A (but not on SST25VF016B)
#define RDSR 0x05              // SPI Flash opcode: Read Status Register
//#define EWSR 0x50              // SPI Flash opcode: Enable Write Status Register
#define WRSR 0x01              // SPI Flash opcode: Write Status Register
#define WREN 0x06              // SPI Flash opcode: Write Enable
#define WRDI 0x04              // SPI Flash opcode: Write Disable / End AAI mode
#define RDID 0x9F              // SPI Flash opcode: Read ID

#define BUSY 0x01 // Mask for Status Register BUSY (Internal Write Operaiton in Progress status) bit
#define WEL 0x02  // Mask for Status Register WEL (Write Enable status) bit
#define BP0 0x04  // Mask for Status Register BP0 (Block Protect 0) bit
#define BP1 0x08  // Mask for Status Register BP1 (Block Protect 1) bit
#define BP2 0x10  // Mask for Status Register BP1 (Block Protect 3) bit
//#define RES 0x10  // Reserved bit
//#define RES 0x20  // Reserved bit
#define AAI 0x40 // Mask for Status Register AAI (Auto Address Increment Programming status) bit
#define BPL 0x80 // Mask for Status Register BPL (BPx block protect bit read-only protect) bit

extern char UART_BufSrting[32];

void ExternalFlash_Init(void);
uint32_t ExternalFlash_ReadIdentification(void);
uint8_t ExternalFlash_ReadManufacturerID(void);
uint8_t ExternalFlash_ReadStatusRegister(void);
uint8_t ExternalFlash_Read(uint32_t *pAddr);
void ExternalFlash_WriteByte(uint8_t *pData, uint32_t *pAddr);
void ExternalFlash_EraseSector(uint32_t *pAddr);
void ExternalFlash_WriteStatusRegister(uint8_t Value);

#endif