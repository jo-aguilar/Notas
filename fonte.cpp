#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <cstring>
#include <conio.h>
#include <filesystem>
#include "fonte.h"
#define USUARIO_MAX 50
#define DEBUG(X) std::cout << #X << std::endl; 

namespace{
	char us[USUARIO_MAX];
	std::string usuario(cuserid(us));
	std::string notas = "/home/"+usuario+"/Documents/Notas/notas.txt";
	std::string path_boletins = "/home/"+usuario+"/Documents/Notas/Boletins/";
	class Boletim{
		char marcador;
		std::string alvo;
		public:
			Boletim(char mm, std::string aa): marcador(mm), alvo(aa){};
			~Boletim(){}
			std::string retornaAlvo(){return alvo;}
			char retornaMarcador(){return marcador;}
			void setaMarcador(char arg){marcador = arg;}
	};

	class Boletins_de_troca{
		std::vector<Boletim>boletim1;
		std::vector<Boletim>boletim2;
		std::string nome1;
		std::string nome2;
		public:
			Boletins_de_troca(std::vector<Boletim>v1, 
					  std::vector<Boletim>v2,
					  std::string nm1, std::string nm2)
				:boletim1(v1), boletim2(v2), nome1(nm1), nome2(nm2){}
			~Boletins_de_troca(){}
			std::vector<Boletim> retornaB1(){return boletim1;}
			std::vector<Boletim> retornaB2(){return boletim2;}
			std::string retornaNome1(){return nome1;}
			std::string retornaNome2(){return nome2;}
	};

};


//========================================================================================//
//              		          NOTAS					          //
//========================================================================================//
static std::vector<std::string> retorna_notas(){
	//enviar mensagem de erro caso não haja um arquivo a ser lido
	std::ifstream ifs(notas);
	std::string leitura = "";
	std::vector<std::string> notas;
	while(getline(ifs, leitura))
		notas.push_back(leitura);
	ifs.close();
	return notas;
}

static void modula_palavras(std::string entrada){
	/* mostra uma nota palavra a palavra para tentar não truncar
	uma palavra no terminal durante a visualização */
	std::stringstream s(entrada);
	std::string leitura;
	while(s>>leitura)
		std::cout << leitura << " ";
	std::cout << std::endl;
}

void mostra_notas(){
	//mostra as notas existentes para o usuário
	std::vector<std::string> notas_vetor = retorna_notas();
	if(notas.size()==0)
		std::cout << "Sem notas para mostra.\nTerminando...\n\n";
	else{
		for(int clk = 0; clk < (int)notas_vetor.size(); clk++){
			std::cout << "#" << std::setfill('0') 
			          << std::setw(2) << clk + 1 << " : ";
			modula_palavras(notas_vetor[clk]);
			}
		}
}

void mostra_ajuda(){
	std::cout << "Mostra ajuda" << std::endl;
}

static void colorir(std::string entrada, std::string cor){
	//adiciona cor a uma string de entrada e a retorna como saída
	(cor=="VERMELHO")?(std::cout << "\033[1;31m" << entrada << "\033[0m"):
	(cor=="VERDE"   )?(std::cout << "\033[1;32m" << entrada << "\033[0m"):
	(cor=="AMARELO" )?(std::cout << "\033[1;33m" << entrada << "\033[0m"):
	(cor=="AZUL"    )?(std::cout << "\033[1;34m" << entrada << "\033[0m"):
	(cor=="MAGENTA" )?(std::cout << "\033[1;35m" << entrada << "\033[0m"):
	(cor=="CIANO"   )?(std::cout << "\033[1;36m" << entrada << "\033[0m"):
	                  (std::cout << "\033[1;37m" << entrada << "\033[0m");
}

void erro_comando(){
	std::cout << "Erro de comando" << std::endl;
}


//===================================== adição de notas ==================================//
static void salva_nota(std::string nota){
	//salva a nota no documento relativo às notas
	std::ofstream ofs(notas, std::ios_base::app);
	ofs << nota << std::endl;
	ofs.close();
}

static bool checa_str(std::string nota){
	/*verifica se a string fornecida pelo usuário tem apenas caracteres em branco
	Se sim, retorna true. Se não, retorna false */
	bool bandeira = false;
	for(int clk = 0; clk < (int)nota.size(); clk++)
		if(nota[clk]!=' ') return false;
	return true;
}

