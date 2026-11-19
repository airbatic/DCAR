#include "DCARApp.h"
#include "ConnectDialog.h"

// Icon resources for linux build
#ifndef __WXMSW__
#include "res/DCAR_16.xpm"
#include "res/DCAR_32.xpm"
#include "res/DCAR_64.xpm"
#include "res/DCAR_256.xpm"
#endif // __WXMSW__

///////////////////////////////////////////////////////////////////////////////
//helper functions

enum wxbuildinfoformat {
    short_f, long_f
};

wxString wxbuildinfo(wxbuildinfoformat format)
{
    wxString wxbuild(wxVERSION_STRING);

    if (format == long_f )
    {
#if defined(__WXMSW__)
        wxbuild << _T("-Windows");
#elif defined(__WXMAC__)
        wxbuild << _T("-Mac");
#elif defined(__UNIX__)
        wxbuild << _T("-Linux");
#endif

#if wxUSE_UNICODE
        wxbuild << _T("-Unicode build");
#else
        wxbuild << _T("-ANSI build");
#endif // wxUSE_UNICODE
    }

    return wxbuild;
}

///////////////////////////////////////////////////////////////////////////////
// Message Event Dispatch Table

wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
    // Socket Read Event
    EVT_SOCKET(SOCKET_ID, MainFrame::OnSocketEvent)
    // Connect Menu
    EVT_MENU(ID_CONNECT_OPEN, MainFrame::OnConnectTcp)
    EVT_UPDATE_UI(ID_CONNECT_OPEN, MainFrame::OnUpdateUIConnectTcp)
    EVT_MENU(ID_CONNECT_CLOSE, MainFrame::OnConnectClose)
    EVT_UPDATE_UI(ID_CONNECT_CLOSE, MainFrame::OnUpdateUIConnectClose)
    EVT_MENU(ID_CONNECT_EXIT, MainFrame::OnExit)
    EVT_MENU(ID_ABOUT, MainFrame::OnAbout)
    // Command Menu
    EVT_MENU(ID_COMMAND_PING_REMOTE, MainFrame::OnPingRemote)
    EVT_UPDATE_UI(ID_COMMAND_PING_REMOTE, MainFrame::OnUpdateUICommand)
    EVT_MENU(ID_COMMAND_SET_RX_MODE, MainFrame::OnSetRxMode)
    EVT_UPDATE_UI(ID_COMMAND_SET_RX_MODE, MainFrame::OnUpdateUICommand)
    EVT_MENU(ID_COMMAND_SET_TX_MODE, MainFrame::OnSetTxMode)
    EVT_UPDATE_UI(ID_COMMAND_SET_TX_MODE, MainFrame::OnUpdateUICommand)
    EVT_MENU(ID_COMMAND_SET_SAFE_MODE, MainFrame::OnSetSafeMode)
    EVT_UPDATE_UI(ID_COMMAND_SET_SAFE_MODE, MainFrame::OnUpdateUICommand)
    EVT_MENU(ID_COMMAND_NULL_DC_OFFSET, MainFrame::OnNullDcOffset)
    EVT_UPDATE_UI(ID_COMMAND_NULL_DC_OFFSET, MainFrame::OnUpdateUICommand)
    EVT_MENU(ID_COMMAND_ALARM_RESET, MainFrame::OnAlarmReset)
    EVT_UPDATE_UI(ID_COMMAND_ALARM_RESET, MainFrame::OnUpdateUICommand)
    EVT_MENU(ID_COMMAND_ALARM_SILENCE, MainFrame::OnAlarmSilence)
    EVT_UPDATE_UI(ID_COMMAND_ALARM_SILENCE, MainFrame::OnUpdateUICommand)
wxEND_EVENT_TABLE()

///////////////////////////////////////////////////////////////////////////////
// The Main Application Frame Window

