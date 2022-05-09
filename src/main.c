/**
 * @file main.c
 * @author Mit Bailey (mitbailey99@gmail.com)
 * @brief 
 * @version See Git tags for version information.
 * @date 2022.05.09
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "spibus.h"
#include "meb_print.h"
#include "fram.h"

int main()
{
    spibus dev[1] = {0};
    uint8_t buf[4] = {0};
    ssize_t buf_len = 4;

    // Initialization test.
    if (fujitsu_fram_init(dev) < 0)
    {
        dbprintlf(FATAL "Device failed to initialize!");
        return -1;
    }

    // Device identification test.
    if (fujitsu_fram_read_id(dev, (uint32_t *) buf) < 0)
    {
        bprintlf(RED_FG "Failed to obtain device identification.");
    }
    else
    {
        bprintlf("Device ID: 0x%08X 0x%08X 0x%08X 0x%08X", buf[0], buf[1], buf[2], buf[3]);

        if (buf[0] != 0b00000100)
        {
            bprintlf(RED_FG "Failed to retrieve the expected manufacturer ID.");
        }
    }

    // Device read status test.
    uint8_t dev_status = fujitsu_fram_read_status(dev);
    if (dev_status == 0xFF)
    {
        bprintlf(RED_FG "Failed to obtain device status.");
    }
    else
    {
        bprintlf("Device status: 0x%08X", dev_status);
    }

    uint32_t data = 0xdeadbeef;
    fujitsu_fram_write_enable(dev, true);
    fujitsu_fram_write(dev, 0x7f, &data, sizeof(data));

    uint32_t idata = 0x0;
    fujitsu_fram_read(dev, 0x7f, &idata, sizeof(idata));

    bprintlf("Read: 0x%x", idata);

    spibus_destroy(dev);

    return 0;
}