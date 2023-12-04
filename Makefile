all: Diff

Diff: main.o tree.o differentiator.o
	g++ -g -DDUMP -o Diff.exe main.o tree.o differentiator.o

main.o: main.cpp
	g++ -g -DDUMP -c main.cpp

tree.o: tree.cpp
	g++ -g -DDUMP -c tree.cpp

differentiator.o: differentiator.cpp
	g++ -g -DDUMP -c differentiator.cpp

clean:
	rm *.o Diff.exe