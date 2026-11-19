#include "DCARApp.h"

///////////////////////////////////////////////////////////////////////////////
// Left Hand View Frame Container Panel

typedef struct _DCAR_PARAM {
    const char* szName;
} DCAR_PARAM;

DCAR_PARAM s_param[] = {
    "Message Type:",
    "Address:",
    "Mode:",
    "Last Set By:",
    "Alarm Red:",
    "Alarm Yellow:",
    "RX Attenuation Ch-1:",
    "TX Attenuation Ch-1:",
    "Low Pass Cutoff Ch-1:"
};

#define NUM_PARAMS  (sizeof(s_param)/sizeof(DCAR_PARAM))


    #if 0
    /* Channel-1 Status */
    int8_t      att_rx_ch1;                 /* Attn dB, -10 to +70 dB   */
    int8_t      att_tx_ch1;                 /* Attn dB, -10 to +70 dB   */
    uint8_t     lpf_cutoff_ch1;             /* 0-73: 5-72 MHz, 0-4 & 73 */
    uint8_t     freq_band_ch1;              /* freqy band-ch1 (1-10)    */
    int16_t     rf_input_power_ch1;         /* RF In Power-CH1 in dBm   */
    int16_t     lo_input_power_ch1;         /* LO In Power-CH1 in dBm   */
    int16_t     I_output_power_ch1;         /* I Output Power-CH1 dBm   */
    int16_t     Q_output_power_ch1;         /* Q Output Power-CH1 dBm   */
    int16_t     I_output_dc_offset_ch1;     /* I Output DC Offset-CH1   */
    int16_t     Q_output_dc_offset_ch1;     /* Q Output DC Offset-CH1   */
    int8_t      temp_ch1;                   /* temp-CH1 1 Receiver      */
    uint8_t     bit_values_ch1;             /* Bit Values-CH1           */
    uint8_t     receiver_ver_ch1;           /* CH-1 firmware version    */
    uint16_t    serial_num_ch1;             /* CH-1 receiver serial#    */
    /* Channel-2 Status */
    int8_t      att_rx_ch2;                 /* Attn dB, -10 to +70 dB   */
    int8_t      att_tx_ch2;                 /* Attn dB, -10 to +70 dB   */
    uint8_t     lpf_cutoff_ch2;             /* 0-73: 5-72 MHz, 0-4 & 73 */
    uint8_t     freq_band_ch2;              /* freqy band-ch1 (1-10)    */
    int16_t     rf_input_power_ch2;         /* RF In Power-CH1 in dBm   */
    int16_t     lo_input_power_ch2;         /* LO In Power-CH1 in dBm   */
    int16_t     I_output_power_ch2;         /* I Output Power-CH1 dBm   */
    int16_t     Q_output_power_ch2;         /* Q Output Power-CH1 dBm   */
    int16_t     I_output_dc_offset_ch2;     /* I Output DC Offset-CH2   */
    int16_t     Q_output_dc_offset_ch2;     /* Q Output DC Offset-CH2   */
    int8_t      temp_ch2;                   /* temp-CH1 2 Receiver      */
    uint8_t     bit_values_ch2;             /* Bit Values-CH1 2         */
    uint8_t     receiver_ver_ch2;           /* CH-2 firmware version    */
    uint16_t    serial_num_ch2;             /* CH-2 receiver serial#    */
    /* General Status & Info */
    uint16_t    power_pos_12V;              /* Power Supply +12V        */
    uint16_t    power_neg_12V;              /* Power Supply -12V        */
    int8_t      power_supply_temp;          /* PSU temp 0.5°C/Count     */
    uint8_t     bit_values;                 /* Individual Bit Values    */
    uint8_t     version;                    /* Panel Firmware Version   */
    uint8_t     serial_num;                 /* DCAR Serial Number       */
    #endif // 0



