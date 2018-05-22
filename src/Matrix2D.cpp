#include "Matrix2D.h"

Matrix2D::Matrix2D()
{
    //ctor
    name = "";
    matrix[0][0] = 0;
    matrix[0][1] = 0;
    matrix[1][0] = 0;
    matrix[1][1] = 0;
}
Matrix2D::Matrix2D(string n)
{
    name = n;
    matrix[0][0] = 0;
    matrix[0][1] = 0;
    matrix[1][0] = 0;
    matrix[1][1] = 0;
}
Matrix2D::Matrix2D(string n, double a1, double a2, double b1, double b2)
{
    name = n;
    matrix[0][0] = a1;
    matrix[0][1] = a2;
    matrix[1][0] = b1;
    matrix[1][1] = b2;
}

void Matrix2D::print()
{
    printf("\n%s:\n",name.c_str());
    for (int i = 0; i < DIM; ++i){
        for (int j = 0; j < DIM; ++j)
        {
            printf("\t%f ",matrix[i][j]);

        }
        printf("\n");
    }
}

void Matrix2D::setRow(int row, double a, double b)
{
    matrix[row][0] = a;
    matrix[row][1] = b;
}
bool Matrix2D::isSymm()
{
    Matrix2D t = Matrix2D::transpose(*this);
    if(*this==t)
        return true;
    else
        return false;
}
pair<double,double> Matrix2D::vectorMultiplication(pair<double,double>& v)
{
    return make_pair(matrix[0][0]*v.first+matrix[0][1]*v.second, matrix[1][0]*v.first+matrix[1][1]*v.second);
}


// compute the eigenvalue decomposition of a symmetric 2x2
// matrix in the form A=[a b;b c], so that
//    A*v1 = v1 * lambda1
//    A*v2 = v2 * lambda2
pair<Matrix2D, Matrix2D> Matrix2D::eigenvalueDecompositionSymm(Matrix2D m)
{
    /// Implemented http://www.math.harvard.edu/archive/21b_fall_04/exhibits/2dmatrices/index.html

    /*
        a   b
        c   d
    */
    double a = m.matrix[0][0];
    double b = m.matrix[0][1];
    double c = m.matrix[1][0];
    double d = m.matrix[1][1];


    double det = a*d - b*c;
    double trace = a+d;

    double disc = std::sqrt( ( pow(trace,2) )/ 4-det);
    double l1 = trace/2 + disc;
    double l2 = trace/2 - disc;

    Matrix2D D("D",l1,0,0,l2);



    pair<double,double> ev1 = make_pair(0,0);
    pair<double,double> ev2 = make_pair(0,0);

    if(c!=0)
    {
        ev1.first  = l1-d;       ev2.first  = l2-d;
        ev1.second = c;          ev2.second = c;
    }
    else if(b!=0)
    {
        ev1.first  = b;           ev2.first  = b;
        ev1.second = l1-a;        ev2.second = l2-a;
    }
    else
    {
        ev1.first  = 1;         ev2.first = 0;
        ev1.second = 0;         ev2.second = 1;
    }

    double v1mag = std::sqrt( pow(ev1.first,2) + pow(ev1.second,2) );
    double v2mag = std::sqrt( pow(ev2.first,2) + pow(ev2.second,2) );

    ev1.first/= v1mag;  ev1.second/= v1mag;
    ev2.first/= v2mag;  ev2.second/= v2mag;

//    cout << "\n"<<ev1.first<<" "<<ev2.first<<"\n"<<ev1.second<<" "<<ev2.second<<endl;

    Matrix2D R("R",ev1.first,ev2.first,ev1.second,ev2.second);



    return make_pair(R,D);
}
