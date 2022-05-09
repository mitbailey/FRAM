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
    dev->bits = 8;
    dev->bus = 0;
    dev->cs = 0;
    dev->cs_gpio = -1;
    dev->cs_internal = 1;
    dev->internal_rotation = false;
    dev->lsb = 0;
    dev->mode = SPI_MODE_0;
    dev->speed = 4000000;
    dev->sleeplen = 0;

    if (spibus_init(dev) < 0)
    {
        dbprintlf(FATAL "Error initializing.");
        return -1;
    }

    return 0;
}

int8_t fram_enable_write(spibus *dev)
{
    uint8_t cmd = OPCODE_WREN;
    return spibus_xfer(dev, &cmd, 1);
}

int8_t fram_disable_write(spibus *dev)
{
    uint8_t cmd = OPCODE_WRDI;
    return spibus_xfer(dev, &cmd, 1);
}

uint8_t fram_read_status(spibus *dev)
{
    uint8_t buf[2] = {0};
    buf[0] = OPCODE_RDSR;
    if (spibus_xfer_full(dev, buf, buf, 2) < 0)
    {
        bprintlf(RED_FG "Failed to perform SPI bus transfer to obtain device status.");
        return 0xFF;
    }

    return buf[1];
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

int8_t fram_read_id(spibus *dev, uint32_t *id)
{
    uint8_t buf[5] = {0};
    buf[0] = OPCODE_RDID;
    if (spibus_xfer_full(dev, buf, buf, 5))
    {
        bprintlf(RED_FG "Failed to perform SPI bus transfer to obtain FRAM ID.");
        return -1;
    }
    id = (uint32_t *) buf[1];

    return 0;
}