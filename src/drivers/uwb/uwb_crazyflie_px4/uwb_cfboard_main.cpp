/**
 * @file uwb_cfboard_main.cpp
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2023-01-29
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "uwb_cfboard.h"

#include <px4_platform_common/getopt.h>
#include <px4_platform_common/module.h>

void UWBCFBOARD::print_usage()
{
	PRINT_MODULE_USAGE_NAME("UWB CF Board","driver");
	PRINT_MODULE_USAGE_SUBCATEGORY("positioning_system");
	PRINT_MODULE_USAGE_COMMAND("start");
	PRINT_MODULE_USAGE_PARAMS_I2C_SPI_DRIVER(true,false);
	PRINT_MODULE_USAGE_PARAMS_I2C_ADDRESS(0x99); // TODO
	PRINT_MODULE_USAGE_DEFAULT_COMMANDS();
}

extern "C" int uwbcfboard_main(int argc, char *argv[])
{
	using ThisDriver = UWBCFBOARD;
	BusCLIArguments cli{true, false};
	cli.i2c_address = I2C_ADDRESS_DEFAULT;
	cli.default_i2c_frequency = I2C_SPEED;

	const char *verb = cli.parseDefaultArguments(argc, argv);

	if(!verb) {
		ThisDriver::print_usage();
		return PX4_ERROR;
	}

	BusInstanceIterator iterator(MODULE_NAME, cli, DRV_DIST_DEVTYPE_UWBCFCU);

	if(!strcmp(verb, "start")) {
		return ThisDriver::module_start(cli, iterator);
	}
	else if(!strcmp(verb, "stop")) {
		return ThisDriver::module_stop(iterator);
	}
	else if(!strcmp(verb, "status")) {
		return ThisDriver::module_status(iterator);
	}
	else {
		ThisDriver::print_usage();
		return PX4_ERROR;
	}
}

