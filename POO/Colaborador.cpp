#include "Colaborador.h"
#include<iostream>


Colaborador::Colaborador(int i, string n) :Membro(i, n) {}

void  Colaborador::print()const
{
	cout << "Colaborador numero: " << id << " - " << name << endl;
	//Membro::print();

}

bool Colaborador::operator<(const Colaborador & outra) const
{
	return id < outra.id;
}





