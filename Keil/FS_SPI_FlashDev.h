/***********************************************************************/
/*  This file is part of the ARM Toolchain package                     */
/*  Copyright KEIL ELEKTRONIK GmbH 2003 - 2008                         */
/***********************************************************************/
/*                                                                     */
/*  FlashDev.H:  Device Description for Winbond 4MBit Serial Flash     */
/*                                            Memory (W25X40    )      */
/*                                                                     */
/* Note: Device fragmented to 64K virtual sectors for the FlashFS.     */
/***********************************************************************/

#define SPI_FLASH_DEVICE                                \
  DSB(0x10000, 0x000000),     /* Sector Size 64kB  0 */ \
  DSB(0x10000, 0x010000),     /* Sector Size 64kB  1 */ \
  DSB(0x10000, 0x020000),     /* Sector Size 64kB  2 */ \
  DSB(0x10000, 0x030000),     /* Sector Size 64kB  3 */ \
  DSB(0x10000, 0x040000),     /* Sector Size 64kB  4 */ \
  DSB(0x10000, 0x050000),     /* Sector Size 64kB  5 */ \
  DSB(0x10000, 0x060000),     /* Sector Size 64kB  6 */ \
  DSB(0x10000, 0x070000),     /* Sector Size 64kB  7 */ \

#define SF_NSECT    8
