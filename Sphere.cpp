//
//  Sphere.cpp
//  MeshEditor[0]
//
//  Created by maks on 3/18/19.
//  Copyright © 2019 maks. All rights reserved.
//

#include "Sphere.hpp"
const std::string& Sphere::getName() const
{
    return name;
}

void  Sphere::computeNormals(TriangleSoup &triangleSoup)
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

int Sphere::execute(const std::map<std::string, std::string>& args)
{
    auto it = args.find("R");
    R = std::stod(it->second);
    
        if(R <= 0)
        {
            return 1;
        }
    
    it = args.find("origin");
    std::string orig = it->second;
    origin.x = std::stod(orig.substr(1,orig.find(",")));
    
    orig.erase(0, orig.find(",")+1);
    origin.y = std::stod(orig.substr(0,orig.find(",")));
    
    orig.erase(0, orig.find(",")+1);
    origin.z = std::stod(orig.substr(0,orig.find(")")));
    
    it = args.find("slices");
    slices = std::stoi(it->second);
    
    it = args.find("rings");
    rings = std::stoi(it->second);
   
    it = args.find("filepath");
    filepath = it->second;
    filepath = filepath.substr(0,filepath.find(","));
    
    
    std::vector<Vec> vertices;
    double x, y, z, xy;
   
    double slicesStep = 2 * M_PI / slices;
    double ringsStep = M_PI / rings;
    double slicesAngle, ringsAngle;
    
    for(int i = 0; i <= rings; ++i)
    {
        ringsAngle = M_PI / 2 - i * ringsStep;        // starting from pi/2 to -pi/2
        xy = R * cosf(ringsAngle);             // r * cos(u)
        z = R * sinf(ringsAngle) + origin.z;              // r * sin(u)
        
        // add (sectorCount+1) vertices per stack
        // the first and last vertices have same position and normal, but different tex coords
        for(int j = 0; j <= slices; ++j)
        {
            slicesAngle = j * slicesStep;           // starting from 0 to 2pi
            
            // vertex position (x, y, z)
            x = xy * cosf(slicesAngle) + origin.x;             // r * cos(u) * cos(v)
            y = xy * sinf(slicesAngle) + origin.y;             // r * cos(u) * sin(v)
            
            // normalized vertex normal (nx, ny, nz)
            vertices.push_back(Vec(x,y,z));
           
        }
    }
    
    int k1, k2;
    for(int i = 0; i < rings; ++i)
    {
        k1 = i * (slices + 1);     // beginning of current stack
        k2 = k1 + slices + 1;      // beginning of next stack
        
        for(int j = 0; j < slices; ++j, ++k1, ++k2)
        {
            if(i == 0)
            {
                Vertex point;
                point.pos = vertices[k1];
                triangleSoup.push_back(point);
                point.pos = vertices[k2];
                triangleSoup.push_back(point);
                point.pos = vertices[k2+1];
                triangleSoup.push_back(point);
            }
           
            if(i != 0)
            {
                Vertex point;
                point.pos = vertices[k1];
                triangleSoup.push_back(point);
                point.pos = vertices[k2];
                triangleSoup.push_back(point);
                point.pos = vertices[k1+1];
                triangleSoup.push_back(point);
            }
            
            if(i != (rings-1))
            {
                Vertex point;
                point.pos = vertices[k1+1];
                triangleSoup.push_back(point);
                point.pos = vertices[k2];
                triangleSoup.push_back(point);
                point.pos = vertices[k2+1];
                triangleSoup.push_back(point);
            }
            
            if(i == (rings-1))
            {
                Vertex point;
                point.pos = vertices[k1+1];
                triangleSoup.push_back(point);
                point.pos = vertices[k2];
                triangleSoup.push_back(point);
                point.pos = vertices[k2+1];
                triangleSoup.push_back(point);
            }
           
        }
        
    }

   
    triangleSoup.resize(rings*slices*6);
    
  /*  for (int i = 0; i<triangleSoup.size(); i+=6)
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
        triangleSoup[i+3].normal = normal;
        triangleSoup[i+4].normal = normal;
        triangleSoup[i+5].normal = normal;
        
    }*/
    computeNormals(triangleSoup);
   
    std::fstream fout(filepath);
    if (!fout.is_open())
    {
        return 2;
    }
    else
    {
        fout.close();
        parser.write(triangleSoup, filepath);
    }
    
    return 0;
}


