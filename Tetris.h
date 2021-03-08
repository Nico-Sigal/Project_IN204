#pragma once
#include <wx/wx.h>

class Tetris : public wxFrame {
public:
	Tetris(wxWindow* parent, const wxString& title);
	wxButton *m_btn1 = nullptr;
	wxTextCtrl *m_txt1 = nullptr;


	wxDECLARE_EVENT_TABLE();

};
