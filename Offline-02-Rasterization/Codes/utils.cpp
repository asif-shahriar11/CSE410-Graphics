#include<iostream>
#include<cmath>
#include "point.h"
#include "matrix.h"
#include "triangle.h"
using namespace std;

static unsigned long int g_seed = 1;

inline int random_util()
{
    g_seed = (214013 * g_seed + 2531011);
    return (g_seed >> 16) & 0x7FFF;
}

static Matrix get_identity_matrix() {
    Matrix tempMatrix;
    for(int i = 0; i < 4; i++) 
        tempMatrix.m[i][i] = 1.0;
    return tempMatrix;
}

static Matrix get_translation_matrix(double tx, double ty, double tz) {
    Matrix tempMatrix;
    for(int i = 0; i < 4; i++) {
        tempMatrix.m[i][i] = 1.0;
        if(i == 0) tempMatrix.m[i][3] = tx;
        else if(i == 1) tempMatrix.m[i][3] = ty;
        else if(i == 2) tempMatrix.m[i][3] = tz;
    }
    return tempMatrix;
}


static Matrix get_scaling_matrix(double sx, double sy, double sz) {
    Matrix tempMatrix;
    tempMatrix.m[0][0] = sx;
    tempMatrix.m[1][1] = sy;
    tempMatrix.m[2][2] = sz;
    tempMatrix.m[3][3] = 1.0;
    return tempMatrix;
}

static Point get_Rodrigues_values(Point x, Point a, double theta) {
    Point x_prime;
    double dot_product = x.getX() * a.getX() + x.getY() * a.getY() + x.getZ() * a.getZ();
    double cross_product_x = a.getY() * x.getZ() - a.getZ() * x.getY();
    double cross_product_y = a.getZ() * x.getX() - a.getX() * x.getZ();
    double cross_product_z = a.getX() * x.getY() - a.getY() * x.getX();

    theta = theta * M_PI / 180.0;

    x_prime.setX(x.getX() * cos(theta) + cross_product_x * sin(theta) + a.getX() * dot_product * (1 - cos(theta)));
    x_prime.setY(x.getY() * cos(theta) + cross_product_y * sin(theta) + a.getY() * dot_product * (1 - cos(theta)));
    x_prime.setZ(x.getZ() * cos(theta) + cross_product_z * sin(theta) + a.getZ() * dot_product * (1 - cos(theta)));
    return x_prime;
}

static Point get_cross_product(Point a, Point x) {
    Point tempPoint;
    tempPoint.setX(a.getY() * x.getZ() - a.getZ() * x.getY());
    tempPoint.setY(a.getZ() * x.getX() - a.getX() * x.getZ());
    tempPoint.setZ(a.getX() * x.getY() - a.getY() * x.getX());
    return tempPoint;
}

static Point point_subtraction(Point a, Point b) {
    Point tempPoint;
    tempPoint.setX(a.getX() - b.getX());
    tempPoint.setY(a.getY() - b.getY());
    tempPoint.setZ(a.getZ() - b.getZ());
    return tempPoint;
}

static Matrix get_rotation_matrix(double angle, double ax, double ay, double az) {
    Matrix tempMatrix;

    Point a(ax, ay, az);
    a.normalize();

    Point i(1.0, 0.0, 0.0);
    Point j(0.0, 1.0, 0.0);
    Point k(0.0, 0.0, 1.0);

    Point c1 = get_Rodrigues_values(i, a, angle);
    Point c2 = get_Rodrigues_values(j, a, angle);
    Point c3 = get_Rodrigues_values(k, a, angle);

    tempMatrix.m[0][0] = c1.getX();
    tempMatrix.m[0][1] = c2.getX();
    tempMatrix.m[0][2] = c3.getX();

    tempMatrix.m[1][0] = c1.getY();
    tempMatrix.m[1][1] = c2.getY();
    tempMatrix.m[1][2] = c3.getY();

    tempMatrix.m[2][0] = c1.getZ();
    tempMatrix.m[2][1] = c2.getZ();
    tempMatrix.m[2][2] = c3.getZ();

    tempMatrix.m[3][3] = 1.0;

    return tempMatrix;
}

// a function to multiply two matrices
static Matrix matrix_multiply(Matrix A, Matrix B) {
    Matrix tempMatrix;
    for(int i = 0; i < 4; i++) 
        for(int j = 0; j < 4; j++) 
            for(int k = 0; k < 4; k++) 
                tempMatrix.m[i][j] += A.m[i][k] * B.m[k][j];
    return tempMatrix;
}

// a function to multiply a matrix with a point
static Point matrix_point_multiply(Matrix A, Point B) {
    Point tempPoint;
    tempPoint.setX(A.m[0][0] * B.getX() + A.m[0][1] * B.getY() + A.m[0][2] * B.getZ() + A.m[0][3] * B.getW());
    tempPoint.setY(A.m[1][0] * B.getX() + A.m[1][1] * B.getY() + A.m[1][2] * B.getZ() + A.m[1][3] * B.getW());
    tempPoint.setZ(A.m[2][0] * B.getX() + A.m[2][1] * B.getY() + A.m[2][2] * B.getZ() + A.m[2][3] * B.getW());
    tempPoint.setW(A.m[3][0] * B.getX() + A.m[3][1] * B.getY() + A.m[3][2] * B.getZ() + A.m[3][3] * B.getW());
    return tempPoint;
}

// a function to find the normal of a plane, given three points
static Point get_normal(Point a, Point b, Point c) {
    Point tempPoint;
    Point V1 = point_subtraction(b, a);
    Point V2 = point_subtraction(c, a);
    tempPoint = get_cross_product(V1, V2);
    tempPoint.normalize();
    return tempPoint;
}