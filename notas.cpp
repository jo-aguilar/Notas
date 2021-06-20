#include <iostream>
#include <unistd.h>
#include <string>
#include <cstring>
#include <fstream>
#include <vector>
#include <dirent.h>
#include <sys/stat.h>
#define DEBUG(X) std::cout << #X << std::endl

namespace{
	std::string path = "/home/joao/Documents/Notas";
	std::string path_boletins = "/home/joao/Documents/Notas/Boletins";
	std::vector<std::string> linhas;
	typedef struct {
		unsigned int indice;
		char marcador;
		std::string alvo;
		}Alvo;
};

void menu_boletim();
void cria_boletim();
void manipula_boletim();
void mostra_boletim(std::vector<Alvo>);
void refaz_boletim(std::string, std::vector<Alvo>);
void mostra_lista_boletim();
void remove_boletim();
void adicionar_nota();
bool testa_entrada(std::string);
void mostra_notas();
void remove_nota();
void refaz_notas();
void mostra_ajuda();
void erro_comando();

//===============================================================================//
//                                  MAIN                                         //
//===============================================================================//
int main(int argc, char** argv){
	struct stat notas;
	if(stat(path.c_str(), &notas)!=0){
		//os diretórios necessários para a utilização do sistema não existem
		//da primeira vez que o programa é usado. Cria-se os diretórios 
		//necessário programaticamente
		std::cout << "Folder /Notas não existe\nCriando /Notas..." << std::endl;	
		mkdir(path.c_str(), 0777);
		std::cout << "Criando /Notas/Boletins...\n" << std::endl;
		mkdir(path_boletins.c_str(), 0777);
	}

	if(argc==1){
		std::cout << "[!!!] Para uma listagem dos comandos existentes\n"\
		"digite " << argv[0] << " ajuda\n" << std::endl;
	}
	if(argc==1)
		mostra_notas();
	else if(argc==2){
		if(argv[1]==std::string("ajuda"))
			mostra_ajuda();
		else if (argv[1]==std::string("nota"))
			adicionar_nota();
		else if (argv[1]==std::string("boletim"))
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
		else
			erro_comando();
	}
	else
		std::cout << "ERRO: Quantidade incompatível de comandos" << std::endl;
	//fim de ifs()
}

//=============================================================================//
//                                 FIM DE MAIN                                 //
//=============================================================================//

void mostra_ajuda(){
	std::cout << "\nUso: ./notas\n"\
	             "     ./notas <arg1>\n"\
		     "     ./notas <arg1> <arg2>\n"\
		     "     ./notas  <arg1> <arg2> <arg3>\n"\
		"./notas           : mostra notas e ajuda\n"\
		"arg1  = ajuda     : mostrar esta mensagem\n"\
		"arg1  = nota      : manipular notas\n"\
		"arg1  = boletim   : adicionar lista-boletim\n"\
		"arg1 arg2 = notas   rm: apagar nota\n"\
		"arg1 arg2 = boletim rm: apagar boletim\n" << std::endl;
}
void erro_comando(){std::cout << "Comando não encontrado.\nTerminando..." << std::endl;}
//=============================================================================//
//                             MANIPULADOR DE NOTAS                            //
//=============================================================================//

void refaz_notas(){
	//refaz a lista com as notas disponíveis no vetor geral
	//de linhas disponíveis
	std::ofstream notas = std::ofstream(path + "/notas.txt");
	for(int clk = 0; clk < linhas.size(); clk++)
		notas << linhas[clk] << "\n*\n";
	notas.close();
}

void remove_nota(){
	//Remove uma nota existente na lista de acordo com um
	//índice fornecido pelo usuário
	//Não toma qualquer ação caso o índice seja menor que notas
	//Não toma qualquer ação caso não existam notas a serem 
	//removidas
	std::ifstream notas = std::ifstream(path + "/notas.txt");
	std::string leitura;
	unsigned int contador = 0;
	unsigned int indice;
	std::cout << "Apagar nota número\n>>";
	std::cin >>  indice;
	if(!notas){
		std::cout << "Não existe arquivo de notas a ser lido.\nEncerrando...\n";
		return;
	}
	while(getline(notas, leitura)){
		if(leitura==std::string("*"))
			continue;
		else
			linhas.push_back(leitura);
	}

	if(linhas.size()==0){
		std::cout << "Não existem notas a serem apagadas.\nEncerrando..." << std::endl;
		notas.close();
		return;
	}
	else if(linhas.size()!=0){
		if(linhas.size()<indice){
			std::cout << "Índice maior que quantidade de notas.\nEncerrando..." << std::endl;
			notas.close();
			return;
		}
	}
	notas.close();
	linhas.erase(linhas.begin() + indice-1);
	std::cout << "Nota #" << indice << " apagada..." << std::endl;
	refaz_notas();
}

