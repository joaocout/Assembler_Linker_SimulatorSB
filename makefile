all: montador ligador simulador

montador:
	g++ -o montador montador.cc
ligador:
	g++ -o ligador ligador.cc
simulador:
	g++ -o simulador simulador.cc

clear:
	rm montador ligador simulador