#include "EllRoutines.h"



EllRoutines::EllRoutines()
{

}
int EllRoutines::elltest(pair<double,double>&C1, Matrix2D& D1, Matrix2D& R1, Matrix2D& M1, pair<double,double>&C2, Matrix2D& D2, Matrix2D& R2, Matrix2D& M2, double tol = 1/pow(10,12))
{
    #if DEBUG == 1
        cout << "elltest()" << endl;
    #endif


    // First transformation:
    // E1 became a unit circle centered on the origin
    pair<double,double> Ctemp = make_pair(C2.first-C1.first, C2.second-C1.second);



    #if DEBUG == 1
      D1.print();
      R1.print();
      //printf("Ctemp: %.32f\n %.32f\n",Ctemp.first, Ctemp.second );
    #endif


    Matrix2D Ctemp2 = Matrix2D::sqrt(D1) * Matrix2D::transpose(R1);

    pair<double,double> C3 = Ctemp2.vectorMultiplication(Ctemp);

    #if DEBUG == 1
        printf("C3:\n %.32f\n %.32f\n",C3.first, C3.second );
    #endif

    Matrix2D S1("S1", 1./std::sqrt(D1.matrix[0][0]), 0, 0, 1./std::sqrt(D1.matrix[1][1]));

    #if DEBUG == 1
        S1.print();
        Matrix2D::transpose(R1).print("R1");
        M2.print("M2");
    #endif


    Matrix2D M3 = S1*Matrix2D::transpose(R1)*M2*R1*S1;

    for(int i=0; i < 2; i++)
    {
      for(int j=0; j < 2; j++)
      {
        if(fabs(M3.matrix[i][j])<=tol){
          #if DEBUG == 1
            cout << "Element " <<i<<","<<j<<" of matrix M3 is approximated to 0" << endl;
          #endif
          M3.matrix[i][j] = 0.;
        }
      }
    }

    #if DEBUG == 1
        M3.print("M3");
    #endif
    /// Second transformation:
    /// Rotate to align E2 with the x/y axis

    Matrix2D R4 = Matrix2D::getVD(M3).first;        /// get V (autovettori)
    Matrix2D D4 = Matrix2D::getVD(M3).second;       /// get D (matrice diagonale di autovalori)

    #if DEBUG == 1
        R4.print("Matrice autovettori");
        D4.print("Diagonal Matrix");
    #endif


    pair<double,double> C4 = Matrix2D::transpose(R4).vectorMultiplication(C3);
    #if DEBUG == 1
        cout << "C4: \n"  << C4.first << "\n " << C4.second;
    #endif


    /// Quartic equation for extremal points
    double d1 = D4.matrix[0][0];
    double d2 = D4.matrix[1][1];
    double c1 = C4.first;
    double c2 = C4.second;

    #if DEBUG == 1
        cout << "\nd1,d2,c1,c2: \n" << d1 << "\n " << d2 << "\n " << c1 << "\n " << c2 <<endl;
    #endif


    vector<double> ds;

    /// Extremal points are along x/y axes at distance sqrt(1/d1) and sqrt(1/d2)
    if( c1==0 && c2 ==0 )
    {
        ds.push_back(sqrt(1./d1));
        ds.push_back(sqrt(1./d2));

    }
    /// Extremal points shall be compute usign a quartic
    else
    {
        double a = pow(d1,2)*pow(d2,2);
        double b = -2*pow(d1,2)*d2 - 2*d1*pow(d2,2);
        double c = pow(d1,2) + pow(d2,2) + 4*d1*d2 - d1*pow(d2,2)*pow(c1,2) - pow(d1,2)*d2*pow(c2,2);
        double d = -2*d2 -2*d1 + 2*d1*d2*pow(c1,2) + 2*d1*d2*pow(c2,2);
        double e = 1 - d1*pow(c1,2) - d2*pow(c2,2);


        double arr[] = {a,b,c,d,e};
        vector<double> k(arr, arr+5);

        /// GetRoots
        vector<double> s = EllRoutines::quartic(k,pow(10,-6));
        #if DEBUG == 1
            cout << "s from quartic():  \n  ";
            for(vector<double>::iterator i = s.begin(); i != s.end(); ++i)
            {
                cout << *i << " ";
            }
        #endif
        if(s.size()==0){
          cout << "============> error! Vector 's' contains 0 elements!!" << endl;
          exit(EXIT_FAILURE);
        }

        /// Get real non-zero roots
        vector<int> ids;
        int index=0;
        for(vector<double>::iterator i = s.begin(); i < s.end(); ++i)
        {

            double s_elem = *i;
            if(s_elem != 0){
                ids.push_back(index);
            }
            index++;
        }

        vector<double> s2;
        for(vector<int>::iterator i = ids.begin(); i != ids.end(); ++i)
        {
            s2.push_back(s[*i]);
        }
        #if DEBUG == 1
        cout << "\nreal non-zero roots: \n   ";
        for(vector<double>::iterator i = s2.begin(); i != s2.end(); ++i)
        {
            cout << *i << " ";
        }
        cout << endl;
        #endif
        if(s2.size()==0){
          cout << "============> error! Vector 's2' contains 0 elements!!" << endl;
          exit(EXIT_FAILURE);
        }

        /// Get extremal points
        vector<double> px;
        vector<double> py;

        #if DEBUG == 1
          cout << "tol: "<< tol << endl;
        #endif

        for(vector<double>::iterator i = s2.begin(); i != s2.end(); ++i)
        {
            double s2_elem = *i;

            double px_elem;
            double px_num = d1*c1*s2_elem;
            double px_den = (d1*s2_elem -1);

            // Approximation to 0
            #if DEBUG == 1
            //cout << "\npx_num_elem = "<< px_num << endl;
            #endif
            if(fabs(px_num)<=tol){
              #if DEBUG == 1
              //cout << " -->px_num_elem reduced to 0" << endl;
              #endif
              px_num = 0.;
            }
            #if DEBUG == 1
            //cout << "px_den_elem = "<< px_den << endl;
            #endif
            if(fabs(px_den)<=tol){
              #if DEBUG == 1
              //cout << " -->px_den_elem reduced to 0" << endl;
              #endif
              px_den = 0.;
            }

            px_elem = px_num/px_den;


            double py_elem;
            double py_num = d2*c2*s2_elem;
            double py_den = (d2*s2_elem -1);

            #if DEBUG == 1
            //cout << "py_num_elem = "<< py_num << endl;
            #endif
            if(fabs(py_num)<=tol){
              #if DEBUG == 1
              //cout << " -->py_num_elem reduced to 0" << endl;
              #endif
              py_num = 0.;
            }
            #if DEBUG == 1
            //cout << "py_den_elem = "<< py_den << endl;
            #endif
            if(fabs(py_den)<=tol){
              #if DEBUG == 1
              //cout << " -->py_den_elem reduced to 0" << endl;
              #endif
              py_den = 0.;
            }

            py_elem=py_num/py_den;

            if(isnan(py_elem))
              py_elem = 0;
            if(isnan(px_elem))
              px_elem = 0;

            px.push_back(px_elem);
            py.push_back(py_elem);

            // Removing NaN and Inf values
            double ds_elem = std::sqrt(pow(px_elem,2)+pow(py_elem,2));
            if(!isnan(ds_elem) && !isinf(ds_elem))
              ds.push_back(ds_elem);
        }




        #if DEBUG == 1
            cout << "\npx:  \n  ";
            for(vector<double>::iterator i = px.begin(); i != px.end(); ++i)
            {
                cout << *i << " ";
            }
            cout << "\npy:  \n  ";
            for(vector<double>::iterator i = py.begin(); i != py.end(); ++i)
            {
                cout << *i << " ";
            }
        #endif

    }



    EllRoutines::printVector(ds,"ds");


    /// Intersection Query

    /// error
    int res = -1;

    double ds_max = *std::max_element(ds.begin(), ds.end());
    double ds_min = *std::min_element(ds.begin(), ds.end());

    #if DEBUG == 1
      cout << "DS_MAX: " << ds_max << endl;
      cout << "DS_MIN: " << ds_min << endl;
    #endif

    if(fabs(1-ds_max) <= tol)
      ds_max = 1;


    if(fabs(1-ds_min) <= tol)
      ds_min = 1;


    double d = 1.0;


    if ( ds_max < 1)
        res = 0;
    else if (ds_max > 1)
        if( ds_min < 1 )
            res = 3;
        else if(ds_min > 1)
        {
            double f0 = d1*pow(c1,2) + d2*pow(c2,2)-1;
            #if DEBUG == 1
              cout << "f0 ---> " << f0 << endl;
            #endif
            if(fabs(f0) <= tol)
              f0 = 0;

            if( f0 > 0 )
                res = 7;
            else
                res = 5;
        }
        else
        {
            double f0 = d1*pow(c1,2) + d2*pow(c2,2) - 1;
            #if DEBUG == 1
              cout << "f0 ---> " << f0 << endl;
            #endif
            if(fabs(f0) <= tol)
              f0 = 0;

            if( f0 > 0 )
                res = 6;
            else
                res = 4;
        }
    else
    {
        if( ds_min < 1)
            res = 1;
        else
            res = 2;
    }


    return res;

}

