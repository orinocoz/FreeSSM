/*
 * Transmission.cpp - Transmission Control Unit dialog
 *
 * Copyright (C) 2008-2010 Comer352l
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

#include "TransmissionDialog.h"


TransmissionDialog::TransmissionDialog(AbstractDiagInterface *diagInterface, QString language) : ControlUnitDialog(tr("Transmission Control Unit"), diagInterface, language)
{
	// *** Initialize global variables:
	_content_DCs = NULL;
	_content_Adjustments = NULL;
	_mode = DCs_mode;	// we start in Diagnostic Codes mode
	// Show information-widget:
	_infoWidget = new CUinfo_Transmission();
	setInfoWidget(_infoWidget);
	_infoWidget->show();
	// Setup functions:
	QPushButton *pushButton = addFunction(tr("&Diagnostic Codes"), QIcon(QString::fromUtf8(":/icons/chrystal/22x22/messagebox_warning.png")), true);
	pushButton->setChecked(true);
	connect( pushButton, SIGNAL( clicked() ), this, SLOT( DTCs() ) );
	pushButton = addFunction(tr("&Measuring Blocks"), QIcon(QString::fromUtf8(":/icons/oxygen/22x22/applications-utilities.png")), true);
	connect( pushButton, SIGNAL( clicked() ), this, SLOT( measuringblocks() ) );
	pushButton = addFunction(tr("&Adjustments"), QIcon(QString::fromUtf8(":/icons/chrystal/22x22/configure.png")), true);
	connect( pushButton, SIGNAL( clicked() ), this, SLOT( adjustments() ) );
	pushButton = addFunction(tr("Clear Memory"), QIcon(QString::fromUtf8(":/icons/chrystal/22x22/eraser.png")), false);
	connect( pushButton, SIGNAL( clicked() ), this, SLOT( clearMemory() ) );
	_clearMemory2_pushButton = addFunction(tr("Clear Memory 2"), QIcon(QString::fromUtf8(":/icons/chrystal/22x22/eraser.png")), false);
	connect( _clearMemory2_pushButton, SIGNAL( clicked() ), this, SLOT( clearMemory2() ) );
	// NOTE: using released() instead of pressed() as workaround for a Qt-Bug occuring under MS Windows
	// Load/Show Diagnostic Code content:
	_content_DCs = new CUcontent_DCs_transmission();
	setContentWidget(tr("Diagnostic Codes:"), _content_DCs);
	_content_DCs->show();
	// Make GUI visible
	this->show();
	// Connect to Control Unit, get data and setup GUI:
	setup();
}


void TransmissionDialog::setup()
{
	// *** Local variables:
	QString sysdescription = "";
	std::string ROM_ID = "";
	bool supported = false;
	std::vector<mb_dt> supportedMBs;
	std::vector<sw_dt> supportedSWs;
	int supDCgroups = 0;
	// ***** Connect to Control Unit *****:
	// Create Status information message box for CU initialisation/setup:
	FSSM_InitStatusMsgBox initstatusmsgbox(tr("Connecting to TCU... Please wait !"), 0, 0, 100, this);
	initstatusmsgbox.setWindowTitle(tr("Connecting to TCU..."));
	initstatusmsgbox.setValue(5);
	initstatusmsgbox.show();
	// Try to establish CU connection:
	if( probeProtocol(SSMprotocol::CUtype_Transmission) )
	{
		// Update status info message box:
		initstatusmsgbox.setLabelText(tr("Processing TCU data... Please wait !"));
		initstatusmsgbox.setValue(40);
		// Query ROM-ID:
		ROM_ID = _SSMPdev->getROMID();
		if (!ROM_ID.length())
			goto commError;
		// Query system description:
		if (!_SSMPdev->getSystemDescription(&sysdescription))
		{
			std::string SYS_ID = _SSMPdev->getSysID();
			if (!SYS_ID.length())
				goto commError;
			sysdescription = tr("unknown");
			if (SYS_ID != ROM_ID)
				sysdescription += " (" + QString::fromStdString(SYS_ID) + ")";
		}
		// Output system description:
		_infoWidget->setTransmissionTypeText(sysdescription);
		// Output ROM-ID:
		_infoWidget->setRomIDText( QString::fromStdString(ROM_ID) );
		// Number of supported MBs / SWs:
		if ((!_SSMPdev->getSupportedMBs(&supportedMBs)) || (!_SSMPdev->getSupportedSWs(&supportedSWs)))
			goto commError;
		_infoWidget->setNrOfSupportedMBsSWs(supportedMBs.size(), supportedSWs.size());
		// OBD2-Support:
		if (!_SSMPdev->hasOBD2system(&supported))
			goto commError;
		_infoWidget->setOBD2Supported(supported);
		// "Clear Memory 2"-support:
		if (!_SSMPdev->hasClearMemory2(&supported))
			goto commError;
		if (supported)
			_clearMemory2_pushButton->setEnabled(true);
		else
			_clearMemory2_pushButton->setEnabled(false);
	}
	else // CU-connection could not be established
		goto commError;
	// Start Diagnostic Codes reading:
	if (!_content_DCs->setup(_SSMPdev))
		goto commError;
	if (!_SSMPdev->getSupportedDCgroups(&supDCgroups))
		goto commError;
	if (supDCgroups != SSMprotocol::noDCs_DCgroup)
	{
		if (!_content_DCs->startDCreading())
			goto commError;
	}
	connect(_content_DCs, SIGNAL( error() ), this, SLOT( close() ) );
	// Update and close status info:
	initstatusmsgbox.setLabelText(tr("TCU-initialisation successful !"));
	initstatusmsgbox.setValue(100);
	QTimer::singleShot(800, &initstatusmsgbox, SLOT(accept()));
	initstatusmsgbox.exec();
	initstatusmsgbox.close();
	return;

commError:
	initstatusmsgbox.close();
	communicationError();
}


void TransmissionDialog::DTCs()
{
	bool ok = false;
	int DCgroups = 0;
	if (_mode == DCs_mode) return;
	_mode = DCs_mode;
	// Show wait-message:
	FSSM_WaitMsgBox waitmsgbox(this, tr("Switching to Diagnostic Codes... Please wait !"));
	waitmsgbox.show();
	// Create, setup and insert content-widget:
	_content_DCs = new CUcontent_DCs_transmission();
	setContentWidget(tr("Diagnostic Codes:"), _content_DCs);
	_content_DCs->show();
	ok = _content_DCs->setup(_SSMPdev);
	// Start DC-reading:
	if (ok)
	{
		ok = _SSMPdev->getSupportedDCgroups(&DCgroups);
		if (ok && DCgroups != SSMprotocol::noDCs_DCgroup)
			ok = _content_DCs->startDCreading();
	}
	// Get notification, if internal error occures:
	if (ok)
		connect(_content_DCs, SIGNAL( error() ), this, SLOT( close() ) );
	// Close wait-message:
	waitmsgbox.close();
	// Check for communication error:
	if (!ok)
		communicationError();
}


void TransmissionDialog::measuringblocks()
{
	bool ok = false;
	if (_mode == MBsSWs_mode) return;
	_mode = MBsSWs_mode;
	// Show wait-message:
	FSSM_WaitMsgBox waitmsgbox(this, tr("Switching to Measuring Blocks... Please wait !"));
	waitmsgbox.show();
	// Create, setup and insert content-widget:
	CUcontent_MBsSWs *content_MBsSWs = new CUcontent_MBsSWs(_MBSWsettings);
	setContentWidget(tr("Measuring Blocks:"), content_MBsSWs);
	content_MBsSWs->show();
	ok = content_MBsSWs->setup(_SSMPdev);
	if (ok)
		ok = content_MBsSWs->setMBSWselection(_lastMBSWmetaList);
	// Get notification, if internal error occures:
	if (ok)
		connect(content_MBsSWs, SIGNAL( error() ), this, SLOT( close() ) );
	// Close wait-message:
	waitmsgbox.close();
	// Check for communication error:
	if (!ok)
		communicationError();
}


void TransmissionDialog::adjustments()
{
	bool ok = false;
	if (_mode == Adaptions_mode) return;
	_mode = Adaptions_mode;
	// Show wait-message:
	FSSM_WaitMsgBox waitmsgbox(this, tr("Switching to Adjustment Values... Please wait !"));
	waitmsgbox.show();
	// Create, setup and insert content-widget:
	_content_Adjustments = new CUcontent_Adjustments();
	setContentWidget(tr("Adjustments:"), _content_Adjustments);
	_content_Adjustments->show();
	ok = _content_Adjustments->setup(_SSMPdev);
	if (ok)
		connect(_content_Adjustments, SIGNAL( communicationError() ), this, SLOT( close() ) );
	// Close wait-message:
	waitmsgbox.close();
	// Check for communication error:
	if (!ok)
		communicationError();
}


void TransmissionDialog::clearMemory()
{
	runClearMemory(SSMprotocol::CMlevel_1);
}


void TransmissionDialog::clearMemory2()
{
	runClearMemory(SSMprotocol::CMlevel_2);
}


void TransmissionDialog::runClearMemory(SSMprotocol::CMlevel_dt level)
{
	bool ok = false;
	ClearMemoryDlg::CMresult_dt result;
	// Create "Clear Memory"-dialog:
	ClearMemoryDlg cmdlg(this, _SSMPdev, level);
	// Temporary disconnect from "communication error"-signal:
	disconnect(_SSMPdev, SIGNAL( commError() ), this, SLOT( communicationError() ));
	// Run "Clear Memory"-procedure:
	result = cmdlg.run();
	// Reconnect to "communication error"-signal:
	connect(_SSMPdev, SIGNAL( commError() ), this, SLOT( communicationError() ));
	// Check result:
	if ((result == ClearMemoryDlg::CMresult_success) && (_mode == Adaptions_mode))
	{
		FSSM_WaitMsgBox waitmsgbox(this, tr("Reading Adjustment Values... Please wait !"));
		waitmsgbox.show();
		ok = _content_Adjustments->setup(_SSMPdev); // refresh adjustment values
		waitmsgbox.close();
		if (!ok)
			communicationError();
	}
	else if (result == ClearMemoryDlg::CMresult_communicationError)
	{
		communicationError();
	}
	else if ((result == ClearMemoryDlg::CMresult_reconnectAborted) || (result == ClearMemoryDlg::CMresult_reconnectFailed))
	{
		close(); // exit engine control unit dialog
	}
}
