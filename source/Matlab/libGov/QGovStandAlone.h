/*
 * QGovStandAlone.h
 *
 *  Created on: Mar 5, 2012
 *      Author: rghunter
 */

#ifndef QGOVSTANDALONE_H_
#define QGOVSTANDALONE_H_

#include <lg/gpdef.h>
#include <lg/gprotm.h>

#include "serial/rs232.h"

class QGovStandAlone {

private:
	SerialPort serialPort;

public:
	QGovStandAlone();
	virtual ~QGovStandAlone();
	int Open(const char *port);



};

#endif /* QGOVSTANDALONE_H_ */