vector<double> EllRoutines::quartic(vector<double>& k, double tol = 1/pow(10,12))
{
    complex<double> complex_zero (0,0);
    complex<double> aa (0.5,0);
    complex<double> bb (2,0);
    complex<double> cc (3,0);

    #if DEBUG == 1
        cout << "\nquartic()" << endl;
    #endif

    vector<complex<double> > s;

    double a,b,c,d = 0;

    /// Normalize to x^4 + a*x^3 + b*x^2 + c*x + d = 0
    a = k[1]/k[0];
    b = k[2]/k[0];
    c = k[3]/k[0];
    d = k[4]/k[0];

    #if DEBUG == 1
        cout << "a,b,c,d:\n " << a << "\n " << b << "\n " << c << "\n " << d << "\n " << endl;
    #endif


    if ( d==0 )
    {

        ///cubic
        double arr[] = {1.,a,b,c};
        vector<double> z(arr, arr+4);


        vector<complex<double> > t = cubic(z, tol);
        // GO ON
        s.push_back(0);
        for(vector<complex<double> >::iterator i = t.begin(); i < t.end(); ++i)
        {
            s.push_back(*i);
        }
        #if DEBUG == 1
            cout << "s from cubic1():  \n  ";
            for(vector<complex<double> >::iterator i = s.begin(); i != s.end(); ++i)
            {
                cout << *i << " ";
            }
        #endif
    }
    else
    {

        /// Depressed quartic y^4 + p*y^2 + q*y + r = 0
        double p = ( 8*b - 3*pow(a,2) )/8;
        double q = ( pow(a,3) - 4*a*b + 8*c)/8;
        double r = ( -3*pow(a,4) + 256*d - 64*c*a + 16*pow(a,2)*b)/256;

        #if DEBUG == 1
            cout << "p,q,r:\n " << p << "\n " << q << "\n " << r << "\n" << endl;
        #endif

        /// Standard depressed quartic
        if( q!=0 )
        {


            /// cubic
            double arr[] = {1. , p*5/2,  2*pow(p,2) - r,   pow(p,3)/2 - p*r/2 - pow(q,2)/8};
            vector<double> z(arr, arr+4);
            #if DEBUG == 1
              EllRoutines::printVector(z,"z");
            #endif


            vector<complex<double> > t = cubic(z, tol);


            #if DEBUG == 1
              EllRoutines::printVector(t,"t from cubic2");
            #endif



             /// % Search a positive non zero root
            vector<complex<double> > g; /// g = p + 2*t
            for(vector<complex<double> >::iterator i = t.begin(); i < t.end(); ++i)
            {
                g.push_back(p+bb* (*i));
            }
            int id = -1;
            int index = 0;
            for(vector<complex<double> >::iterator i = g.begin(); i < g.end(); ++i)
            {
                complex<double> d = *i;
                if( d.real() > complex_zero.real() )
                {
                    id = index;
                    break;
                }

                index++;
            }
            if( index == -1 )
            {
                index = 0;
                for(vector<complex<double> >::iterator i = g.begin(); i < g.end(); ++i)
                {
                    complex<double> d = *i;
                    if ( d != complex_zero )
                    {
                        id = index;
                        break;
                    }

                }
            }

            /// build the roots of the quartic

            complex<double> u = t[id];
            complex<double> v = std::sqrt(g[id]);

            double term = -k[1]/ (4*k[0]);



            s.push_back(term + aa * (   v + sqrt(-(cc*p + bb*u + bb*q/v)) ) );
            s.push_back(term + aa * (   v - sqrt(-(cc*p + bb*u + bb*q/v)) ) );
            s.push_back(term + aa * ( - v + sqrt(-(cc*p + bb*u - bb*q/v)) ) );
            s.push_back(term + aa * ( - v - sqrt(-(cc*p + bb*u - bb*q/v)) ) );



        }
        else
        {

            /// biquadratic

            d = pow(p,2) - 4*r;

            if ( d != 0)
            {

                double zp = (-1*p + std::sqrt(d))/2;
                double zm = (-1*p - std::sqrt(d))/2;
                s.push_back(std::sqrt(zp));
                s.push_back(-1*std::sqrt(zp));
                s.push_back(std::sqrt(zm));
                s.push_back(-1*std::sqrt(zm));
            }
            else
            {

                double z = -p/2;
                s.push_back(sqrt(z));
                s.push_back(-1*sqrt(z));
            }
        }
    }


    vector<double> s_real;

    for(vector<complex<double> >::iterator i = s.begin(); i != s.end(); i++)
    {
        complex<double> cd = *i;

        if(std::abs(cd.imag()) < tol)
        {
            s_real.push_back(cd.real());
        }

    }

    return s_real;
}