static bool guarda_nota(std::string nota){
	//verifica se a string fornecida pelo usuário tem um formato de leitura válido
	return
		(nota.empty()   ==true)?false: //se a string estiver vazia, retorna false
		(nota.size ()   == 0  )?false: //se a string não tiver tamanho, retorna false
		(checa_str(nota)==true)?false: //se só houver caracteres em branco, retorna false
		true;
}

static std::string escreve_nota(){
	//demanda uma nota a ser fornecida pelo usuário
	std::string nota;
	std::cout << ">nota: ";
	std::getline(std::cin, nota);
	return nota;
}

void adiciona_nota(){
	/* administra a entrada de notas fornecidas pelo usuário, checando sua integridade
	e permitindo ou não que as notas sejam adicionadas ao arquivo */
	system("clear");
	std::ofstream ofs;
	if(std::ifstream(notas).fail()){
		std::cout << "Arquivo de notas inexistente\nCriando arquivo...\n" << std::endl;
		ofs = std::ofstream(notas);
		ofs.close();
		}
	else{
		std::string nota = escreve_nota();
		if(guarda_nota(nota)==false){
			std::cout << "\n[!!!] ERRO: formato inválido de nota." 
			          << "\nTerminando...\n\n";
			exit(0);
			}
		else{
			salva_nota(nota);
			std::cout << "\nNota salva.\n\n";
		};
	}
}

//================================ remoção de notas ======================================//
static bool erroNumerico(std::string entrada){
	//[!!!] NÃO FUNCIONA CASO HAJA UM NÚMERO SEGUIDO DE UMA STRING INVÁLIDA	

	/*checa se uma string de entrada é composta apenas por números
	caso não seja, retorna true. Caso seja, retorna false*/
	for(int clk = 0; clk < (int)entrada.size(); clk++){
		if(isdigit(entrada[clk])==false)
			return true;
	}
	return false;
}

static std::string checaString(bool (*func)(std::string ent), std::string mensagem){
	/*checa se uma entrada é numericamente válida para remoção de uma nota
	Caso não seja, continua pedindo por uma entrada válida até que seja fornecida.
	Caso a entrada seja igual a q, termina a execução do programa.
	Caso a entrada seja válida, retorna a string contendo o valor */
	std::string entrada;
	std::cin>>entrada;

	while(func(entrada)==true){
		if(entrada=="q"){
			std::cout << "\nTerminando...\n\n";
			exit(0);
		}
		else{
			std::cout << "\nEntrada inválida.\nPor favor, insira um valor numérico\n";
			std::cout<< mensagem;
			std::cin>>entrada;
		}
	}
	return entrada;
}

static void refaz_notas(std::vector<std::string> notas_vetor){
	/* recebe um vetor com uma lista de notas rearranjada e sobrescreve o 
	documento com as notas no novo formato */
	std::ofstream ofs(notas, std::ios_base::trunc);
	for(int clk = 0; clk < (int)notas_vetor.size(); clk++)
		ofs << notas_vetor[clk] << std::endl;
	ofs.close();
}

static void removedorNotas(std::vector<std::string> notas_vetor){
	/* verifica se o vetor de entrada contém notas a serem removidas. Caso
	não tenha, termina o programa. Caso tenha, pede ao usuário por um índice
	válido de uma nota a ser removida e a remove*/
	if(notas_vetor.size()==0){
		std::cout << "\nSem Notas a serem removidas.\nTerminando...\n\n";
		exit(0);
	}
	else{		
		std::cout << "\n>Nota a ser removida: ";
		std::string entrada;
		bool (*fpointer)(std::string) = erroNumerico;
		entrada = checaString(fpointer, ">Nota a ser removida: ");
		if(std::stoi(entrada)>(int)notas_vetor.size() or std::stoi(entrada)<=0){
			std::cout <<"\nÍndice inexistente.\nTerminando...\n\n";
			exit(0);
		}
		else{
			notas_vetor.erase(notas_vetor.begin() + std::stoi(entrada) - 1);
			std::cout << "Valor da entrada " << std::stoi(entrada) << std::endl;
			system("clear");
			refaz_notas(notas_vetor);
			mostra_notas();
			std::cout << "\n\nNota removida.\n";
		}
	}
}

void remove_nota(){
	/* função chamada diretamente no arquivo principal para
	a remoção de notas */
	system("clear");
	mostra_notas();
	std::vector<std::string> notas_vetor = retorna_notas();
	if(notas_vetor.size()==0){
		std::cout << "Sem notas a serem removidas.\nTerminando...\n\n";
		exit(0);
		}
	else
		removedorNotas(notas_vetor);
}