MainFrame::MainFrame() :
	wxFrame(nullptr, wxID_ANY, wxT("DCAR - Remote Control"), wxDefaultPosition, wxSize(925, 300))
{
	m_strHostname = wxT("");
	m_nPortNumber = 1200;

    // Explicitly loading a bundle with multiple sizes (Cleaner for high-DPI)
    wxIconBundle iconBundle;
#ifdef __WXMSW__
    // Loads embedded multi-res ICO from Windows Resource
    iconBundle.AddIcon(wxIcon("aaaa_DCAR", wxBITMAP_TYPE_ICO_RESOURCE));
#else
    // Loads a standard standalone image file on Linux/Mac
    //iconBundle.AddIcon(wxIcon("DCAR.png", wxBITMAP_TYPE_PNG));
    iconBundle.AddIcon(DCAR_16_xpm);
    iconBundle.AddIcon(DCAR_32_xpm);
    iconBundle.AddIcon(DCAR_64_xpm);
    iconBundle.AddIcon(DCAR_256_xpm);
#endif
    SetIcons(iconBundle);

	CreateMenuBar();

	wxColor colorFrame(wxT("#484848"));
	//wxColor colorPanel(wxT("#333333"));

	// Create the main frame panel covering the entire client area
	m_panel = new wxPanel(this, wxID_ANY);
	m_panel->SetBackgroundColour(colorFrame);

	wxBoxSizer* hbox = new wxBoxSizer(wxHORIZONTAL);

	m_panelLeft  = new LeftContainer(m_panel);
	m_panelRight = new RightContainer(m_panel);

	hbox->Add(m_panelLeft, 2, wxEXPAND | wxALL, 5);
	hbox->Add(m_panelRight, 1, wxEXPAND | wxUP | wxDOWN | wxRIGHT, 5);

	m_panel->SetSizer(hbox);

	SetMinSize(wxSize(760, 285));
	//SetMaxSize(wxSize(-1, 250));

	// Restore the previously saved geometry, if any, and register this frame
	// for its geometry to be saved when it is closed using the given wxConfig
	// key name.

	if (!wxPersistentRegisterAndRestore(this, "DCAR_main_frame"))
	{
		// Choose some custom default size for the first run -- or don't do
		// anything at all and let the system use the default initial size.

		SetClientSize(FromDIP(wxSize(925, 300)));
	}

	int widths[2] = { -1, 175 };
    // Status bar
	CreateStatusBar(2);
	SetStatusWidths(2, widths);
	SetStatusText(wxT("Ready"), 0);
    SetStatusText(wxT("(closed)"), 1);

	// Create the socket
	m_sockState = new wxSocketClient(wxSOCKET_NOWAIT);
	// Setup the event handler and subscribe to most events
	m_sockState->SetEventHandler(*this, SOCKET_ID);
	m_sockState->SetNotify(wxSOCKET_CONNECTION_FLAG | wxSOCKET_INPUT_FLAG | wxSOCKET_LOST_FLAG);
	m_sockState->Notify(true);
}

MainFrame::~MainFrame()
{

}


