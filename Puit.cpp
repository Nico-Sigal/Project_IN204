#include "Puit.h"

Puit::Puit(wxFrame *parent) :wxPanel(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_NONE)
{
	timer = new wxTimer(this, 1);

	m_stsbar = parent->GetStatusBar();
	NeTombePlus = false;
	EnPause = false;
	numLignesEff = 0;
	posX = 0;
	posY = 0;
	EffacerPuit();
	Connect(wxEVT_PAINT, wxPaintEventHandler(Puit::OnPaint));
	Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(Puit::OnKeyDown));
	Connect(wxEVT_TIMER, wxCommandEventHandler(Puit::OnTimer));
}

void Puit::Start()
{
	if (EnPause)
		return;
	EnJeu = true;
	NeTombePlus = false;
	numLignesEff = 0;
	EffacerPuit();

	NouveauBloc();
	wxString str;
	str.Printf(wxT("%d"), pieceActuelle.getForme());
	m_stsbar->SetStatusText(str);
	timer->Start(300);
}

void Puit::Pause()
{
	if (!EnJeu)
		return;
	EnPause = !EnPause;
	if (EnPause) {
		timer->Stop();
		m_stsbar->SetStatusText(wxT("Pause"));
	}
	else {
		timer->Start(300);
		wxString str;
		str.Printf(wxT("%d"), numLignesEff);
		m_stsbar->SetStatusText(str);
	}
	Refresh();
}

void Puit::OnPaint(wxPaintEvent& event) {
	wxPaintDC dc(this);
	wxSize size = GetClientSize();
	int hautPuit = size.GetHeight() - Hauteur * HauteurCube();

	for (int i = 0; i < Hauteur; i++) {
		for (int j = 0; j < Largeur; j++) {
			pieces forme = FormeA(j, Hauteur - i - 1);
			if (forme != PasDeForme) {
				DrawCube(dc, j * LargeurCube(), hautPuit + i * HauteurCube(), forme,true);
			}
				
		}
	}

	if (pieceActuelle.getForme() != PasDeForme) {
		for (int i = 0; i < 4; i++) {
			int x = posX + pieceActuelle.x(i);
			int y = posY + pieceActuelle.y(i);
			DrawCube(dc, x*LargeurCube(), hautPuit + (Hauteur - y - 1)*HauteurCube(), pieceActuelle.getForme(),true);
		}
		PrevisualisationCube(dc, hautPuit);
	}
}

void Puit::OnKeyDown(wxKeyEvent& event) {
	if (!EnJeu || pieceActuelle.getForme() == PasDeForme) {
		event.Skip();
		return;
	}
	int keycode = event.GetKeyCode();
	if (keycode == 'p' || keycode == 'P') {
		Pause();
		return;
	}

	if (EnPause)
		return;
	switch (keycode) {
	case WXK_LEFT:
		TenteBouger(pieceActuelle, posX - 1, posY);
		break;
	case WXK_RIGHT:
		TenteBouger(pieceActuelle, posX + 1, posY);
		break;
	case WXK_DOWN:
		TenteBouger(pieceActuelle.RotationHoraire(), posX, posY);
		break;
	case WXK_UP:
		TenteBouger(pieceActuelle.RotationAntiHoraire(), posX, posY);
		break;
	case WXK_SPACE:
		TombeCube();
		break;
	default:
		event.Skip();
	}
}

void Puit::OnTimer(wxCommandEvent& event) {
	if (NeTombePlus) {
		NeTombePlus = false;
		NouveauBloc();
	}
	else {
		TombeUneLigne();
	}
}

void Puit::TombeCube() {
	int nY = posY;
	while (nY > 0 && TenteBouger(pieceActuelle, posX, posY - 1)) {
		nY--;
	}
	PlacerPiece();
}


void Puit::PrevisualisationCube(wxPaintDC &dc, int hautPuit){
	int nY = posY;
	while (nY > 0 && PeutBouger(pieceActuelle, posX, nY - 1)) {
		nY--;
	}
	for (int i = 0; i < 4; i++) {
		int x = posX + pieceActuelle.x(i);
		int y = nY + pieceActuelle.y(i);
		DrawCube(dc, x*LargeurCube(), hautPuit + (Hauteur - y - 1)*HauteurCube(), pieceActuelle.getForme(), false);
		wxString str;
		str.Printf(wxT("%d "), x);
		str.Printf(wxT("%d"), y);
		m_stsbar->SetStatusText(str);
	}
}

void Puit::TombeUneLigne() {
	if (!TenteBouger(pieceActuelle, posX, posY - 1)) {
		PlacerPiece();
	}
}

void Puit::PlacerPiece() {
	for (int i = 0; i < 4; i++) {
		int x = posX + pieceActuelle.x(i);
		int y = posY + pieceActuelle.y(i);
		FormeA(x, y) = pieceActuelle.getForme();
	}

	EffacerLignesPleines();

	if (!NeTombePlus) {
		NouveauBloc();
	}
}

