/**
 * @file fram.h
 * @author Sunip K. Mukherjee (sunipkmukherjee@gmail.com)
 * @brief 
 * @version 1.0
 * @date 2022-05-10
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <unistd.h>
#include <stdbool.h>
#include "spibus.h"

/**
 * @brief Fujitsu FRAM device struct.
 * 
 */
typedef struct
{
    spibus bus[1];
    int len_ofst;
}
fujitsu_fram;

/**
 * @brief Initializes the FRAM.
 * 
 * @param dev fujitsu_fram device pointer.
 * @param bus SPI bus number (X in /dev/spidevX.Y).
 * @param cs SPI chip select number (Y in /dev/spidevX.Y).
 * @param cs_gpio GPIO chip select pin (set to -1 to disable, set to gpiodev pin number and SPIBUS_CS_GPIO to 1.).
 * @param speed SPI Bus speed (10 MHz max).
 * @return int Positive on success, negative on failure.
 */
int fujitsu_fram_init(fujitsu_fram *dev, int bus, int cs, int cs_gpio, uint32_t speed);

/**
 * @brief Close connection to the FRAM device.
 * 
 * @param dev fujitsu_fram device pointer.
 */
void fujitsu_fram_destroy(fujitsu_fram *dev);

/**
 * @brief Enable/disable writing to the FRAM.
 * 
 * @param dev fujitsu_fram device pointer.
 * @param en Enable/disable writing.
 * @return int Positive on success, negative on failure.
 */
int fujitsu_fram_write_enable(fujitsu_fram *dev, bool en);

/**
 * @brief Read the FRAM's status.
 * 
 * @param dev fujitsu_fram device pointer.
 * @return uint8_t The FRAM's 8-bit status, or 0xFF on failure.
 */
uint8_t fujitsu_fram_read_status(fujitsu_fram *dev);

/**
 * @brief Writes to the FRAM's writable status bits.
 * 
 * Bit no. | Bit Name | Writability
 * ------- | -------- | -----------
 * 7       | WPEN     | Writable
 * 6 - 4   | Not Used | Writable
 * 3       | BP1      | Writable
 * 2       | BP0      | Writable
 * 1       | WEL      | Unwritable (set with WREN)
 * 0       | 0        | Unwritable (fixed)
 * 
 * @param dev fujitsu_fram device pointer.
 * @param data The new desired value of the status byte. 
 * @return int Positive on success, negative on failure.
 */
int fujitsu_fram_write_status(fujitsu_fram *dev, uint8_t data);

/**
 * @brief Read bytes from the FRAM starting at the specified address with automatic rollover.
 * 
 * @param dev fujitsu_fram device pointer.
 * @param address 16/24 bit start address.
 * @param buf Data destination buffer.
 * @param len Length of data destination buffer.
 * @return int Positive on success, negative on failure .
 */
int fujitsu_fram_read(fujitsu_fram *dev, uint32_t address, uint8_t *buf, size_t len);

/**
 * @brief Write bytes to the FRAM starting at the specified address with automatic rollover.
 * 
 * @param dev fujitsu_fram device pointer.
 * @param address 16/24 bit start address.
 * @param buf Data source buffer.
 * @param len Length of data source buffer.
 * @return int Positive on success, negative on failure.
 */
int fujitsu_fram_write(fujitsu_fram *dev, uint32_t address, uint8_t *buf, size_t len);

/**
 * @brief Read the FRAM's ID.
 * 
 * @param dev fujitsu_fram device pointer.
 * @param id The output buffer to store the ID.
 * @return int Positive on success, negative on failure.
 */
int fujitsu_fram_read_id(fujitsu_fram *dev, uint32_t *id);