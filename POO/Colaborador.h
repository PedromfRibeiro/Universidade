#pragma once
#include "Membro.h"


using namespace std;
class Membro;

class Colaborador : public Membro {
	
public:
	Colaborador(int i, string nome);
	void  print() const;
	bool operator<(const Colaborador &outra) const;
};

