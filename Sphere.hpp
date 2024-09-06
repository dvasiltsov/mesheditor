//
//  Sphere.hpp
//  MeshEditor[0]
//
//  Created by maks on 3/18/19.
//  Copyright © 2019 maks. All rights reserved.
//

#pragma once
#include "Command.h"
#include <math.h>

class Sphere : public Command
{
public:
    const std::string& getName() const override;
    int execute(const std::map<std::string, std::string>& args) override;
private:
    std::string name = "Sphere";
    double R = 0.0;
    int rings = 0, slices = 0;
    Vec origin = Vec(0.0, 0.0, 0.0);
    std::string filepath = "";
    TriangleSoup triangleSoup;
    STLParser parser;
    void  computeNormals(TriangleSoup &triangleSoup);
};

