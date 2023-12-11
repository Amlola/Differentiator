all: Diff

Diff: main.o tree.o differentiator.o
	g++ -g -DDUMP -o Diff.exe obj/main.o obj/tree.o obj/differentiator.o

main.o: src/main.cpp
	g++ -g -DDUMP -c src/main.cpp -o obj/main.o

tree.o: src/tree.cpp
	g++ -g -DDUMP -c src/tree.cpp -o obj/tree.o

differentiator.o: src/differentiator.cpp
	g++ -g -DDUMP -c src/differentiator.cpp -o obj/differentiator.o

clean:
	rm -rf obj/* Diff.exe