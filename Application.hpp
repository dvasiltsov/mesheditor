//
//  Application.hpp
//  MeshEditor[0]
//
//  Created by maks on 3/18/19.
//  Copyright © 2019 maks. All rights reserved.
//


#pragma once
#include <stdio.h>
#include <string>
#include "Command.h"
#include "STLParser.hpp"

class Application
{
public:
    Application();
    void registerCommand(std::unique_ptr<Command> command);
    int execute(int argc, char *argv[]);
private:
    std::map<std::string,std::unique_ptr<Command>> ptrs;
    std::map<std::string,std::string> args;
    std::string input;
    std::string name;
};