vector<complex<double> > EllRoutines::cubic(vector<double>& k, double tol = 1/pow(10,12)){
    #if DEBUG == 1
        cout << "\ncubic()" << endl;
    #endif

    vector<complex<double> > s;

    complex<double> a = k[0];
    complex<double> b = k[1];
    complex<double> c = k[2];
    complex<double> d = k[3];


    complex<double> d0 = pow(b,2) - 3.*a*c;
    complex<double> d1 = 2.*pow(b,3) - 9.*a*b*c + 27.*pow(a,2)*d;
    complex<double> dl = (4.*pow(d0,3)-pow(d1,2))/(27.*pow(a,2));

    #if DEBUG == 1
        cout << "\nd0,d1,dl: \n" << d0 << "\n " << d1 << "\n " << dl <<endl;
    #endif

    complex<double> C;

    if (dl != 0.)
    {

        if (d0 != 0.)
        {

            complex<double> const4_c(4,0);
            complex<double> const2_c(2,0);

            complex<double> term1 = pow(d1,2) - const4_c*pow(d0,3);
            complex<double> term2 = sqrt(term1);
            complex<double> term3 = ( d1+term2 )/const2_c;

            C = pow(term3,1/3.);
        }
        else
        {

            C = pow(d1,1/3.);
        }

        #if DEBUG == 1
          cout << "C (cubic):\n" << C << endl;
        #endif

        vector<complex<double> > u;

        u.push_back(1);

        complex<double> termPlus(-1,sqrt(3));
        u.push_back( termPlus/2. );

        complex<double> termMinus(-1,-sqrt(3));
        u.push_back( termMinus/2. );

        #if DEBUG == 1
            cout << "u: ";
            for(vector<complex<double> >::iterator i = u.begin(); i != u.end(); ++i)
            {
                cout << *i << " ";
            }
            cout << endl;
        #endif

        for(vector<complex<double> >::iterator i = u.begin(); i!=u.end(); ++i)
        {
            complex<double> u_elem = *i;
            complex<double> temp = -(b + u_elem*C + d0/(u_elem*C))/(3.*a);
            s.push_back( temp );
        }

        #if DEBUG == 1
            cout << "s: ";
            for(vector<complex<double> >::iterator i = s.begin(); i != s.end(); ++i)
            {
                cout << *i << " ";
            }
            cout << endl;
        #endif
    }
    else
    {

        if (d0 != 0.)
        {
            s.push_back((9.*a*d - b*c)/(2.*d0));
            s.push_back((4.*a*b*c - 9.*pow(a,2)*d - pow(b,3))/(a*d0) );



        }
        else
        {

            s.push_back( -b/3.*a );

        }

    }



    vector<complex<double> > s_after_check_tol;

    for(vector<complex<double> >::iterator i = s.begin(); i != s.end(); i++)
    {
        complex<double> c = *i;
        if(std::fabs(c.imag()) < tol)
        {
            std::complex<double> cd (c.real(),0);
            s_after_check_tol.push_back(cd);
        }
    }

    #if DEBUG == 1
      EllRoutines::printVector(s_after_check_tol,"s_after_check_tol end of cubic");
    #endif


    return s_after_check_tol;
}

