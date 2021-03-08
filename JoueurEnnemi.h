#pragma once
#include "wx/wx.h"
#include "wx/socket.h"
#include "Puit.h"

class JoueurEnnemi
{
	enum { Largeur = 10, Hauteur = 22 };
private:
	wxString name;
	pieces matrice[Largeur*Hauteur];
	wxIPV4address adresse;

public:
	JoueurEnnemi(wxString nom, wxIPV4address adr)
	{
		name = nom;
		adresse = adr;
	}
	wxString getNom() { return name; }
	wxIPV4address getAdresse() { return adresse; }
	pieces* getMatrice() { return matrice; }
	void setMatrice(pieces* mat) { memcpy(matrice,mat,Largeur*Hauteur*sizeof(pieces)); }
};