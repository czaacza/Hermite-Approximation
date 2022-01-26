hermite: main.o splines.o points.o hermite.o gaus/libge.a
	$(CC) -o hermite  main.o splines.o points.o hermite.o -L gaus -l ge

hermite.o: makespl.h points.h gaus/piv_ge_solver.h
	$(CC) -I gaus -c hermite.c

.PHONY: clean

clean:
	-rm *.o hermite
