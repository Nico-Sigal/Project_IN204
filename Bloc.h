#pragma once

enum pieces {PasDeForme,FormeI,FormeO,FormeT,FormeL,FormeJ,FormeZ,FormeS};

class Bloc
{
private:
	void MajX(int i, int x) { coords[i][0] = x; }
	void MajY(int i, int y) { coords[i][0] = y; }
	pieces formeBloc;
	int coords[4][2];
public:
	Bloc() { BlocInit(PasDeForme); }
	void BlocInit(pieces forme);
	void ChoisirFormeRandom();
	pieces ObtenirForme() const { return formepiece; }
	int x(int i) const { return coords[i][0]; }
	int y(int i) const { return coords[i][1]; }

	int XMin() const;
	int YMin() const;
	int XMax() const;
	int YMax() const;

	Bloc RotationAntiHoraire() const;
	Bloc RotationHoraire() const;
};