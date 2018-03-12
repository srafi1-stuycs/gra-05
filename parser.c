#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "ml6.h"
#include "display.h"
#include "draw.h"
#include "matrix.h"
#include "parser.h"


void parse_file ( char * filename, 
        struct matrix * transform, 
        struct matrix * edges,
        screen s) {

    FILE *f;
    char line[256];
    clear_screen(s);

    color c;
    c.red = 255;
    c.green = 255;
    c.blue = 255;

    if ( strcmp(filename, "stdin") == 0 ) {
        f = stdin;
        printf("> ");
    } else
        f = fopen(filename, "r");

    ident(transform);

    while ( fgets(line, 255, f) != NULL ) {
        line[strlen(line) - 1] = '\0';
        //printf(":%s:\n", line);
        char params[256];
        if (strcmp(line, "line") == 0) {
            fgets(params, 255, f);
            params[strlen(params) - 1] = '\0';
            //printf(":%s:\n", params);
            int x1, y1, z1, x2, y2, z2;
            sscanf(params, "%d %d %d %d %d %d", &x1, &y1, &z1, &x2, &y2, &z2); 
            printf("line from %d %d %d to %d %d %d\n", x1, y1, z1, x2, y2, z2);
            add_edge(edges, x1, y1, z1, x2, y2, z2);
        } else if (strcmp(line, "ident") == 0) {
            printf("reverting transformation matrix to identity matrix\n");
            free_matrix(transform);
            transform = new_matrix(4, 4);
            ident(transform);
        } else if (strcmp(line, "scale") == 0) {
            fgets(params, 255, f);
            params[strlen(params) - 1] = '\0';
            //printf(":%s:\n", params);
            double sx, sy, sz;
            sscanf(params, "%lf %lf %lf", &sx, &sy, &sz); 
            printf("scale by %lf %lf %lf\n", sx, sy, sz);
            struct matrix * scale_matrix = make_scale(sx, sy, sz);
            matrix_mult(scale_matrix, transform);
            free_matrix(scale_matrix);
        } else if (strcmp(line, "move") == 0) {
            fgets(params, 255, f);
            params[strlen(params) - 1] = '\0';
            //printf(":%s:\n", params);
            int tx, ty, tz;
            sscanf(params, "%d %d %d", &tx, &ty, &tz); 
            printf("translate by %d %d %d\n", tx, ty, tz);
            struct matrix * transation_matrix = make_translate(tx, ty, tz);
            matrix_mult(transation_matrix, transform);
            free_matrix(transation_matrix);
        } else if (strcmp(line, "color") == 0) {
            fgets(params, 255, f);
            params[strlen(params) - 1] = '\0';
            //printf(":%s:\n", params);
            int r, g, b;
            sscanf(params, "%d %d %d", &r, &g, &b); 
            printf("changing color to %d %d %d\n", r, g, b);
            c.red = r;
            c.green = g;
            c.blue = b;
        } else if (strcmp(line, "rotate") == 0) {
            fgets(params, 255, f);
            params[strlen(params) - 1] = '\0';
            //printf(":%s:\n", params);
            char axis;
            int theta;
            sscanf(params, "%c %d", &axis, &theta); 
            printf("rotating %c axis by %d degrees\n", axis, theta);
            struct matrix * rotation_matrix;
            if (axis == 'x') {
                rotation_matrix = make_rotX(theta);
            } else if (axis == 'y') {
                rotation_matrix = make_rotY(theta);
            } else if (axis == 'z') {
                rotation_matrix = make_rotZ(theta);
            } else {
                printf("invalid rotation axis\n");
                rotation_matrix = new_matrix(4, 4);
                ident(rotation_matrix);
            }
            matrix_mult(rotation_matrix, transform);
            free_matrix(rotation_matrix);
        } else if (strcmp(line, "apply") == 0) {
            printf("applying transformation matrix to edge matrix\n");
            matrix_mult(transform, edges);
        } else if (strcmp(line, "display") == 0) {
            clear_screen(s);
            draw_lines(edges, s, c);
            display(s);
        } else if (strcmp(line, "save") == 0) {
            fgets(params, 255, f);
            params[strlen(params) - 1] = '\0';
            //printf(":%s:\n", params);
            printf("save screen as %s", params);
            save_ppm(s, params);
        } else if (strcmp(line, "print") == 0) {
            printf("edge matrix:\n");
            print_matrix(edges);
            printf("transformation matrix:\n");
            print_matrix(transform);
        } else if (strcmp(line, "circle") == 0) {
            fgets(params, 255, f);
            params[strlen(params) - 1] = '\0';
            double cx, cy, cz, r;
            sscanf(params, "%lf %lf %lf %lf", &cx, &cy, &cz, &r);
            printf("drawing a cricle centered at (%lf, %lf, %lf) with radius %lf\n", cx, cy, cz, r);
            add_circle(edges, cx, cy, cz, r, 0.01);
        } else if (strcmp(line, "bezier") == 0) {
            fgets(params, 255, f);
            params[strlen(params) - 1] = '\0';
            double x0, y0, x1, y1, x2, y2, x3, y3;
            sscanf(params, "%lf %lf %lf %lf %lf %lf %lf %lf", &x0, &y0, &x1, &y1, &x2, &y2, &x3, &y3);
            printf("drawing bezier curve\n");
            add_curve(edges, x0, y0, x1, y1, x2, y2, x3, y3, 0.01, BEZIER);
        } else if (strcmp(line, "hermite") == 0) {
            fgets(params, 255, f);
            params[strlen(params) - 1] = '\0';
            double x0, y0, x1, y1, rx0, ry0, rx1, ry1;
            sscanf(params, "%lf %lf %lf %lf %lf %lf %lf %lf", &x0, &y0, &x1, &y1, &rx0, &ry0, &rx1, &ry1);
            printf("drawing hermite curve\n");
            add_curve(edges, x0, y0, x1, y1, rx0, ry0, rx1, ry1, 0.01, HERMITE);
        } else if (strcmp(line, "quit") == 0 || strcmp(line, "exit") == 0 ) {
            break;
        }
        if (f == stdin) {
            printf("> ");
        }
    }
}

