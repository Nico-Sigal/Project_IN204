#pragma once
#include "wx/wx.h"
#include "Puit.h"

class Joueur
{
private:
	wxString name;
	Puit* puit;

public:
	Joueur(wxString nom, wxFrame* parent)
	{
		name = nom;
		puit = new Puit(parent);
	}
	wxString getNom() { return name; }
	Puit* getPuit() { return puit; }
};