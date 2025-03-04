#include <iostream>
#include "CI.h"
#include <string>
#include<iostream>


Projeto * CI::findProjeto(int id)
{
	Projeto p = Projeto(id, NULL);
	return Projetos.find(p);
}

MIntegrado * CI::findMIntegrado(int id)
{
	MIntegrado m = MIntegrado(id, "");
	return mintegrado.find(m);
}

Colaborador * CI::findColaborador(int id)
{
	Colaborador c = Colaborador(id, "");
	return colaborador.find(c);
}

bool CI::addProjeto(int id, double fin) {
	Projeto *pI = findProjeto(id);
	if (pI != NULL) { cout << "!!Erro do resgisto do projeto, este ja e existente." << endl; return false; }
	else {
		Projeto p(id, fin);
		cout<<"Projeto Criado com sucesso!"<<endl;
		return Projetos.insert(p);
	}
}

bool CI::addMIntegrado(int id, string nome)
{
	MIntegrado *pIdm = findMIntegrado(id);
	if (pIdm != NULL) { cout << "!!Erro do resgisto de "<<nome<<", ID "<<id<<" ja existente." << endl; return false; }
	else
	{
		MIntegrado m (id, nome);
		cout << id << " - " << nome << " - Registo criado com sucesso!" << endl;
		return mintegrado.insert(m);
	}
}
	
bool CI::addColaborador(int id, string nome)
{
	Colaborador *pIdC = findColaborador(id);
	if (pIdC != NULL) { cout << "!!Erro do resgisto de " << nome << ", ID " << id << " ja existente." << endl; return false;
	}
	else
	{
		Colaborador co (id, nome);
		cout << id <<" - " <<nome<<" - Registo criado com sucesso!"<<endl;
		return colaborador.insert(co);
	}
}

bool CI::associarMembroAProjeto(int idMemb, int idProj)
{ 
	bool flag = false;
	MIntegrado *m = findMIntegrado(idMemb);
	Colaborador *c = findColaborador(idMemb);
	Projeto *p = findProjeto(idProj);
	if (p != NULL) {
		if (m != NULL) {
			m->associarProjeto(p) && p->associarMIntegrado(m); 
			cout<<"Membro:"<<idMemb<<" associado ao projeto:"<<idProj<<endl;
			flag = true;
		}
		else if (c != NULL) { 
			c->associarProjeto(p) && p->associarColaborador(c); flag = true;
			cout << "Membro:" << idMemb << " associado ao projeto:" << idProj << endl;
		}
	}
	return flag;
}

bool CI::distribuirVerbaPorMIntegrados(int idProj)
{
	bool flag=false;
	Projeto *p = findProjeto(idProj);
	if (p != NULL) {
		p->distribuirVerbaPorMIntegrados();
		cout << "Verba Distibuida" << endl;
		flag = true;
	}
	else {
		cout << "Verba nao Distibuida, erro ID" << endl;
		flag = false;
	}
	return flag;
}

void CI::mostrarMembros()
{
	Colecao <MIntegrado>::iterator cont;
	for (cont = mintegrado.begin(); cont != mintegrado.end(); cont++) {
		cont->print();
	}
	Colecao <Colaborador>::iterator count;
	for (count = colaborador.begin(); count != colaborador.end(); count++) {
		count->print();
	}	
}
