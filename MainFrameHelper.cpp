#include "DCARApp.h"
#include "ConnectDialog.h"

///////////////////////////////////////////////////////////////////////////////
// Helpers

void MainFrame::RefreshTimePanel(void)
{
	DCARApp* theApp = (DCARApp*)wxTheApp;

	if (theApp->m_panelTime)
		theApp->m_panelTime->Refresh(false);
}

void MainFrame::RefreshVelocityPanel(void)
{
	//DCARApp* theApp = (DCARApp*)wxTheApp;
}

void MainFrame::UpdateStatusBar(void)
{
#if wxUSE_STATUSBAR
	wxString str;

	if (!m_sockState->IsConnected())
	{
		str = wxT("Not Connected");
	}
	else
	{
#if wxUSE_IPV6
		wxIPV6address addr;
#else
		wxIPV4address addr;
#endif
		m_sockState->GetPeer(addr);
		str.Printf(wxT("%s : %s"), addr.Hostname(), m_strHostname);
	}

	SetStatusText(str, 1);
#endif // wxUSE_STATUSBAR
	m_menuConnect->Enable(ID_CONNECT_TCP, !m_sockState->IsConnected() && !m_busy);
#if wxUSE_IPV6
	//m_menuSocket->Enable(CLIENT_OPENIPV6, !m_sock->IsConnected() && !m_busy);
#endif
	m_menuConnect->Enable(ID_CONNECT_CLOSE, m_sockState->IsConnected());
}

//
// Return the current transport mode of operation as a string
//

void MainFrame::GetModeText(STC_STATE_MSG& msg, wxString& str)
{
	uint16_t mode = (msg.transportMode & STC_MODE_MASK);

	if (msg.transportMode & STC_M_SEARCH)
	{
		str = wxT("SEARCH...");
		return;
	}
	else if (msg.transportMode & STC_M_LOOP)
	{
		str = wxT("LOOP");
		return;
	}

	switch (mode)
	{
	case STC_MODE_HALT:
		str = wxT("TAPE OUT");
		break;

	case STC_MODE_THREAD:
		str = wxT("THREAD");
		break;

	case STC_MODE_STOP:
		str = wxT("STOP");
		break;

	case STC_MODE_PLAY:
		str = (msg.transportMode & STC_M_RECORD) ? wxT("RECORD") : wxT("PLAY");
		break;

	case STC_MODE_FWD:
		str = wxT("FWD");
		if (msg.transportMode & STC_M_LIBWIND)
			str += wxT(" (LIB)");
		break;

	case STC_MODE_REW:
		str = wxT("REW");
		if (msg.transportMode & STC_M_LIBWIND)
			str += wxT(" (LIB)");
		break;

	default:
		str = wxT("???");
		break;
	}
}




#if 0
void CRemoteView::DrawInfo(CHwndRenderTarget* pRenderTarget)
{
	CString str;

	ASSERT_VALID(pRenderTarget);
	CMainFrame* pMainFrame = GetMainFrame();

	STC_STATE_MSG& state = pMainFrame->GetTransportState();

	CD2DSolidColorBrush* brush;

	// Draw the current info panel information
	str.Format(_T("%u\" TAPE %u IPS"), state.tapeSize, state.tapeSpeed);
	//pRenderTarget->FillRectangle(m_rectInfo1, m_pLightGrayBrush);
	pRenderTarget->DrawText(str, m_rectInfo1, m_pLightGrayBrush, m_pTextInfo);

	// Display Auto-Play Flag State

	pRenderTarget->DrawRectangle(m_rectInfo2, m_pLightGrayBrush);

	if (pMainFrame->IsAutoPlay())
	{
		pRenderTarget->FillRectangle(m_rectInfo2, m_pLightGrayBrush);
		brush = m_pDarkGrayBrush;
	}
	else
	{
		brush = m_pLightGrayBrush;
	}

	pRenderTarget->DrawText(_T("AUTO"), m_rectInfo2, brush, m_pTextInfo);

	// Display Library Wind Flag State

	if (pMainFrame->IsLibWind())
	{
		pRenderTarget->FillRectangle(m_rectInfo3, m_pLightGrayBrush);
		brush = m_pDarkGrayBrush;
	}
	else
	{
		pRenderTarget->DrawRectangle(m_rectInfo3, m_pLightGrayBrush);
		brush = m_pLightGrayBrush;
	}

	pRenderTarget->DrawText(_T("LIB-W"), m_rectInfo3, brush, m_pTextInfo);

#if (DEBUG_SHOW_RECTS > 0)
	pRenderTarget->DrawRectangle(m_rectInfo1, m_pWhiteBrush, 0.5f);
	pRenderTarget->DrawRectangle(m_rectInfo2, m_pWhiteBrush, 0.5f);
	pRenderTarget->DrawRectangle(m_rectInfo3, m_pWhiteBrush, 0.5f);
	//pRenderTarget->DrawRectangle(m_rectInfoPane, m_pWhiteBrush, 0.5f);
#endif
}


