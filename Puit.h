#pragma once
#include "Bloc.h"
#include "wx/wx.h"
#include <algorithm>


class Puit : public wxPanel
{
public:
	Puit(wxFrame *parent);
	void Start();
	void Pause();
	enum { Largeur = 10, Hauteur = 22 };

	
protected:
	void OnPaint(wxPaintEvent& event);
	void OnKeyDown(wxKeyEvent& event);
	void OnTimer(wxCommandEvent& event);


private:
	pieces & FormeA(int x, int y) { return matrice[y*Largeur + x]; }
	int LargeurCube() { return GetClientSize().GetHeight() / Hauteur; }
	int HauteurCube() { return GetClientSize().GetHeight() / Hauteur; }
	int TailleMiniCube() { return std::min(double(GetClientSize().GetHeight() / Hauteur), (GetClientSize().GetWidth()-(Largeur+1.)*LargeurCube())/10.); }
	void EffacerPuit();
	void NouveauBloc();
	void DrawCube(wxPaintDC &dc, int x, int y, int idpiece,bool plein,bool mini);
	void DrawGrille(wxPaintDC &dc, int x, int y);
	void PrevisualisationCube(wxPaintDC &dc,int hautPuit);
	void TombeCube();
	bool TenteBouger(const Bloc& nouvellePiece, int nouveauX, int nouveauY);
	bool PeutBouger(const Bloc& nouvellePiece, int nouveauX, int nouveauY);
	void TombeUneLigne();
	void PlacerPiece();
	void EffacerLignesPleines();
	void GainScore(int nblignes);
	pieces* getMatrice() { return matrice; }

	wxTimer *timer;
	wxBitmap fond;
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