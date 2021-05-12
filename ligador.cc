//JOAO PEDRO ASSUNCAO COUTINHO - 18/0019813

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <list>
#include <utility>

using namespace std;

//checa se uma string contem outra string, retorna a posicao da primeira ocorrencia da
//substring, caso encontrada, ou -1, caso nao encontrada.
int contains(string s, string search){
    if(s.find(search) != string::npos){
        return s.find(search);
    }
    return -1;
}



//tabela global de definicoes
list<pair<string, int>> global_definitions_table;

//busca label na tab de definicoes, e retorna o valor associado
int search_in_def_tab(string s){
    for(auto a: global_definitions_table){
        if(contains(s, a.first) != -1){
            return a.second;
        }
    }
    return -1;
}


bool more_than_one_file = false;

int main (int argc, char **argv) {

    //onde os codigos serão unidos
    vector<int> T;
    
    //tamanho de cada obj
    int size_mod_A = 0, size_mod_B = 0, size_mod_C = 0;

    //info de realocacao de cada modulo
    string realloc_mod_A, realloc_mod_B, realloc_mod_C;

    //tabelas de uso
    list<pair<string, int>> use_mod_A, use_mod_B, use_mod_C;

    if(argc < 2){
        cout << "erro, necessario informar nome do arquivo" << endl;
        return 0;
    }

    if(argc > 2){
        more_than_one_file = true;
    }

    if(more_than_one_file){

        //enviando arquivos para serem montados
        for(int i = 1; i<argc; i++){
            
            ifstream input_file(argv[i]);
            
            string line;
            
            int j = 0;
            while(getline(input_file, line)) {
                string processed_line = line.substr(3, line.size());
                
                //linha contendo tamanho dos obj
                if(j == 1){
                    //modulo A
                    if(i == 1){
                        size_mod_A = stoi(processed_line);
                    }
                    //modulo B
                    if(i == 2){
                        size_mod_B = stoi(processed_line);
                    }
                    //modulo C
                    if(i == 3){
                        size_mod_C = stoi(processed_line);
                    }
                }

                //linha contendo informacao de realocacao;
                if(j == 2){
                    //modulo A
                    if(i == 1){
                        realloc_mod_A = line.substr(3, line.size());
                    }
                    //modulo B
                    if(i == 2){
                        realloc_mod_B = line.substr(3, line.size());
                    }
                    //modulo C
                    if(i == 3){
                        realloc_mod_C = line.substr(3, line.size());
                    }
                }

                if(contains(line, "T:") != -1){
                    stringstream ss(processed_line);
                    string word;
                    while(ss >> word){
                        T.push_back(stoi(word));
                    }
                }

                //montar tabela de definicoes
                if(contains(line, "D:") != -1){
                    stringstream ss(processed_line);
                    string word;
                    int k = 0;
                    pair<string, int> aux;
                    while(ss >> word){
                        //primeiro arquivo
                        if(i == 1){
                            if(k == 0){
                                //label
                                aux.first = word;
                            }
                            if(k == 1){
                                //valor
                                aux.second = stoi(word);
                            }
                        }
                        //segundo arquivo
                        else if(i == 2){
                            if(k == 0){
                                //label
                                aux.first = word;
                            }
                            if(k == 1){
                                //valor + fator de  correcao (tamanho do primeiro arquivo)
                                aux.second = stoi(word) + size_mod_A;
                            }
                        }
                        //terceiro arquivo
                        else if(i == 3){
                            if(k == 0){
                                //label
                                aux.first = word;
                            }
                            if(k == 1){
                                //valor + fator de  correcao (tamanho do primeiro arquivo + do segundo)
                                aux.second = stoi(word) + size_mod_A + size_mod_B;
                            }
                        }
                        k++;
                    }
                    global_definitions_table.push_back(aux);
                }

                //montar tabelas de uso
                if(contains(line, "U:") != -1){
                    stringstream ss(processed_line);
                    string word;
                    int k = 0;
                    pair<string, int> aux;
                    while(ss >> word){
                        //primeiro arquivo
                        if(i == 1){
                            if(k == 0){
                                //label
                                aux.first = word;
                            }
                            if(k == 1){
                                //valor
                                aux.second = stoi(word);
                            }
                        }
                        //segundo arquivo
                        else if(i == 2){
                            if(k == 0){
                                //label
                                aux.first = word;
                            }
                            if(k == 1){
                                //valor + fator de  correcao (tamanho do primeiro arquivo)
                                aux.second = stoi(word) + size_mod_A;
                            }
                        }
                        //terceiro arquivo
                        else if(i == 3){
                            if(k == 0){
                                //label
                                aux.first = word;
                            }
                            if(k == 1){
                                //valor + fator de  correcao (tamanho do primeiro arquivo + do segundo)
                                aux.second = stoi(word) + size_mod_A + size_mod_B;
                            }
                        }
                        k++;
                    }
                    if(i == 1) use_mod_A.push_back(aux);
                    else if(i == 2) use_mod_B.push_back(aux);
                    else if(i == 3) use_mod_C.push_back(aux);
                }

                j++;
            }

            input_file.close();
        }

        //corrigir enderecos cruzados de cada tabela de uso
        for(auto a: use_mod_A){
            T[a.second] += search_in_def_tab(a.first);
        }
        for(auto a: use_mod_B){
            T[a.second] += search_in_def_tab(a.first);
        }
        for(auto a: use_mod_C){
            T[a.second] += search_in_def_tab(a.first);
        }

        //corrigir restante de enderecos

        //no modulo A nao eh necessario poois ele vem primeiro
        for(int i=0; i<realloc_mod_B.size(); i++){
            
            //realocacao necessaria
            if(realloc_mod_B[i] == '1'){
                //fator de correcao eh igual ao tamanho do arquivo anterior
                T[size_mod_A+i] += size_mod_A;
            }
        }
        for(int i=0; i<realloc_mod_C.size(); i++){
            
            //realocacao necessaria
            if(realloc_mod_C[i] == '1'){
                //fator de correcao eh igual ao tamanho dos dois arquivos anteriores
                T[size_mod_A+size_mod_B+i] += size_mod_A + size_mod_B;
            }
        }


        //preparar output
        ofstream output("result.obj");
        for(auto a: T){
            output << a << " ";
        }
        output << endl;
        output.close();
    }

    

    //nao necessita ligacao, somente executar o que esta apos T:
    else{
        ifstream input_file(argv[1]);
        string line;
        while(getline(input_file, line)){
            if(contains(line, "T:") != -1){

                ofstream output("result.obj");
                output << line.substr(3, line.size()) << endl;
                output.close();
            }
        }
        input_file.close();
    }

    return 0;
}