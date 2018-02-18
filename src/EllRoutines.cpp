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

    Matrix2D Ctemp2 = Matrix2D::sqrt(D1) * Matrix2D::transpose(R1);

    pair<double,double> C3 = Ctemp2.vectorMultiplication(Ctemp);


    Matrix2D S1("S1", 1./std::sqrt(D1.matrix[0][0]), 0, 0, 1./std::sqrt(D1.matrix[1][1]));

    Matrix2D M3 = S1*Matrix2D::transpose(R1)*M2*R1*S1;


    /// Second transformation:
    /// Rotate to align E2 with the x/y axis

    Matrix2D R4 = Matrix2D::getVD(M3).first;        /// get V (autovettori)
    Matrix2D D4 = Matrix2D::getVD(M3).second;       /// get D (matrice diagonale di autovalori)


    pair<double,double> C4 = Matrix2D::transpose(R4).vectorMultiplication(C3);





    /// Quartic equation for extremal points
    double d1 = D4.matrix[0][0];
    double d2 = D4.matrix[1][1];
    double c1 = C4.first;
    double c2 = C4.second;

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

        /// Get extremal points
        vector<double> px;
        vector<double> py;

        for(vector<double>::iterator i = s2.begin(); i != s2.end(); ++i)
        {
            double s2_elem = *i;
            double px_elem = d1*c1*s2_elem/(d1*s2_elem -1);
            double py_elem = d2*c2*s2_elem/(d2*s2_elem -1);
            px.push_back(px_elem);
            py.push_back(py_elem);
            ds.push_back( std::sqrt(pow(px_elem,2)+pow(py_elem,2)));
        }


    }



    EllRoutines::printVector(ds,"ds");


    /// Intersection Query

    /// error
    int res = -1;

    double ds_max = *std::max_element(ds.begin(), ds.end());
    double ds_min = *std::min_element(ds.begin(), ds.end());


    if ( ds_max < 1)
        res = 0;
    else if (ds_max > 1)
        if( ds_min < 1 )
            res = 3;
        else if(ds_min > 1)
        {
            double f0 = d1*pow(c1,2) + d2*pow(c2,2)-1;

            if( f0 > 0 )
                res = 7;
            else
                res = 5;
        }
        else
        {
            double f0 = d1*pow(c1,2) + d2*pow(c2,2) - 1;
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

    #if DEBUG == 1
        cout << "quartic()" << endl;
    #endif

    vector<complex<double> > s;

    double a,b,c,d = 0;

    /// Normalize to x^4 + a*x^3 + b*x^2 + c*x + d = 0
    a = k[1]/k[0];
    b = k[2]/k[0];
    c = k[3]/k[0];
    d = k[4]/k[0];




    if ( d==0 )
    {

        ///cubic
        double arr[] = {1.,a,b,c};
        vector<double> z(arr, arr+4);

        s.push_back(0);

        vector<double> t = cubic(z, tol);

        for(vector<double>::iterator i = t.begin(); i < t.end(); ++i)
        {
            s.push_back(*i);
        }
    }
    else
    {

        /// Depressed quartic y^4 + p*y^2 + q*y + r = 0
        double p = ( 8*b - 3*pow(a,2) )/8;
        double q = ( pow(a,3) - 4*a*b + 8*c)/8;
        double r = ( -3*pow(a,4) + 256*d - 64*c*a + 16*pow(a,2)*b)/256;


        /// Standard depressed quartic
        if( q!=0 )
        {


            /// cubic
            double arr[] = {1. , p*5/2,  2*pow(p,2) - r,   pow(p,3)/2 - p*r/2 - pow(q,2)/8};
            vector<double> z(arr, arr+4);

            vector<double> t = cubic(z, tol);

             /// % Search a positive non zero root
            vector<double> g; /// g = p + 2*t
            for(vector<double>::iterator i = t.begin(); i < t.end(); ++i)
            {
                g.push_back(p+2* (*i));
            }



            int id = -1;
            int index = 0;
            for(vector<double>::iterator i = g.begin(); i < g.end(); ++i)
            {
                double d = *i;
                if( d > 0 )
                {
                    id = index;
                    break;
                }

                index++;
            }
            if( index == -1 )
            {
                index = 0;
                for(vector<double>::iterator i = g.begin(); i < g.end(); ++i)
                {
                    double d = *i;
                    if ( d != 0 )
                    {
                        id = index;
                        break;
                    }

                }
            }

            /// build the roots of the quartic

            double u = t[id];
            double v = std::sqrt(g[id]);

            double term = -k[1]/ (4*k[0]);

            s.push_back(term + 0.5 * (   v + sqrt(-(3*p + 2*u + 2*q/v)) ) );
            s.push_back(term + 0.5 * (   v - sqrt(-(3*p + 2*u + 2*q/v)) ) );
            s.push_back(term + 0.5 * ( - v + sqrt(-(3*p + 2*u - 2*q/v)) ) );
            s.push_back(term + 0.5 * ( - v - sqrt(-(3*p + 2*u - 2*q/v)) ) );



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

vector<double> EllRoutines::cubic(vector<double>& k, double tol = 1/pow(10,12)){
    #if DEBUG == 1
        cout << "cubic()" << endl;
    #endif

    vector<complex<double> > s;

    complex<double> a = k[0];
    complex<double> b = k[1];
    complex<double> c = k[2];
    complex<double> d = k[3];


    complex<double> d0 = pow(b,2) - 3.*a*c;
    complex<double> d1 = 2.*pow(b,3) - 9.*a*b*c + 27.*pow(a,2)*d;
    complex<double> dl = (4.*pow(d0,3)-pow(d1,2))/(27.*pow(a,2));


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

        vector<complex<double> > u;

        u.push_back(1);

        complex<double> termPlus(-1,sqrt(3));
        u.push_back( termPlus/2. );

        complex<double> termMinus(-1,-sqrt(3));
        u.push_back( termMinus/2. );


        for(vector<complex<double> >::iterator i = u.begin(); i!=u.end(); ++i)
        {
            complex<double> u_elem = *i;
            complex<double> temp = -(b + u_elem*C + d0/(u_elem*C))/(3.*a);
             s.push_back( temp );
        }


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
    cout <<"\n"<<name << ": "<<endl;
    for(vector<double>::iterator i = k.begin(); i < k.end(); ++i)
    {
        cout << "\t "<< std::fixed << *i << " ";
    }
        cout << "\n";

}
void EllRoutines::printVector(vector<int>& k, string name){
    cout <<"\n"<<name << ": "<<endl;
    for(vector<int>::iterator i = k.begin(); i < k.end(); ++i)
    {
        cout << "\t "<< std::fixed << *i << " ";
    }
        cout << "\n";

}
void EllRoutines::printComplexVector(vector< complex<double> >& k, string name){
    cout <<"\n"<<name << ": "<<endl;
    for(vector< complex<double> >::iterator i = k.begin(); i < k.end(); ++i)
    {
        complex<double> cd = *i;
        cout << "\t" <<cd << " ";
    }
    cout << "\n";
}
