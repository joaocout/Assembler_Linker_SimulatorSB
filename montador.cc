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


//BUSCA ITEM NA TABELA DE SIMBOLOS, retorna a posicao na memoria, 
//ou -1 caso a label nao seja encontrada.
int search_symbol(list<pair<string, int>> symbols_table, string label){
    for(auto symbol : symbols_table){
        if(symbol.first == label){
            return symbol.second;
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


//flag controlando se ha mais de um arquivo
bool more_than_one_file = false;


//recebe qual arquivo deve ser montado
void mount(string file_name){
    
    //TABELA DE SIMBOLOS, first = label, second = valor armazenado
    list<pair<string, int>> symbols_table;
    
    //TABELA DE USO, first = label, second = posicao que foi usado
    list<pair<string, int>> use_table;

    //TABELA DE DEFINICOES
    list<pair<string, int>> definitions_table;

    ifstream input_file(file_name);
    if(!input_file.is_open()){
        cout << "falha ao abrir o arquivo informado" << endl;
        return;
    }


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
            
            string begin;
            string end;
            int i=0;
            if(more_than_one_file == true){
                begin = processed_lines.front();
                end = processed_lines.back();
                processed_lines.pop_front();
                processed_lines.pop_back();
                i=1;
            }


            //levando a section data de cima para baixo
            for(; i < text_pos; i++){
                string aux = processed_lines.front();
                processed_lines.pop_front();
                processed_lines.push_back(aux);
            }

            if(more_than_one_file == true){
                processed_lines.push_front(begin);
                processed_lines.push_back(end);
            }

        }
    }

    //PRIMEIRA PASSAGEM
    int location_counter=0;

    //usado para verificar presenca de begin e end, quando necessario
    int begin_end = 0;
    list <string> temp_definitions_table;
    for(auto line: processed_lines){
        
        //pulando linhas de declaracao de secao
        if(contains(line, "section") != -1){
            continue;
        }

        //removendo virgulas
        if(contains(line, "copy") != -1) {
            replace(line.begin(), line.end(), ',', ' ');
        }

        //label externa encontrada
        if(contains(line, "extern") != -1){
            string aux = line.substr(0, line.find(':'));
            //-9 no tamanho, indica que a label eh externa
            symbols_table.push_back(make_pair(aux, -9));
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
        bool last_was_public = false;
        for(string token: tokens){

            //esperam-se operandos, porem no momento, nao é necessario fazer nada
            if(expected_next > 0){
                if(last_was_public){
                    temp_definitions_table.push_back(token);
                    last_was_public = false;
                    expected_next--;
                }
                else {
                    continue;
                }
            }

            //o token eh uma label
            else if(contains (token, ":") != -1){
                string aux = token.substr(0, token.size()-1);
                if(search_symbol(symbols_table, aux) == -1){
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
                    else if(contains(token, "public") != -1){
                        last_was_public=true;
                        expected_next = 1;
                    }
                    else if(contains(token, "begin") != -1 || contains(token, "end") != -1){
                        begin_end++;
                    }
                    else{
                        expected_next = 0;
                    }
                }

            }
            token_counter++;
        }
    }

    if(more_than_one_file){
        if(begin_end != 2){
            cout << "begin e end são obrigatórios, erro!" << endl;
            return;
        }
    }
    else{
        if(begin_end > 0){
            cout << "begin e end só devem ser usados quando montando mais de um arquivo, erro!" << endl;
            return;
        }
    }

    //copiando dados da tabela de simbolos para a de definicoes
    for(string s: temp_definitions_table){
        pair <string, int> aux = make_pair(s, search_symbol(symbols_table, s));
        definitions_table.push_back(aux);
    }

    //lista com as strings resultantes da montagem
    list<string> result;
    string realocation = "";
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
                        return;
                    }
                    result.push_back(token);
                    realocation.append("0");
                    last_was_const = false;
                }
                else {
                    int aux = search_symbol(symbols_table, token);
                    //quando extern
                    if(aux == -9){
                        use_table.push_back(make_pair(token, location_counter-1));
                        result.push_back("0");
                        realocation.append("0");
                    }
                    else if(aux != -1){
                        result.push_back(to_string(aux));
                        realocation.append("1");
                    }
                    else{
                        cout << "Erro Semantico: Simbolo nao encontrado/nao declarado." << endl << line << endl;
                        return;
                    }
                }
                expected_next--;
            }

            else{
                int inst_size = search_instruction(token);
                int dir_size = search_directive(token);

                if(inst_size != -1) {
                    result.push_back(to_string(get_opcode(token)));
                    realocation.append("0");
                    location_counter += inst_size;
                    expected_next = inst_size - 1;
                }

                else if(dir_size != -1){
                    location_counter += dir_size;
                    if(contains(token, "const") != -1){
                        last_was_const = true;
                        expected_next = 1;
                    }
                    else if (contains(token, "space") != -1){
                        result.push_back("0");
                        realocation.append("0");
                        expected_next = dir_size - 1;
                    }
                }

            token_counter++;
            }
        }
    }
    //GERANDO ARQUIVO OBJ
    string output_name = file_name;

    //gernado nome do arquivo resultante
    if(output_name.find('.') != string::npos){
        output_name = output_name.substr(0, output_name.find('.')).append(".obj");
    }
    else{
        output_name.append(".obj");
    }

    ofstream output (output_name);
    output << "H: " << output_name.substr(0, output_name.find('.')) << endl;
    output << "H: " << location_counter << endl;
    output << "H: " << realocation << endl;

    if(more_than_one_file){
        for(auto a: use_table){
            output << "U: " << a.first << " " << a.second << endl;
        }   
    
        for(auto a: definitions_table) {
            output << "D: " << a.first << " " << a.second << endl;
        }
    }

    output << "T: ";
    for(auto r : result){
        output << r + " ";
    }
    output << endl;

    output.close();

    return;
}


int main (int argc, char **argv) {

    //INICIALIZANDO TABELA DE DIRETIVAS    
    directives_table.push_back(make_pair("const", 1));
    directives_table.push_back(make_pair("space", 1));
    directives_table.push_back(make_pair("extern", 0));
    directives_table.push_back(make_pair("public", 0));
    directives_table.push_back(make_pair("begin", 0));
    directives_table.push_back(make_pair("end", 0));

    //INICIALIZANDO TABELA DE INSTRUCOES
    instructions_table.push_back(make_pair("add", 2));
    instructions_table.push_back(make_pair("sub", 2));
    instructions_table.push_back(make_pair("mul", 2));
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

    if(argc < 2){
        cout << "erro, necessario informar nome do arquivo" << endl;
        return 0;
    }

    if(argc > 2) {
        more_than_one_file = true;
    }
    else {
        more_than_one_file = false;
    }

    //enviando arquivos para serem montados
    for(int i = 1; i<argc; i++){
        mount(argv[i]);
    }

    return 0;
}
