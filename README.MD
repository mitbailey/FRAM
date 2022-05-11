# SPI Userspace Driver for Fujitsu FRAM Devices
This is a userspace driver for Fujitsu FRAM storage devices on the SPI interface.
SPI backend is provided by the [spibus](https://github.com/sunipkm/spibus.git) library.<br>
<br>
Depending on the `SPIBUS_CS_GPIO` flag that enables the `spibus` library to use GPIO pins as
chip selects, the [gpiodev](https://github.com/sunipkm/gpiodev.git) may be needed alongside
for compilation.

### Building and Testing
In the root of the cloned repository, execute `make` to build and run the example.
The example assumes the device is on SPI bus 0, chip select 0 (`/dev/spidev0.0`).<br>

In case that is not true, edit the `fujitsu_fram_init()` call in `example/main.c` to select the appropriate bus and chip select.<br>

The program is compiled and run automatically. In case of permission errors, manually execute the built binary `fram_test.out` with super-user privilege:

        $ sudo ./fram_test.out

### Basic Usage
An instance of a Fujitsu FRAM device can be initialized:

        fujitsu_fram dev; // Fujitsu FRAM device instance
        int ret = fujitsu_fram_init(&dev, 0, 0, -1, 1000000); // Initialize the instance at Bus 0, chip select 0, without GPIO chip select, at speed 1 MHz.

The init function returns negative on error.

Writes can be enabled on the device using:

        fujitsu_fram_write_enable(&dev, true);

Bytes can be written and read to and from the device:

        uint32_t data = 0xdeadbeef;
        fujitsu_fram_write(&dev, 0xa, (uint8_t *) &data, sizeof(data)); // write 4 bytes starting at address 0xa.
        fujitsu_fram_read(&dev, 0xa, (uint8_t *) &data, sizeof(data)); // read 4 bytes starting at address 0xa.

Refer to `example/main.c` for a more complete example.