void CRemoteView::DrawVelocity(CHwndRenderTarget* pRenderTarget)
{
	ASSERT_VALID(pRenderTarget);
	CMainFrame* pMainFrame = GetMainFrame();

	STC_STATE_MSG& state = pMainFrame->GetTransportState();

	CD2DRectF rect = m_rectPlot;

	rect.left += 5.0f;
	rect.top += 5.0f;
	rect.right -= 5.0f;
	rect.bottom -= 5.0f;

	// Draw the graph first
	DrawVelocityGraph(pRenderTarget, rect);

	// Now plot the velocity graph data
	DrawVelocityPlot(pRenderTarget, rect);

	if (state.errorCount > 0)
	{
		CString str;
		str.Format(_T("ENCODER ERRORS\n%u"), state.errorCount);
		pRenderTarget->DrawText(str, rect, m_pRedBrush, m_pTextButtonSmall);
	}
}


void CRemoteView::DrawVelocityGraph(CHwndRenderTarget* pRenderTarget, CD2DRectF rect)
{
	ASSERT_VALID(pRenderTarget);
	CMainFrame* pMainFrame = GetMainFrame();

	CD2DSolidColorBrush* brush = m_pLightGrayBrush;

	// Plot the graph first

	rect.left += 8.0f;

	pRenderTarget->DrawRectangle(rect, brush, 0.50);

	rect.left += 0.50f;
	rect.top += 0.50f;
	rect.right -= 0.50f;
	rect.bottom -= 0.50f;

	float h = rect.bottom - rect.top;
	float w = rect.right - rect.left;

	brush = m_pGrayBrush;

	// draw vertical grid lines
	for (int i = 0; i < 10; i++)
	{
		float x = (w / 11.0f) * (float)(i + 1);

		pRenderTarget->DrawLine(
			D2D1::Point2F(rect.left + x, rect.top),
			D2D1::Point2F(rect.left + x, rect.bottom),
			brush,
			0.25f);
	}

	// draw horizontal grid lines
	for (int i = 0; i < 3; i++)
	{
		float y = (h / 4.0f) * (float)(i + 1);

		pRenderTarget->DrawLine(
			D2D1::Point2F(rect.left, rect.top + y),
			D2D1::Point2F(rect.right, rect.top + y),
			brush,
			0.25f);
	}

	// add the labels
	CD2DRectF rectText(rect.left - 13.0f, rect.top, rect.left - 5.0f, rect.bottom);
	pRenderTarget->DrawText(_T("VEL"), rectText, m_pGrayBrush, m_pTextPlotSmall);
	//pRenderTarget->DrawRectangle(rectText, m_pWhiteBrush, 0.5f);
}


void CRemoteView::DrawVelocityPlot(CHwndRenderTarget* pRenderTarget, CD2DRectF rect)
{
	ASSERT_VALID(pRenderTarget);
	CMainFrame* pMainFrame = GetMainFrame();

	STC_STATE_MSG& state = pMainFrame->GetTransportState();

	CD2DSolidColorBrush* brush1 = m_pPlot1Brush;
	CD2DSolidColorBrush* brush2 = m_pPlot2Brush;

	// Now plot the velocity graph data

	rect.left += 2.0f + 8.0f;
	rect.top += 2.0f;
	rect.right -= 2.0f;
	rect.bottom -= 2.0f;

	float h = rect.bottom - rect.top;
	float w = rect.right - rect.left;

	float step = w / (float)MAX_VEL_SAMPLES;

	for (size_t i = 0; i < MAX_VEL_SAMPLES; i++)
	{
		float x = (float)i * step;

		float v = pMainFrame->m_velSample[i];

		if (v > 100.0f)
			v = 100.0f;

		float y1 = rect.bottom;
		float y2 = rect.top;

		float val = (FLOAT)v * 0.01f;

		float y = ((y2 - y1) * val) + y1;
#if 0
		pRenderTarget->DrawLine(
			D2D1::Point2F(rect.left + x, rect.top), 		// draw from point
			D2D1::Point2F(rect.left + x, y),				// draw to point
			brush1,
			1.0f);
#endif
		pRenderTarget->DrawLine(
			D2D1::Point2F(rect.left + x, rect.bottom),		// draw from point
			D2D1::Point2F(rect.left + x, y),				// draw to point
			brush2,
			1.0f);

		// Now draw black line indicating progress

		if (state.searching)
		{
			FLOAT x1 = rect.left;
			FLOAT x2 = rect.right;

			FLOAT progress = (FLOAT)state.searchProgress * 0.01f;

			FLOAT x = ((x2 - x1) * progress) + x1;

			if (x > x2)
				x = x2;

			if (x < x1)
				x = x1;

			pRenderTarget->DrawLine(
				D2D1::Point2F(x, rect.top),		// draw from point
				D2D1::Point2F(x, rect.bottom),	// draw to point
				m_pDarkGrayBrush,
				1.0f);
		}
	}
}

