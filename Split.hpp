//
//  Split.hpp
//  MeshEditor[0]
//
//  Created by maks on 3/18/19.
//  Copyright © 2019 maks. All rights reserved.
//


#pragma once
#include "Command.h"
#include <math.h>


class Split : public Command
{
public:
    const std::string& getName() const override;
    int execute(const std::map<std::string, std::string>& args) override;
private:
    std::string name = "Split";
    Vec origin = Vec(0.0, 0.0, 0.0), direction = Vec(0.0, 0.0, 0.0);
    std::string input, filepath1, filepath2;
    TriangleSoup triangleSoup, output1, output2;
    STLParser parser;
    int intersect_Plane( Vec p1, Vec p2, Vec origin, Vec normal, Vec *ptr );
    
};