void Puit::EffacerPuit() {
	for (int i = 0; i < Hauteur*Largeur; i++) {
		matrice[i]=PasDeForme;
	}
}

void Puit::EffacerLignesPleines(){
	int nombreLignesPleines = 0;
	for (int i = Hauteur - 1; i >= 0; i--) {
		bool lignePleine = true;

		for (int j = 0; j < Largeur; j++) {
			if (FormeA(j, i) == PasDeForme) {
				lignePleine = false;
				break;
			}
		}

		if (lignePleine) {
			nombreLignesPleines++;
			for (int k = i; k < Hauteur - 1; k++) {
				for (int j = 0; j < Largeur; j++) {
					FormeA(j, k) = FormeA(j, k + 1);
				}
			}
		}
		if (nombreLignesPleines > 0) {
			//GainScore(nombreLignesPleines);
			wxString str;
			str.Printf(wxT("%d"), bourse);

			NeTombePlus = true;
			pieceActuelle.BlocInit(PasDeForme);
		}
	}
}

void Puit::NouveauBloc() {
	pieceActuelle.ChoisirFormeRandom();
	posX = Largeur / 2 + 1;
	posY = Hauteur - 1 + pieceActuelle.YMin();

	if(!TenteBouger(pieceActuelle,posX,posY-1)){
		Update();
		pieceActuelle.BlocInit(PasDeForme);
		timer->Stop();
		EnJeu = false;
		m_stsbar->SetStatusText(wxT("Game Over"));
	}
}

bool Puit::PeutBouger(const Bloc& nouvellePiece, int nouveauX, int nouveauY) {
	for (int i = 0; i < 4; i++) {
		int x = nouveauX + nouvellePiece.x(i);
		int y = nouveauY + nouvellePiece.y(i);
		if (x < 0 || x >= Largeur || y < 0 || y >= Hauteur) {
			return false;
		}
		if (FormeA(x, y) != PasDeForme) {
			return false;
		}
	}
	return true;
}

bool Puit::TenteBouger(const Bloc& nouvellePiece, int nouveauX, int nouveauY) {
	for (int i = 0; i < 4; i++) {
		int x = nouveauX + nouvellePiece.x(i);
		int y = nouveauY + nouvellePiece.y(i);
		if (x < 0 || x >= Largeur || y<0 || y>=Hauteur) {
			return false;
		}
		if (FormeA(x, y) != PasDeForme) {
			return false;
		}
	}
	pieceActuelle = nouvellePiece;
	posX = nouveauX;
	posY = nouveauY;
	Refresh();
	return true;
}

void Puit::GainScore(int nblignes) {
	switch (nblignes) {
	case 1:
		bourse += 1;
	case 2:
		bourse += 3;
	case 3:
		bourse += 6;
	case 4:
		bourse += 12;
	}
}

void Puit::DrawCube(wxPaintDC& dc, int x, int y, pieces forme,bool plein) {
	const wxColour reflet[] = {wxColour(0,0,0),wxColour(0,255,255),wxColour(255,255,0),wxColour(255,255,102),wxColour(255,102,255),wxColour(102,102,255),wxColour(255,255,102) };
	const wxColour couleur[] = { wxColour(0,0,0),wxColour(0,200,200),wxColour(200,200,0),wxColour(200,200,61),wxColour(200,61,200),wxColour(61,61,200),wxColour(200,200,61) };
	const wxColour ombre[] = { wxColour(0,0,0),wxColour(0,128,128),wxColour(128,128,0),wxColour(128,128,30),wxColour(128,30,128),wxColour(30,30,128),wxColour(128,128,30) };

	int value;
	value = int(forme);
	wxPen pinceau(reflet[value]);
	pinceau.SetCap(wxCAP_PROJECTING);
	dc.SetPen(pinceau);

	dc.DrawLine(x, y + HauteurCube() - 1, x, y);
	dc.DrawLine(x, y, x+ LargeurCube()-1, y);

	wxPen pinceauOmbre(ombre[value]);
	pinceauOmbre.SetCap(wxCAP_PROJECTING);
	dc.SetPen(pinceauOmbre);
	dc.DrawLine(x+1, y + HauteurCube() - 1, x+LargeurCube()-1, y+ HauteurCube() - 1);
	dc.DrawLine(x + LargeurCube() - 1, y + HauteurCube()-1, x + LargeurCube() - 1, y+1);
	if (plein) {
		dc.SetPen(*wxTRANSPARENT_PEN);
		dc.SetBrush(wxBrush(couleur[value]));
		dc.DrawRectangle(x + 1, y + 1, LargeurCube() - 2, HauteurCube() - 2);
	}
}