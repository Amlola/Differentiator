all: Diff

Diff: src/main.o src/tree.o src/differentiator.o
	g++ -g -DDUMP -o Diff.exe src/main.o src/tree.o src/differentiator.o

main.o: src/main.cpp
	g++ -g -DDUMP -c src/main.cpp

tree.o: src/tree.cpp
	g++ -g -DDUMP -c src/tree.cpp

differentiator.o: src/differentiator.cpp
	g++ -g -DDUMP -c src/differentiator.cpp

clean:
	rm *.o Diff.exe