// ==========================================================================
// Ellipse Matching C++ https://github.com/Leofaber/EllipseMatching
//
// Copyright (C) 2018 Leonardo Baroncelli
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
// ==========================================================================
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

        static void printVector(vector<double>& k, string name);
        static void printVector(vector<int>& k, string name);
        static void printVector(vector<complex<double> >& k, string name);


    private:
        EllRoutines();

};

#endif // ELLROUTINES_H
