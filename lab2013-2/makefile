all : lab2-1 lab2-2 lab2-3 lab2-4 lab2-4n lab2-5 lab2-6 lab2-7 lab2-7n lab2-8

lab2-1: lab2-1.c GL_utilities.c loadobj.c
	gcc -Wall -std=c99 -o lab2-1 -lGL -lglut -lm -DGL_GLEXT_PROTOTYPES lab2-1.c GL_utilities.c loadobj.c

lab2-2: lab2-2.c GL_utilities.c loadobj.c LoadTGA.c
	gcc -Wall -std=c99 -o lab2-2 -lGL -lm -lglut -DGL_GLEXT_PROTOTYPES lab2-2.c GL_utilities.c loadobj.c LoadTGA.c

lab2-3: lab2-3.c GL_utilities.c loadobj.c LoadTGA.c VectorUtils2.c
	gcc -Wall -std=c99 -o $@ -lGL -lm -lglut -DGL_GLEXT_PROTOTYPES $^

lab2-4: lab2-4.c GL_utilities.c loadobj.c LoadTGA.c VectorUtils2.c
	gcc -Wall -std=c99 -o $@ -lGL -lm -lglut -DGL_GLEXT_PROTOTYPES $^

lab2-4n: lab2-4n.c GL_utilities.c loadobj.c LoadTGA.c VectorUtils2.c
	gcc -Wall -std=c99 -o $@ -lGL -lm -lglut -DGL_GLEXT_PROTOTYPES $^

lab2-5: lab2-5.c GL_utilities.c loadobj.c LoadTGA.c VectorUtils2.c
	gcc -Wall -std=c99 -o $@ -lGL -lm -lglut -DGL_GLEXT_PROTOTYPES $^

lab2-6: lab2-6.c GL_utilities.c loadobj.c LoadTGA.c VectorUtils2.c
	gcc -Wall -std=c99 -o $@ -lGL -lm -lglut -DGL_GLEXT_PROTOTYPES $^

lab2-7: lab2-7.c GL_utilities.c loadobj.c LoadTGA.c VectorUtils2.c
	gcc -Wall -std=c99 -o $@ -lGL -lm -lglut -DGL_GLEXT_PROTOTYPES $^

lab2-7n: lab2-7n.c GL_utilities.c loadobj.c LoadTGA.c VectorUtils2.c
	gcc -Wall -std=c99 -o $@ -lGL -lm -lglut -DGL_GLEXT_PROTOTYPES $^

lab2-8: lab2-8.c GL_utilities.c loadobj.c LoadTGA.c VectorUtils2.c
	gcc -Wall -std=c99 -o $@ -lGL -lm -lglut -DGL_GLEXT_PROTOTYPES $^

clean :
	rm lab2-{1,2,3,4,4n,5,6,7,7n,8}