//========================================================================================//
//              			BOLETINS			                  //
//========================================================================================//
void mostra_lista_boletim(){
	/* Mostra os arquivos presentes no diretório de boletins diretamente
	para que o usuário possa escolher entre eles */
	DIR* dir;
	system("clear");
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
		if((strcmp(arquivo, ".")==0) or (strcmp(arquivo,"..")==0))
			continue;
		else{
			colorir(std::string(arquivo).substr(0, std::string(arquivo).size()-4),
			        "VERDE");
			std::cout << std::endl;
		}
}

static std::string recebe_nome_boletim(){
	//Recebe o nome do boletim a ser criado, verificando a integridade do nome
	//[!!!] ADICIONAR FUNÇÃO QUE CHEQUE PRÉ-EXISTÊNCIA DE DETERMINADO BOLETIM
	std::string mensagem = ">Nome do boletim [sem espaços]";
	bool (*fpointer)(std::string) = checa_str;
	std::string nome = checaString(fpointer,mensagem);
	return nome;
}

static std::vector<Boletim> retorna_boletim(std::string nome){
	/* Retorna um vetor com elementos Boletim a partir de uma leitura de um arquivo
	de alvos especificado pelo usuário */
	//system("clear");
	std::vector<Boletim> boletim;
	colorir("[ ", "AMARELO"); colorir(nome, "AMARELO"); colorir(" ]", "AMARELO");
	std::cout << std::endl;
	std::ifstream ifs(path_boletins + nome + ".txt");
	while(!ifs.eof()){
		std::string alvo;     getline(ifs, alvo);
		std::string marcador; getline(ifs, marcador);
		Boletim bol(marcador[0], alvo);
		boletim.push_back(bol);
	}
	boletim.erase(boletim.begin() + boletim.size()-1);
	return boletim;
}

//=================================== visualiza boletim ==================================//
void visualiza_boletim(std::string nome_boletim = "", bool apaga = true){
	/* A partir de um vetor de boletins entregue de acordo com um arquivo de alvos
	especifiado pelo usuário, mostra de forma organizada todos os alvos do boletim 
	e seus respectivos estados de marcação */
	std::string nome;
	std::vector<Boletim>boletim;
	if(nome_boletim==""){
		mostra_lista_boletim();
		std::cout << "\nBoletim a ser visualizado: \n>";
		nome = recebe_nome_boletim();
		}
	else
		nome = nome_boletim;	
	if(apaga==true)
		system("clear");
	boletim = retorna_boletim(nome);
	for(int clk = 0; clk < (int)boletim.size(); clk++){
		std::cout << "#" << std::setfill('0') << std::setw(2) << clk+1;
		if(boletim[clk].retornaMarcador()=='-')
			std::cout << " [ ] ";
		else{
			std::cout << " ["; colorir("*", "VERDE"); std::cout<< "] ";
			}
		std::cout << boletim[clk].retornaAlvo() << std::endl;
	}
	std::cout << std::endl;
}


//=================================== adiciona boletim ===================================//
static std::vector<std::string> vetor_boletim(){
	/* Requere seguidamente do usuário alvos a serem adicionador a um vetor de strings
	que se refere aos alvos do boletim a ser criado. Quando o usuário sinaliza o término
	da lista de alvos, a função retorna o vetor criado */
	system("clear");
	std::string entrada = "";
	std::vector<std::string>boletim;
	std::cout << "Entre com elementos do boletim.\n"
		  << "[ENTER] passa para o próximo elemento\n"
		  << "[q] termina a listagem\n\n";
	getline(std::cin, entrada);
	while(entrada!="q"){
		if(!entrada.empty());
			boletim.push_back(entrada);
		std:: cout<< ">alvo: ";
		getline(std::cin, entrada);
	}
	boletim.erase(boletim.begin(), boletim.begin()+1);
	return boletim;
}

static void cria_boletim(){
	/* Requere do usuário o nome do boletim a ser criado, e chama um função para a 
	entrada dos dados fornecidos para a formação do boletim, chamando uma função que salva
	um vetor de strings e então passa os elementos e estados prévios para esses
	documentos para um arquivo estruturado elementos e estados*/
	system("clear");
	std::string mensagem = "Nome do boletim a ser criado [sem espaços]:\n>";
	std::cout << mensagem;
	bool (*fpointer)(std::string) = checa_str;
	std::string entrada = checaString(fpointer, mensagem);     //nome do novo arquivo
	std::ofstream ofs(path_boletins + entrada + ".txt", std::ios_base::trunc);
	std::vector<std::string> boletins = vetor_boletim();       //vetor com elementos do boletim
	for(int n = 0; n < (int)size(boletins); n++){
		ofs << boletins[n] << std::endl;
		ofs << '-' << std::endl;
	}
	ofs.close();
	std::cout << "\nBoletim salvo." << std::endl;
}


