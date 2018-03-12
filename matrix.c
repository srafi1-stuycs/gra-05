#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "matrix.h"

struct matrix * make_hermite() {
    struct matrix * ret = new_matrix(4, 4);
    ident(ret);
    ret->m[0][0] = 2;
    ret->m[0][1] = -2;
    ret->m[0][2] = 1;
    ret->m[0][3] = 1;
    ret->m[1][0] = -3;
    ret->m[1][1] = 3;
    ret->m[1][2] = -2;
    ret->m[1][3] = -1;
    ret->m[2][0] = 0;
    ret->m[2][1] = 0;
    ret->m[2][2] = 1;
    ret->m[2][3] = 0;
    ret->m[3][0] = 1;
    ret->m[3][1] = 0;
    ret->m[3][2] = 0;
    ret->m[3][3] = 0;
    return ret;
}

struct matrix * make_bezier() {
    struct matrix * ret = new_matrix(4, 4);
    ident(ret);
    ret->m[0][0] = -1;
    ret->m[0][1] = 3;
    ret->m[0][2] = -3;
    ret->m[0][3] = 1;
    ret->m[1][0] = 3;
    ret->m[1][1] = -6;
    ret->m[1][2] = 3;
    ret->m[1][3] = 0;
    ret->m[2][0] = -3;
    ret->m[2][1] = 3;
    ret->m[2][2] = 0;
    ret->m[2][3] = 0;
    ret->m[3][0] = 1;
    ret->m[3][1] = 0;
    ret->m[3][2] = 0;
    ret->m[3][3] = 0;
    return ret;
}

struct matrix * generate_curve_coefs(double p1, double p2, double p3, double p4, int type) {
    struct matrix * points = new_matrix(4, 1);
    ident(points);
    points->m[0][0] = p1;
    points->m[1][0] = p2;
    points->m[2][0] = p3;
    points->m[3][0] = p4;

    if (type == HERMITE) {
        struct matrix * hermite_matrix = make_hermite();
        matrix_mult(hermite_matrix, points);
        free_matrix(hermite_matrix);
    } else {
        struct matrix * bezier_matrix = make_bezier();
        matrix_mult(bezier_matrix, points);
        free_matrix(bezier_matrix);
    }

    return points;
}

struct matrix * make_translate(double x, double y, double z) {
    struct matrix * transformer = new_matrix(4, 4);
    ident(transformer);
    transformer->m[0][3] = x;
    transformer->m[1][3] = y;
    transformer->m[2][3] = z;
    return transformer;
}

struct matrix * make_scale(double x, double y, double z) {
    struct matrix * transformer = new_matrix(4, 4);
    ident(transformer);
    transformer->m[0][0] = x;
    transformer->m[1][1] = y;
    transformer->m[2][2] = z;
    return transformer;
}

double degrees_to_radians(double theta) {
    return theta * M_PI / 180;
}

struct matrix * make_rotX(double theta) {
    struct matrix * transformer = new_matrix(4, 4);
    ident(transformer);
    theta = degrees_to_radians(theta);
    transformer->m[1][1] = cos(theta);
    transformer->m[1][2] = -sin(theta);
    transformer->m[2][1] = sin(theta);
    transformer->m[2][2] = cos(theta);
    return transformer;
}

struct matrix * make_rotY(double theta) {
    struct matrix * transformer = new_matrix(4, 4);
    ident(transformer);
    theta = degrees_to_radians(theta);
    transformer->m[0][0] = cos(theta);
    transformer->m[0][2] = -sin(theta);
    transformer->m[2][0] = sin(theta);
    transformer->m[2][2] = cos(theta);
    return transformer;
}

struct matrix * make_rotZ(double theta) {
    struct matrix * transformer = new_matrix(4, 4);
    ident(transformer);
    theta = degrees_to_radians(theta);
    transformer->m[0][0] = cos(theta);
    transformer->m[0][1] = -sin(theta);
    transformer->m[1][0] = sin(theta);
    transformer->m[1][1] = cos(theta);
    return transformer;
}

void print_matrix(struct matrix *m) {

    int r, c;
    for (r=0; r < m->rows; r++) {
        for (c=0; c < m->lastcol; c++) 
            printf("%0.2f ", m->m[r][c]);
        printf("\n");
    }
}//end print_matrix

void ident(struct matrix *m) {
    int r, c;

    for (r=0; r < m->rows; r++) 
        for (c=0; c < m->cols; c++) 
            if ( r == c )
                m->m[r][c] = 1;
            else
                m->m[r][c] = 0;
    m->lastcol = m->cols;
}//end ident


void matrix_mult(struct matrix *a, struct matrix *b) {
    int r, c;
    struct matrix *tmp;
    tmp = new_matrix(4, 1);

    for (c=0; c < b->lastcol; c++) {

        //copy current col (point) to tmp
        for (r=0; r < b->rows; r++)      
            tmp->m[r][0] = b->m[r][c];

        for (r=0; r < b->rows; r++) 
            b->m[r][c] = a->m[r][0] * tmp->m[0][0] +
                a->m[r][1] * tmp->m[1][0] +
                a->m[r][2] * tmp->m[2][0] +
                a->m[r][3] * tmp->m[3][0];
    }
    free_matrix(tmp);
}//end matrix_mult

struct matrix *new_matrix(int rows, int cols) {
    double **tmp;
    int i;
    struct matrix *m;

    tmp = (double **)malloc(rows * sizeof(double *));
    for (i=0;i<rows;i++) {
        tmp[i]=(double *)malloc(cols * sizeof(double));
    }

    m=(struct matrix *)malloc(sizeof(struct matrix));
    m->m=tmp;
    m->rows = rows;
    m->cols = cols;
    m->lastcol = 0;

    return m;
}

void free_matrix(struct matrix *m) {

    int i;
    for (i=0;i<m->rows;i++) {
        free(m->m[i]);
    }
    free(m->m);
    free(m);
}

void grow_matrix(struct matrix *m, int newcols) {

    int i;
    for (i=0;i<m->rows;i++) {
        m->m[i] = realloc(m->m[i],newcols*sizeof(double));
    }
    m->cols = newcols;
}

void copy_matrix(struct matrix *a, struct matrix *b) {

    int r, c;

    for (r=0; r < a->rows; r++) 
        for (c=0; c < a->cols; c++)  
            b->m[r][c] = a->m[r][c];  
}

