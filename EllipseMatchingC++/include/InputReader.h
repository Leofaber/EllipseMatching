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
