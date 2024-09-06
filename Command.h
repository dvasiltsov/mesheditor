//
//  Command.h
//  MeshEditor[0]
//
//  Created by maks on 3/18/19.
//  Copyright © 2019 maks. All rights reserved.
//

#pragma once
#include <string>
#include <map>
#include "STLParser.hpp"

class Command
{
public:
    virtual ~Command() {}
    virtual const std::string& getName() const = 0;
    virtual int execute(const std::map<std::string, std::string>& args) = 0;
};

