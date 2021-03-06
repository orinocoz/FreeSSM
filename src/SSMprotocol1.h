/*
 * SSMprotocol1.h - Application Layer for the old Subaru SSM protocol
 *
 * Copyright (C) 2009-2012 Comer352L
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef SSMPROTOCOL1_H
#define SSMPROTOCOL1_H



#include <string>
#include <vector>
#include <math.h>
#include "AbstractDiagInterface.h"
#include "SSMprotocol.h"
#include "SSMP1communication.h"
#include "SSM1definitionsInterface.h"
#include "SSM2definitionsInterface.h"



class SSMprotocol1 : public SSMprotocol
{
	Q_OBJECT

public:
	SSMprotocol1(AbstractDiagInterface *diagInterface, QString language="en");
	~SSMprotocol1();
	// NON-COMMUNICATION-FUNCTIONS:
	CUsetupResult_dt setupCUdata(CUtype_dt CU);
	protocol_dt protocolType() { return SSM1; };
	bool hasClearMemory(bool *CMsup);
	bool getSupportedDCgroups(int *DCgroups);
	// COMMUNICATION BASED FUNCTIONS:
	bool startDCreading(int DCgroups);
	bool stopDCreading();
	bool startMBSWreading(std::vector<MBSWmetadata_dt> mbswmetaList);
	bool stopMBSWreading();
	bool getAdjustmentValue(unsigned char index, unsigned int *rawValue);
	bool getAllAdjustmentValues(std::vector<unsigned int> *rawValues);
	bool setAdjustmentValue(unsigned char index, unsigned int rawValue);
	bool startActuatorTest(unsigned char actuatorTestIndex);
	bool stopActuatorTesting();
	bool stopAllActuators();
	bool clearMemory(CMlevel_dt level, bool *success);
	bool testImmobilizerCommLine(immoTestResult_dt *result);
	bool isEngineRunning(bool *isrunning);
	bool isInTestMode(bool *testmode);
	bool waitForIgnitionOff();

private:
	SSMP1communication *_SSMP1com;
	bool _uses_SSM2defs;
	unsigned int _CMaddr;
	char _CMvalue;

	bool readExtendedID(char ID[5]);

public slots:
	void resetCUdata();

};



#endif

