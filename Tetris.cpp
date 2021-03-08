#include "Tetris.h"
#include "Joueur.h"
#include "Puit.h"

wxBEGIN_EVENT_TABLE(Tetris,wxFrame)
	EVT_PAINT(OnPaint)

wxEND_EVENT_TABLE()



Tetris::Tetris(wxWindow* parent, const wxString& title) : wxFrame(parent, wxID_ANY, title, wxDefaultPosition, wxSize(1500, 900))
{
	wxStatusBar *sb = CreateStatusBar();
	sb->SetStatusText(wxT("0"));
	Joueur* player1 = new Joueur("Utilisateur", this);
	//notre clavier a désormais influence sur cette fenêtre.
	(*player1).getPuit()->SetFocus();
	(*player1).getPuit()->Start();
}
