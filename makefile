all: clean exec

exec: main.o
	g++ main.o -o app.exec -lGL -lGLU -lGLEW -lglfw3 -lX11 -lXxf86vm -lXrandr -lpthread -lXi -ldl -lXinerama -lXcursor 
main.o: main.cpp 
	g++ -std=c++11 -c main.cpp

clean:
	rm -f *.o
