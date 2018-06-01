#ifndef ELLROUTINES_H
#define ELLROUTINES_H

#include <utility>
#include <vector>
#include <string>
#include <cmath>
#include <complex>
#include <algorithm>
#include <iomanip>
#include <iostream>
#include "Matrix2D.h"

using std::pair;
using std::string;
using std::make_pair;
using std::vector;
using std::cout;
using std::endl;
using std::complex;



class EllRoutines
{
    public:
        static int elltest(pair<double,double>&C1, Matrix2D& D1, Matrix2D& R1, Matrix2D& M1, pair<double,double>&C2, Matrix2D& D2, Matrix2D& R2, Matrix2D& M2, double tol);
        static pair<string,short int> ellmsg(int res);
        static vector<double> quartic(vector<double>& k, double tol);
        static vector<complex<double> > cubic(vector<double>& k, double tol);

        static vector<double> approximateVectorElementToZero(vector<double>& v, double tol);
        static vector<double> removeNanAndInfFromVector(vector<double>& v);

        static void printVector(vector<double>& k, string name);
        static void printVector(vector<int>& k, string name);
        static void printVector(vector<complex<double> >& k, string name);


    private:
        EllRoutines();

};

#endif // ELLROUTINES_H
