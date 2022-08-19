all: compile link clean

observer:
	main.o

compile:
	g++ -c main.cpp

link:
	g++ main.o -o main -lsfml-graphics -lsfml-window -lsfml-system

clean:
	rm -f *o observer
