#pragma once
#include <wx/wx.h>

class Tetris : public wxFrame {
public:
	Tetris(const wxString& title);
	wxButton *m_btn1 = nullptr;
	wxTextCtrl *m_txt1 = nullptr;

	void ClicSurBouton(wxCommandEvent &evt);
	void OnEraseBackGround(wxEraseEvent& event);
	void LancerPartie(wxCommandEvent &evt);

	wxDECLARE_EVENT_TABLE();

};
