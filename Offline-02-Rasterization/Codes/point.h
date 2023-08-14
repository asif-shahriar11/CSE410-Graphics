#include<iostream>
#include<cmath>
#include<fstream>
#include <iomanip> // required for std::setprecision
using namespace std;

#ifndef POINT_H
#define POINT_H

class Point {
    double x;
    double y;
    double z;
    double w;
public:
    Point() {
        x = 0.0;
        y = 0.0;
        z = 0.0;
        w = 1.0;
    }

    Point(double x, double y, double z, double w = 1.0) {
        this->x = x;
        this->y = y;
        this->z = z;
        this->w = w;
    }

    void setX(double x) { this->x = x; }

    void setY(double y) { this->y = y; }

    void setZ(double z) { this->z = z; }

    void setW(double w) { this->w = w; }

    double getX() { return x; }

    double getY() { return y; }

    double getZ() { return z; }

    double getW() { return w; }

    void print() {
        cout << x << " " << y << " " << z << endl;
    }

    // print in a file
    void print(ofstream &file) {
        //file << x << " " << y << " " << z << endl;
        // print x, y, z in the file with a precision of 6 digits after the end of decimal
        file << std::fixed << std::setprecision(7) << x << " " << y << " " << z << endl;
    }

    void scale() {
        x = x / w;
        y = y / w;
        z = z / w;
        w = 1.0;
    }

    void normalize() {
        double mag = sqrt(x * x + y * y + z * z);
        x = x / mag;
        y = y / mag;
        z = z / mag;
    }


};

#endif