void CRemoteView::DrawTapeTime(CHwndRenderTarget* pRenderTarget)
{
	TCHAR szBuf[64];

	ASSERT_VALID(pRenderTarget);
	CMainFrame* pMainFrame = GetMainFrame();

	STC_STATE_MSG& state = pMainFrame->GetTransportState();

	TCHAR sign = (state.tapeTime.flags & F_TAPETIME_PLUS) ? '+' : '-';

	// Draw the tape time h:mm:ss.t in big font first
	_snwprintf_s(szBuf, sizeof(szBuf) / sizeof(TCHAR), _T("%c%1u:%2.2u:%2.2u"),
		sign,
		state.tapeTime.hour,
		state.tapeTime.mins,
		state.tapeTime.secs);

	pRenderTarget->DrawText(szBuf, m_rectTimeBig, m_pYellowBrush, m_pTextBigTime);
#if (DEBUG_SHOW_RECTS > 0)
	pRenderTarget->DrawRectangle(m_rectTimeBig, m_pWhiteBrush, 0.5f);		// DEBUG
#endif

	// Draw the tape time tens of secs in smaller font
	_snwprintf_s(szBuf, sizeof(szBuf) / sizeof(TCHAR), _T(":%u"), state.tapeTime.tens);

	pRenderTarget->DrawText(szBuf, m_rectTimeSmall, m_pYellowBrush, m_pTextSmallTime);
#if (DEBUG_SHOW_RECTS > 0)
	pRenderTarget->DrawRectangle(m_rectTimeSmall, m_pWhiteBrush, 0.5f);		// DEBUG
#endif
}


void CRemoteView::DrawTapeMode(CHwndRenderTarget* pRenderTarget)
{
	ASSERT_VALID(pRenderTarget);
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	CString str;

	CD2DSolidColorBrush* pBrush = m_pYellowBrush;

	STC_STATE_MSG& state = pMainFrame->GetTransportState();

	GetModeText(state, str);

	pRenderTarget->DrawText(str, m_rectTimeMode, pBrush, m_pTextMode);
#if (DEBUG_SHOW_RECTS > 0)
	pRenderTarget->DrawRectangle(m_rectTimeMode, m_pWhiteBrush, 0.5f);	// DEBUG
#endif
}


void CRemoteView::DrawProgressBar(CHwndRenderTarget* pRenderTarget)
{
	ASSERT_VALID(pRenderTarget);
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();

	CD2DRectF rect = m_rectTimeProgress;
	CD2DRectF rect2;

	STC_STATE_MSG& state = pMainFrame->GetTransportState();

#if (DEBUG_SHOW_RECTS == 0)
	if (state.transportMode & STC_M_SEARCH)
#endif
	{
		if (0)
		{
			/* Draw progress as text only */
			//GrContextFontSet(&g_context, g_psFontFixed6x8);
			//sprintf(buf, "%d%%", g_sysData.searchProgress);
			//GrStringDraw(&g_context, buf, -1, 100, y, 0);
		}
		else
		{
			/* Draw progress bar */

			rect2 = rect;

			rect2.left += 2.0f;
			rect2.top += 2.0f;
			rect2.right -= 2.0f;
			rect2.bottom -= 2.0f;

			FLOAT x1 = rect2.left;
			FLOAT x2 = rect2.right;

			FLOAT progress = (FLOAT)state.searchProgress * 0.01f;

			FLOAT x = ((x2 - x1) * progress) + x1;

			if (x > x2)
				x = x2;

			if (x < x1)
				x = x1;

			rect2.right = x - 1.0f;

			pRenderTarget->DrawRectangle(rect, m_pYellowBrush, 0.5f);
			pRenderTarget->FillRectangle(rect2, m_pYellowBrush);
		}
	}
}

#endif
