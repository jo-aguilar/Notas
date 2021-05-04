#include <iostream>
#include <string>
#include <cstring>
#define DEBUG(X) std::cout << #X << std::endl

void adicionar_nota();
void adicionar_boletim();


//===============================================================================//
//                                  MAIN                                         //
//===============================================================================//
int main(int argc, char** argv){
   if(argc==1){
      std::cout << "Uso: <notas arg1 arg2>\n"\
                   "     <notas arg1 arg2 arg3>\n"\
                   "arg1 arg2 = acd nota  : adicionar nova nota\n"\
                   "arg1 arg2 = acd boletim: adicionar lista-boletim\n"\
                   "arg1 arg2 = lista notas  : ver todas as notas\n"\
                   "arg1 arg2 = lista boletim: ver boletins por nome \n"\
                   "arg1 arg2 arg3 = rm nota #[n] : apagar nota n" << std::endl;
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
   }
   //fim de ifs()
}
//===============================================================================//
//                                  FIM DE MAIN                                  //
//===============================================================================//


void adicionar_nota(){
   std::cout << "Adicionando nota" << std::endl;
}

void adicionar_boletim(){
   std::cout << "Adicionando boletim" << std::endl;
}
