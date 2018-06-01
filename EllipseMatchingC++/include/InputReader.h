#ifndef INPUTREADER_H
#define INPUTREADER_H

#include <vector>
#include <string>
#include <fstream>
#include <sstream>

#include "Ellipse.h"

using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::ifstream;
using std::istringstream;

class InputReader
{
    public:
        static std::vector<Ellipse> readEllipsesFromFile(std::string filename);
    private:
        InputReader();
};

#endif // INPUTREADER_H
