#include "Tetris.h"
#include "Joueur.h"
#include "Puit.h"

wxBEGIN_EVENT_TABLE(Tetris,wxFrame)
	EVT_BUTTON(10001,ClicSurBouton)
	EVT_BUTTON(10002, LancerPartie)
	EVT_PAINT(OnPaint)
	EVT_ERASE_BACKGROUND(Tetris::OnEraseBackGround)
wxEND_EVENT_TABLE()

void Tetris::OnEraseBackGround(wxEraseEvent& event)
{
	// Turlututu...
}

Tetris::Tetris(const wxString& title) : wxFrame(nullptr, wxID_ANY, title, wxDefaultPosition, wxSize(1500, 900))
{
	m_btn1 = new wxButton(this, 10001, "Jouer", wxPoint(GetClientSize().GetHeight()/2., GetClientSize().GetHeight()*2./3.), wxSize(150, 50));
	m_txt1 = new wxTextCtrl(this, wxID_ANY, "", wxPoint(GetClientSize().GetHeight() / 2., GetClientSize().GetHeight() * 1. / 3.), wxSize(300, 30));

}

void Tetris::ClicSurBouton(wxCommandEvent &evt) {
	wxStatusBar *sb = CreateStatusBar();
	sb->SetStatusText(wxT("0"));
	Joueur* player1=new Joueur((*m_txt1).GetValue(),this);
	(*player1).getPuit()->SetFocus();
	(*player1).getPuit()->Start();
	m_txt1->Destroy();
	m_btn1->Destroy();
	m_btn1 = new wxButton(this, 10002, "Lancer Partie", wxPoint(GetClientSize().GetHeight() / 2, GetClientSize().GetHeight() * 2 / 3), wxSize(150, 50));
	evt.Skip();
}

void Tetris::LancerPartie(wxCommandEvent &evt) {

}