void MainFrame::CreateMenuBar(void)
{
	// Create CONNECT menu items

	m_menuConnect = new wxMenu;

	m_menuConnect->Append(ID_CONNECT_OPEN,
		wxT("&Open\tCtrl-O"),
		wxT("Open port to server"));
	m_menuConnect->Append(ID_CONNECT_CLOSE,
		wxT("&Close\tCtrl-C"),
		wxT("Close port to server"));
	m_menuConnect->AppendSeparator();
	m_menuConnect->Append(ID_CONNECT_EXIT,
		wxT("E&xit"),
		wxT("Close and exit the program"));

	// Create COMMAND menu items

	m_menuCommand = new wxMenu;

	m_menuCommand->Append(ID_COMMAND_PING_REMOTE,
		wxT("&Ping\tF1"),
		wxT("Ping the DCAR receiver"));
	m_menuCommand->Append(ID_COMMAND_SET_RX_MODE,
		wxT("&RX Mode\tF2"),
		wxT("Set receiver to RX mode"));
	m_menuCommand->Append(ID_COMMAND_SET_TX_MODE,
		wxT("&TX Mode\tF3"),
		wxT("Set receiver to TX mode"));
	m_menuCommand->Append(ID_COMMAND_SET_SAFE_MODE,
		wxT("&SAFE Mode\tF4"),
		wxT("Set receiver to safe mode"));
	m_menuCommand->Append(ID_COMMAND_NULL_DC_OFFSET,
		wxT("&NULL DC Offset\tF5"),
		wxT("NULL the receivers DC offset"));
	m_menuCommand->Append(ID_COMMAND_ALARM_RESET,
		wxT("&Alarm Reset\tF6"),
		wxT("Reset the receiver alarm"));
	m_menuCommand->Append(ID_COMMAND_ALARM_SILENCE,
		wxT("A&larm Silence\tF7"),
		wxT("Silence the receiver alarm"));

	// Create HELP menu items

	wxMenu* helpMenu = new wxMenu;

	helpMenu->Append(ID_ABOUT,
		wxT("About"),
		wxT("Display program information"));

	// Now add the menu bar items

	wxMenuBar* menuBar = new wxMenuBar;

	menuBar->Append(m_menuConnect, wxT("Co&nnect"));
	menuBar->Append(m_menuCommand, wxT("&Command"));
	menuBar->Append(helpMenu, wxT("&Help"));

	// Associate the menu bar with the frame
	SetMenuBar(menuBar);
}

///////////////////////////////////////////////////////////////////////////////
// Helpers

bool MainFrame::ConnectionOpen(wxSockAddress::Family family, wxString hostname)
{
	wxUnusedVar(family); // unused in !wxUSE_IPV6 case
	bool connected = false;
	wxIPaddress* addr;
	wxIPV4address addr4;
#if wxUSE_IPV6
	wxIPV6address addr6;
	if (family == wxSockAddress::IPV6)
		addr = &addr6;
	else
#endif
		addr = &addr4;

	m_menuConnect->Enable(ID_CONNECT_OPEN, false);
#if wxUSE_IPV6
	//m_menuSocket->Enable(CLIENT_OPENIPV6, false);
#endif
	m_menuConnect->Enable(ID_CONNECT_CLOSE, false);

	m_strHostname = hostname;

	addr->Hostname(hostname);
	addr->Service(m_nPortNumber);

	// we connect asynchronously and will get a wxSOCKET_CONNECTION event when
	// the connection is really established

	wxString msg;
	msg.Printf(wxT("Connecting..."), hostname, addr->Service());
	SetStatusText(msg, 1);

	connected = m_sockState->Connect(*addr, true);

    if (connected)
        msg.Printf(wxT("%s"), m_strHostname);
    else
        msg.Printf(wxT("Connect Failed!"));

    SetStatusText(msg, 1);

	return connected;
}

void MainFrame::ConnectionClose(void)
{
    SetStatusText(wxT("(closed)"), 1);

	if (m_sockState->IsConnected())
	{
		m_sockState->Close();
	}

	m_sockState->Notify(false);
}

void MainFrame::OnSocketEvent(wxSocketEvent& event)
{
	switch (event.GetSocketEvent())
	{
	case wxSOCKET_INPUT:
		HandleReceiveData();
		break;

	case wxSOCKET_LOST:
		wxLogMessage("Socket connection failed");
		break;

	case wxSOCKET_CONNECTION:
		break;

	default:
		wxLogMessage("Unknown socket event!!!");
		break;
	}
}

void MainFrame::HandleReceiveData(void)
{
	//m_sockState->Read(&m_state, sizeof(STC_STATE_MSG));

	//++m_nRxPacketCount;

	//UpdateStatusBar();
}

