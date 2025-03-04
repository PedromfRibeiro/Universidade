#pragma once
#include "Colaborador.h"
#include "MIntegrado.h"
#include "Projeto.h"
using namespace std;
class CI
{
	Colecao<Projeto>Projetos;
//	Colecao<Membro>membros;
	Colecao<MIntegrado>mintegrado;
	Colecao<Colaborador>colaborador;
private:
	Projeto* findProjeto(int id);
	MIntegrado* findMIntegrado(int);
	Colaborador* findColaborador(int);
public:
	bool addProjeto(int id, double fin);
	bool addMIntegrado(int id, string nome);
	bool addColaborador(int id, string nome);
	bool associarMembroAProjeto(int idMemb, int idProj);
	bool distribuirVerbaPorMIntegrados(int idProj);
	void mostrarMembros();


};