pair<string,short int> EllRoutines::ellmsg(int res)
{
    pair<string,short int> msg;

    switch (res)
    {
    case 0:
        msg = make_pair("is contained in",0);
        break;
    case 1:
        msg = make_pair("is contained but tangent to",1);
        break;
    case 2:
        msg = make_pair("is equal to",2);
        break;
    case 3:
        msg = make_pair("overlap with",3);
        break;
    case 4:
        msg = make_pair("contains but tangent to",4);
        break;
    case 5:
        msg = make_pair("contains",5);
        break;
    case 6:
        msg = make_pair("is external but tangent to",6);
        break;
    case 7:
        msg = make_pair("is external to",7);
        break;
    default:
        msg = make_pair("error",-1);
        break;
    }
    return msg;

}

void EllRoutines::printVector(vector<double>& k, string name){
    cout <<"\n"<<name << ": ";
    // std::cout.precision(15);
    for(vector<double>::iterator i = k.begin(); i < k.end(); ++i)
    {
        cout << " "<< std::setprecision(15) << *i << " ";
    }
    cout << "\n";

}
void EllRoutines::printVector(vector<int>& k, string name){
    cout <<"\n"<<name << ": ";
    for(vector<int>::iterator i = k.begin(); i < k.end(); ++i)
    {
        cout << " "<< std::setprecision(10) << *i << " ";
    }
    cout << "\n";

}
void EllRoutines::printVector(vector< complex<double> >& k, string name){
    cout <<"\n"<<name << ": ";
    for(vector< complex<double> >::iterator i = k.begin(); i < k.end(); ++i)
    {
        complex<double> cd = *i;
        cout << " " << cd << " ";
    }
    cout << "\n";
}
