#include "Puit.h"
#include "p2psocket.h"

Puit::Puit(wxFrame *parent) :wxPanel(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_DEFAULT)
{
	timer = new wxTimer(this, 1);

	m_stsbar = parent->GetStatusBar();
	NeTombePlus = false;
	EnPause = false;
	numLignesEff = 0;
	posX = 0;
	posY = 0;
	EffacerPuit();
	wxInitAllImageHandlers();
	SetBackgroundColour(wxColor(0,0,0));
	Connect(wxEVT_PAINT, wxPaintEventHandler(Puit::OnPaint));
	Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(Puit::OnKeyDown));
	Connect(wxEVT_TIMER, wxCommandEventHandler(Puit::OnTimer));
}
//initialise notre grille de jeu.
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
	str.Printf(wxT("%d"), bourse);
	m_stsbar->SetStatusText(str);
	timer->Start(300);
}


//fonction qui s'occupe de l'affichage de notre grille  (en grand à gauche) et des grilles adverses en plus petit à droite
void Puit::OnPaint(wxPaintEvent& event) {

	wxPaintDC dc(this);
	wxSize size = GetClientSize();
	int hautPuit = size.GetHeight() - Hauteur * HauteurCube();
	int idpiece;
	//affiche notre grille.
	DrawGrille(dc, 0, 0);

	for (int i = 0; i < Hauteur; i++) {
		for (int j = 0; j < Largeur; j++) {
			pieces forme = FormeA(j, Hauteur - i - 1);
			idpiece = int(forme);
			if (forme != PasDeForme) {
				DrawCube(dc, j * LargeurCube(), hautPuit + i * HauteurCube(), forme,true,false);
			}
				
		}
	}

	if (pieceActuelle.getForme() != PasDeForme) {
		for (int i = 0; i < 4; i++) {
			int x = posX + pieceActuelle.x(i);
			int y = posY + pieceActuelle.y(i);
			idpiece = int(pieceActuelle.getForme());
			DrawCube(dc, x*LargeurCube(), hautPuit + (Hauteur - y - 1)*HauteurCube(), idpiece,true,false);
		}
		PrevisualisationCube(dc, hautPuit);
	}
	//affiche la grille des adversaires.
	int XGrid;
	int YGrid= GetClientSize().GetHeight()/2. - TailleMiniCube()*Hauteur/2;
	mysocket* grandparent = wxDynamicCast(this->GetGrandParent(), mysocket);
	for (unsigned int u = 0; u < grandparent->nbEnnemis;u++) {
		XGrid = (Largeur + 1)*HauteurCube() + Largeur * u*TailleMiniCube();
		for (int i = 0; i < Hauteur; i++) {
			for (int j = 0; j < Largeur; j++) {
				pieces forme = grandparent->listeJoueurs[u].getMatrice()[(Hauteur - i - 1)*Largeur + j];
				idpiece = int(forme);
				if (forme != PasDeForme) {
					DrawCube(dc, XGrid + j * LargeurCube(), YGrid + hautPuit + i * HauteurCube(), forme, true,true);
				}

			}
		}
	}
}
//méthode propre a wxwidget qui choisit ce que le programme doit faire en fonction de la touche appuyée.
void Puit::OnKeyDown(wxKeyEvent& event) {
	if (!EnJeu || pieceActuelle.getForme() == PasDeForme) {
		event.Skip();
		return;
	}
	int keycode = event.GetKeyCode();
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
	case 's':
		TombeUneLigne();
		break;
	case 'S':
		TombeUneLigne();
		break;
	default:
		event.Skip();
	}
}
//méthode propre a wxwidget qui est appelée toutes les 300ms (actualisation de l'état de jeu) et qui appelle tombe une ligne si le bloc est en train e tomber, et en recrée un sinon
void Puit::OnTimer(wxCommandEvent& event) {
	if (NeTombePlus) {
		NeTombePlus = false;
		NouveauBloc();
	}
	else {
		TombeUneLigne();
	}
}
//fait tomber directement le cube le plus bas possible en dessous de sa position
void Puit::TombeCube() {
	int nY = posY;
	while (nY > 0 && TenteBouger(pieceActuelle, posX, posY - 1)) {
		nY--;
	}
	PlacerPiece();
}

//permet d'afficher ou le cube va tomber si l'on ne fait rien
void Puit::PrevisualisationCube(wxPaintDC &dc, int hautPuit){
	int nY = posY;
	while (nY > 0 && PeutBouger(pieceActuelle, posX, nY - 1)) {
		nY--;
	}
	for (int i = 0; i < 4; i++) {
		int x = posX + pieceActuelle.x(i);
		int y = nY + pieceActuelle.y(i);
		DrawCube(dc, x*LargeurCube(), hautPuit + (Hauteur - y - 1)*HauteurCube(), pieceActuelle.getForme(), false,false);
		wxString str;
		str.Printf(wxT("%d "), bourse);
		m_stsbar->SetStatusText(str);
	}
}

//fait tomber le le bloc actuel d'une ligne s'il le peut
void Puit::TombeUneLigne() {
	if (!TenteBouger(pieceActuelle, posX, posY - 1)) {
		PlacerPiece();
	}
}

