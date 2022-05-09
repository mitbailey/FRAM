/**
 * @file fram.c
 * @author Mit Bailey (mitbailey99@gmail.com)
 * @brief 
 * @version See Git tags for version information.
 * @date 2022.05.09
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "spibus.h"
#include "fram.h"
#include "meb_print.h"

int8_t fram_init(spibus *dev)
{
    // Device initialization.
    spibus device[1] = {0};
    device->bits = 8;
    device->bus = 0;
    device->cs = 0;
    device->cs_gpio = -1;
    device->cs_internal = 1;
    device->internal_rotation = false;
    device->lsb = 0;
    device->mode = SPI_MODE_0;
    device->speed = 4000000;
    device->sleeplen = 0;

    if (spibus_init(device) < 0)
    {
        dbprintlf(FATAL "Error initializing.");
        return -1;
    }

    return 0;
}

int8_t fram_enable_write(spibus *dev)
{
    uint8_t cmd = OPCODE_WREN;
    return spibus_xfer(dev, cmd, 1);
}

int8_t fram_disable_write(spibus *dev)
{
    uint8_t cmd = OPCODE_WRDI;
    return spibus_xfer(dev, cmd, 1);
}

uint8_t fram_read_status(spibus *dev)
{
    uint8_t status = OPCODE_RDSR;

    if (spibus_xfer(dev, &status, 1) < 0)
    {
        bprintlf(RED_FG "Failed to perform SPI bus transfer to obtain device status.");
        return -1;
    }

    return 0;
}

int8_t fram_write_status(spibus *dev, uint8_t data)
{
    uint8_t buf[2] = {0};
    buf[0] = OPCODE_WRSR;
    buf[1] = data; // <-- 8-bits of data to write.
    return spibus_xfer(dev, buf, 2);
}

// uint8_t fram_read(spibus *dev, uint16_t address)
// {
//     uint8_t buf[3] = {0};
//     buf[0] = OPCODE_READ;
    
// }

int8_t fram_read_id(spibus *dev, uint8_t *out_buf, ssize_t buf_len)
{
    if (buf_len != 4)
    {
        bprintlf(RED_FG "ERROR: Buffer length must be 4 bytes!");
        return -1;
    }

    // Read ID
    out_buf[0] = OPCODE_RDID;

    if (spibus_xfer(dev, out_buf, buf_len) < 0)
    {
        bprintlf(RED_FG "Failed to perform SPI bus transfer to obtain device ID.");
        return -1;
    }

    return 0;
}