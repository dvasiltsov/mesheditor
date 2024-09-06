//
//  Cube.cpp
//  MeshEditor[0]
//
//  Created by maks on 3/18/19.
//  Copyright © 2019 maks. All rights reserved.
//

#include "Cube.hpp"

const std::string& Cube::getName() const
{
    return name;
}

void  Cube::computeNormals(TriangleSoup &triangleSoup)
{
    
    for (int i = 0; i<triangleSoup.size(); i+=3 )
    {
        Vec d1;
        Vec d2;
        Vec normal;
        
        d1.x = triangleSoup[i].pos.x - triangleSoup[i+1].pos.x ;
        d1.y = triangleSoup[i].pos.y - triangleSoup[i+1].pos.y ;
        d1.z = triangleSoup[i].pos.z - triangleSoup[i+1].pos.z ;
        
        d2.x = triangleSoup[i+2].pos.x - triangleSoup[i+1].pos.x ;
        d2.y = triangleSoup[i+2].pos.y - triangleSoup[i+1].pos.y ;
        d2.z = triangleSoup[i+2].pos.z - triangleSoup[i+1].pos.z ;
        
        normal.x = d1.y * d2.z - d1.z * d2.y;
        normal.y = d1.z * d2.x - d1.x * d2.z;
        normal.z = d1.x * d2.y - d1.y * d2.x;
        
        double length = sqrt(pow(normal.x, 2) + pow(normal.y, 2) + pow(normal.z, 2));
        normal.x /= length;
        normal.y /= length;
        normal.z /= length;
        
        triangleSoup[i].normal = normal;
        triangleSoup[i+1].normal = normal;
        triangleSoup[i+2].normal = normal;

    }
    
};

int Cube::execute(const std::map<std::string, std::string>& args)
{
    auto it = args.find("L");
    L = std::stod(it->second);
    if(L <= 0)
    {
        return 1;
    }

    hL = L/2.0;
    
    it = args.find("origin");
    std::string orig = it->second;
    origin.x = std::stod(orig.substr(1,orig.find(",")));
  
    orig.erase(0, orig.find(",")+1);
    origin.y = std::stod(orig.substr(0,orig.find(",")));
   
    orig.erase(0, orig.find(",")+1);
    origin.z = std::stod(orig.substr(0,orig.find(")")));
   
    it = args.find("filepath");
    filepath = it->second;
    filepath = filepath.substr(0,filepath.find(","));
    
    triangleSoup.resize(36);
    
    
    //    v5----- v6
    //   /|      /|
    //  v1------v2|
    //  | |     | |
    //  | |v4---|-|v7
    //  |/      |/
    //  v0------v3
    
    Vec vecMas[8]=
    {
        Vec(origin.x - hL,origin.y - hL, origin.z - hL),    //      5   6
        Vec(origin.x - hL,origin.y + hL, origin.z - hL),    //
        Vec(origin.x + hL,origin.y + hL, origin.z - hL),    //      4   7
        Vec(origin.x + hL,origin.y - hL, origin.z - hL),    //
        Vec(origin.x - hL,origin.y - hL, origin.z + hL),    //  1   2
        Vec(origin.x - hL,origin.y + hL, origin.z + hL),    //
        Vec(origin.x + hL,origin.y + hL, origin.z + hL),    //  0   3
        Vec(origin.x + hL,origin.y - hL, origin.z + hL)     //
        
    };
    
    
    
    triangleSoup[0].pos = vecMas[2];
    triangleSoup[1].pos = vecMas[3];
    triangleSoup[2].pos = vecMas[0];
    triangleSoup[3].pos = vecMas[0];
    triangleSoup[4].pos = vecMas[1];
    triangleSoup[5].pos = vecMas[2];
    
    triangleSoup[6].pos = vecMas[4];
    triangleSoup[7].pos = vecMas[7];
    triangleSoup[8].pos = vecMas[6];
    triangleSoup[9].pos = vecMas[6];
    triangleSoup[10].pos = vecMas[5];
    triangleSoup[11].pos = vecMas[4];
    
    triangleSoup[12].pos = vecMas[6];
    triangleSoup[13].pos = vecMas[2];
    triangleSoup[14].pos = vecMas[1];
    triangleSoup[15].pos = vecMas[1];
    triangleSoup[16].pos = vecMas[5];
    triangleSoup[17].pos = vecMas[6];
    
    triangleSoup[18].pos = vecMas[0];
    triangleSoup[19].pos = vecMas[3];
    triangleSoup[20].pos = vecMas[7];
    triangleSoup[21].pos = vecMas[7];
    triangleSoup[22].pos = vecMas[4];
    triangleSoup[23].pos = vecMas[0];
    
    triangleSoup[24].pos = vecMas[0];
    triangleSoup[25].pos = vecMas[4];
    triangleSoup[26].pos = vecMas[5];
    triangleSoup[27].pos = vecMas[5];
    triangleSoup[28].pos = vecMas[1];
    triangleSoup[29].pos = vecMas[0];
    
    triangleSoup[30].pos = vecMas[6];
    triangleSoup[31].pos = vecMas[7];
    triangleSoup[32].pos = vecMas[3];
    triangleSoup[33].pos = vecMas[3];
    triangleSoup[34].pos = vecMas[2];
    triangleSoup[35].pos = vecMas[6];
   
    
   /* for (int i = 0; i<triangleSoup.size(); i+=3 )
    {
        Vec d1;
        Vec d2;
        Vec normal;
        
        d1.x = triangleSoup[i].pos.x - triangleSoup[i+1].pos.x ;
        d1.y = triangleSoup[i].pos.y - triangleSoup[i+1].pos.y ;
        d1.z = triangleSoup[i].pos.z - triangleSoup[i+1].pos.z ;

        d2.x = triangleSoup[i+2].pos.x - triangleSoup[i+1].pos.x ;
        d2.y = triangleSoup[i+2].pos.y - triangleSoup[i+1].pos.y ;
        d2.z = triangleSoup[i+2].pos.z - triangleSoup[i+1].pos.z ;
        
        normal.x = d1.y * d2.z - d1.z * d2.y;
        normal.y = d1.z * d2.x - d1.x * d2.z;
        normal.z = d1.x * d2.y - d1.y * d2.x;
        
        auto length = sqrt(pow(normal.x, 2) + pow(normal.y, 2) + pow(normal.z, 2));
        normal.x /= length;
        normal.y /= length;
        normal.z /= length;
        
        triangleSoup[i].normal = normal;
        triangleSoup[i+1].normal = normal;
        triangleSoup[i+2].normal = normal;
        
    }*/
    computeNormals(triangleSoup);
    std::ofstream fout;
    fout.open(filepath);
    
    if(!fout.is_open())
    {
        std::cout<<"Error of file\n";
        return 2;
    }
    else
    {
        fout.close();
        parser.write(triangleSoup, filepath);
    }
    return 0;
}
