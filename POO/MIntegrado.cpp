#include "MIntegrado.h"
#include <iostream>

MIntegrado::MIntegrado(int i, string n) :Membro(i, n) {
	saldo = 0;
}

bool MIntegrado::adicionarSaldo(double val) {
	 saldo += val;
	 return true;
}

void MIntegrado::print() const
{
	cout << "Membro Integrado numero: " << id <<" - "<< name << ", com um Saldo de: " << saldo <<" euros"<< endl;
//	Membro::print();	
}

bool MIntegrado::operator<(const MIntegrado & outra) const
{
	return id < outra.id;
}



