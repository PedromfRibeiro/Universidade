#include "Membro.h"
#include <iostream>


Membro::Membro(int i, string n) {
	id = i;
	name = n;
}

bool Membro::associarProjeto(Projeto * p)
{
	return Projetos.insert(p);
}

void  Membro::print() const
{
	cout << id << " - " << name;
}


