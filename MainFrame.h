#pragma once

///////////////////////////////////////////////////////////////////////////////
// Left hand view container panel, left hand side view

class LeftContainer : public wxPanel
{
public:
	LeftContainer(wxWindow* parent);

	wxFrame* m_mainframe;
};

///////////////////////////////////////////////////////////////////////////////
// Right hand view container panel, locator buttons

class CommandButton : public wxButton
{
public:
	CommandButton(wxPanel* panel, int id, const wxString& label);

	void OnCommandButtonClicked(wxCommandEvent& event);
};

class RightContainer : public wxPanel
{
public:
	RightContainer(wxWindow* parent);

	wxFrame* m_mainframe;

	CommandButton* m_btnDigit0;
	CommandButton* m_btnDigit1;
	CommandButton* m_btnDigit2;
	CommandButton* m_btnDigit3;
	CommandButton* m_btnDigit4;
	CommandButton* m_btnDigit5;
	CommandButton* m_btnDigit6;
	CommandButton* m_btnDigit7;
	CommandButton* m_btnDigit8;
	CommandButton* m_btnDigit9;
};

///////////////////////////////////////////////////////////////////////////////
// Main Application Window Frame

class MainFrame : public wxFrame
{
public:
	MainFrame();
	~MainFrame();

public:
	wxSocketClient* GetSocketState()
	{ return m_sockState; }
	bool IsConnected(void)
	{ return m_sockState->IsConnected() ? true : false; }

private:
	void CreateMenuBar(void);
	bool ConnectionOpen(wxSockAddress::Family family, wxString hostname);
	void ConnectionClose(void);
	void HandleReceiveData(void);

private:
	wxMenu*				m_menuConnect;
	wxMenu*             m_menuCommand;
	wxPanel*			m_panel;
	LeftContainer*		m_panelLeft;
	RightContainer*		m_panelRight;
	wxSocketClient*		m_sockState;
	wxString			m_strHostname;
	int					m_nPortNumber;

	// Event Handlers
	wxDECLARE_EVENT_TABLE();

	void OnSocketEvent(wxSocketEvent& event);
	void OnConnectTcp(wxCommandEvent& WXUNUSED(event));
    void OnUpdateUIConnectTcp(wxUpdateUIEvent& event);
	void OnConnectClose(wxCommandEvent& WXUNUSED(event));
    void OnUpdateUIConnectClose(wxUpdateUIEvent& event);
	void OnExit(wxCommandEvent& WXUNUSED(event));
    void OnUpdateUICommand(wxUpdateUIEvent& event);
	void OnPingRemote(wxCommandEvent& WXUNUSED(event));
    void OnSetRxMode(wxCommandEvent& WXUNUSED(event));
    void OnSetTxMode(wxCommandEvent& WXUNUSED(event));
    void OnSetSafeMode(wxCommandEvent& WXUNUSED(event));
    void OnNullDcOffset(wxCommandEvent& WXUNUSED(event));
    void OnAlarmReset(wxCommandEvent& WXUNUSED(event));
    void OnAlarmSilence(wxCommandEvent& WXUNUSED(event));
	void OnAbout(wxCommandEvent& WXUNUSED(event));
};

// End-Of-File