//=================================== remove boletim =====================================//
void remove_boletim(){
	/*Remove um boletim segundo especificado pelo usuário, ou retorna
	uma mensagem de erro caso o arquivo não existe no diretório de boletins */
	system("clear");
	std::string arquivo;
	mostra_lista_boletim();
	std::cout << "\nBoletim a ser removido:\n>";
	std::cin >> arquivo;
	if(remove((path_boletins + "/" + arquivo +".txt").c_str())==false)
		std::cout << "\nBoletim removido.\nTerminando...\n" << std::endl;
	else{
		std::cout << "Arquivo não encontrado. \nTerminando..." << std::endl;
		exit(0);
	}
}


//=================================== manipula boletim ===================================//
static void manipula_generica(std::string mensagem, void (*func)(std::string)){
	/* Função genérica para generalizar as funções de mapulação de
	alvos em boletins */
	std::string nome_boletim;
	mostra_lista_boletim();
	std::cout << mensagem;
	std::cin>>nome_boletim;
	std::string path = path_boletins+nome_boletim+".txt";
	while(std::filesystem::exists(std::string(path))==false){
		if(nome_boletim=="q"){
			std::cout << "\nTerminando...\n";
			exit(0);
		}else{
			std::cout << "\nBoletim invalido. Tente novamente\n";
			std::cout << mensagem;
			std::cin>>nome_boletim;
			path = path_boletins+nome_boletim+".txt";
		}
	}
	(*func)(nome_boletim);
}

static void mostra_menu(const int tamanho, std::string _menu[], 
			const int contador, const char m, const std::string cor){
	/* Configura determinada lista de redirecionamento para funções conforme
	especificado pelo usuário, com diferentes cores e uma configuração
	vertical ou horizontal de listagem */
	if(m=='v'){
		for(int n = 0; n < tamanho; n++){
			if(n==contador-1){
				std::cout << ">";
				colorir(_menu[n], cor);
				std::cout << std::endl;
			}else{
				(std::cout << " ");
				std::cout << _menu[n];
				std::cout << std::endl;
			}
		}
	}
	else if(m=='h'){
		for(int n = 0; n < tamanho; n++){
			if(n==contador-1){
				std::cout << ">";
				colorir(_menu[n], cor);
			}else{
				(std::cout << " ");
				std::cout << _menu[n];
			}
		}
		std::cout << std::endl;
	}
}

static void refaz_boletim(std::string nome_boletim, std::vector<Boletim> boletim){
	/* Refaz um boletim de acordo com um novo vector preenchido com objetos
	Boletim, usando seus elementos "alvo" e "marcador" como items para a 
	formação de um novo arquivo que serve de base para a nova configuração
	do boletim */
	std::string path = path_boletins+nome_boletim+".txt";
	std::ofstream ofs = std::ofstream(path, std::ios::trunc);
	for(int n = 0; n<(int)size(boletim); n++){
		ofs << boletim[n].retornaAlvo() << std::endl;
		ofs << boletim[n].retornaMarcador() << std::endl;
	}
	ofs.close();
}

static void atira_em_alvo(std::string nome_boletim){
	/* Demanda ao usuário qual alvo do boletim aberto deve ser marcado
	como atirado. Verifica se o valor fornecido não é negativo ou maior
	que o próprio boletim. Caso esteja fora do domínio, demanda ao usuário
	que tente novamente com um novo valor. Caso o alvo já esteja marcado, 
	avisa o usuário e termina o programa. Caso o alvo não esteja marcado,
	executa a marcação, mostra o boletim atualizado e termina o programa */	
	int indice = 0;
	std::vector<Boletim>boletim = retorna_boletim(nome_boletim);
	visualiza_boletim(nome_boletim);
	std::cout << "\n>Alvo a atirar:\n>";
	std::cin>>indice;
	while(0>=indice or indice>(int)size(boletim)){
		std::cout << "\nÍndice invalido. Tente novamente\n";
		std::cout << ">Alvo a atirar:\n>";
		std::cin>>indice;
	}
	if(boletim[indice-1].retornaMarcador()=='*'){
		std::cout << "Alvo já marcado.\nTerminando...\n\n";
		exit(0);
		}
	else
		boletim[indice-1].setaMarcador('*');
	refaz_boletim(nome_boletim, boletim);
	visualiza_boletim(nome_boletim);
	std::cout << std::endl;	
}

