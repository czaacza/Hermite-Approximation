# Hermite-Approximation
Least-squares function approximation calculator using Hermite polynomials
## Theoretical Description
The function approximation theory is all about how mathematical functions can best be approximated with simpler functions. What is meant by 'best' and 'simpler' depends on the application we are going to use.

In this case, we will use the Least-squared function approximation method, which consists in minimizing the squared differences between the original function and the approximation. Moreover, we are using Hermite polynomials as the base function used to approximate.

The main purpose of the program is to calculate and find the approximation function formula by defining its slopes.
## Program structure
The calculator is built in C languange. It's structure is divided into a few modules:
- points.c - Points structure, storing number of points, x and y. Functions responsible for reading points from file
- splines.c - Spline structure, storing items needed to approximate the slopes using Taylor's theorem.
- hermite.c - The approximation program, providing the function's slopes to splines structure.
- main.c - Main program module, responsible for reading arguments, creating the files and solve errors.
- gauss folder - Gauss elimination method packet, used to create and solve the matrix.

## Usage
To build the program, we can call the Makefile

`make hermite`

To call the program:

 `./hermite -s spline-file [-p points-file] [ -g gnuplot-file [-f from_x -t to_x -n n_points ] ]`
 
- spline-file - name of the file to which the spline functions will be written
- points-file - name of the file from which the base 2D points will be read, if not given, spline-file will be read
- gnuplot-file - name of the file to which n_points generated approximation points will be written
- from_x - x-coordinate of the first point of approximation in points-file
- to_x - x-coordinate of the last point of approximation in points-file
- n_points - number of points to calculate, by default 100

## Tests
Here are some examples of the functions and the program-generated approximation graphs:
