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

    m_strHostArg = "";
}

DCARApp::~DCARApp()
{
}


bool DCARApp::OnInit()
{
    // Create the configuration file manager object
    wxFileName fname(argv[0]);
    wxString ini_filename;

#ifdef __WXMSW__
    // Windows: keep the existing behavior (ini next to the exe)
    ini_filename = fname.GetPath(wxPATH_GET_VOLUME|wxPATH_GET_SEPARATOR) + _("DRC1200.ini");
#else
    // Linux/Flatpak: per-user config directory
    wxString cfgdir = wxStandardPaths::Get().GetUserConfigDir()
                      + wxFILE_SEP_PATH + _("DCAR");
    wxFileName::Mkdir(cfgdir, 0755, wxPATH_MKDIR_FULL);
    ini_filename = cfgdir + wxFILE_SEP_PATH + _("DCAR.ini");
#endif

	m_pConfig = new wxConfig(_("DCAR"), wxEmptyString, ini_filename);

    // If host name was passed on the command line at startup,
    // save the name and the mainframe will attempt to connect
    // after initializing the main frame.

    if (argc > 1)
    {
        m_strHostArg = argv[1];
    }

    // Load formats like PNG or JPEG directly from a file
    wxInitAllImageHandlers();

	// Create the main application frame window
	m_pMainFrame = new MainFrame();
	m_pMainFrame->Show();

	return true;
}


int DCARApp::OnExit()
{
    if (m_pConfig)
    {
        m_pConfig->Flush();

        delete m_pConfig;
    }

    return 0;
}

// End-Of-File
