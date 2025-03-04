#include "Projeto.h"



Projeto::Projeto(int i, double fin) {
	id = i;
	financiamento = fin;
}

bool Projeto::associarMIntegrado(MIntegrado *m)
{
	return mintegrado.insert(m);
}

bool Projeto::associarColaborador(Colaborador *c)
{
	return colaborador.insert(c);
}

void Projeto::distribuirVerbaPorMIntegrados()
{
	Colecao <MIntegrado*>::iterator cont;
	int n = mintegrado.size();
	double a = financiamento / n;
	for (cont = mintegrado.begin(); cont != mintegrado.end(); cont++) {
		(*cont)->adicionarSaldo(a);
	}
	
}
bool Projeto::operator<(const Projeto &outra) const {
	return id < outra.id;
}