///////////////////////////////////////////////////////////////////////////////
// CONNECT Menu Handlers

void MainFrame::OnConnectTcp(wxCommandEvent& WXUNUSED(event))
{
	//ConnectDialog dlg(this, wxID_ANY, wxT("TCP/IP Connect"));

	// Ask user for server address
	wxString hostname = wxGetTextFromUser(
		_("Enter the IP address of the DCAR to connect with"),
		_("Connect..."),
		_("192.168.1.143"));

	if (hostname.empty())
	{
		return;
	}

	ConnectionOpen(wxSockAddress::IPV4, hostname);
}

void MainFrame::OnUpdateUIConnectTcp(wxUpdateUIEvent& event)
{
	if (IsConnected())
	{
		event.Enable(false);
		return;
	}

	event.Enable(true);
}

void MainFrame::OnConnectClose(wxCommandEvent& WXUNUSED(event))
{
	ConnectionClose();
}

void MainFrame::OnUpdateUIConnectClose(wxUpdateUIEvent& event)
{
	if (IsConnected())
	{
		event.Enable(true);
		return;
	}

	event.Enable(false);
}

void MainFrame::OnExit(wxCommandEvent& WXUNUSED(event))
{
	Close(true);
}

///////////////////////////////////////////////////////////////////////////////
// COMMAND Menu Handlers

void MainFrame::OnUpdateUICommand(wxUpdateUIEvent& event)
{
	if (IsConnected())
	{
		event.Enable(true);
		return;
	}

	event.Enable(false);
}

void MainFrame::OnPingRemote(wxCommandEvent& WXUNUSED(event))
{
    wxLogMessage("OnPingRemote");
}

void MainFrame::OnSetRxMode(wxCommandEvent& WXUNUSED(event))
{
    wxLogMessage("OnSetRxMode");
}

void MainFrame::OnSetTxMode(wxCommandEvent& WXUNUSED(event))
{
    wxLogMessage("OnSetTxMode");
}

void MainFrame::OnSetSafeMode(wxCommandEvent& WXUNUSED(event))
{
    wxLogMessage("OnSetSafeMode");
}

void MainFrame::OnNullDcOffset(wxCommandEvent& WXUNUSED(event))
{
    wxLogMessage("OnNullDcOffset");
}

void MainFrame::OnAlarmReset(wxCommandEvent& WXUNUSED(event))
{
    wxLogMessage("OnAlarmReset");
}

void MainFrame::OnAlarmSilence(wxCommandEvent& WXUNUSED(event))
{
    wxLogMessage("OnAlarmSilence");
}

///////////////////////////////////////////////////////////////////////////////
// HELP Menu Handlers

void MainFrame::OnAbout(wxCommandEvent& WXUNUSED(event))
{
	wxAboutDialogInfo info;
	wxString strVersion;
	wxString strLongVersion;

	strVersion.Printf(wxT("%u.%u.%u"), VERSION_MAJOR, VERSION_MINOR, VERSION_BUILD);
	strLongVersion = wxT("Version ") + strVersion;

#if defined(__WXMSW__)
	info.SetIcon(wxIcon("AAAA_DCAR"));
#elif defined(__WXMAC__)
	info.SetIcon(wxIcon("DCAR.png"));
#elif defined(__UNIX__)
	info.SetIcon(wxIcon(DCAR_256_xpm));
#endif
	info.SetName("DCAR Remote Control");
	info.SetVersion(strVersion, strLongVersion);
	info.SetDescription(wxT("Dual Channel Agile Receiver Remote Control"));
	info.SetCopyright(wxT("Copyright (C) 2026, Pinnacle Technologies"));
	info.AddDeveloper(wxT("John MacConnell"));
	info.AddDeveloper(wxT("Robert E Starr, Jr."));
	info.SetWebSite(wxT("https://pinnaclerf.com"), wxT("Pinnacle Technologies"));

	wxAboutBox(info, this);
}
