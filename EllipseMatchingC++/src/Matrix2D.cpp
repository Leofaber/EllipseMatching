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
    printf("%s:\n",name.c_str());
    for (int i = 0; i < DIM; ++i){
        for (int j = 0; j < DIM; ++j)
        {
            printf("\t%f ",matrix[i][j]);

        }
        printf("\n");
    }
}
void Matrix2D::print(string name)
{
    printf("%s:\n",name.c_str());
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

// M * EigenVector = EigenValues * EigenVectors
bool computeEigenvalueDecompositionEquation(Matrix2D m, pair<double,double> ev1,pair<double,double> ev2, Matrix2D D)
{
  pair<double,double> mv1 = m.vectorMultiplication(ev1);
  pair<double,double> lambav1 = make_pair(ev1.first*D.matrix[0][0],ev1.second*D.matrix[0][0]);

  pair<double,double> mv2 = m.vectorMultiplication(ev2);
  pair<double,double> lambav2 = make_pair(ev2.first*D.matrix[1][1],ev2.second*D.matrix[1][1]);

  #if DEBUG == 1
  cout << "\nM*v1 = lamba1*v1" << endl;
  cout << "\t" << mv1.first << "   " << lambav1.first << endl;
  cout << "\t" << mv1.second << "   " << lambav1.second << endl;

  cout << "M*v2 = lamba1*v2" << endl;
  cout << "\t" << mv2.first << "   " << lambav2.first << endl;
  cout << "\t" << mv2.second << "   " << lambav2.second << endl;
  #endif

  double tol = 1/pow(10,6);

  bool ok = ( fabs(mv1.first - lambav1.first)<tol && fabs(mv1.second - lambav1.second)<tol && fabs(mv2.first - lambav2.first)<tol && fabs(mv2.second - lambav2.second)<tol);

  return ok;
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

    Matrix2D D("Autovalori",l2,0,0,l1);



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
    else if(b==0 && c==0)
    {

      pair<double,double> p = make_pair(1.0,0.0);
      pair<double,double> v1 = m.vectorMultiplication(p);

      if(v1.first == l2 && v1.second == 0){
        ev1.first  = 1;         ev2.first  = 0;
        ev1.second = 0;         ev2.second = 1;
      }
      else{
        ev1.first  = 0;         ev2.first  = 1;
        ev1.second = 1;         ev2.second = 0;
      }
    }
    else{
      cout << "Error on EIG!" << endl;
    }

    double v1mag = std::sqrt( pow(ev1.first,2) + pow(ev1.second,2) );
    double v2mag = std::sqrt( pow(ev2.first,2) + pow(ev2.second,2) );

    ev1.first/= v1mag;  ev1.second/= v1mag;
    ev2.first/= v2mag;  ev2.second/= v2mag;


    /*
      --> VERIFICARE CHE IL REVERSE E IL *-1 VADA SEMPRE BENE...
      -->   Non va sempre bene vedi:
        3FGLJ1911.4-1908 17.8167 -12.871 0.0497 0.0451 -36.04
        2AGLJ1914-1928 17.78 -13.6102 0.7 0.7 0
    */
     if( computeEigenvalueDecompositionEquation(m, ev1, ev2, D) )
     {
         Matrix2D R("Autovettori",ev1.first,ev2.first,ev1.second,ev2.second);
         return make_pair(R,D);
     }

     #if DEBUG == 1
     cout << "Trying to reverse order and sign of autovectors.." << endl;
     #endif
     // reverse order and (-1)
    double temp1 = ev1.first;
    double temp2 = ev1.second;

    ev1.first = (-1)*ev2.first;
    ev1.second = (-1)*ev2.second;
    ev2.first = (-1)*temp1;
    ev2.second = (-1)*temp2;


    if( computeEigenvalueDecompositionEquation(m, ev1, ev2, D) )
    {
        Matrix2D R("Autovettori",ev1.first,ev2.first,ev1.second,ev2.second);
        return make_pair(R,D);
    }
    else
    {
      cout << "Error on Eigenvalues/EigenVector decomposition.." << endl;
      exit(EXIT_FAILURE);
    }

}