//==================================== trocar alvos  =====================================//
static Boletins_de_troca retorna_boletins(){
	/* Retorna um objeto com os vetores representativos e nomes dos dois boletins 
	selecionados pelo usuário para efetuar a troca de alvos do primeiro para o segundo. 
	É dada a chance para o usuário de tentar seguidamente entrar os nomes de dos boletins
	a partir de uma lista prévia de boletins já existtentes */
	std::string b1; 
	std::string b2;
	mostra_lista_boletim();
	std::cout << std::endl;
	std::cout << "Boletins a trocar items [diferentes]:\nBoletim 1:\n>";
	std::cin>>b1;
	while(std::filesystem::exists(path_boletins+b1+".txt")==false){
		if(b1=="q"){
			std::cout << "\nTerminando...\n";
			exit(0);
		}
		std::cout << "Boletim inexistente.\nBoletim 1:\n>";
		std::cin>>b1;
	}
	
	std::cout << "\nBoletim 2:\n>";
	std::cin>>b2;
	while(std::filesystem::exists(path_boletins+b2+".txt")==false or b1==b2){
		if(b2=="q"){
			std::cout << "\nTerminando...\n";
			exit(0);
		}
		else if(b1==b2)
			std::cout << "\nBoletins 1 e 2 não podem ser iguais.";
		else
			std::cout << "Boletim inexistente.\n";
		std::cout << "\nBoletim 2: \n>";
		std::cin>>b2;
	}
	std::vector<Boletim>resultado1 = retorna_boletim(b1);
	std::vector<Boletim>resultado2 = retorna_boletim(b2);
	visualiza_boletim(b1); std::cout;
	visualiza_boletim(b2, false); std::cout;
	return Boletins_de_troca(resultado1, resultado2, b1, b2);
}

static void trocando_boletim(){
	/* Efetua a troca de alvos do primeiro para o segundo boletim especificado pelo usuário
	desde que o índice fornecido esteja dentro do domínio de seu número de alvos. Caso o 
	boletim tenha seu último alvo existente removido, ele também é removido */
	int entrada;
	std::string mensagem = "\nQual alvo trocar de\nBoletim 1 para Boletim 2?:\n>";
	mostra_lista_boletim();
	std::cout << std::endl;
	std::cout << "Trocando boletim" << std::endl;
	Boletins_de_troca troca = retorna_boletins();
	std::vector<Boletim>b1 = troca.retornaB1();
	std::vector<Boletim>b2 = troca.retornaB2();
	std::cout << mensagem;
	std::cin>>entrada;
	while(entrada<=0 or entrada>(int)size(b1)){
		std::cout << "Índice inválido." << mensagem;
		std::cin>>entrada;
	}
	Boletim blt('-', b1[entrada-1].retornaAlvo());
	b2.insert(b2.begin(), blt);
	b1.erase(b1.begin() + (entrada-1));
	refaz_boletim(troca.retornaNome1(), b1);
	refaz_boletim(troca.retornaNome2(), b2);
	system("clear");
	visualiza_boletim(troca.retornaNome1());
	visualiza_boletim(troca.retornaNome2(), false);
	if(size(b1)==0)
		std::filesystem::remove(path_boletins+troca.retornaNome1()+".txt");
}

//============================== remove em alvo de boletim =================================//
static void removendo_alvo(std::string nome){
	/* Remove um alvo especificado pelo usuário dado um índice fornecido, desde
	que o boletim exista */
	int indice;
	std::string path = path_boletins + nome + ".txt";
	std::vector<Boletim>alvos = retorna_boletim(nome);
	visualiza_boletim(nome);
	std::cout << "\nAlvo a ser removido:\n>";
	std::cin >> indice;
	while(indice <=0 or indice>(int)size(alvos)){
		std::cout << "\nÍndice inválido.\nTente novamente.\n";
		std::cout << "\nAlvo a ser removido:\n>";
		std::cin >>indice;
	}
	alvos.erase(alvos.begin() + indice-1);
	refaz_boletim(nome, alvos);
	if(size(alvos)==0){
		system("clear");
		std::filesystem::remove(path);
		std::cout << "Boletim sem alvos.\nApagando...\n";
	}
	else{
		visualiza_boletim(nome);
		std::cout << std::endl;
	}
}

