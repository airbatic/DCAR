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
#include "wx/config.h"
#include "wx/stdpaths.h"

//#include <stdio.h>
//#include <fcntl.h>      // Contains file controls like O_RDWR
//#include <errno.h>      // Error integer and strerror() function
//#include <termios.h>    // Contains POSIX terminal control definitions
//#include <unistd.h>     // write(), read(), close()
//#include <string.h>     // For strlen()
//#include <math.h>

#define VERSION_MAJOR   1
#define VERSION_MINOR   0
#define VERSION_BUILD   1

#include "DCARMessage.h"
#include "MainFrame.h"


class DCARApp : public wxApp
{
public:
	DCARApp();
	~DCARApp();

	MainFrame* GetMainFrame(void)
	{ return m_pMainFrame; }
	    wxConfig* GetConfig(void)
    { return m_pConfig; }


public:
	virtual bool OnInit() override;
    virtual int OnExit() override;

//private:
	MainFrame* m_pMainFrame;
    wxConfig* m_pConfig;
	size_t  m_trackCount;       /* num of tracks machine supports 8, 16 or 24 */
    bool    m_dcsFound;         /* true if DCS track controller found */
	wxString m_strHostArg;
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
