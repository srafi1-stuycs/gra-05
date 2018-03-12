/*====================== display.c ========================
  Contains functions for basic manipulation of a screen 
  represented as a 2 dimensional array of colors.

  A color is an ordered triple of ints, with each value standing
  for red, green and blue respectively
  ==================================================*/

#include <stdio.h>
#include <stdlib.h>

#include "ml6.h"
#include "display.h"

void plot( screen s, color c, int x, int y) {
    int newy = YRES - 1 - y;
    if ( x >= 0 && x < XRES && newy >=0 && newy < YRES )
        s[x][newy] = c;
}

void clear_screen( screen s ) {

    int x, y;
    color c;

    c.red = 0;
    c.green = 0;
    c.blue = 0;

    for ( y=0; y < YRES; y++ )
        for ( x=0; x < XRES; x++)      
            s[x][y] = c;
}

void save_ppm( screen s, char *file) {

    int x, y;
    FILE *f;

    f = fopen(file, "w");
    fprintf(f, "P3\n%d %d\n%d\n", XRES, YRES, MAX_COLOR);
    for ( y=0; y < YRES; y++ ) {
        for ( x=0; x < XRES; x++) 

            fprintf(f, "%d %d %d ", s[x][y].red, s[x][y].green, s[x][y].blue);
        fprintf(f, "\n");
    }
    fclose(f);
}

void save_extension( screen s, char *file) {

    int x, y;
    FILE *f;
    char line[256];

    sprintf(line, "convert - %s", file);

    f = popen(line, "w");
    fprintf(f, "P3\n%d %d\n%d\n", XRES, YRES, MAX_COLOR);
    for ( y=0; y < YRES; y++ ) {
        for ( x=0; x < XRES; x++) 

            fprintf(f, "%d %d %d ", s[x][y].red, s[x][y].green, s[x][y].blue);
        fprintf(f, "\n");
    }
    pclose(f);
}


void display( screen s) {

    int x, y;
    FILE *f;

    f = popen("display", "w");

    fprintf(f, "P3\n%d %d\n%d\n", XRES, YRES, MAX_COLOR);
    for ( y=0; y < YRES; y++ ) {
        for ( x=0; x < XRES; x++) 

            fprintf(f, "%d %d %d ", s[x][y].red, s[x][y].green, s[x][y].blue);
        fprintf(f, "\n");
    }
    pclose(f);
}