void mostra_notas(){
	//mostra as notas em formato amigável para o usuário
	//com a numeração posicional de cada nota nas lista
	std::ifstream notas = std::ifstream(path + "/notas.txt");
	std::string leitura;
	unsigned int contador = 0;
	if(!notas){
		std::cout << "Não existe arquivo de notas a ser lido.\nEncerrando...\n";
		return;
	}
	while(getline(notas, leitura)){
		if(leitura==std::string("*"))
			std::cout << "____________________________________" << std::endl;
		else
			std::cout << "#" << ++contador << "| " << leitura << std::endl;
	}
	notas.close();
}

void adicionar_nota(){
	//adiciona uma nota nova ao documento geral de notas, criando
	//um documento para isso, caso nenhum documento de notas exista
	unsigned int quantidade;
	std::string entrada;
	std::ofstream notas;
	if(!std::ifstream(path  + "/notas.txt")){
		std::cout << "Não existe arquivo de notas.\nCriando...\n" << std::endl;
		notas = std::ofstream(path + "/notas.txt");
		notas.close();
	}

	notas = std::ofstream(path + "/notas.txt", std::ios_base::app);
	std::cout << "nota\n>> ";
	getline(std::cin, entrada);
	if(testa_entrada(entrada)==false){
		std::cout << "Nenhuma nota adicionada.\nEncerrando..." << std::endl;
		return;
	}
	else{
		notas << entrada << "\n*" << std::endl;
		notas.close();
		std::cout << "Nota salva..." << std::endl;
		return;
	}
	notas.close(); 
}

bool testa_entrada(std::string entrada){
	//verifica se a entrada dada pelo usuário como uma nova nota se
	//enquadra em um formato válido de nota, i.e, não é apenas uma
	//string com espaços vazios ou uma string de tamanho 0
	if(entrada.size()==0)
		//se a entrada for vazia
		return false;
	else{
		for(int clk = 0; clk < entrada.size(); clk++){
			if(entrada[clk]!=' ');
			//se algum caractere for diferente de espaço em branco
			return true;
		}
	}
	//todos os caracteres são espaços em branco
	return false;
}


//=============================================================================//
//                       MANIPULADOR DE BOLETINS                               //
//=============================================================================//

void menu_boletim(){
	std::string comando;
	std::cout << "Adicionar boletim ou manipular existente?\n"\
		     "[cr]: criar               [ma]: manipular\n"\
		     "[rm]: remover" 
		 << std::endl << ">>";
	std::cin >> comando;
	if(comando=="cr" or comando=="criar")
		cria_boletim();
	else if(comando=="ma" or comando=="manipular")
		manipula_boletim();
	else if(comando=="rm" or comando=="remover")
		remove_boletim();
	else{
		std::cout << "Comando não encontrado\nTerminando programa..." << std::endl;
		exit(0);
	}
}

void cria_boletim(){
	//Cria um novo boletim a partir de especificações dadas pelo usuário
	std::cout << "Criando um novo boletim" << std::endl;
	std::vector<std::string> alvos_a_adicionar;
	std::string alvo, nome_doc;
	std::cout << "Nome do novo boletim (sem espaços): " << std::endl << ">>";
	
	std::cin >> nome_doc;
	std::ofstream ofs = std::ofstream((path_boletins + "/" + nome_doc + ".txt").c_str());	
	
	getline(std::cin, alvo);
	while(true){
		std::cout << ">>[alvo]: ";
		getline(std::cin, alvo);
		if(alvo=="q")
			break;
		else{
			ofs << "-" << std::endl;
			ofs << alvo << std::endl;
			alvos_a_adicionar.push_back(alvo);
		}
	}
	for(int clk = 0; clk < alvos_a_adicionar.size(); clk++)
		std::cout << alvos_a_adicionar[clk] << std::endl;
	ofs.close();
}

