/**
 * @file uwb_cfboard.h
 * @author Christopher Ruwisch (christopher.ruwisch@gmail.com)
 * @brief UWB Interface for individual board with DMW1004C connected to I2C
 * @version 0.1
 * @date 2023-01-29
 *
 * @copyright Copyright (c) 2023
 *
 */

/**
 *
 * Driver for UWB Board
 *
 * Interface application notes:
 *
 *   -tbd
 *
 */

#pragma once

#include <drivers/drv_hrt.h>
#include <lib/drivers/device/i2c.h>
#include <lib/perf/perf_counter.h>
#include <px4_platform_common/i2c_spi_buses.h>
#include <uORB/PublicationMulti.hpp>
#include <uORB/topics/uwb_crazyflie_px4.h>

static constexpr uint32_t I2C_SPEED = 100 * 1000;
static constexpr uint8_t  I2C_ADDRESS_DEFAULT = 0x99; // <-- NEEDS CONFIGURATION

/* Register address */
#define ADDR_READ_MR 0x00

class UWBCFBOARD : public device::I2C, public I2CSPIDriver<UWBCFBOARD>
{
public:
	UWBCFBOARD(const I2CSPIDriverConfig &config);
	~UWBCFBOARD() override;

	static void print_usage();

	void RunImpl();

	int init() override;
	void print_status() override;

private:
	int probe() override;

	enum class STATE : uint8_t {
		MEASURE,
		READ,
	} _state{STATE::MEASURE};

	hrt_abstime _timestamp_sample{0};

	uORB::PublicationMulti<uwb_crazyflie_px4_s> _uwb_crazyflie_px4_pub{ORB_ID(uwb_crazyflie_px4)};

	perf_counter_t _sample_perf{perf_alloc(PC_ELAPSED, MODULE_NAME": read")};
	perf_counter_t _comms_errors{perf_alloc(PC_COUNT, MODULE_NAME": communication errors")};
	perf_counter_t _fault_perf{perf_alloc(PC_ELAPSED, MODULE_NAME": fault detected")};
};
