/*
[!!!] ATENÇÃO: primeira vez que o programa for instalado, deve-se usar sudo
[!!!] ATENÇÃO: comando para .bashrc: export PATH="/home/<path>:$PATH"
*/

#include <iostream>
#include <string>
#include "fonte.h"
#include <filesystem>

namespace{
	std::string diretorio_principal = "/var/Notas/";
};

int main(int argc, char** argv){
	system("clear");
	if(!std::filesystem::is_directory(diretorio_principal)){
		std::cout << "\nCriando diretórios de base.\n\n";
		std::filesystem::create_directory(diretorio_principal);
		std::filesystem::create_directory(diretorio_principal + "Boletins/");
		}

	if(argc==1){
		std::cout << "[!!!] Para uma listagem de todos os comandos existentes\n"
		<< "digite " << argv[0] << " ajuda\n\n";
		mostra_notas();
		}

	else if(argc==2){
		if(argv[1]==std::string("ajuda"))
			mostra_ajuda();
		else if(argv[1]==std::string("nota"))
			adiciona_nota();
		else if(argv[1]==std::string("boletim"))
			menu_boletim();
		else
			erro_comando();
	}

	else if(argc==3){
		if(argv[2]==std::string("rm")){
			if(argv[1]==std::string("nota"))
				remove_nota();
			else if(argv[1]==std::string("boletim"))
				remove_boletim();
			else
				erro_comando();
		}
		else if(argv[2]==std::string("vis")){
			if(argv[1]==std::string("boletim"))
				visualiza_boletim("", true);
			else
				erro_comando();
		}
		else
			erro_comando();
		}
	else
		std::cout << "[ERRO]: Quandidade incompatível de comandos\n\n";

}
















