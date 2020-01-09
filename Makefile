project4: main.o bezier_c.o bspline_c.o utility.o
	g++ -g -std=c++11 -Wall -o draw main.o bezier_c.o bspline_c.o utility.o -lglut -lGL

main.o: main.cpp utility.h bezier_c.h bspline_c.h
	g++ -g -std=c++11 -Wall -c -o main.o main.cpp -lglut -lGL

bezier_c.o: bezier_c.cpp bezier_c.h utility.h
	g++ -g -std=c++11 -Wall -c -o bezier_c.o bezier_c.cpp -lglut -lGL

bspline_c.o: bspline_c.cpp bspline_c.h utility.h
	g++ -g -std=c++11 -Wall -c -o bspline_c.o bspline_c.cpp -lglut -lGL

utility.o: utility.cpp utility.h
	g++ -g -std=c++11 -Wall -c -o utility.o utility.cpp -lglut -lGL

clean:
	rm -f *.o project4