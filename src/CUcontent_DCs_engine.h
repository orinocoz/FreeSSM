/*
 * CUcontent_DCs_engine.h - Widget for ECU Diagnostic Codes Reading
 *
 * Copyright (C) 2008-2009 Comer352l
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

#ifndef CUCONTENT_DCS_ENGINE_H
#define CUCONTENT_DCS_ENGINE_H



#include <QtGui>
#include <string>
#include "ui_CUcontent_DCs_engine.h"
#include "SSMprotocol.h"



class CUcontent_DCs_engine : public QWidget, private Ui::engineDCcontent_Form
{
	Q_OBJECT

public:
	CUcontent_DCs_engine(QWidget *parent, SSMprotocol *SSMPdev, QString progversion);
	~CUcontent_DCs_engine();
	void show();
	bool setup();
	bool startDCreading();
	bool stopDCreading();

private:
	SSMprotocol *_SSMPdev;
	QString _progversion;
	int _supportedDCgroups;
	bool _obd2DTCformat;
	bool _testMode;
	bool _DCheckActive;
	QStringList _currOrTempDTCs;
	QStringList _currOrTempDTCdescriptions;
	QStringList _histOrMemDTCs;
	QStringList _histOrMemDTCdescriptions;
	QStringList _latestCCCCs;
	QStringList _latestCCCCdescriptions;
	QStringList _memorizedCCCCs;
	QStringList _memorizedCCCCdescriptions;

	void setupUiFonts();
	void setDCtableContent(QTableWidget *tableWidget, QStringList DCs, QStringList DCdescriptions);
	void setNrOfTableRows(QTableWidget *tablewidget, unsigned int nrofUsedRows);
	void setNrOfRowsOfAllTableWidgets();
	void insertDCtable(QTextCursor cursor, QString title, QStringList codes, QStringList descriptions);
	void setTitleOfFirstDTCtable(bool obd2, bool testMode);
	void resizeEvent(QResizeEvent *event);
	bool eventFilter(QObject *obj, QEvent *event);
	void communicationError(QString errstr);

private slots:
	void callStart();
	void callStop();
	void updateCurrentOrTemporaryDTCsContent(QStringList currOrTempDTCs, QStringList currOrTempDTCdescriptions, bool testMode, bool DCheckActive);
	void updateHistoricOrMemorizedDTCsContent(QStringList histOrMemDTCs, QStringList histOrMemDTCdescriptions);
	void updateCClatestCCsContent(QStringList latestCCCCs, QStringList latestCCCCdescriptions);
	void updateCCmemorizedCCsContent(QStringList memorizedCCCCs, QStringList memorizedCCCCdescriptions);
	void printDCprotocol();

signals:
	void error();

};



#endif
