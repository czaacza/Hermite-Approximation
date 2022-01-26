test_sin: gen_sin.o
	gcc -o test_sin gen_sin.o -lm
	./test_sin 1000 1 100 punkty_sin

test_potega: gen_potega.o
	gcc -o test_potega gen_potega.o -lm
	./test_potega 1000 1 10 punkty_potega

test_stala: gen_stala.o
	gcc -o test_stala gen_stala.o
	./test_stala 1000 1 10 punkty_stala

gen_punkty.o:
	gcc -c gen_potega.c gen_stala.c gen_sin.c -lm

