/***************************************************************
 * Name:      DCARApp.h
 * Purpose:   Defines Application Class
 * Author:    Robert Starr (rtzaudio@comcast.net)
 * Created:   2019-12-12
 * Copyright: Robert Starr (rtzaudio.com)
 * License:
 **************************************************************/

#include "DCARApp.h"

wxIMPLEMENT_APP(DCARApp);

DCARApp::DCARApp()
{
	m_pMainFrame = nullptr;
}

DCARApp::~DCARApp()
{
}


bool DCARApp::OnInit()
{
	// Create the main application frame window
	m_pMainFrame = new MainFrame();
	m_pMainFrame->Show();
	return true;
}

// End-Of-File
