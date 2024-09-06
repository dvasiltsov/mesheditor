//
//  Cube.hpp
//  MeshEditor[0]
//
//  Created by maks on 3/18/19.
//  Copyright © 2019 maks. All rights reserved.
//
#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include "Command.h"




class Cube : public Command
{
public:
    const std::string& getName() const override;
    int execute(const std::map<std::string, std::string>& args) override;
private:
    std::string name = "Cube";
    double L = 0.0;
    double hL = 0.0;
    Vec origin = Vec(0.0,0.0,0.0);
    std::string filepath = "";
    TriangleSoup triangleSoup;
    STLParser parser;
    void  computeNormals(TriangleSoup &triangleSoup);
};
