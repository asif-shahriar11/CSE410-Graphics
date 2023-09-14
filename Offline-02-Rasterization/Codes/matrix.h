#include<iostream>
#include<cmath>
#include<fstream>
#include "point.h"
using namespace std;

#ifndef MATRIX_H
#define MATRIX_H

const int order = 4;

class Matrix {
    
public:
    double m[order][order];
    Matrix() {
        for(int i = 0; i < order; i++) 
            for(int j = 0; j < order; j++) 
                m[i][j] = 0.0;
    }

    // print in a file
    void print(ofstream &file) {
        for(int i = 0; i < order - 1; i++) {
            file << std::fixed << std::setprecision(7) << m[i][0] << " " << m[i][1] << " " << m[i][2] << endl;
        }
    }

};

#endif