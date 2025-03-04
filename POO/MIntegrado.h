#pragma once 
#include "Membro.h"
#include "Colecao.h"


class MIntegrado : public Membro {

private:
	double saldo;

public:
	MIntegrado(int, string);
	bool adicionarSaldo(double);
	void  print() const;
	bool operator<(const MIntegrado &outra) const;


};

