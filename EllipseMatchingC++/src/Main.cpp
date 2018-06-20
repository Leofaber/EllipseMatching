
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

#include <stdio.h>
#include "InputReader.h"
#include "EllRoutines.h"

using std::cout;
using std::endl;
using std::vector;
using std::string;
using std::iterator;

#ifndef DEBUG
    #define DEBUG 0
#endif

int main(int argc, char*argv[]){

#ifndef DEBUG
    cout<<"******************************************************************"<<endl;
    cout<<"****************** ELLIPSE MATCHING C++ ROUTINE ******************"<<endl;
#endif

    #if DEBUG == 1
        cout << "** DEBUG MODE ON **" <<endl;
    #endif

    if (argc < 4){
        cout << "\n\nNot enough input provided.\nInsert:\n- the name of the first input file\n- the name of the second input file\n- the name of the output file"<<std::endl;
        exit(EXIT_FAILURE);
    }
    string filename1 = argv[1];
    string filename2 = argv[2];
    string outfilename = argv[3];


    vector<Ellipse> e1 = InputReader::readEllipsesFromFile(filename1);
    vector<Ellipse> e2 = InputReader::readEllipsesFromFile(filename2);
    /*
    #if DEBUG
        cout << filename1 << " -> " << e1.size() << endl;
        cout << filename2 << " -> " << e2.size() << endl;
    #endif
    */

    // output
    FILE * outF;
    outF = fopen (outfilename.c_str(),"w");


    // CORE ALGORITHM
    int index1=1;
    for (vector<Ellipse>::iterator i = e1.begin(); i!=e1.end(); ++i)
    {
        Ellipse e1 = (Ellipse) *i;

        #if DEBUG
            //fprintf(outF,"[%.5d] %s\n",index1,e1.name.c_str());
            printf("\n\n[%.5d] %s\n",index1,e1.name.c_str());
        #endif

        int index2=1;
        for (vector<Ellipse>::iterator j = e2.begin(); j!=e2.end(); ++j)
        {
            Ellipse e2 = (Ellipse) *j;

            if ( e1.isEqual(e2) )
            {
                #if DEBUG
                  fprintf(outF,"[%.5d] %s is equal to [%.5d] %s--2 %s %3.3f %3.2f %f %f %f\n",index2,e2.name.c_str(),index1,e1.name.c_str(), e2.name.c_str(), e2.x,e2.y, e2.a, e2.b, e2.p);
                  printf("[%.5d] %s is equal to [%.5d] %s--2 %s %3.3f %3.2f %f %f %f\n",index2,e2.name.c_str(),index1,e1.name.c_str(), e2.name.c_str(), e2.x,e2.y, e2.a, e2.b, e2.p);
                #else
                  fprintf(outF,"[%.5d] %s is equal to [%.5d] %s--2\n",index2,e2.name.c_str(),index1,e1.name.c_str());
                  printf("[%.5d] %s is equal to [%.5d] %s--2\n",index2,e2.name.c_str(),index1,e1.name.c_str());
                #endif

            }
            else
            {

                /// check if overlapping is possible using radius
                double dx = e2.x - e1.x;
                double dy = e2.y - e1.y;
                double d  = sqrt(pow(dx,2)+pow(dy,2));

                /// accurate overlapping
                if ( d <= (e2.r + e1.r) )
                {
                    /// ellmatrix() -> ellmatrix routine is encapsulated in Ellipse constructor

                    // One degree rotation if the axis are parallel
                    if (fabs(e1.p - e2.p) <= 1/pow(10,6)){
                      #if DEBUG == 1
                        cout << "==> Ellipses are parallel!! e1.p: "<< e1.p <<"  e2.p: "<< e2.p << "fabs(e1.p-e2.p) = "<< fabs(e1.p - e2.p)<<endl;
                      #endif
                      e1.p = e1.p + 1*M_PI/180;
                      e1.updateMatrices();
                      #if DEBUG == 1
                        cout << "==> Ellipses new rotation: e1.p: "<< e1.p <<"  e2.p: "<< e2.p <<endl;
                      #endif
                    }

                    int res = EllRoutines::elltest(e1.C,e1.D,e1.R,e1.M,e2.C,e2.D,e2.R,e2.M,1/pow(10,6));

                    pair<string,short int> msg = EllRoutines::ellmsg(res);
                    #if DEBUG
                      fprintf(outF,"[%.5d] %s %s [%.5d] %s--%d %s %3.3f %3.2f %f %f %f\n",index2, e2.name.c_str(), msg.first.c_str(), index1, e1.name.c_str(), msg.second, e2.name.c_str(), e2.x,e2.y, e2.a, e2.b, e2.p);
                      printf("[%.5d] %s %s [%.5d] %s--%d %s %3.3f %3.2f %f %f %f\n",index2, e2.name.c_str(), msg.first.c_str(), index1, e1.name.c_str(), msg.second, e2.name.c_str(), e2.x,e2.y, e2.a, e2.b, e2.p);
                    #else
                      fprintf(outF,"[%.5d] %s %s [%.5d] %s--%d\n",index2, e2.name.c_str(), msg.first.c_str(), index1, e1.name.c_str(), msg.second);
                      printf("[%.5d] %s %s [%.5d] %s--%d\n",index2, e2.name.c_str(), msg.first.c_str(), index1, e1.name.c_str(), msg.second);
                    #endif
                }
                else
                {
                  #if DEBUG
                    fprintf(outF,"[%.5d] %s is external to [%.5d] %s--7 %s %3.3f %3.2f %f %f %f\n",index2, e2.name.c_str(), index1, e1.name.c_str(), e2.name.c_str(), e2.x,e2.y, e2.a, e2.b, e2.p);
                    printf("[%.5d] %s is external to [%.5d] %s--7 %s %3.3f %3.2f %f %f %f\n",index2, e2.name.c_str(), index1, e1.name.c_str(), e2.name.c_str(), e2.x,e2.y, e2.a, e2.b, e2.p);
                  #else
                    fprintf(outF,"[%.5d] %s is external to [%.5d] %s--7\n",index2, e2.name.c_str(), index1, e1.name.c_str());
                    printf("[%.5d] %s is external to [%.5d] %s--7\n",index2, e2.name.c_str(), index1, e1.name.c_str());
                  #endif
                }
            }

            index2++;
        } // end for

        index1++;
    } // end for

    cout<<"\n**************** ELLIPSE MATCHING C++ ROUTINE END ****************"<<endl;
    cout<<"******************************************************************"<<endl;
    #if DEBUG
      cout<<"\nEND OF THE PROGRAM"<<endl;
    #endif
    return 0;
}
