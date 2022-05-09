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
#include <stdbool.h>
#include "spibus.h"

typedef spibus fujitsu_fram;
/**
 * @brief Initializes the FRAM.
 * 
 * @param dev FRAM device.
 * @return int8_t 0 on success, negative on failure.
 */
int fujitsu_fram_init(fujitsu_fram *dev);

/**
 * @brief Enables writing to FRAM memory.
 * 
 * @param dev FRAM device.
 * @return int8_t 0 on success, negative on failure.
 */
int fujitsu_fram_write_enable(fujitsu_fram *dev, bool en);

/**
 * @brief Read the FRAM's status.
 * 
 * @param dev FRAM device.
 * @return uint8_t The FRAM's 8-bit status, or 0xFF on failure.
 */
uint8_t fujitsu_fram_read_status(fujitsu_fram *dev);

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
int fujitsu_fram_write_status(fujitsu_fram *dev, uint8_t data);

int fujitsu_fram_read(fujitsu_fram *dev, uint16_t address, uint8_t *buf, ssize_t len);

int fujitsu_fram_write(fujitsu_fram *dev, uint16_t address, uint8_t *buf, ssize_t len);

/**
 * @brief Read the FRAM's ID.
 * 
 * @param dev spibus-type device. 
 * @param id The output buffer to store the ID.
 * @return int8_t Zero on success, negative on failure.
 */
int fujitsu_fram_read_id(fujitsu_fram *dev, uint32_t *id);