#pragma once
#include "Bloc.h"
#include "wx/wx.h"

class Puit : public wxPanel
{
public:
	Puit(wxFrame *parent);
	void Start();
	void Pause();
	void GainScore(int nblignes);

	
protected:
	void OnPaint(wxPaintEvent& event);
	void OnKeyDown(wxKeyEvent& event);
	void OnTimer(wxCommandEvent& event);


private:
	enum { Largeur = 10, Hauteur = 22 };
	pieces & FormeA(int x, int y) { return matrice[y*Largeur + x]; }
	int LargeurCube() { return GetClientSize().GetWidth() / Largeur; }
	int HauteurCube() { return GetClientSize().GetHeight() / Hauteur; }
	void EffacerPuit();
	void NouveauBloc();
	void DrawCube(wxPaintDC &dc, int x, int y, int idpiece,bool plein);
	void PrevisualisationCube(wxPaintDC &dc,int hautPuit);
	void TombeCube();
	bool TenteBouger(const Bloc& nouvellePiece, int nouveauX, int nouveauY);
	bool PeutBouger(const Bloc& nouvellePiece, int nouveauX, int nouveauY);
	void TombeUneLigne();
	void PlacerPiece();
	void EffacerLignesPleines();

	wxTimer *timer;
	bool EnJeu;
	bool EnPause;
	bool NeTombePlus;
	Bloc pieceActuelle;
	int posX;
	int posY;
	int numLignesEff;
	int bourse;
	pieces matrice[Hauteur*Largeur];
	wxStatusBar *m_stsbar;
};