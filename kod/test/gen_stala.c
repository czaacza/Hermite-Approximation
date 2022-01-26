#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
// generator punktow dla funkcji stalej i zapisujacej wspolrzedne
// punktow do pliku
int main( int argc, char **argv ) {
				int n= argc > 1 ? atoi( argv[1] ) : 100;
				double a= argc > 2 ? atof( argv[2] ) : -3;
				double b= argc > 3 ? atof( argv[3] ) : 2;
				FILE *out= argc > 4 ? fopen( argv[4], "w" ) : stdout;

				int i;
				double dx = (b-a)/(n-1);

                // wpisuje do pliku out punkty funkcji stalej o wartosciach dx
                // dla kazdego x z <a, b>
				for( i =0; i < n; i++ ) {
								fprintf( out, "%g %g\n", a+i*dx, a );
				}
				fclose( out );

				return 0;
}
