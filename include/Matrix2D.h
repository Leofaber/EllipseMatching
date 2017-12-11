#ifndef MATRIX2D_H
#define MATRIX2D_H

#define DIM 2

#include <iostream>
using std::cout;
using std::endl;

#include <stdio.h>
#include <string>
#include <cmath>
#include <utility>

using std::string;
using std::pair;
using std::make_pair;

class Matrix2D
{
    public:
        Matrix2D();
        Matrix2D(string name);
        Matrix2D(string name, double a1, double a2, double b1, double b2);

        string name;

        double matrix[DIM][DIM];

        void setRow(int row, double a, double b);
        void print();
        bool isSymm();
        pair<double,double> vectorMultiplication(pair<double,double>& v);

        /// Static Operations
        static Matrix2D sqrt(const Matrix2D& m)
        {
            Matrix2D rad("sqrt_of "+m.name);
            for(int i = 0; i < DIM; ++i)
                for(int j = 0; j < DIM; ++j)
                    rad.matrix[i][j] = std::sqrt(m.matrix[i][j]);
            return rad;
        }
        static Matrix2D transpose(const Matrix2D& m)
        {
            Matrix2D transposed(m.name+"'");
            for(int i = 0; i < DIM; ++i)
                for(int j = 0; j < DIM; ++j)
                    transposed.matrix[i][j] = m.matrix[j][i];
            return transposed;
        }
        static pair<Matrix2D, Matrix2D> getVD(Matrix2D& m)
        {
            return Matrix2D::eigenvalueDecompositionSymm(m);
        }

        /// Operator Overloading
        Matrix2D operator+(const Matrix2D& m) {
            Matrix2D mat;
            mat.name = this->name+" + "+m.name;
            for(int i = 0; i < DIM; ++i)
                for(int j = 0; j < DIM; ++j)
                    mat.matrix[i][j] = this->matrix[i][j] + m.matrix[i][j];
            return mat;
        }
        Matrix2D operator-(const Matrix2D& m) {
            Matrix2D mat;
            mat.name = this->name+" - "+m.name;
            for(int i = 0; i < DIM; ++i)
                for(int j = 0; j < DIM; ++j)
                    mat.matrix[i][j] = this->matrix[i][j] - m.matrix[i][j];
            return mat;
        }
        Matrix2D operator*(const Matrix2D& m) {

            Matrix2D mat;

            mat.name = this->name+" * "+m.name;
            mat.matrix[0][0] = this->matrix[0][0] * m.matrix[0][0] + this->matrix[0][1] * m.matrix[1][0];
            mat.matrix[0][1] = this->matrix[0][0] * m.matrix[0][1] + this->matrix[0][1] * m.matrix[1][1];
            mat.matrix[1][0] = this->matrix[1][0] * m.matrix[0][0] + this->matrix[1][1] * m.matrix[1][0];
            mat.matrix[1][1] = this->matrix[1][0] * m.matrix[0][1] + this->matrix[1][1] * m.matrix[1][1];

            return mat;
        }
        Matrix2D operator/(const Matrix2D& m) {
            Matrix2D mat;
            mat.name = this->name+" / "+m.name;
            for(int i = 0; i < DIM; ++i)
                for(int j = 0; j < DIM; ++j)
                    mat.matrix[i][j] = this->matrix[i][j] / m.matrix[i][j];
            return mat;
        }
        bool operator==(const Matrix2D& m) {
            bool equals = true;
            for(int i = 0; i < DIM; ++i)
                for(int j = 0; j < DIM; ++j)
                    if(this->matrix[i][j]!=m.matrix[i][j])
                        equals = false;
            return equals;
        }


    private:
        static pair<Matrix2D, Matrix2D> eigenvalueDecompositionSymm(Matrix2D);

};

#endif // MATRIX2D_H
