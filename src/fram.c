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

#include <string.h>
#include <stdlib.h>

#include "spibus.h"
#include "fram.h"
#include "meb_print.h"

typedef enum
{
    FRAM_WREN = 0b00000110,
    FRAM_WRDI = 0b00000100,
    FRAM_RDSR = 0b00000101,
    FRAM_WRSR = 0b00000001,
    FRAM_READ = 0b00000011,
    FRAM_WRITE = 0b00000010,
    FRAM_RDID = 0b10011111
} FUJITSU_OPCODE;

int fujitsu_fram_init(fujitsu_fram *dev)
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
    dev->speed = 1000000;
    dev->sleeplen = 0;

    int retval = spibus_init((fujitsu_fram *) dev);
    if (retval < 0)
    {
        dbprintlf(FATAL "Error initializing (%d).", retval);
        return -1;
    }

    return 0;
}

int fujitsu_fram_enable_write(fujitsu_fram *dev, bool en)
{
    uint8_t cmd = en ? FRAM_WREN : FRAM_WRDI;
    return spibus_xfer(dev, &cmd, 1);
}

uint8_t fujitsu_fram_read_status(fujitsu_fram *dev)
{
    uint8_t buf[2] = {0};
    buf[0] = FRAM_RDSR;
    int retval = spibus_xfer_full(dev, buf, buf, 2);
    if (retval < 0)
    {
        bprintlf(RED_FG "Failed to perform SPI bus transfer to obtain device status (%d).", retval);
        return 0xFF;
    }

    return buf[1];
}

int fujitsu_fram_write_status(fujitsu_fram *dev, uint8_t data)
{
    uint8_t buf[2] = {0};
    buf[0] = FRAM_WRSR;
    buf[1] = data; // <-- 8-bits of data to write.
    return spibus_xfer(dev, buf, 2);
}

// uint8_t fram_read(spibus *dev, uint16_t address)
// {
//     uint8_t buf[3] = {0};
//     buf[0] = OPCODE_READ;
    
// }

int fujitsu_fram_read_id(fujitsu_fram *dev, uint32_t *id)
{
    uint8_t buf[5] = {0};
    buf[0] = FRAM_RDID;
    int retval = spibus_xfer_full(dev, buf, buf, 5);
    if (retval < 0)
    {
        bprintlf(RED_FG "Failed to perform SPI bus transfer to obtain FRAM ID (%d).", retval);
        return -1;
    }
    *id = *((uint32_t *)(buf + 1));

    return 0;
}

int fujitsu_fram_write(fujitsu_fram *dev, uint16_t address, uint8_t *buf, ssize_t len)
{
    if (len <= 0)
    {
        return -2;
    }
    if (buf == NULL)
    {
        return -4;
    }
    uint8_t *obuf = NULL;
    obuf = (uint8_t *) malloc(len + 3);
    if (obuf == NULL)
    {
        return -3;
    }
    obuf[0] = FRAM_WRITE;
    obuf[1] = (address & 0xff00) >> 1;
    obuf[2] = (address & 0xff);
    memcpy(obuf + 3, buf, len);
    int ret = spibus_xfer(dev, obuf, len + 3);
    free(obuf);
    return ret;
}

int fujitsu_fram_read(fujitsu_fram *dev, uint16_t address, uint8_t *buf, ssize_t len)
{
    if (len <= 0)
    {
        return -2;
    }
    if (buf == NULL)
    {
        return -4;
    }
    uint8_t *obuf = NULL;
    uint8_t *ibuf = NULL;
    obuf = (uint8_t *) malloc(len + 3);
    ibuf = (uint8_t *) malloc(len + 3);
    if (obuf == NULL || ibuf == NULL)
    {
        return -3;
    }
    obuf[0] = FRAM_READ;
    obuf[1] = (address & 0xff00) >> 1;
    obuf[2] = (address & 0xff);
    int ret = spibus_xfer_full(dev, ibuf, obuf, len + 3);
    free(obuf);
    memcpy(ibuf + 3, buf, len);
    free(ibuf);
    return ret;
}