void manipula_boletim(){
	/*Entra em modo de manipulação de boletins, dando ao usuário as 
	opções existentes para criação, remoção ou mostra de conteúdo dos
	boletins já criados */
	std::cout << "Manipulando um boletim existente" << std::endl;
	std::vector<Alvo>alvos;
	int contador = 1, acerto;
	std::string arquivo, leitura;
	char opcao;
	std::ifstream ifs;

	mostra_lista_boletim();
	std::cout << "Boletim a ser aberto (sem espaços): " << std::endl << ">>";
	std::cin >> arquivo;
	ifs = std::ifstream((path_boletins + "/" + arquivo + ".txt").c_str());
	if(ifs.fail()){
		std::cout << "Arquivo não pôde ser aberto\n"\
			"Falha na abertura ou arquivo inexistente\n"\
			"Terminando programa..." << std::endl;
		exit(0);
	}
	while(true){
		if(ifs.eof())
			break;
		else{
			Alvo al;
			al.indice = contador++;
			getline(ifs, leitura); al.marcador = leitura[0]; 
			getline(ifs, leitura); al.alvo = leitura; 
			if(al.alvo.size()>0)
				alvos.push_back(al);
			}
	}
	ifs.close();
	mostra_boletim(alvos);
	std::cout << "Atirar em item? [S/n]\n" << ">>";
	std::cin >> opcao;
	if(opcao=='s' or opcao=='S'){
		std::cout << "Item a ser atirado: \n" << ">>";
		std::cin >> acerto;
		if(acerto>0 and (acerto<alvos.size()+1)){
			alvos[acerto-1].marcador = '*';
			//mostra_alvos() mostra o vector, não o arquivo	
			refaz_boletim(arquivo, alvos);
			mostra_boletim(alvos);
			}
		else{
			std::cout << "Índice inexistente\nTerminando..." << std::endl;
			exit(0);
			}
	}
	else if(opcao=='N' or opcao=='N'){;;}
	
}

void mostra_boletim(std::vector<Alvo>alvos){
	/*Mostra o conteúdo de um boletim previamente selecionado pelo usuário
	de acordo com o conteúdo já adicionado em um vector de leitura de conteúdo
	pré-formatado */
	for(int clk = 0; clk < alvos.size(); clk++){
		if(alvos[clk].marcador=='-')
			std::cout << "[ ]";
		else
			std::cout << "[" << alvos[clk].marcador << "]";
		std::cout << " ";
		std::cout << "#" << alvos[clk].indice << "| "\
		<< alvos[clk].alvo << std::endl << std::endl;
	}
}


void refaz_boletim(std::string nome, std::vector<Alvo>alvos){
	/*Refaz o conteúdo de um boletim modificado pelo usuário de acordo
	com o novo conteúdo especificado por ele */
	std::ofstream ofs = std::ofstream((path_boletins + "/" + nome + ".txt").c_str());
	for(int clk = 0; clk < alvos.size(); clk++){
		ofs << alvos[clk].marcador << std::endl;
		ofs << alvos[clk].alvo << std::endl;
		}
	ofs.close();
}

void mostra_lista_boletim(){
	/*Mostra os arquivos presentes no diretório de boletins diretamente
	para que o usuário possa escolher entre eles */
	DIR* dir;
	struct dirent* leitura;
	std::vector<char*> arquivos;
	std::cout << "Boletins existentes: " << std::endl;
	if((dir = opendir(path_boletins.c_str()))!=nullptr){
		while((leitura = readdir(dir))!=nullptr)
			arquivos.push_back(leitura->d_name);
		closedir(dir);
	}
	else{
		perror("Sem boletins a serem abertos.\nFinalizando...\n");
		return;
	}
	for(auto arquivo:arquivos)
		std::cout << arquivo << std::endl;
	std::cout << std::endl;
}

void remove_boletim(){
	/*Remove um boletim segundo especificado pelo usuário, ou retorna
	uma mensagem de erro caso o arquivo não existe no diretório de boletins */
	std::cout << "Removendo boletim existente" << std::endl;
	std::string arquivo;
	mostra_lista_boletim();
	std::cout << "Qual boletim deve ser removido?:" << std:: endl;
	std::cout << ">>";
	std::cin >> arquivo;
	if(remove((path_boletins + "/" + arquivo +".txt").c_str())==false)
		std::cout << "Boletim removido." << std::endl;
	else{
		std::cout << "Arquivo não encontrado. \nTerminando..." << std::endl;
		exit(0);
	}
}