//============================ adiciona em alvo de boletim =================================//
static void adiciona_alvo(std::string nome){
	/* Adiciona um novo alvo dado pelo usuário a um boletim já existente */
	std::string path = path_boletins + nome + ".txt";
	std::string alvo;
	std::ofstream ofs = std::ofstream(path, std::ios::app);
	
	visualiza_boletim(nome);
	std::cout << "\nAlvo a ser adicionado:\n>";
	getline(std::cin, alvo);

	while(alvo.empty()){
		if(alvo=="q"){
			std::cout << "\nTerminando...\n";
			ofs.close();
			exit(0);
		}
		else{
			std::cout << "Alvo vazio. Por favor, insira um alvo.\n";
			std::cout << "\nAlvo a ser adicionado:\n>";
			getline(std::cin, alvo);
		}
	}
	ofs << alvo << std::endl;
	ofs << '-' << std::endl;
	ofs.close();
	system("clear");
	visualiza_boletim(nome);
	std::cout << std::endl;
}

//============================= atira em alvo de boletim ===================================//
static void acessa_func(const int item, std::string nome_boletim){
	/*Faz as chamadas paras as tarefas de acordo com o valor
	das variável de entrada fornecida pelas escolhas do usuário
	em menu_boletim */
	(item==1)?(atira_em_alvo(nome_boletim)):
	(item==2)?(adiciona_alvo(nome_boletim)):
	(item==3)?(removendo_alvo(nome_boletim)):
		  (erro_comando());
}

static void menu_manipula_boletim(std::string nome_boletim){
	/* Função de chamada que dá as opções de manipulação para o usuário
	para que ele possa escolher o que deseja fazer com o arquivo antes
	selecionado e já verificado. Leva a uma função de seleção de ações
	que chamam a função requerida */
	std::string lista[] = {"atirar   ", "adicionar  ", "remover"};
	int contador = 1;
	char entrada;
	system("clear");
	std::cout << "Ação a executar:" << std::endl;
	mostra_menu(size(lista), lista, contador, 'h', "VERDE");
	getch();
	while((entrada=getch())!=10){
		if(entrada==67){
			contador++;
			if(contador==4) contador=1;
		}
		else if(entrada==68){
			contador--;
			if(contador==0) contador=3;
		}
		system("clear");
		std::cout << "Ação a executar:" << std::endl;
		mostra_menu(size(lista), lista, contador, 'h', "VERDE");
	}
	acessa_func(contador, nome_boletim);
}


static void manipula_boletim(){
	/* Função cabeçalho que demanda qual arquivo o usuário deseja 
	manipular. Caso não exista, avisa ao usuário. Caso exista, faz
	a chamada da função que dá as opções de manipulação */
	void (*func)(std::string) = menu_manipula_boletim;
	manipula_generica("\nBoletim a ser manipulado:\n>", func);
}



//===================================== menu boletim =====================================//
static void direciona_menu(const int contador){
	/* Faz as chamadas paras as tarefas de acordo com o valor
	das variável de entrada fornecida pelas escolhas do usuário
	em menu_boletim */
	if      (contador==1) cria_boletim();
	else if (contador==2) manipula_boletim();
	else if (contador==3) visualiza_boletim();
	else if (contador==4) remove_boletim();
	else if (contador==5) trocando_boletim();
}

void menu_boletim(){
	/* Mostra o menu principal para as funções dos boletins, que fazem as 
	chamadas para as suas respectivas tarefas */
	std::string _menu[] = {"Criar", "Manipular", "Visualizar", "Apagar", "Trocar alvos"};
	std::string opc = "Qual ação executar sobre boletim?: \n\n";
	int contador = 1;
	char c;
	system("clear");
	std::cout << opc;
	mostra_menu(size(_menu), _menu, contador, 'v', "VERDE");
	while((c=getch())!=10){
		system("clear");
		if(c==66){
			if(contador>=1 and contador<(int)size(_menu)) contador++;
			else if(contador==(int)size(_menu)) contador=1;
		}
		else if(c==65){
			if(contador<=(int)size(_menu) and contador>1) contador--;
			else if(contador==1) contador=size(_menu);
		}
		else if(c=='q'){
			std::cout << "Terminando..." << std::endl;
			exit(0);
		}
		system("clear");
		std::cout << opc;
		mostra_menu(size(_menu), _menu, contador, 'v', "VERDE");
	}
	direciona_menu(contador);
}

