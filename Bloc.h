#pragma once

enum pieces {PasDeForme=0,FormeI=1,FormeO=2,FormeT=3,FormeL=4,FormeJ=5,FormeZ=6,FormeS=7};

class Bloc
{
private:
	void MajX(int i, int x) { coords[i][0] = x; }
	void MajY(int i, int y) { coords[i][1] = y; }
	pieces formeBloc;
	int coords[4][2];
public:
	Bloc() { BlocInit(PasDeForme); }
	void BlocInit(pieces forme);
	void ChoisirFormeRandom();
	pieces getForme() const { return formeBloc; }
	int x(int i) const { return coords[i][0]; }
	int y(int i) const { return coords[i][1]; }

	int XMin() const;
	int YMin() const;
	int XMax() const;
	int YMax() const;

	Bloc RotationAntiHoraire() const;
	Bloc RotationHoraire() const;
};