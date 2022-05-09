/**
 * @file fram.h
 * @author Mit Bailey (mitbailey99@gmail.com)
 * @brief 
 * @version See Git tags for version information.
 * @date 2022.05.09
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <unistd.h>
#include "spibus.h"

/* OP-CODES
 * "MB85RS64V accepts 7 kinds of command specified in op-code. Op-code is a code composed of 8 bits shown in the table
 * below. Do not input invalid codes other than those codes. If CS is risen while inputting op-code, the command are 
 * not performed," (Fujitsu, pg.6).
 */

/**
 * @brief Set Write Enable Latch (enable writing)
 * 
 * The WREN command sets WEL (Write Enable Latch). WEL shall be set with the WREN command before writing operation
 * (WRSR command and WRITE command).
 * 
 */
#define OPCODE_WREN 0b00000110

/**
 * @brief Reset Write Enable Latch (disable writing)
 * 
 * The WRDI command resets WEL (Write Enable Latch). Writing operation (WRITE command and WRSR command) are not
 * performed when WEL is reset.
 * 
 */
#define OPCODE_WRDI 0b00000100

/**
 * @brief Read Status Register
 * 
 * The RDSR command reads status register data. After op-code of RDSR is input to SI, 8-cycle clock is input
 * to SCK. The SI value is invalid during this time. SO is output synchronously to a falling edge of SCK. In the
 * RDSR command, repeated reading of status register is enabled by sending SCK continuously before rising
 * of CS.
 * 
 */
#define OPCODE_RDSR 0b00000101

/**
 * @brief Write Status Register
 * 
 * The WRSR command writes data to the nonvolatile memory bit of status register. After performing WRSR
 * op-code to a SI pin, 8 bits writing data is input. WEL (Write Enable Latch) is not able to be written with WRSR
 * command. A SI value correspondent to bit 1 is ignored. Bit 0 of the status register is fixed to “0” and cannot
 * be written. The SI value corresponding to bit 0 is ignored. The WPsignal level shall be fixed before performing
 * the WRSR command, and do not change the WP signal level until the end of command sequence.
 * 
 */
#define OPCODE_WRSR 0b00000001

/**
 * @brief Read Memory Code
 * 
 * The READ command reads FRAM memory cell array data. Arbitrary 16 bits address and op-code of READ
 * are input to SI. The 3-bit upper address bit is invalid. Then, 8-cycle clock is input to SCK. SO is output
 * synchronously to the falling edge of SCK. While reading, the SI value is invalid. When CS is risen, the READ
 * command is completed, but keeps on reading with automatic address increment which is enabled by continuously sending
 * clocks to SCK in unit of 8 cycles before CS rising. When it reaches the most significant address, it rolls over 
 * to the starting address, and reading cycle keeps on infinitely.
 * 
 */
#define OPCODE_READ 0b00000011

/**
 * @brief Write Memory Code
 * 
 * The WRITE command writes data to FRAM memory cell array. WRITE op-code, arbitrary 16 bits of address
 * and 8 bits of writing data are input to SI. The 3-bit upper address bit is invalid. When 8 bits of writing data is
 * input, data is written to FRAM memory cell array. Risen CS will terminate the WRITE command. However,
 * if you continue sending the writing data for 8 bits each before CS rising, it is possible to continue writing with
 * automatic address increment. When it reaches the most significant address, it rolls over to the starting
 * address, and writing cycle keeps on infinitely.
 * 
 */
#define OPCODE_WRITE 0b00000010

/**
 * @brief Read Device ID
 * 
 * The RDID command reads fixed Device ID. After performing RDID op-code to SI, 32-cycle clock is input to
 * SCK. The SI value is invalid during this time. SO is output synchronously to a falling edge of SCK. The output
 * is in order of Manufacturer ID (8bit)/Continuation code (8bit)/Product ID (1st Byte)/ Product ID (2nd Byte).
 * In the RDID command, SO holds the output state of the last bit in 32-bit Device ID until CS is risen.
 * 
 */
#define OPCODE_RDID 0b10011111

/**
 * @brief Initializes the FRAM.
 * 
 * @param dev FRAM device.
 * @return int8_t 0 on success, negative on failure.
 */
int8_t fram_init(spibus *dev);

/**
 * @brief Enables writing to FRAM memory.
 * 
 * @param dev FRAM device.
 * @return int8_t 0 on success, negative on failure.
 */
int8_t fram_enable_write(spibus *dev);

/**
 * @brief 
 * 
 * @param dev FRAM device.
 * @return int8_t 0 on success, negative on failure.
 */
int8_t fram_disable_write(spibus *dev);

/**
 * @brief Read the FRAM's status.
 * 
 * @param dev FRAM device.
 * @return uint8_t The FRAM's 8-bit status, or 0xFF on failure.
 */
uint8_t fram_read_status(spibus *dev);

/**
 * @brief Writes to the FRAM's writable status bits.
 * 
 * Bit no. | Bit Name | Writability
 * --------------------------------
 * 7       | WPEN     | Writable
 * 6 - 4   | Not Used | Writable
 * 3       | BP1      | Writable
 * 2       | BP0      | Writable
 * 1       | WEL      | Unwritable (set with WREN)
 * 0       | 0        | Unwritable (fixed)
 * 
 * @param dev FRAM device.
 * @param data The new desired value of the status byte. 
 * @return int8_t Zero on success, negative on failure.
 */
int8_t fram_write_status(spibus *dev, uint8_t data);

uint8_t fram_read(spibus *dev, uint16_t address);

/**
 * @brief Read the FRAM's ID.
 * 
 * @param dev spibus-type device. 
 * @param id The output buffer to store the ID.
 * @return int8_t Zero on success, negative on failure.
 */
int8_t fram_read_id(spibus *dev, uint32_t *id);