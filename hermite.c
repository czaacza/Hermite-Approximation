#include "makespl.h"
#include "piv_ge_solver.h"

#include <stdio.h>
#include <stdlib.h>
#include <float.h>

/* UWAGA: liczbę używanych f. bazowych można ustawić przez wartość
		  zmiennej środowiskowej APPROX_BASE_SIZE
*/

/*
 * Base functions: n - number of functions
 * a,b - limits of the approximation interval
 * i - number of function
 * x - coordinate
 */
double
fi(int i, double x)	//hermite polynomials base function
{
	if (i == 0)
		return 1;
	else if (i == 1)
		return 2 * x;

	else
		return 2 * x * fi(i - 1, x) - 2 * (i - 1) * fi(i - 2, x);
}

/* First fi derivative */
double
dfi(int i, double x)
{
	if (i == 0)
		return 0;
	else if (i == 1)
		return 2;
	else
		return 2 * i * fi(i - 1, x);
}

/* Second fi derivative */
double
d2fi(int i, double x)
{
	if (i == 0)
		return 0;
	else if (i == 1)
		return 0;
	else if (i == 2)
		return 8;
	else
		return 4 * i * (i - 1) * fi(i - 2, x);
}

/* Third fi derivative */
double
d3fi(int i, double x)
{
	if (i == 0)
		return 0;
	else if (i == 1)
		return 0;
	else if (i == 2)
		return 0;
	else if (i == 3)
		return 48;
	else
		return 8 * i * (i - 1) * (i - 2) * fi(i - 3, x);
}


void make_spl(points_t *pts, spline_t *spl)
{

	matrix_t *eqs = NULL;
	double *x = pts->x;
	double *y = pts->y;
	double a = x[0];
	double b = x[pts->n - 1];
	int i, j, k;
	int nb = pts->n - 3 > 10 ? 10 : pts->n - 3;
	char *nbEnv = getenv("APPROX_BASE_SIZE"); 

		if (nbEnv != NULL && atoi(nbEnv) > 0)	//if APPROX_BASE_SIZE is specified, give nb its value
			nb = atoi(nbEnv);
	
	eqs = make_matrix(nb, nb + 1);					//create matrix [nb x (nb+1)]

	for (j = 0; j < nb; j++)
	{
		for (i = 0; i < nb; i++)
			for (k = 0; k < pts->n; k++)				//add a sum of n elements of [fi(i)(xk) * fi(j)(xk)] to matrix[i,j] 
				add_to_entry_matrix(eqs, j, i, fi(i, x[k]) * fi(j, x[k]));

		for (k = 0; k < pts->n; k++)					//add the sum of [fi(j)(xk) * y[k]] to the last column of the matrix
			add_to_entry_matrix(eqs, j, nb, y[k] * fi(j, x[k]));
	}


	if (piv_ge_solver(eqs))		//solve the matrix with gauss elimination through pivoting
	{
		spl->n = 0;
		return;
	}

	if (alloc_spl(spl, nb) == 0)	//allocate memory for the spline elements
	{
		for (i = 0; i < spl->n; i++)
		{
			double xx = spl->x[i] = a + i * (b - a) / (spl->n - 1);		//divide values into intervals
			xx += 10.0 * DBL_EPSILON; // zabezpieczenie przed ulokowaniem punktu w poprzednim przedziale
			spl->f[i] = 0;
			spl->f1[i] = 0;
			spl->f2[i] = 0;
			spl->f3[i] = 0;
			for (k = 0; k < nb; k++)
			{
				double ck = get_entry_matrix(eqs, k, nb); // ck - function slope
				spl->f[i] += ck * fi(k, xx);							//write to spline
				spl->f1[i] += ck * dfi(k, xx);
				spl->f2[i] += ck * d2fi(k, xx);
				spl->f3[i] += ck * d3fi(k, xx);
			}
		}
	}

	free_matrix(eqs);

}
