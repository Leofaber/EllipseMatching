#ifndef ELLIPSE_H
#define ELLIPSE_H

#include <string>
#include <iostream>
#include <math.h>
#include "Matrix2D.h"

#define PI 3.1415926535

using std::cout;
using std::endl;

class Ellipse
{
    public:
        Ellipse(): name(""), x(0), y(0), a(0), b(0), p(0), r(0) {}
        Ellipse(std::string _n, double _x, double _y, double _a, double _b, double _p)
                : name(_n), x(_x), y(_y), a(_a), b(_b), p(_p*PI/180),
                  D("D", 1/pow(a,2), 0, 0, 1/pow(b,2)),
                  R("R",cos(p), -1*sin(p), sin(p), cos(p))
                {

                    /// ellmatrix routine
		
                    Matrix2D Rt = Matrix2D::transpose(R);

                    r = std::max(a,b);

                    C = std::make_pair(x,y);

                    M = R*D*Matrix2D::transpose(R);


                }

        std::string name;
        double x; // center x
        double y; // center y
        double a; // a axis
        double b; // b axis
        double p; // phi
        double r; // major axis
        std::pair<double,double> C; // center
        Matrix2D D;
        Matrix2D R;
        Matrix2D M;

        void print(){
            cout << "\nEllipse: "<<name<<"\nx: "<<x<<"\ny: "<<y<<"\na: "<<a<<"\nb: "<<b<<"\np: "<<p<<"\nr: "<<r<< endl;
        }

        bool isEqual(Ellipse& other){
            if(this == &other)
                return true;
            if(x==other.x && y==other.y && a==other.a && b==other.b && p==other.p)
                return true;
            return false;
        }

 };

#endif // ELLIPSE_H
