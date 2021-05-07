#include <iostream>
#include <string>
#include <cstring>
#include <fstream>
#include <vector>
#define DEBUG(X) std::cout << #X << std::endl

namespace{
   std::vector<std::string> linhas;
};

void adicionar_boletim();
void adicionar_nota();
bool testa_entrada(std::string);
void mostra_notas();
void remove_nota();
void refaz_notas();

//===============================================================================//
//                                  MAIN                                         //
//===============================================================================//
int main(int argc, char** argv){
   if(argc==1)
      mostra_notas();
   else if(argc==2){
      if(argv[1]==std::string("ajuda")){
         std::cout << "Uso: <notas arg1 arg2>\n"\
                      "     <notas arg1 arg2 arg3>\n"\
                      "arg1 arg2 = acd nota  : adicionar nova nota\n"\
                      "arg1 arg2 = acd boletim: adicionar lista-boletim\n"\
                      "arg1 arg2 = lista notas  : ver todas as notas\n"\
                      "arg1 arg2 = lista boletim: ver boletins por nome \n"\
                      "arg1 arg2 arg3 = rm nota #[n] : apagar nota n" << std::endl;
      }
   }
   else if (argc==3){
      if(argv[1]==std::string("adc")){
         if(argv[2]==std::string("nota")){
            adicionar_nota();
         }
         else if(argv[2]==std::string("boletim")){
            adicionar_boletim();
         }
      }
      else if(argv[1]==std::string("rm")){
         if(argv[2]==std::string("nota"))
            remove_nota();
      }
   }
   else{
      std::cout << "ERRO: Quantidade incompatível de comandos" << std::endl;
   }
   //fim de ifs()
}
//===============================================================================//
//                                  FIM DE MAIN                                  //
//===============================================================================//

void refaz_notas(){
   std::ofstream notas = std::ofstream("notas.txt");
   for(int clk = 0; clk < linhas.size(); clk++)
      notas << linhas[clk] << "\n*\n";
   notas.close();
}

void remove_nota(){
   //fazer checagem numérica
   //fazer checagem de range
   //checar se documento existe para que se possa remover
   std::ifstream notas = std::ifstream("notas.txt");
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
      else{
         linhas.push_back(leitura);
      }
   }
   notas.close();
   linhas.erase(linhas.begin() + indice-1);
   refaz_notas();
}

void mostra_notas(){
   std::ifstream notas = std::ifstream("notas.txt");
   std::string leitura;
   unsigned int contador = 0;
   if(!notas){
      std::cout << "Não existe arquivo de notas a ser lido.\nEncerrando...\n";
      return;
   }
   while(getline(notas, leitura)){
      if(leitura==std::string("*"))
         std::cout << "____________________________________" << std::endl;
      else{
         std::cout << "#" << ++contador << "| " << leitura << std::endl;
      }
   }
   notas.close();

}

void adicionar_nota(){
   unsigned int quantidade;
   std::string entrada;
   std::ofstream notas;
   if(!std::ifstream("notas.txt")){
      std::cout << "Não existe arquivo de notas.\nCriando...\n" << std::endl;
      notas = std::ofstream("notas.txt");
      notas.close();
      }
   
   notas = std::ofstream("notas.txt", std::ios_base::app);
   std::cout << "nota\n>> ";
   getline(std::cin, entrada);
   if(testa_entrada(entrada)==false){
      std::cout << "Nenhuma nota adicionada.\nEncerrando..." << std::endl;
      return;
      }
   else{
      notas << entrada << "\n*" << std::endl;
      notas.close();
      return;
   }
   notas.close(); 
}

bool testa_entrada(std::string entrada){
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

void adicionar_boletim(){
   std::cout << "Adicionando boletim" << std::endl;
}
