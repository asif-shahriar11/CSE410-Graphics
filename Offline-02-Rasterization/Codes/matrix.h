#include<iostream>
#include<cmath>
#include<fstream>
#include "point.h"
using namespace std;

#ifndef MATRIX_H
#define MATRIX_H

class Matrix {
    
public:
    double m[4][4];
    Matrix() {
        for(int i = 0; i < 4; i++) 
            for(int j = 0; j < 4; j++) 
                m[i][j] = 0.0;
    }

    // print in a file
    void print(ofstream &file) {
        for(int i = 0; i < 3; i++) {
            file << std::fixed << std::setprecision(7) << m[i][0] << " " << m[i][1] << " " << m[i][2] << endl;
        }
    }

};

#endif