#include<iostream>
#include "point.h"
using namespace std;

#ifndef TRIANGLE_H
#define TRIANGLE_H

class Color {
public:
    int R, G, B;
    Color() {}
    Color(int R, int G, int B) {
        this->R = R;
        this->G = G;
        this->B = B;
    }
    int getR() { return R; }
    int getG() { return G; }
    int getB() { return B; }
};



class Triangle {
public:

    Point points[3];
    int R, G, B;
    Color color;
    Triangle() {}
    Triangle(Point p1, Point p2, Point p3) {
        points[0] = p1;
        points[1] = p2;
        points[2] = p3;
    }

    void setColor(int R, int G, int B) {
        this->R = R;
        this->G = G;
        this->B = B;
    }

    void setColor(Color color) { this->color = color; }

    Color getColor() { return color; }

    int getR() { return color.getR(); }

    int getG() { return color.getG(); }

    int getB() { return color.getB(); }
};

#endif