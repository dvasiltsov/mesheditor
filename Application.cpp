//
//  Application.cpp
//  MeshEditor[0]
//
//  Created by maks on 3/18/19.
//  Copyright © 2019 maks. All rights reserved.
//

#include "Application.hpp"


Application::Application()
{
    ptrs.clear();
}

 void Application::registerCommand(std::unique_ptr<Command> command)
{
    ptrs.emplace(command->getName(), std::move(command));
  
}

int Application::execute(int argc, char *argv[])
{
   
    if(argc > 2)
    {   name = argv[1];
        
        for (int i = 2; i<argc; i++) {
            input +=argv[i];
        }
      
        if(name == "Cube")
        {   if(argc == 11)
            {
            args.emplace("L",argv[4]);
            args.emplace("origin",argv[7]);
            args.emplace("filepath",argv[10]);
            }
            else return 3;
        }
        else if (name == "Sphere")
        {   if(argc == 17)
            {
            args.emplace("R",argv[4]);
            args.emplace("origin",argv[7]);
            args.emplace("slices",argv[10]);
            args.emplace("rings",argv[13]);
            args.emplace("filepath",argv[16]);
            }
            else return 3;
        }
        else if (name == "Split")
        {   if(argc == 17)
            {
            args.emplace("input",argv[4]);
            args.emplace("origin",argv[7]);
            args.emplace("direction",argv[10]);
            args.emplace("output1",argv[13]);
            args.emplace("output2",argv[16]);
            }
            else return 3;
        }
    }
    else if(argc == 2)
    {
        input = argv[1];
        name = input.substr(0,input.find(" "));
        input.erase(0, input.find(" ")+1);
       
        if (name=="Cube")
        {
            args.emplace("L",input.substr(input.find("=")+2,input.find(",")-(input.find("=")+2)));
            input.erase(0, input.find(",")+1);
            args.emplace("origin",input.substr(input.find("(")+1,input.find(")")-(input.find("(")+1)));
            input.erase(0, input.find(")")+1);
            args.emplace("filepath",input.substr(input.find("=")+2, input.back() - (input.find("=")+2)));
            input.erase(0, input.back());
        }
        else if(name=="Sphere")
            {
                args.emplace("R",input.substr(input.find("=")+2,input.find(",")-(input.find("=")+2)));
                input.erase(0, input.find(",")+1);
                args.emplace("origin",input.substr(input.find("(")+1,input.find(")")-(input.find("(")+1)));
                input.erase(0, input.find(")")+2);
                args.emplace("slices",input.substr(input.find("=")+2,input.find(",")-(input.find("=")+2)));
                input.erase(0, input.find(",")+1);
                args.emplace("rings",input.substr(input.find("=")+2,input.find(",")-(input.find("=")+2)));
                input.erase(0, input.find(",")+1);
                args.emplace("filepath",input.substr(input.find("=")+2, input.back() - (input.find("=")+2)));
                input.erase(0, input.back());
            }
        else if(name=="Split")
            {
                args.emplace("input",input.substr(input.find("=")+2,input.find(",")-(input.find("=")+2)));
                input.erase(0, input.find(",")+1);
                args.emplace("origin",input.substr(input.find("(")+1,input.find(")")-(input.find("(")+1)));
                input.erase(0, input.find(")")+1);
                args.emplace("direction",input.substr(input.find("(")+1,input.find(")")-(input.find("(")+1)));
                input.erase(0, input.find(")")+1);
                args.emplace("output1",input.substr(input.find("=")+2,input.find(",")-(input.find("=")+2)));
                input.erase(0, input.find(",")+1);
                args.emplace("output2",input.substr(input.find("=")+2, input.back() - (input.find("=")+2)));
                input.erase(0, input.back());
            }
    }
    else return 3;
 
    
    auto choise = ptrs.find(name);
    return choise->second->execute(args);
}
