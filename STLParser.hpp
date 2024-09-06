//
//  STLParser.hpp
//  MeshEditor[0]
//
//  Created by maks on 3/18/19.
//  Copyright © 2019 maks. All rights reserved.
//
#pragma once


#include <vector>
#include <iostream>
#include <string>
#include <math.h>
#include <fstream>
#include <iomanip>



struct Vec {
    double x, y, z;
   
    Vec()
    {
        x = 0.0;
        y = 0.0;
        z = 0.0;
    };
    
    Vec(double X,double Y,double Z )
    {
        x = X;
        y = Y;
        z = Z;
        
    };
    
};

struct Vertex
{
    Vec pos;
    Vec normal;
};

using TriangleSoup = std::vector<Vertex>;

class STLParser
{
public:
    TriangleSoup read(const std::string& filename);
    void write(const TriangleSoup& triangleSoup, const std::string& filename);
private:
   
};

