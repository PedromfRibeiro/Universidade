#pragma once
#include <string>
#include "MIntegrado.h"
#include "colaborador.h"
#include "Colecao.h"
using namespace std;
class Projeto {
	Colecao<MIntegrado*>mintegrado;
	Colecao<Colaborador*>colaborador;
private:

	int id;
	double financiamento;

public:
	Projeto(int, double);
	bool associarMIntegrado(MIntegrado *m);
	bool associarColaborador(Colaborador *c);
	void distribuirVerbaPorMIntegrados();
	bool operator<(const Projeto &outra) const;
};