LeftContainer::LeftContainer(wxWindow* parent)
	: wxPanel(parent, wxID_ANY, wxDefaultPosition, wxSize(150, 50), wxBORDER_NONE)
{
	m_parent = parent;

	wxColor colorPanel(wxT("#333333"));
	SetBackgroundColour(colorPanel);

	wxColor colorText(wxT("#C9C9C9"));
    SetForegroundColour(colorText);

	//const long style = 0;

	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

	wxPanel* panel = new wxPanel(this, -1);

	wxGridSizer* gs = new wxGridSizer(NUM_PARAMS, 2, 0, 5);

	for (size_t i=0; i < NUM_PARAMS; i++)
    {
        wxChar str[64];

        gs->Add(new wxStaticText(panel, wxID_ANY, wxString(s_param[i].szName), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER), 1);

        wxSprintf(str, "value [%1]", i+1);
        gs->Add(new wxStaticText(panel, wxID_ANY, str, wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT), 2);
    }

	sizer->Add(panel, 1, wxTILE, 5);   // wxTILE | wxALL
	panel->SetSizer(gs);
	SetSizer(sizer);
}

///////////////////////////////////////////////////////////////////////////////
// Right Hand View Frame Container Panel

CommandButton::CommandButton(wxPanel* mypanel, int id, const wxString& label)
	: wxButton(mypanel, id, label, wxDefaultPosition, wxSize(50, 25))
{
	Connect(id, wxEVT_BUTTON, wxCommandEventHandler(CommandButton::OnCommandButtonClicked));
}

void CommandButton::OnCommandButtonClicked(wxCommandEvent& event)
{
    // Get pointer to MainFrame (passed in constructor or via wxGetApp)
    wxWindow* mainFrame = wxGetApp().GetMainFrame();

    // Forward the event
    if (mainFrame)
    {
        wxCommandEvent* event = new wxCommandEvent(wxEVT_COMMAND_MENU_SELECTED, GetId());
        wxQueueEvent(mainFrame->GetEventHandler(), event);
    }
}


RightContainer::RightContainer(wxWindow* parent)
	: wxPanel(parent, wxID_ANY, wxDefaultPosition, wxSize(-1, -1), wxBORDER_NONE)
{
	m_parent = parent;

	m_mainframe = (MainFrame*)(parent->GetParent());

	wxColor colorPanel(wxT("#333333"));
	SetBackgroundColour(colorPanel);

	wxPanel* panel = new wxPanel(this, -1);
    panel->SetBackgroundColour(colorPanel);

	m_btnDigit1 = new CommandButton(panel, ID_COMMAND_PING_REMOTE, wxT("Ping"));
	m_btnDigit2 = new CommandButton(panel, ID_COMMAND_SET_RX_MODE, wxT("RX Mode"));
	m_btnDigit3 = new CommandButton(panel, ID_COMMAND_SET_TX_MODE, wxT("TX Mode"));
	m_btnDigit4 = new CommandButton(panel, ID_COMMAND_SET_SAFE_MODE, wxT("SAFE Mode"));
	m_btnDigit5 = new CommandButton(panel, ID_COMMAND_NULL_DC_OFFSET, wxT("Null DC Offsets"));
	m_btnDigit6 = new CommandButton(panel, ID_COMMAND_ALARM_RESET, wxT("Alarm Reset"));
	m_btnDigit7 = new CommandButton(panel, ID_COMMAND_ALARM_SILENCE, wxT("Alarm Silence"));

	wxGridSizer* gs = new wxGridSizer(7, 1, 2, 2);

	gs->Add(m_btnDigit1, 1, wxEXPAND);
	gs->Add(m_btnDigit2, 1, wxEXPAND);
	gs->Add(m_btnDigit3, 1, wxEXPAND);
	gs->Add(m_btnDigit4, 1, wxEXPAND);
	gs->Add(m_btnDigit5, 1, wxEXPAND);
	gs->Add(m_btnDigit6, 1, wxEXPAND);
	gs->Add(m_btnDigit7, 1, wxEXPAND);

	wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

	sizer->Add(panel, 1, wxEXPAND | wxALL, 5);

	panel->SetSizer(gs);

	SetSizer(sizer);
}
