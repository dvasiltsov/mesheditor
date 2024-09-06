//
//  STLParser.cpp
//  MeshEditor[0]
//
//  Created by maks on 3/18/19.
//  Copyright © 2019 maks. All rights reserved.
//

#include "STLParser.hpp"

void STLParser::write(const TriangleSoup& triangleSoup, const std::string& filename)
{
    std::ofstream fout;
    fout.open(filename);
    if (!fout.is_open())
    {
        std::cout<<"Error of file\n";
    }
    
    fout<<"solid cube"<<std::endl;
    
    for (int i = 0; i < triangleSoup.size(); i+=3)
    {
        fout<<"facet normal "<<std::fixed<<std::setprecision(8)<< (double)triangleSoup[i].normal.x<<" "<<(double)triangleSoup[i].normal.y<<" "<<(double)triangleSoup[i].normal.z<<std::endl;
        fout<<" outer loop"<<std::endl;
        fout<<"    vertex "<< triangleSoup[i].pos.x<<" "<< triangleSoup[i].pos.y<<" "<< triangleSoup[i].pos.z<<"\n";
        fout<<"    vertex "<< triangleSoup[i+1].pos.x<<" "<< triangleSoup[i+1].pos.y<<" "<< triangleSoup[i+1].pos.z<<"\n";
        fout<<"    vertex "<< triangleSoup[i+2].pos.x<<" "<< triangleSoup[i+2].pos.y<<" "<< triangleSoup[i+2].pos.z<<"\n";
        fout<<" endloop\n";
        fout<<"endfacet\n";
        
    }
    fout<<"endsolid";
    fout.close();
}

TriangleSoup STLParser::read(const std::string& filename)
{
    TriangleSoup readTriangleSoup;
    std::ifstream fin;
    fin.open(filename);
    std::string str,str1,str2,str3;
    int count = 0;
    while(getline(fin,str))
    {
        ++count;
    }
    fin.close();
    fin.open(filename);
   
    readTriangleSoup.resize((int)((count-2)/7*3));
    std::getline(fin,str);
    for (int i = 0; i< (count - 2)/7; i++)
    {
        fin>>str;
        fin>>str;
        fin>>str1;
        fin>>str2;
        fin>>str3;
    
        readTriangleSoup[0 + i * 3].normal.x = std::stod(str1);
        readTriangleSoup[1 + i * 3].normal.x = std::stod(str1);
        readTriangleSoup[2 + i * 3].normal.x = std::stod(str1);
        
        readTriangleSoup[0 + i * 3].normal.y = std::stod(str2);
        readTriangleSoup[1 + i * 3].normal.y = std::stod(str2);
        readTriangleSoup[2 + i * 3].normal.y = std::stod(str2);
        
        readTriangleSoup[0 + i * 3].normal.z = std::stod(str3);
        readTriangleSoup[1 + i * 3].normal.z = std::stod(str3);
        readTriangleSoup[2 + i * 3].normal.z = std::stod(str3);
       
        fin>>str;
        fin>>str;
        fin>>str;
        fin>>str1;
        fin>>str2;
        fin>>str3;
        
        readTriangleSoup[0 + i * 3].pos.x = std::stod(str1);
        readTriangleSoup[0 + i * 3].pos.y = std::stod(str2);
        readTriangleSoup[0 + i * 3].pos.z = std::stod(str3);
        
        fin>>str;
        fin>>str1;
        fin>>str2;
        fin>>str3;
        
        readTriangleSoup[1 + i * 3].pos.x = std::stod(str1);
        readTriangleSoup[1 + i * 3].pos.y = std::stod(str2);
        readTriangleSoup[1 + i * 3].pos.z = std::stod(str3);
        
        fin>>str;
        fin>>str1;
        fin>>str2;
        fin>>str3;
        
        readTriangleSoup[2 + i * 3].pos.x = std::stod(str1);
        readTriangleSoup[2 + i * 3].pos.y = std::stod(str2);
        readTriangleSoup[2 + i * 3].pos.z = std::stod(str3);
        
        fin>>str;
        fin>>str;
       
    }
    
    fin.close();
    return readTriangleSoup;
}
