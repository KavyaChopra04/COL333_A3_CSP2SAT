maxClique:	maxClique.cpp
	g++ -o maxClique maxClique.cpp
	./maxClique mc1
part1: SATwriterP1better.cpp
	g++ -o part1 SATwriterP1better.cpp
	./part1 p1
clear:
	rm -f maxClique
	rm -f part1GraphReconstruct
	rm -f part1SATInput
	rm -f *.satinput
	rm -f *.satoutput
	rm -f *.mapping
	rm -f *.txt
	