#include "DCARApp.h"

///////////////////////////////////////////////////////////////////////////////
// Helpers that return menu ID's for a track number

int MainFrame::GetRecordMenuID(uint32_t uIndex)
{
	const uint32_t s_recMenuID[STC_MAX_TRACKS] = {
		ID_RECORD_TRACK1,  ID_RECORD_TRACK2,  ID_RECORD_TRACK3,  ID_RECORD_TRACK4,
		ID_RECORD_TRACK5,  ID_RECORD_TRACK6,  ID_RECORD_TRACK7,  ID_RECORD_TRACK8,
		ID_RECORD_TRACK9,  ID_RECORD_TRACK10, ID_RECORD_TRACK11, ID_RECORD_TRACK12,
		ID_RECORD_TRACK13, ID_RECORD_TRACK14, ID_RECORD_TRACK15, ID_RECORD_TRACK16,
		ID_RECORD_TRACK17, ID_RECORD_TRACK18, ID_RECORD_TRACK19, ID_RECORD_TRACK20,
		ID_RECORD_TRACK21, ID_RECORD_TRACK22, ID_RECORD_TRACK23, ID_RECORD_TRACK24
	};

	if (uIndex >= STC_MAX_TRACKS)
		return 0;

	return s_recMenuID[uIndex];
}

int MainFrame::GetReproMenuID(uint32_t uIndex)
{
	const uint32_t s_reproMenuID[STC_MAX_TRACKS] = {
		ID_REPRO_TRACK1,  ID_REPRO_TRACK2,  ID_REPRO_TRACK3,  ID_REPRO_TRACK4,
		ID_REPRO_TRACK5,  ID_REPRO_TRACK6,  ID_REPRO_TRACK7,  ID_REPRO_TRACK8,
		ID_REPRO_TRACK9,  ID_REPRO_TRACK10, ID_REPRO_TRACK11, ID_REPRO_TRACK12,
		ID_REPRO_TRACK13, ID_REPRO_TRACK14, ID_REPRO_TRACK15, ID_REPRO_TRACK16,
		ID_REPRO_TRACK17, ID_REPRO_TRACK18, ID_REPRO_TRACK19, ID_REPRO_TRACK20,
		ID_REPRO_TRACK21, ID_REPRO_TRACK22, ID_REPRO_TRACK23, ID_REPRO_TRACK24
	};

	if (uIndex >= STC_MAX_TRACKS)
		return 0;

	return s_reproMenuID[uIndex];
}

int MainFrame::GetInputMenuID(uint32_t uIndex)
{
	const uint32_t s_syncMenuID[STC_MAX_TRACKS] = {
		ID_INPUT_TRACK1,  ID_INPUT_TRACK2,  ID_INPUT_TRACK3,  ID_INPUT_TRACK4,
		ID_INPUT_TRACK5,  ID_INPUT_TRACK6,  ID_INPUT_TRACK7,  ID_INPUT_TRACK8,
		ID_INPUT_TRACK9,  ID_INPUT_TRACK10, ID_INPUT_TRACK11, ID_INPUT_TRACK12,
		ID_INPUT_TRACK13, ID_INPUT_TRACK14, ID_INPUT_TRACK15, ID_INPUT_TRACK16,
		ID_INPUT_TRACK17, ID_INPUT_TRACK18, ID_INPUT_TRACK19, ID_INPUT_TRACK20,
		ID_INPUT_TRACK21, ID_INPUT_TRACK22, ID_INPUT_TRACK23, ID_INPUT_TRACK24
	};

	if (uIndex >= STC_MAX_TRACKS)
		return 0;

	return s_syncMenuID[uIndex];
}

int MainFrame::GetSyncMenuID(uint32_t uIndex)
{
	const uint32_t s_syncMenuID[STC_MAX_TRACKS] = {
		ID_SYNC_TRACK1,  ID_SYNC_TRACK2,  ID_SYNC_TRACK3,  ID_SYNC_TRACK4,
		ID_SYNC_TRACK5,  ID_SYNC_TRACK6,  ID_SYNC_TRACK7,  ID_SYNC_TRACK8,
		ID_SYNC_TRACK9,  ID_SYNC_TRACK10, ID_SYNC_TRACK11, ID_SYNC_TRACK12,
		ID_SYNC_TRACK13, ID_SYNC_TRACK14, ID_SYNC_TRACK15, ID_SYNC_TRACK16,
		ID_SYNC_TRACK17, ID_SYNC_TRACK18, ID_SYNC_TRACK19, ID_SYNC_TRACK20,
		ID_SYNC_TRACK21, ID_SYNC_TRACK22, ID_SYNC_TRACK23, ID_SYNC_TRACK24
	};

	if (uIndex >= STC_MAX_TRACKS)
		return 0;

	return s_syncMenuID[uIndex];
}

int MainFrame::GetMonitorMenuID(uint32_t uIndex)
{
	const uint32_t s_syncMenuID[STC_MAX_TRACKS] = {
		ID_MONITOR_TRACK1,  ID_MONITOR_TRACK2,  ID_MONITOR_TRACK3,  ID_MONITOR_TRACK4,
		ID_MONITOR_TRACK5,  ID_MONITOR_TRACK6,  ID_MONITOR_TRACK7,  ID_MONITOR_TRACK8,
		ID_MONITOR_TRACK9,  ID_MONITOR_TRACK10, ID_MONITOR_TRACK11, ID_MONITOR_TRACK12,
		ID_MONITOR_TRACK13, ID_MONITOR_TRACK14, ID_MONITOR_TRACK15, ID_MONITOR_TRACK16,
		ID_MONITOR_TRACK17, ID_MONITOR_TRACK18, ID_MONITOR_TRACK19, ID_MONITOR_TRACK20,
		ID_MONITOR_TRACK21, ID_MONITOR_TRACK22, ID_MONITOR_TRACK23, ID_MONITOR_TRACK24
	};

	if (uIndex >= STC_MAX_TRACKS)
		return 0;

	return s_syncMenuID[uIndex];
}

///////////////////////////////////////////////////////////////////////////////
//

void MainFrame::CreateMenuBar(void)
{
	// Create CONNECT menu items

	m_menuConnect = new wxMenu;

	m_menuConnect->Append(ID_CONNECT_TCP,
		wxT("&TCP/IP\tF1"),
		wxT("Connect to server"));
	m_menuConnect->Append(ID_CONNECT_CLOSE,
		wxT("&Disconnect\tF2"),
		wxT("Disconnect from server"));
	m_menuConnect->AppendSeparator();
	m_menuConnect->Append(ID_CONNECT_EXIT,
		wxT("E&xit"),
		wxT("Close and exit the program"));

	// Create VIEW menu items

	wxMenu* menuView = new wxMenu;

	menuView->AppendCheckItem(ID_VIEW_SHOWTRACKS,
		wxT("&Track Window"),
		wxT("Hide/Show Track Assign Window"));

	// Create HELP menu items

	wxMenu* helpMenu = new wxMenu;

	helpMenu->Append(ID_ABOUT,
		wxT("About"),
		wxT("Display program information"));

	// Now add the menu bar items

	wxMenuBar* menuBar = new wxMenuBar;

	menuBar->Append(m_menuConnect, wxT("&Connect"));
	menuBar->Append(menuView, wxT("&View"));
	menuBar->Append(helpMenu, wxT("&Help"));

	// Associate the menu bar with the frame
	SetMenuBar(menuBar);
}
