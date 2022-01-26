#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

// generator punktow dla funkcji potegowej 2^x i zapisujacej
// wspolrzedne punktow do pliku
int main( int argc, char **argv ) {
				int n= argc > 1 ? atoi( argv[1] ) : 100;
				double a= argc > 2 ? atof( argv[2] ) : -3;
				double b= argc > 3 ? atof( argv[3] ) : 2;
				FILE *out= argc > 4 ? fopen( argv[4], "w" ) : stdout;

				srand( argc > 5 ? atoi(argv[5]) : time(NULL) );

				int i;
				double dx = (b-a)/(n-1);

				for( i =0; i < n; i++ ) {
								fprintf( out, "%g %g\n", a+i*dx, pow(2, a+i*dx) );
				}
				fclose( out );

				return 0;
}