//fonction appelée pour placer le bloc actuel dans la grille, place le nouveau bloc dans la matrice de jeu, retire les lignes pleines communique l'état de la grille de jeu aux autres joueurs à l'aide de comGrille et appelle nouveaubloc si le  bloc actuel ne peux plus tomber.
void Puit::PlacerPiece() {
	for (int i = 0; i < 4; i++) {
		int x = posX + pieceActuelle.x(i);
		int y = posY + pieceActuelle.y(i);
		FormeA(x, y) = pieceActuelle.getForme();
	}

	EffacerLignesPleines();
	mysocket* grandparent = wxDynamicCast(this->GetGrandParent(), mysocket);
	grandparent->comGrille(getMatrice());

	if (!NeTombePlus) {
		NouveauBloc();
	}
}
//réinitialise la matrice à 0
void Puit::EffacerPuit() {
	for (int i = 0; i < Hauteur*Largeur; i++) {
		matrice[i]=PasDeForme;
	}
}

//efface les lignes lorsqu'elles sont pleines et appelle gainscore pour incrémenter le score
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
	}
	if (nombreLignesPleines > 0) {
		GainScore(nombreLignesPleines);
		wxString str;
		str.Printf(wxT("%d"), bourse);

		NeTombePlus = true;
		pieceActuelle.BlocInit(PasDeForme);
	}
}

//nouveau bloc essaie de créer un nouveau bloc dans la grille et si il ne le peut pas indique au joueur qu'il a perdu
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

//peutbouger teste si l'on peut deplacer notre piece à une nouvelle position donnée mais ne la déplace pas (utile pour la prévisualisation d'où le bloc va tomber)
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
//tentebouger teste si l'on peut deplacer notre piece à une nouvelle position donnée et si oui la déplace
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
//gain score est appelé pour incrémenter le score du joueur lorsqu'il efface des lignes.
void Puit::GainScore(int nblignes) {
	switch (nblignes) {
	case 1:
		bourse += 1;
		break;
	case 2:
		bourse += 3;
		break;
	case 3:
		bourse += 6;
		break;
	case 4:
		bourse += 12;
		break;
	}
}

//dessine un cube à la position (x,y) la couleur est obtenue à l'aide de l'identifiant idpièce, plein indique si on doit remplir le cube ou non (le cube vide sera utile dans prévisualisation cube) et mini indique si on doit dessiner le cube en grand (pour l'affichage de l'utilisateur) ou petit (pour les adversaires).
void Puit::DrawCube(wxPaintDC& dc, int x, int y, int idpiece,bool plein, bool mini) {
	// Ordre des couleurs PasDeForme, FormeI, FormeO, FormeT, FormeL, FormeJ, FormeZ, FormeS
	const wxColour reflet[] = {wxColour(0,0,0),wxColour(255,0,255),wxColour(0,255,255),wxColour(255,255,0),wxColour(255,255,102),wxColour(255,102,255),wxColour(102,102,255),wxColour(255,255,102) };
	const wxColour couleur[] = { wxColour(0,0,0),wxColour(169,255,547),wxColour(244,238,210),wxColour(252,255,221),wxColour(255,209,227),wxColour(105,221,255),wxColour(255,167,171),wxColour(167,255,175) };
	const wxColour ombre[] = { wxColour(0,0,0),wxColour(128,0,128),wxColour(0,128,128),wxColour(128,128,0),wxColour(128,128,30),wxColour(128,30,128),wxColour(30,30,128),wxColour(128,128,30) };
	int H;
	if (mini) {
		H = TailleMiniCube();
	}
	else {
		H = HauteurCube();
	}
	wxPen pinceau(reflet[idpiece]);
	pinceau.SetCap(wxCAP_PROJECTING);
	dc.SetPen(pinceau);

	dc.DrawLine(x, y + H - 1, x, y);
	dc.DrawLine(x, y, x+ H-1, y);

	wxPen pinceauOmbre(ombre[idpiece]);
	pinceauOmbre.SetCap(wxCAP_PROJECTING);
	dc.SetPen(pinceauOmbre);
	dc.DrawLine(x+1, y + H - 1, x+H-1, y+ H - 1);
	dc.DrawLine(x + H - 1, y + H-1, x + H - 1, y+1);
	if (plein) {
		dc.SetPen(*wxTRANSPARENT_PEN);
		dc.SetBrush(wxBrush(couleur[idpiece]));
		dc.DrawRectangle(x + 1, y + 1, H - 2, H - 2);
	}
}

//dessine une grille en fond pour une meilleure visualisation de l'espace
void Puit::DrawGrille(wxPaintDC& dc, int x, int y) {
	wxPen pinceau(wxColor(3,194,212));
	pinceau.SetCap(wxCAP_PROJECTING);
	dc.SetPen(pinceau);
	for (int i = 1; i < Hauteur; i++) {
		dc.DrawLine(x, y +i* HauteurCube()+2, x + LargeurCube()*Largeur, y + i * HauteurCube()+2);
	}
	for (int i = 1; i < Largeur; i++) {
		dc.DrawLine(x+i* LargeurCube(), y, x + i * LargeurCube(), y + Hauteur * HauteurCube());
	}
}