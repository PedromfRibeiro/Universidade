#include "CI.h"
#include <iostream>
#include <string>
#include <cstdlib>

using namespace std;
int main() {
	int escolha, sub_escolha, Ival,Ival2;
	double Pfin;
	string Sval;
	CI o;
	cout << "		Menu" << endl << "1 - Automatico" << endl << "2 - Manual" <<endl<<"3 - Informacoes"<<endl<<"0 - Exit"<< endl;
	cin >> escolha;
	
	switch (escolha)
	{
	case 1 :

	cout<<"-------------------------------"<<endl;
			o.addColaborador(1, "Paulo");
			o.addColaborador(2, "Joao");
		
			o.addMIntegrado(3, "Bruno");
			o.addMIntegrado(4, "Domingos");

			//Repetição de ID's
			o.addColaborador(1, "Diogo");
			o.addMIntegrado(3, "Vitor");
	cout << "-------------------------------" << endl;
			o.addProjeto(1, 10010);
			o.addProjeto(2, 5000);
	cout << "-------------------------------" << endl;
			o.associarMembroAProjeto(1, 1);
			o.associarMembroAProjeto(2, 1);
			o.associarMembroAProjeto(3, 1);
			o.associarMembroAProjeto(4, 1);		
	cout << "-------------------------------" << endl;
			o.distribuirVerbaPorMIntegrados(1);
			o.distribuirVerbaPorMIntegrados(3);
	cout << "-------------------------------" << endl;
			o.mostrarMembros();
			system("pause");
			

			break;
	case 2:
		do { 
			cout << "		Sub-Menu"
				 <<endl<< "1 - Adicionar Colaborador" 
				 <<endl<< "2 - Adicionar Membro Integrado" 
				 <<endl<< "3 - Adicionar Projeto"
				 <<endl<< "4 - Associar Membro a Projeto"
				 <<endl<< "5 - Distribuir Verba Por Membros"
				 <<endl<< "6 - Mostrar Membros"
				 <<endl<< "0 - Exit"
				 <<endl;
			cin >> sub_escolha;
			switch (sub_escolha)
			{
			case 1:
				cout << "-------------Adicionar Colaborador------------------" << endl;
				cout << "ID" << endl;
				cin >> Ival;
				cout << "Nome" << endl;
				cin >> Sval;
				o.addColaborador(Ival, Sval);
				cout << "-------------------------------" << endl;
				
				break;
			case 2:
				cout << "--------------Adicionar Membro Integrado-----------------" << endl;
				cout << "ID: ";
				cin >> Ival;
				cout << "Nome: ";
				cin >> Sval;
				o.addMIntegrado(Ival, Sval); 
				cout << "-------------------------------" << endl;
				break;
			case 3:
				cout << "------------Adicionar Projeto-------------------" << endl;
				cout << "ID: ";
				cin >> Ival;
				cout << "Finaciamento: ";
				cin >> Pfin;
				o.addProjeto(Ival, Pfin);
				cout << "-------------------------------" << endl;
				
				break;
			case 4:
				cout << "-------------Associar Membro a Projeto------------------" << endl;
				cout << "id Membro: ";
				cin >> Ival;
				cout << "id Projeto: ";
				cin >> Ival2;
				o.associarMembroAProjeto(Ival, Ival2);
				cout << "-------------------------------" << endl;
				
				break;
			case 5:
				cout << "----------------Associar Membro a Projeto---------------" << endl;
				cout << "id Projeto: ";
				cin >> Ival;
				o.distribuirVerbaPorMIntegrados(Ival);
				cout << "-------------------------------" << endl;
				break;

			case 6:
				cout << "-------------Mostrar Membros------------------" << endl;
				o.mostrarMembros();
				break;

			case 0:break;
			default:
				break;
			}
		
		
		
		} while (sub_escolha != 0);
		break;
	case 3:
		cout << "Trabalho realizado por: " << endl << "Pedro Ribeiro a37556" << endl << "Diogo Quintas a32652" << endl << "Marcelos Martins a37544" << endl;
	default:
		break;
	}
	system("pause");

}