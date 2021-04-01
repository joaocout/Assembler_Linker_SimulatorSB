//JOAO PEDRO ASSUNCAO COUTINHO - 18/0019813

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;


int main (int argc, char** argv){

    ifstream input(argv[1]);
    string raw_line;
    
    vector<int> words;
    vector<int> outputs;

    //LENDO ARQUIVO E GRAVANDO NUM VETOR
    while(getline(input, raw_line)){
        stringstream ss(raw_line);
        string word;

        while(ss >> word){
            words.push_back(stoi(word));            
        }
    }
    
    input.close();


    //LOOP DE EXECUCAO
    int acc=0;
    for(int pc = 0; pc<words.size(); pc++) {
        int word = words[pc];
        
        //add
        if(word == 1){
            //pegar o proximo operando
            pc++;
            acc += words[words[pc]];
            std::cout << "Apos ADD:" << endl;
        }
        
        //sub
        else if(word == 2){
            pc++;
            acc -= words[words[pc]];
            std::cout << "Apos SUB:" << endl;
        }

        //mult
        else if(word == 3){
            pc++;
            acc *= words[words[pc]];
            std::cout << "Apos MULT:" << endl;
        }

        //div
        else if(word == 4){
            pc++;
            acc /= words[words[pc]];
            std::cout << "Apos DIV:" << endl;
        }

        //jmp
        else if(word == 5){
            pc++;
            pc = words[pc] - 1;
            std::cout << "Apos JMP:" << endl;
        }

        //jmpn
        else if(word == 6){
            pc++;
            if(acc < 0){
                pc = words[pc] - 1;
            }
            std::cout << "Apos JMPN:" << endl;
        }

        //jmpp
        else if(word == 7){
            pc++;
            if(acc > 0){
                pc = words[pc] - 1;
            }
            std::cout << "Apos JMPP:" << endl;
        }
        
        //jmpz
        else if(word == 8){
            pc++;
            if(acc == 0){
                pc = words[pc] - 1;
            }
            std::cout << "Apos JMPZ:" << endl;
        }
        
        //copy
        else if(word == 9){
            //copy ocupa um a mais que o resto
            words[words[pc+2]] = words[words[pc+1]];
            //pulando ambos operandos do copy
            pc+=2;
            std::cout << "Apos COPY:" << endl;
        }
        
        //load
        else if(word == 10){
            pc++;
            acc = words[words[pc]];
            std::cout << "Apos LOAD:" << endl;
        }

        //store  
        else if(word == 11){
            pc++;
            words[words[pc]] = acc;
            std::cout << "Apos STORE:" << endl;
        }

        //input 
        else if(word == 12){
            pc++;
            int temp;
            cin >> temp;
            words[words[pc]] = temp;
            std::cout << "Apos INPUT:" << endl;
        }

        //output 
        else if(word == 13){
            pc++;
            std::cout << "Apos OUTPUT:" << endl;
            std::cout << "OUTPUT: " << words[words[pc]] << endl;
            outputs.push_back(words[words[pc]]);
        }
        
        //stop
        else if(word == 14){
            //stop ocupa um a menos que o resto
            return 0;
        }

        std::cout << "PC -> " << pc +1  << endl;
        std::cout << "ACC -> " << acc << endl << endl;

        //GERANDO ARQUIVO OUT
        string output_name (argv[1]);

        //gernado nome do arquivo resultante
        if(output_name.find('.') != string::npos){
            output_name = output_name.substr(0, output_name.find('.')).append(".out");
        }
        else{
            output_name.append(".out");
        }

        ofstream output (output_name);
        for(auto r : outputs){
            output << r << endl;
        }
        output << endl;

        output.close();

        }

    return 0;
}