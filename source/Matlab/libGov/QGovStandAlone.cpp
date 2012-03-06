/*
 * QGovStandAlone.cpp
 *
 *  Created on: Mar 5, 2012
 *      Author: rghunter
 */

#include "QGovStandAlone.h"

QGovStandAlone::QGovStandAlone() {

	(void)gpc_init(gprot_trigger_output, NULL, gprot_register_changed, NULL);

	gprot_flag_reg = 0;
	gprot_flag_reg_old = 0;
	(void)gpc_setup_reg(GPROT_FLAG_REG_ADDR, &gprot_flag_reg);

	(void)gpc_setup_reg(GPROT_NEW_CYCLE_TIME, (u16 *) & new_cycle_time);

	(void)gpc_setup_reg(GPROT_PWM_VAL_REG_ADDR, (u16 *) & gprot_pwm_power);

	serialPort = new SerialPort();

}

QGovStandAlone::~QGovStandAlone() {
	// TODO Auto-generated destructor stub

	serialPort.Close();
}

int QGovStandAlone::Open(const char *port)
{

	return serialPort.Open(port,230400);

}
