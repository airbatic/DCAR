#pragma once

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif
#include "wx/sizer.h"
#include "wx/gbsizer.h"
#include "wx/minifram.h"
#include "wx/statline.h"
#include "wx/notebook.h"
#include "wx/spinctrl.h"
#include "wx/wrapsizer.h"
#include "wx/generic/stattextg.h"
#include "wx/html/htmlwin.h"
#include "wx/persist/toplevel.h"
#include "wx/aboutdlg.h"
#include "wx/socket.h"
#include "wx/url.h"
#include "wx/sstream.h"
#include "wx/thread.h"
#include "wx/dcbuffer.h"
#include "wx/dcgraph.h"
#include "wx/overlay.h"
#include "wx/graphics.h"
#include "wx/minifram.h"

#define VERSION_MAJOR   1
#define VERSION_MINOR   0
#define VERSION_BUILD   1

#include "MainFrame.h"


class DCARApp : public wxApp
{
public:
	DCARApp();
	~DCARApp();

	MainFrame* GetMainFrame(void)
	{ return m_pMainFrame; }

public:
	bool OnInit();

//private:
	MainFrame* m_pMainFrame;
};

// controls and menu constants
enum
{
	ID_CONNECT_EXIT = wxID_EXIT,

	ID_USER_START = wxID_HIGHEST + 1001,

	ID_CONNECT_CLOSE,
	ID_CONNECT_OPEN,

	ID_COMMAND_PING_REMOTE,
	ID_COMMAND_SET_RX_MODE,
	ID_COMMAND_SET_TX_MODE,
	ID_COMMAND_SET_SAFE_MODE,
	ID_COMMAND_NULL_DC_OFFSET,
	ID_COMMAND_ALARM_RESET,
	ID_COMMAND_ALARM_SILENCE,

	ID_HELP,
	ID_ABOUT,

	SOCKET_ID
};

wxDECLARE_APP(DCARApp);
