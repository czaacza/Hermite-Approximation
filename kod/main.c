#include "points.h"
#include "splines.h"
#include "makespl.h"

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

char *usage =
    "Usage: %s -s spline-file [-p points-file] [ -g gnuplot-file [-f from_x -t to_x -n n_points ] ]\n"
    "            if points-file is given then\n"
    "               reads discrete 2D points from points-file\n"
    "               writes spline approximation to spline-file\n"
    "               - number of points should be >= 4\n"
    "            else (points-file not given)\n"
    "               reads spline from spline-file\n"
    "            endfi\n"
    "            if gnuplot-file is given then\n"
    "               makes table of n_points within <from_x,to_x> range\n"
    "               - from_x defaults to x-coordinate of the first point in points-file,\n"
    "               - to_x defaults to x-coordinate of the last point\n"
    "               - n_points defaults to 100\n"
    "               - n_points must be > 1\n"
    "            endif\n";

int main(int argc, char **argv)
{
  int opt;
  char *inp = NULL; //input points file
  char *out = NULL; //output spline file
  char *gpt = NULL; //plot file (for gnuplot)
  double fromX = 0; //minimal x value
  double toX = 0;   //maximal x value
  int n = 100;      //numer of points
  char *progname = argv[0]; //program name

  points_t *pts = malloc(sizeof *pts);  //allocating memory for points pointer
  spline_t *spl = malloc(sizeof *spl);  //allocating memory for spline pointer

  pts->n = 0;
  spl->n = 0;

  /* process options, save user choices */
  while ((opt = getopt(argc, argv, "p:s:g:f:t:n:")) != -1)
  {
    switch (opt)
    {
    case 'p':
      inp = optarg; //get input points file name
      break;
    case 's':
      out = optarg; //get output spline name
      break;
    case 'g':
      gpt = optarg; //get plot file name
      break;
    case 'f':
      fromX = atof(optarg); //get minimal x value
      break;
    case 't':
      toX = atof(optarg); //get maximal x value
      break;
    case 'n':
      n = atoi(optarg); //get number of points
      break;
    default: /* '?' */
      fprintf(stderr, usage, progname); //print help
      exit(EXIT_FAILURE);
    }
  }
  if (optind < argc)  //if bad parameters were given
  {
    fprintf(stderr, "\nBad parameters!\n");
    for (; optind < argc; optind++)
      fprintf(stderr, "\t\"%s\"\n", argv[optind]);
    fprintf(stderr, "\n");
    fprintf(stderr, usage, progname); //print help
    exit(EXIT_FAILURE);
  }

  /* if points-file was given, then read points, generate spline, save it to file */
  if (inp != NULL)
  {
    FILE *ouf = NULL; /* we shall open it later, when we shall get points */

    FILE *inf = fopen(inp, "r");
    if (inf == NULL)
    {
      fprintf(stderr, "%s: can not read points file: %s\n\n", argv[0], inp);
      exit(EXIT_FAILURE);
    }

    if (read_pts_failed(inf, pts))  //if points were not read successfully exit the program
    {
      fprintf(stderr, "%s: bad contents of points file: %s\n\n", argv[0],
              inp);
      exit(EXIT_FAILURE);
    }
    else
      fclose(inf);

    ouf = fopen(out, "w");
    if (ouf == NULL)
    {
      fprintf(stderr, "%s: can not write spline file: %s\n\n", argv[0], out);
      exit(EXIT_FAILURE);
    }

    make_spl(pts, spl);     //generate spline

    if (spl->n > 0)
      write_spl(spl, ouf);  //write spline to the file

    fclose(ouf);
  }

  else if (out != NULL)
  { /* if point-file was NOT given, try to read splines from a file */
    FILE *splf = fopen(out, "r");
    if (splf == NULL)
    {
      fprintf(stderr, "%s: can not read spline file: %s\n\n", argv[0], inp);
      exit(EXIT_FAILURE);
    }
    if (read_spl(splf, spl))
    {
      fprintf(stderr, "%s: bad contents of spline file: %s\n\n", argv[0],
              inp);
      exit(EXIT_FAILURE);
    }
    fclose(splf);
  }
  else
  { /* ponts were not given nor spline was given -> it is an error */
    fprintf(stderr, usage, argv[0]);
    exit(EXIT_FAILURE);
  }

  if (spl->n < 1)
  { /* check if there is a valid spline */
    fprintf(stderr, "%s: bad spline: n=%d\n\n", argv[0], spl->n);
    exit(EXIT_FAILURE);
  }

  /* check if plot was requested and generate it if yes */
  if (gpt != NULL && n > 1)
  {
    FILE *gpf = fopen(gpt, "w");
    int i;
    double dx;
    if (fromX == 0 && toX == 0)
    { /* calculate plot range if it was not specified */
      if (pts->n > 1)
      {
        fromX = pts->x[0];
        toX = pts->x[pts->n - 1];
      }
      else if (spl->n > 1)
      {
        fromX = spl->x[0];
        toX = spl->x[spl->n - 1];
      }
      else
      {
        fromX = 0;
        toX = 1;
      }
    }
    dx = (toX - fromX) / (n - 1);

    if (gpf == NULL)
    {
      fprintf(stderr, "%s: can not write gnuplot file: %s\n\n", argv[0],
              gpt);
      exit(EXIT_FAILURE);
    }

    for (i = 0; i < n; i++)
      fprintf(gpf, "%g %g\n", fromX + i * dx,
              value_spl(spl, fromX + i * dx));

    free_spline(spl);
    free_pts(pts);
    fclose(gpf);
  }
  return 0;
}
