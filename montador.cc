//JOAO PEDRO ASSUNCAO COUTINHO - 18/0019813

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <list>
#include <utility>

using namespace std;


//TABELA DE INSTRUCOES, first = nome, second = tamanho
list<pair<string, int>> instructions_table;

//BUSCA ITEM NA TABELA DE INSTRUCOES, retorna o tamanho da instrucao, 
//ou -1 caso nao seja encontrada.
int search_instruction(string ins){
    for(auto instruction : instructions_table) {
        if(instruction.first == ins){
            return instruction.second;
        }
    }
    return -1;
}
//RETORNA O OPCODE DE UMA INSTRUCAO
int get_opcode(string ins){
    int opcode = 1;
    for(auto instruction : instructions_table) {
        if(instruction.first == ins){
            return opcode;
        }
        opcode++;
    }
    return -1;
}



//TABELA DE SIMBOLOS, first = label, second = valor armazenado
list<pair<string, int>> symbols_table;

//BUSCA ITEM NA TABELA DE SIMBOLOS, retorna a posicao na memoria, 
//ou -1 caso a label nao seja encontrada.
int search_symbol(string label){
    for(auto symbol : symbols_table){
        if(symbol.first == label){
            return symbol.second;
        }
    }
    return -1;
}



//TABELA DE DIRETIVAS, first = nome da diretiva, second = espaco ocupado
list<pair<string, int>> directives_table;

//BUSCA DIRETIVA NA TABELA, retorna o espaco reservado pela diretiva, caso encontrado,
//ou -1 caso a diretiva nao seja encontrada.
int search_directive(string name) {
    for(auto directive : directives_table) {
        if(directive.first == name){
            return directive.second;
        }
    }
    return -1;
}



//checa se uma string contem outra string, retorna a posicao da primeira ocorrencia da
//substring, caso encontrada, ou -1, caso nao encontrada.
int contains(string s, string search){
    if(s.find(search) != string::npos){
        return s.find(search);
    }
    return -1;
}



//recebe uma string e retorna true se essa string eh composta somente de digitos e falso caso contrario
bool isInteger(string s){
    return s.find_first_not_of("-0123456789") == string::npos;
}



