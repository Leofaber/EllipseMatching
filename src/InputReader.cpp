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
