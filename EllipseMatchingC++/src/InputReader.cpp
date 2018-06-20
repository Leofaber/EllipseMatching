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
#include "InputReader.h"



InputReader::InputReader()
{
    //ctor
}

std::vector<Ellipse> InputReader::readEllipsesFromFile(std::string filename)
{
    vector<Ellipse> ellipses;

    ifstream infile;
    infile.open(filename.c_str());

    if(infile.is_open()){

        string line;
        while (getline(infile, line))
        {

            if(!line.empty()){

                //cout << "Found ellipse" <<endl;
                //replace(line.begin(), line.end(), ',', ' ');

                istringstream iss(line);

                string n;
                double x;
                double y;
                double a;
                double b;
                double p;

                iss >> n;
                iss >> x;
                iss >> y;
                iss >> a;
                iss >> b;
                iss >> p;

                //   cout <<pathToFile <<endl;
                //   cout <<classification<<endl;
                //   cout <<l<<endl;
                //   cout <<b<<endl;
                //   cout <<confidence<<endl;
                //   cout <<date<<endl;
                ellipses.push_back(Ellipse(n,x,y,a,b,p));

            }
            else{
                #if DEBUG==1
                    cout << "[InputReader] Line is empty!! " << endl;
                #endif
            }
        }


    }else{
        cout << "[Error 00] InputReader: can't open dataset" << endl;
    }

    return ellipses;


}
