# TRABALHO SB

Montador e Simulador para o assembly hipotético visto em aula, feito em C++

ALUNO: JOAO PEDRO ASSUNCAO COUTINHO - 18/0019813


- INSTRUCOES DE COMPILACAO:

Compile com MakeFile usando:
	"make"

Ou, caso necessário compilar os arquivos individualmente,
Para o montador:
        "g++ -o <nomedesejado> montador.cc"
Para o ligador:
        "g++ -o <nomedesejado> ligador.cc"
Para o simulador:
        "g++ -o <nomedesejado> simulador.cc"



- OBSERVACOES:

1. O formato utilizado espera que as declarações PUBLIC e EXTERN estejam DENTRO da seção DATA, como apresentado nos arquivos de exemplo:
"fat_mod_A - Copia.asm" e "fat_mod_B - Copia.asm"

2. MAPA DE BITS foi usado como método de realocação.

3. A saída do ligador sempre terá como nome "result.obj".