#include "Bloc.h"
#include <random>
#include <algorithm>

//initialise les coordonnées de la pièce à l'aide d'une table des coordonnées qui contient les coordonnées de chaque forme
void Bloc::BlocInit(pieces forme)
{
	static const int Tablecoords[8][4][2] = {
		// Ordre des couleurs PasDeForme, FormeI, FormeO, FormeT, FormeL, FormeJ, FormeZ, FormeS
		{{0,0}, {0,0}, {0,0}, {0,0}},
		{{-1,0}, {0,0}, {1,0}, {2,0}},
		{{0,0}, {0,1}, {1,0}, {1,1}},
		{{0,-1}, {0,0}, {0,1}, {1,0}},
		{{-1,-1}, {-1,0}, {0,0}, {1,0}},
		{{-1,1}, {-1,0}, {0,0}, {1,0}},
		{{-1,1}, {0,1}, {0,0}, {1,0}},
		{{-1,0}, {0,0}, {0,-1}, {-1,1}}
	};
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 2; j++)
			coords[i][j] = Tablecoords[forme][i][j];
	}
	formeBloc = forme;
}
	//choisit une forme aléatoure de bloc parmi les formes disponiblaces
	void Bloc::ChoisirFormeRandom() {
		int n = 1 + rand() % 7;
		BlocInit(pieces(n));
	}
	//return la position X minimum du bloc actuel
	int Bloc::XMin() const
	{
		int m = coords[0][0];
		for (int i = 1; i < 4; i++) {
			m = std::min(m, coords[i][0]);
		}
		return m;
	}
	//return la position Y minimum du bloc actuel
	int Bloc::YMin() const
	{
		int m = coords[0][0];
		for (int i = 1; i < 4; i++) {
			m = std::min(m, coords[i][1]);
		}
		return m;
	}
	//return la position X maximum du bloc actuel
	int Bloc::XMax() const
	{
		int m = coords[0][0];
		for (int i = 1; i < 4; i++) {
			m = std::max(m, coords[i][0]);
		}
		return m;
	}
	//return la position Y maximum du bloc actuel
	int Bloc::YMax() const
	{
		int m = coords[0][0];
		for (int i = 1; i < 4; i++) {
			m = std::max(m, coords[i][1]);
		}
		return m;
	}
	//fait tourner le bloc dans le sens horaire
	Bloc Bloc::RotationHoraire() const
	{
		if (formeBloc == FormeO)
			return *this;

		Bloc resultat;
		resultat.formeBloc = formeBloc;
		for (int i = 0; i < 4; i++) {
			resultat.MajX(i, -y(i));
			resultat.MajY(i, x(i));
		}
		return resultat;
	}
	//fait tourner le bloc dans le sens antihoraire
	Bloc Bloc::RotationAntiHoraire() const {
		if (formeBloc == FormeO)
			return *this;
		Bloc resultat;
		resultat.formeBloc = formeBloc;
		for (int i = 0; i < 4; i++) {
			resultat.MajX(i, y(i));
			resultat.MajY(i, -x(i));
		}
		return resultat;
	}

