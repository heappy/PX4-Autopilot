/**
 * @file uwb_cfboard.cpp
 * @author Christopher Ruwisch (christopher.ruwisch@gmail.com)
 * @brief UWB Interface for individual board with DMW1004C connected to I2C
 * @version 0.1
 * @date 2023-01-29
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "uwb_cfboard.h"

using namespace time_literals;

UWBCFBOARD::UWBCFBOARD(const I2CSPIDriverConfig &config) :
	I2C(config),
	I2CSPIDriver(config)
{
}

UWBCFBOARD::~UWBCFBOARD()
{
	perf_free(_sample_perf);
	perf_free(_comms_errors);
	perf_free(_fault_perf);
}

int UWBCFBOARD::probe()
{
	_retries = 1;
	for(int i = 0; i < 10; ++i) {
		// Place check if connection is okay - TBD
		uint8_t data[1] {};
		ret = transfer(nullptr, 0, &data[0], sizeof(data));

		if(ret == PX4_OK)
		{
			_retries = 1;
			return PX4_OK;
		}
		else {
			px4_usleep(10000); // WAIT 10 Seconds and try again
		}
	}
}


int UWBCFBOARD::init()
{
	int ret = I2C::init();

	if(ret != PX4_OK) {
		DEVICE_DEBUG("I2C::init failed (%d)", ret);
		return ret;
	}

	if(ret == PX4_OK) {
		ScheduleNow();
	}

	return ret;
}

void UWBCFBOARD::print_status()
{
	I2CSPIDriverBase::print_status();

	perf_print_counter(_sample_perf);
	perf_print_counter(_comms_errors);
	perf_print_counter(_fault_perf);
}

void UWBCFBOARD::RunImpl()
{
	switch(_state) {
		case STATE::MEASURE:
			uint8_t cmd = ADDR_READ_MR;
			if(transfer(&cmd, 1, nullptr, 0) == PX4_OK)
			{
				_timestamp_sample = hrt_absolute_time();
				_state = STATE::READ;
				ScheduleDelayed(2_ms);
			}
			else {
				perf_count(_comms_errors);
				_state = STATE::MEASURE;
				ScheduleDelayed(2_ms);
			}
			break;

		case STATE::READ:

			perf_begin(_sample_perf);
			uin8t_t data[13] {0x00};
			int ret = transfer(nullptr, 0, &data[0], sizeof(data));
			perf_end(_sample_perf);
			if(ret != PX4_OK) {
				perf_count(_comms_errors);
			}
			else {
				const uint8_t status = data[0];


				const float32_t px_data = float32(data[0] << 24 | data[1] << 16 | data[2] << 8 | data[3]);
				const float32_t px_data = float32(data[0] << 24 | data[1] << 16 | data[2] << 8 | data[3]);
				const float32_t px_data = float32(data[0] << 24 | data[1] << 16 | data[2] << 8 | data[3]);

				if(hrt_elapsed_time(&_timestamp_sample) < 100_ms) {
					uwb_crazyflie_px4_s uwb_crazyflie_px4{};
					uwb_crazyflie_px4.timestamp_sample = _timestamp_sample;
					uwb_crazyflie_px4.time_uwb_ms = 0;
					uwb_crazyflie_px4.counter++;
					uwb_crazyflie_px4.time_offset = 0;
					uwb_crazyflie_px4.position[0] = px_data;
					uwb_crazyflie_px4.position[1] = py_data;
					uwb_crazyflie_px4.position[2] = pz_data;
					uwb_crazyflie_px4.status = PX4_OK;

				}
			}
	}
}
