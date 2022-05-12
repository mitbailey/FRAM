/**
 * @file fram.c
 * @author Sunip K. Mukherjee (sunipkmukherjee@gmail.com)
 * @brief 
 * @version 1.0
 * @date 2022-05-10
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include <string.h>
#include <stdlib.h>

#include "spibus.h"
#include "fram.h"

enum FUJITSU_OPCODE
{
    FRAM_WREN = 0b00000110,
    FRAM_WRDI = 0b00000100,
    FRAM_RDSR = 0b00000101,
    FRAM_WRSR = 0b00000001,
    FRAM_READ = 0b00000011,
    FRAM_WRITE = 0b00000010,
    FRAM_RDID = 0b10011111
};

int fujitsu_fram_init(fujitsu_fram *dev, int bus, int cs, int cs_gpio, uint32_t speed)
{
    int retval;
    uint32_t id;
    if (dev == NULL || dev->bus == NULL)
    {
        return -5;
    }
    // Device initialization.
    dev->bus->bits = 8;
    dev->bus->bus = bus;
    dev->bus->cs = cs;
    dev->bus->cs_gpio = cs_gpio;
    dev->bus->cs_internal = cs_gpio > 0 ? 0 : CS_INTERNAL;
    dev->bus->internal_rotation = false;
    dev->bus->lsb = 0;
    dev->bus->mode = SPI_MODE_0;
    dev->bus->speed = speed > 10000000 ? 10000000 : speed;
    dev->bus->sleeplen = 0;

    dev->len_ofst = 3;

    retval = spibus_init(dev->bus);

    if (retval < 0)
    {
        return -1;
    }

    if ((retval = fujitsu_fram_read_id(dev, &id)) < 0)
    {
        return -6;
    }

    if (((id >> 16) & 0xff) > 3) // 24 bit addressing required
    {
        dev->len_ofst = 4;
    }

    dev->id = id;

    return 1;
}

void fujitsu_fram_destroy(fujitsu_fram *dev)
{
    if (dev == NULL || dev->bus == NULL)
    {
        return;
    }
    spibus_destroy(dev->bus);
}

int fujitsu_fram_write_enable(fujitsu_fram *dev, bool en)
{
    uint8_t cmd = en ? FRAM_WREN : FRAM_WRDI;
    if (dev == NULL || dev->bus == NULL)
    {
        return -5;
    }
    return spibus_xfer(dev->bus, &cmd, 1);
}

uint8_t fujitsu_fram_read_status(fujitsu_fram *dev)
{
    uint8_t buf[2] = {0};
    int retval = 0;
    if (dev == NULL || dev->bus == NULL)
    {
        return -5;
    }
    buf[0] = FRAM_RDSR;
    retval = spibus_xfer_full(dev->bus, buf, buf, 2);
    if (retval < 0)
    {
        buf[1] = 0xff;
    }
    return buf[1];
}

int fujitsu_fram_write_status(fujitsu_fram *dev, uint8_t data)
{
    uint8_t buf[2] = {0};
    if (dev == NULL || dev->bus == NULL)
    {
        return -5;
    }
    buf[0] = FRAM_WRSR;
    buf[1] = data; // <-- 8-bits of data to write.
    return spibus_xfer(dev->bus, buf, 2);
}

int fujitsu_fram_read_id(fujitsu_fram *dev, uint32_t *id)
{
    uint8_t buf[5] = {0};
    int retval = 0;
    if (dev == NULL || dev->bus == NULL)
    {
        return -5;
    }
    if (id == NULL)
    {
        return -4;
    }
    buf[0] = FRAM_RDID;
    retval = spibus_xfer_full(dev->bus, buf, buf, 5);
    if (retval < 0)
    {
        return -1;
    }

    *id = *((uint32_t *)(buf + 1));

    return 1;
}

int fujitsu_fram_write(fujitsu_fram *dev, uint32_t address, uint8_t *buf, size_t len)
{
    int len_ofst = 0, ret = 0;
    uint8_t *obuf = NULL;
    if (dev == NULL || dev->bus == NULL)
    {
        return -5;
    }
    if (len <= 0)
    {
        return -2;
    }
    if (buf == NULL)
    {
        return -4;
    }
    len_ofst = dev->len_ofst;
    obuf = (uint8_t *)malloc(len + len_ofst);
    if (obuf == NULL)
    {
        return -3;
    }
    obuf[0] = FRAM_WRITE;
    for (int i = len_ofst - 1; i > 0; i--)
    {
        obuf[len_ofst - i] = address >> ((i - 1) * 8);
    }
    memcpy(obuf + len_ofst, buf, len);
    ret = spibus_xfer(dev->bus, obuf, len + len_ofst);
    free(obuf);
    return ret;
}

int fujitsu_fram_read(fujitsu_fram *dev, uint32_t address, uint8_t *buf, size_t len)
{
    int len_ofst = 0, ret = 0;
    uint8_t *obuf = NULL, *ibuf = NULL;
    if (dev == NULL || dev->bus == NULL)
    {
        return -5;
    }
    if (len <= 0)
    {
        return -2;
    }
    if (buf == NULL)
    {
        return -4;
    }
    len_ofst = dev->len_ofst;
    obuf = (uint8_t *)malloc(len + len_ofst);
    ibuf = (uint8_t *)malloc(len + len_ofst);
    if (obuf == NULL || ibuf == NULL)
    {
        return -3;
    }
    obuf[0] = FRAM_READ;
    for (int i = len_ofst - 1; i > 0; i--)
    {
        obuf[len_ofst - i] = address >> ((i - 1) * 8);
    }
    memcpy(obuf + len_ofst, buf, len);
    ret = spibus_xfer_full(dev->bus, ibuf, obuf, len + len_ofst);
    free(obuf);
    memcpy(buf, ibuf + len_ofst, len);
    free(ibuf);
    return ret;
}

int fujitsu_fram_get_capacity(fujitsu_fram *dev)
{
    if (dev == NULL || dev->bus == NULL)
    {
        return -5;
    }
    int cap = (dev->id >> 16) & 0xff;
    cap = (1 << cap) * 1024;
    return cap;
}