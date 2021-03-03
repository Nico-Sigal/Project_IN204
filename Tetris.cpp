#include "Tetris.h"
#include "Puit.h"

Tetris::Tetris(const wxString& title) : wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize(180, 380))
{
	wxStatusBar *sb = CreateStatusBar();
	sb->SetStatusText(wxT("0"));
	Puit *puit = new Puit(this);
	puit->SetFocus();
	puit->Start();
}