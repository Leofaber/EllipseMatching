/*
 * Copyright (c) 2017
 *     Leonardo Baroncelli
 *
 * Any information contained in this software
 * is property of the AGILE TEAM and is strictly
 * private and confidential.
 *
 *
 */

#include <stdio.h>
#include "InputReader.h"
#include "EllRoutines.h"

using std::cout;
using std::endl;
using std::vector;
using std::string;
using std::iterator;

int main(int argc, char*argv[]){

    cout<<"****************************************"<<endl;
    cout<<"***** ELLIPSE MATCHING C++ ROUTINE *****"<<endl;

    if (argc < 4){
        cout << "\n\nNot enough input provided.\nInsert:\n- the name of the first input file\n- the name of the second input file\n- the name of the output file"<<std::endl;
        exit(EXIT_FAILURE);
    }
    string filename1 = argv[1];
    string filename2 = argv[2];
    string outfilename = argv[3];


    vector<Ellipse> e1 = InputReader::readEllipsesFromFile(filename1);
    vector<Ellipse> e2 = InputReader::readEllipsesFromFile(filename2);


    // output
    FILE * outF;
    outF = fopen (outfilename.c_str(),"w");


    // CORE ALGORITHM
    int index1=1;
    for (vector<Ellipse>::iterator i = e1.begin(); i!=e1.end(); ++i)
    {
        Ellipse e1 = (Ellipse) *i;
	fprintf(outF,"[%.5d] %s\n",index1,e1.name.c_str());
        printf("[%.5d] %s\n",index1,e1.name.c_str());        
	
        int index2=1;
        for (vector<Ellipse>::iterator j = e2.begin(); j!=e2.end(); ++j)
        {
            Ellipse e2 = (Ellipse) *j;

            if ( e1.isEqual(e2) )
            {
                fprintf(outF,"[%.5d] %s is equal to [%.5d] %s--2\n",index2,e2.name.c_str(),index1,e1.name.c_str());
		printf("[%.5d] %s is equal to [%.5d] %s--2\n",index2,e2.name.c_str(),index1,e1.name.c_str());
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
                    
                    int res = EllRoutines::elltest(e1.C,e1.D,e1.R,e1.M,e2.C,e2.D,e2.R,e2.M,1/pow(10,6));

                    pair<string,short int> msg = EllRoutines::ellmsg(res);

                    fprintf(outF,"[%.5d] %s %s [%.5d] %s--%d\n",index2, e2.name.c_str(), msg.first.c_str(), index1, e1.name.c_str(), msg.second);
                    printf("[%.5d] %s %s [%.5d] %s--%d\n",index2, e2.name.c_str(), msg.first.c_str(), index1, e1.name.c_str(), msg.second);

                }
                else
                {
                    fprintf(outF,"[%.5d] %s is external to [%.5d] %s--7\n",index2, e2.name.c_str(), index1, e1.name.c_str());
                    printf("[%.5d] %s is external to [%.5d] %s--7\n",index2, e2.name.c_str(), index1, e1.name.c_str());
                }
            }

            index2++;
        } // end for

        index1++;
    } // end for

    cout<<"***** ELLIPSE MATCHING C++ ROUTINE END *****"<<endl;    
    cout<<"********************************************"<<endl;
    
    return 0;
}
