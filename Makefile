maxClique:	maxClique.cpp
	g++ -o maxClique maxClique.cpp
	./maxClique mc1
part1: SATwriterP1better.cpp
	g++ -o part1 SATwriterP1better.cpp
	./part1 p1
clear:
	rm *.satinput
	rm *.satoutput
	rm *.mapping
	rm *.txt
	rm maxClique
	rm part1