int main (int argc, char **argv) {

    if(argc < 2){
        cout << "erro, necessario informar nome do arquivo" << endl;
        return 0;
    }

    ifstream input_file(argv[1]);
    if(!input_file.is_open()){
        cout << "falha ao abrir o arquivo informado" << endl;
        return 0;
    }

    //INICIALIZANDO TABELA DE DIRETIVAS    
    directives_table.push_back(make_pair("const", 1));
    directives_table.push_back(make_pair("space", 1));

    //INICIALIZANDO TABELA DE INSTRUCOES
    instructions_table.push_back(make_pair("add", 2));
    instructions_table.push_back(make_pair("sub", 2));
    instructions_table.push_back(make_pair("mult", 2));
    instructions_table.push_back(make_pair("div", 2));
    instructions_table.push_back(make_pair("jmp", 2));
    instructions_table.push_back(make_pair("jmpn", 2));
    instructions_table.push_back(make_pair("jmpp", 2));
    instructions_table.push_back(make_pair("jmpz", 2));
    instructions_table.push_back(make_pair("copy", 3));
    instructions_table.push_back(make_pair("load", 2));
    instructions_table.push_back(make_pair("store", 2));
    instructions_table.push_back(make_pair("input", 2));
    instructions_table.push_back(make_pair("output", 2));
    instructions_table.push_back(make_pair("stop", 1));

    //LINHAS DO PROGRAMA, JÁ PROCESSADAS
    list<string> processed_lines;
    

    string raw_line;
    //LENDO E PROCESSANDO O ARQUIVO
    while(getline(input_file, raw_line)) {
        //convertendo para lowercase
        transform(raw_line.begin(), raw_line.end(), raw_line.begin(), ::tolower);
        
        stringstream ss(raw_line);
        string processed_line;
        string word;
        while(ss >> word){
            //processar a palavra somente se nao for um comentario
            if(word[0] != ';'){
                if(processed_line.empty()){
                    processed_line = word;
                }
                else{
                    processed_line.append(" " + word);
                }
            }
        }
        if(!processed_line.empty()){
            processed_lines.push_back(processed_line);
        }
    }
    input_file.close();



    //SEPARANDO TEXT E DATA, E INVERTENDO, CASO NECESSARIO
    int text_pos = -1, data_pos = -1;
    int pos = 0;
    for(auto line : processed_lines){
        //se a linha atual contem section text
        if(contains(line, "section text") != -1){
            text_pos = pos;
        }
        //se a linha atual contem section data
        if(contains(line, "section data") != -1){
            data_pos = pos;
        }
        pos++;
    }



    //SE HA AS DUAS SECOES, E DATA VEM ANTES DE TEXT, INVERTEM-SE AS SECOES
    if(text_pos != -1 && data_pos != -1){
        if(data_pos < text_pos){
            //levando a section data de cima para baixo
            for(int i = 0; i < text_pos; i++){
                string aux = processed_lines.front();
                processed_lines.pop_front();
                processed_lines.push_back(aux);
            }

        }
    }



    //PRIMEIRA PASSAGEM
    int location_counter=0;
    for(auto line: processed_lines){
        
        //pulando linhas de declaracao de secao
        if(contains(line, "section") != -1){
            continue;
        }

        //removendo virgulas
        if(contains(line, "copy") != -1) {
            replace(line.begin(), line.end(), ',', ' ');
        }

        stringstream ss(line);
        string word;
        //num de operandos esperados apos uma operacao encontrada

        //tokens da linha separados
        list<string> tokens;
        while(ss >> word){
            tokens.push_back(word);
        }

        //ajuda a saber se o erro de numero de operandos deve ser lancado
        int token_counter = 0;

        //numero de operandos esperados a seguir
        int expected_next = 0;
        for(string token: tokens){

            //esperam-se operandos, porem no momento, nao é necessario fazer nada
            if(expected_next > 0){
                continue;
            }

            //o token eh uma label
            else if(contains (token, ":") != -1){
                string aux = token.substr(0, token.size()-1);
                if(search_symbol(aux) != -1){
                    cout << "Erro Semantico: Label ja definida." << endl << line << endl;
                    return 0; 
                }
                else{
                    symbols_table.push_back(make_pair(aux, location_counter));
                }
            }

            //o token nao eh operando nem label
            else{
                int inst_size = search_instruction(token);
                int dir_size = search_directive(token);

                if(inst_size != -1) {
                    location_counter += inst_size;
                    expected_next = inst_size - 1;
                }

                else if(dir_size != -1){
                    location_counter += dir_size;
                    if(contains(token, "const") != -1){
                        expected_next = 1;
                    }
                    else{
                        expected_next = dir_size - 1;
                    }
                }
                //se o token nao eh uma operacao, um erro pode ser lancado
                else {
                    cout << "Erro Lexico: Operacao invalida / Token nao identificado." << endl  << line << endl;
                    return 0;
                }

                //se a quantidade de tokens restantes na linha, for diferente da qtd de tokens que a operacao espera, lanca-se um erro
                if(tokens.size() - token_counter - 1 != expected_next){
                    cout << "Erro Sintatico: Numero de operandos incorreto" << endl << line << endl;
                    return 0;
                }

            }
            token_counter++;
        }
    }

    //lista com as strings resultantes da montagem
    list<string> result;


    //SEGUNDA PASSAGEM
    location_counter = 0;
    for(auto line: processed_lines){

        //pulando linhas de declaracao de secao
        if(contains(line, "section") != -1){
            continue;
        }

        //removendo virgulas
        if(contains(line, "copy") != -1) {
            replace(line.begin(), line.end(), ',', ' ');
        }

        stringstream ss(line);
        string word;
        //num de operandos esperados apos uma operacao encontrada

        //tokens da linha separados
        list<string> tokens;
        while(ss >> word){
            tokens.push_back(word);
        }

        //ajuda a saber se o erro de numero de operandos deve ser lancado
        int token_counter = 0;

        //numero de operandos esperados
        int expected_next = 0;

        //facilita saber se a ultima operacao foi um const
        bool last_was_const = false;
        for(string token: tokens){

            //ignorando label
            if(contains(token, ":") != -1){
                continue;
            }

            //esperam-se operandos
            else if(expected_next > 0) {
                if(last_was_const){
                    //se for const, o resultado eh o proprio resultado
                    if(!isInteger(token)){
                        cout << "Erro Lexico: Valor de operando incorreto" << endl << line << endl;
                        return 0;
                    }
                    result.push_back(token);
                    last_was_const = false;
                }
                else {
                    int aux = search_symbol(token);
                    if(aux != -1){
                        result.push_back(to_string(aux));
                    }
                    else{
                        cout << "Erro Semantico: Simbolo nao encontrado/nao declarado." << endl << line << endl;
                        return 0;
                    }
                }
                expected_next--;
            }

            else{
                int inst_size = search_instruction(token);
                int dir_size = search_directive(token);

                if(inst_size != -1) {
                    result.push_back(to_string(get_opcode(token)));
                    location_counter += inst_size;
                    expected_next = inst_size - 1;
                }

                else if(dir_size != -1){
                    location_counter += dir_size;
                    if(contains(token, "const") != -1){
                        last_was_const = true;
                        expected_next = 1;
                    }
                    else{
                        result.push_back("0");
                        expected_next = dir_size - 1;
                    }
                }

            token_counter++;
            }
        }
    }
    


    //GERANDO ARQUIVO OBJ
    string output_name (argv[1]);

    //gernado nome do arquivo resultante
    if(output_name.find('.') != string::npos){
        output_name = output_name.substr(0, output_name.find('.')).append(".obj");
    }
    else{
        output_name.append(".obj");
    }

    ofstream output (output_name);
    for(auto r : result){
        output << r + " ";
    }
    output << endl;

    output.close();

    return 0;
}