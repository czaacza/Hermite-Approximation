#include "points.h"
#include <stdlib.h>

static int
realloc_pts_failed(points_t *pts, int size)
{
  pts->x = realloc(pts->x, size * sizeof *pts->x);
  pts->y = realloc(pts->y, size * sizeof *pts->y);
  if (pts->x == NULL || pts->y == NULL) {
    return 1;
  }
  return 0;
  /*
    returns 0 if points reallocated succesfully
    returns 1 if realloc returned NULL
  */
}


void free_pts(points_t *pts) {
  free(pts->x);
  free(pts->y);
  free(pts);
  /*
    freeing the memory allocated for points
  */
}

int read_pts_failed(FILE *inf, points_t *pts)
{
  int size;
  double x, y;

  if (pts->n == 0)  //if there are no points, allocate memory 100 elements of (x, y) and set the size to 100
  {
    pts->x = malloc(100 * sizeof *pts->x);
    if (pts->x == NULL)
      return 1;
    pts->y = malloc(100 * sizeof *pts->y);
    if (pts->y == NULL)
    {
      free(pts->x);
      return 1;
    }
    size = 100;
  }
  else
    size = pts->n;

  while (fscanf(inf, "%lf %lf", &x, &y) == 2)
  {
    pts->x[pts->n] = x; 
    pts->y[pts->n] = y;                       //get x and y, then put them in the last element of the vector
    pts->n++;                                 //increment the number of elements           
    if (!feof(inf) && pts->n == size)         //if number of elements reached size
    {
      if (realloc_pts_failed(pts, 2 * size))  //allocate points 2 times more memory
      {
        free_pts(pts);
        return 1;
      }
      else
        size *= 2;
    }
  }

  if (pts->n != size)                         //if number of elements is not equal to size
    if (realloc_pts_failed(pts, pts->n))      //allocate points memory equal to size and free the rest
    {
      free(pts->x);
      free(pts->y);
      free(pts);
      return 1;
    } 

  return 0;
}
