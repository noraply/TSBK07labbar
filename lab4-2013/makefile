
all : lab4-1 lab4-2 lab4-3

lab4-1: lab4-1.c GL_utilities.c VectorUtils2.c loadobj.c LoadTGA2.c
	gcc -Wall -std=c99 -o $@ -DGL_GLEXT_PROTOTYPES $^  -lGL -lglut -lm

lab4-2: lab4-2.c GL_utilities.c VectorUtils2.c loadobj.c LoadTGA2.c
	gcc -Wall -std=c99 -o $@ -DGL_GLEXT_PROTOTYPES $^  -lGL -lglut -lm

lab4-3: lab4-3.c GL_utilities.c VectorUtils2.c loadobj.c LoadTGA2.c
	gcc -Wall -std=c99 -o $@ -DGL_GLEXT_PROTOTYPES $^  -lGL -lglut -lm -g

clean :
	rm lab4-{1,2,3}
