CC=g++ -std=c++2a
all: notas

LINKS: -stdc++fs
notas: notas.cpp fonte.cpp
	$(CC) notas.cpp fonte.cpp -o  notas $(LINKS)

clear:
	rm *.txt
