//
//  Split.cpp
//  MeshEditor[0]
//
//  Created by maks on 3/18/19.
//  Copyright © 2019 maks. All rights reserved.
//

#include "Split.hpp"
#define SMALL_NUM   0.00000001
#define dot(u,v)   ((u).x * (v).x + (u).y * (v).y + (u).z * (v).z)

const std::string& Split::getName() const
{
    return name;
}

int Split::intersect_Plane( Vec p1, Vec p2, Vec origin, Vec normal, Vec *ptr )
{
    Vec   u = Vec(p2.x - p1.x, p2.y - p1.y, p2.z - p1.z );
    Vec   w = Vec(p1.x - origin.x, p1.y - origin.y, p1.z - origin.z );
    
    double     D = dot(normal, u);
    double     N = -dot(normal, w);
    
    if (fabs(D) < SMALL_NUM) {           // segment is parallel to plane
        if (N == 0)                      // segment lies in plane
            return 2;
        else
            return 0;                    // no intersection
    }
    // they are not parallel
    // compute intersect param
    double sI = N / D;
    if (sI < 0 || sI > 1)
        return 0;                        // no intersection
    
    *ptr = Vec(p1.x + sI * u.x, p1.y + sI * u.y, p1.z + sI * u.z);
                      // compute segment intersect point
    return 1;
};

void  computeNormals(TriangleSoup &triangleSoup)
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

int Split::execute(const std::map<std::string, std::string>& args)
{
    auto it = args.find("input");
    input = it->second.substr(0,it->second.find(","));
    
    it = args.find("output1");
    filepath1 = it->second.substr(0,it->second.find(","));
    
    it = args.find("output2");
    filepath2 = it->second.substr(0,it->second.find(","));
    
    it = args.find("origin");
    std::string orig = it->second;
    origin.x = std::stod(orig.substr(1,orig.find(",")));
    orig.erase(0, orig.find(",")+1);
    origin.y = std::stod(orig.substr(0,orig.find(",")));
    orig.erase(0, orig.find(",")+1);
    origin.z = std::stod(orig.substr(0,orig.find(")")));
    
    it = args.find("direction");
    orig = it->second;
    direction.x = std::stod(orig.substr(1,orig.find(",")));
    orig.erase(0, orig.find(",")+1);
    direction.y = std::stod(orig.substr(0,orig.find(",")));
    orig.erase(0, orig.find(",")+1);
    direction.z = std::stod(orig.substr(0,orig.find(")")));

    std::ifstream fin;
    std::ofstream fout1,fout2;
    fin.open(input);
    fout1.open(filepath1);
    fout2.open(filepath2);
  
    auto directionLength = sqrt(pow(direction.x, 2) + pow(direction.y, 2) + pow(direction.z, 2));
    if (directionLength <= 0)
    {
        return 1;
    }
    
    if (!fin.is_open() || !fout1.is_open() || !fout2.is_open())
    {
        fin.close();
        fout1.close();
        fout2.close();
        return 2;
    }
    else
    {
        fin.close();
        fout1.close();
        fout2.close();
    }
    
    triangleSoup = parser.read(input);
    
    for (int i = 0; i<triangleSoup.size(); i+=3 )
    {
        int count_p = 0 , count_n = 0 , zero = 0;
        
        Vec in1, in2 ,in3;
        Vec* ptr1 = &in1;
        Vec* ptr2 = &in2;
        Vec* ptr3 = &in3;
        
        bool p0 = false, p1 = false, p2 = false;
        int i01 = 0, i12 = 0, i20 = 0;
      
        double signe0 = direction.x * (triangleSoup[i].pos.x - origin.x) + direction.y * (triangleSoup[i].pos.y - origin.y) + direction.z * (triangleSoup[i].pos.z - origin.z);
        if(signe0 > 0) {++count_p;   p0 = true;}
        if(signe0 < 0) {++count_n;   p0 = false;}
        if(signe0 == 0) {++zero;  }
        // std :: cout << signe0 << std::endl;
        
        double signe1 = direction.x * (triangleSoup[i+1].pos.x - origin.x) + direction.y * (triangleSoup[i+1].pos.y - origin.y) + direction.z * (triangleSoup[i+1].pos.z - origin.z);
        if(signe1 > 0) {++count_p;   p1 = true;}
        if(signe1 < 0) {++count_n;   p1 = false;}
        if(signe1 == 0) {++zero;  }
        //std :: cout << signe1 << std::endl;
        
        
        double signe2 = direction.x * (triangleSoup[i+2].pos.x - origin.x) + direction.y * (triangleSoup[i+2].pos.y - origin.y) + direction.z * (triangleSoup[i+2].pos.z - origin.z);
        if(signe2 > 0) {++count_p;   p2 = true;}
        if(signe2 < 0) {++count_n;   p2 = false;}
         if(signe2 == 0) {++zero;  }
        //std :: cout << signe2 << std::endl;
        i01 = intersect_Plane(triangleSoup[i].pos, triangleSoup[i+1].pos, origin, direction, ptr1);
        i12 = intersect_Plane(triangleSoup[i+1].pos,triangleSoup[i+2].pos, origin, direction, ptr2);
        i20 = intersect_Plane(triangleSoup[i+2].pos, triangleSoup[i].pos, origin, direction, ptr3);
        
                        //std :: cout << i01 << " " << i12 << " " << i20 << std::endl;
                        //std :: cout << count_p << " " << count_n << " "<< zero << std::endl;
                        // std :: cout << p0 << " " << p1 << " " << p2 << std::endl<< std::endl;
        
        if (count_p == 3 || (zero == 2 && count_p == 1) || (zero == 1 && count_p == 2))
        {
             output1.push_back(triangleSoup[i]);
             output1.push_back(triangleSoup[i+1]);
             output1.push_back(triangleSoup[i+2]);
        }
        else if (count_n == 3 || (zero == 2 && count_n == 1) || (zero == 1 && count_n == 2))
        {
            output2.push_back(triangleSoup[i]);
            output2.push_back(triangleSoup[i+1]);
            output2.push_back(triangleSoup[i+2]);
        }
        //1
         else if (signe0 < 0 && signe1 > 0 && signe2 == 0 )
        {
            Vertex point;
            
            point.pos =  triangleSoup[i+2].pos;
            point.normal = triangleSoup[i].normal;
            output2.push_back(point);
            
            point.pos =  triangleSoup[i].pos;
            point.normal = triangleSoup[i].normal;
            output2.push_back(point);
            
            point.pos = in1;
            point.normal = triangleSoup[i].normal;
            output2.push_back(point);
            
            point.pos =  triangleSoup[i+1].pos;
            point.normal = triangleSoup[i].normal;
            output1.push_back(point);
            
            point.pos = triangleSoup[i+2].pos;
            point.normal = triangleSoup[i].normal;
            output1.push_back(point);
            
            point.pos = in1;
            point.normal = triangleSoup[i].normal;
            output1.push_back(point);
           
        }
        //2
        else if (signe0 > 0 && signe1 < 0 && signe2 == 0 )
        {
            Vertex point;
            
            point.pos =  triangleSoup[i+2].pos;
            point.normal = triangleSoup[i].normal;
            output1.push_back(point);
            
            point.pos =  triangleSoup[i].pos;
            point.normal = triangleSoup[i].normal;
            output1.push_back(point);
            
            point.pos = in1;
            point.normal = triangleSoup[i].normal;
            output1.push_back(point);
            
            point.pos =  triangleSoup[i+1].pos;
            point.normal = triangleSoup[i].normal;
            output2.push_back(point);
            
            point.pos = triangleSoup[i+2].pos;
            point.normal = triangleSoup[i].normal;
            output2.push_back(point);
            
            point.pos = in1;
            point.normal = triangleSoup[i].normal;
            output2.push_back(point);
        }
  ////////////////////////////////////////////////////////////////////////////////
       //3
       else   if (signe1 > 0 && signe2 < 0 && signe0 == 0 )
        {
            Vertex point;
            point.pos =  triangleSoup[i].pos;
            point.normal = triangleSoup[i].normal;
            output1.push_back(point);
            
            point.pos =  triangleSoup[i+1].pos;
            point.normal = triangleSoup[i].normal;
            output1.push_back(point);
            
            point.pos = in2;
            point.normal = triangleSoup[i].normal;
            output1.push_back(point);
            
            point.pos =  triangleSoup[i+2].pos;
            point.normal = triangleSoup[i].normal;
            output2.push_back(point);
            
            point.pos = triangleSoup[i].pos;
            point.normal = triangleSoup[i].normal;
            output2.push_back(point);
            
            point.pos = in2;
            point.normal = triangleSoup[i].normal;
            output2.push_back(point);
            
        }
        ///4
        else  if (signe1 < 0 && signe2 > 0 && signe0 == 0 )
        {
            Vertex point;
            point.pos =  triangleSoup[i].pos;
            point.normal = triangleSoup[i].normal;
            output2.push_back(point);
            
            point.pos =  triangleSoup[i+1].pos;
            point.normal = triangleSoup[i].normal;
            output2.push_back(point);
            
            point.pos = in2;
            point.normal = triangleSoup[i].normal;
            output2.push_back(point);
            
            point.pos =  triangleSoup[i+2].pos;
            point.normal = triangleSoup[i].normal;
            output1.push_back(point);
            
            point.pos = triangleSoup[i].pos;
            point.normal = triangleSoup[i].normal;
            output1.push_back(point);
            
            point.pos = in2;
            point.normal = triangleSoup[i].normal;
            output1.push_back(point);
            
        }
       //5
        else  if (signe0 > 0 && signe2 < 0 && signe1 == 0 )
        {
            Vertex point;
            point.pos =  triangleSoup[i+1].pos;
            point.normal = triangleSoup[i].normal;
            output2.push_back(point);
            
            point.pos =  triangleSoup[i+2].pos;
            point.normal = triangleSoup[i].normal;
            output2.push_back(point);
            
            point.pos = in3;
            point.normal = triangleSoup[i].normal;
            output2.push_back(point);
            
            point.pos =  triangleSoup[i].pos;
            point.normal = triangleSoup[i].normal;
            output1.push_back(point);
            
            point.pos = triangleSoup[i+1].pos;
            point.normal = triangleSoup[i].normal;
            output1.push_back(point);
            
            point.pos = in3;
            point.normal = triangleSoup[i].normal;
            output1.push_back(point);
            
        }
        ///6
       else   if (signe0 < 0 && signe2 > 0 && signe1 == 0 )
        {
            Vertex point;
            point.pos =  triangleSoup[i+1].pos;
            point.normal = triangleSoup[i].normal;
            output1.push_back(point);
            
            point.pos =  triangleSoup[i+2].pos;
            point.normal = triangleSoup[i].normal;
            output1.push_back(point);
            
            point.pos = in3;
            point.normal = triangleSoup[i].normal;
            output1.push_back(point);
            
            point.pos =  triangleSoup[i].pos;
            point.normal = triangleSoup[i].normal;
            output2.push_back(point);
            
            point.pos = triangleSoup[i+1].pos;
            point.normal = triangleSoup[i].normal;
            output2.push_back(point);
            
            point.pos = in3;
            point.normal = triangleSoup[i].normal;
            output2.push_back(point);
            
        }
        
     //////////////////////////////////////////zverhu norm
      
       else   if(i12 == 1 && i20 == 1 && count_p == 2 && zero == 0)
        {
            Vertex point;
            point.pos =  triangleSoup[i].pos;
            point.normal = triangleSoup[i].normal;
            output1.push_back(point);
            
            point.pos =  triangleSoup[i+1].pos;
            point.normal = triangleSoup[i+1].normal;
            output1.push_back(point);
            
            point.pos = in3;
            point.normal = triangleSoup[i].normal;
            output1.push_back(point);
            
            point.pos =  triangleSoup[i+1].pos;
            point.normal = triangleSoup[i+1].normal;
            output1.push_back(point);
            
            point.pos = in2;
            point.normal = triangleSoup[i].normal;
            output1.push_back(point);
            
            point.pos = in3;
            point.normal = triangleSoup[i].normal;
            output1.push_back(point);
            
            point.pos = in3;
            point.normal = triangleSoup[i].normal;
            output2.push_back(point);
            
            point.pos = in2;
            point.normal = triangleSoup[i].normal;
            output2.push_back(point);
            
            point.pos =  triangleSoup[i+2].pos;
            point.normal = triangleSoup[i+2].normal;
            output2.push_back(point);
            
        }
        else if (i12 == 1 && i20 == 1 && count_n == 2 && zero == 0)
        {
            Vertex point;
            point.pos =  triangleSoup[i].pos;
            point.normal = triangleSoup[i].normal;
            output2.push_back(point);
            
            point.pos =  triangleSoup[i+1].pos;
            point.normal = triangleSoup[i+1].normal;
            output2.push_back(point);
            
            point.pos = in3;
            point.normal = triangleSoup[i].normal;
            output2.push_back(point);
            
            point.pos =  triangleSoup[i+1].pos;
            point.normal = triangleSoup[i+1].normal;
            output2.push_back(point);
            
            point.pos = in2;
            point.normal = triangleSoup[i].normal;
            output2.push_back(point);
            
            point.pos = in3;
            point.normal = triangleSoup[i].normal;
            output2.push_back(point);
            
            point.pos = in3;
            point.normal = triangleSoup[i].normal;
            output1.push_back(point);
            
            point.pos = in2;
            point.normal = triangleSoup[i].normal;
            output1.push_back(point);
            
            point.pos =  triangleSoup[i+2].pos;
            point.normal = triangleSoup[i+2].normal;
            output1.push_back(point);
            
        }
 /////////////////////////////////////////
       
       else   if(i12 == 1 && i01 == 1 && count_p == 2 && zero == 0)
        {
            Vertex point;
            point.pos =  triangleSoup[i+2].pos;
            point.normal = triangleSoup[i].normal;
            output1.push_back(point);
            
            point.pos =  triangleSoup[i].pos;
            point.normal = triangleSoup[i].normal;
            output1.push_back(point);
            
            point.pos = in2;
            point.normal = triangleSoup[i].normal;
            output1.push_back(point);
            
            point.pos =  triangleSoup[i].pos;
            point.normal = triangleSoup[i].normal;
            output1.push_back(point);
            
            point.pos = in1;
            point.normal = triangleSoup[i].normal;
            output1.push_back(point);
            
            point.pos = in2;
            point.normal = triangleSoup[i].normal;
            output1.push_back(point);
            
            point.pos = in2;
            point.normal = triangleSoup[i].normal;
            output2.push_back(point);
            
            point.pos = in1;
            point.normal = triangleSoup[i].normal;
            output2.push_back(point);
            
            point.pos =  triangleSoup[i+1].pos;
            point.normal = triangleSoup[i].normal;
            output2.push_back(point);
        
        }
        else if (i12 == 1 && i01 == 1 && count_n == 2 && zero == 0)
        {
            Vertex point;
            point.pos =  triangleSoup[i+2].pos;
            point.normal = triangleSoup[i].normal;
            output2.push_back(point);
            
            point.pos =  triangleSoup[i].pos;
            point.normal = triangleSoup[i].normal;
            output2.push_back(point);
            
            point.pos = in2;
            point.normal = triangleSoup[i].normal;
            output2.push_back(point);
            
            point.pos =  triangleSoup[i].pos;
            point.normal = triangleSoup[i].normal;
            output2.push_back(point);
            
            point.pos = in1;
            point.normal = triangleSoup[i].normal;
            output2.push_back(point);
            
            point.pos = in2;
            point.normal = triangleSoup[i].normal;
            output2.push_back(point);
            
            point.pos = in2;
            point.normal = triangleSoup[i].normal;
            output1.push_back(point);
            
            point.pos = in1;
            point.normal = triangleSoup[i].normal;
            output1.push_back(point);
            
            point.pos =  triangleSoup[i+1].pos;
            point.normal = triangleSoup[i].normal;
            output1.push_back(point);
        }
//////////////////////////////
        else  if(i20 == 1 && i01 == 1 && count_p == 2 && zero == 0)
        {
            Vertex point;
            point.pos =  triangleSoup[i+1].pos;
            point.normal = triangleSoup[i].normal;
            output1.push_back(point);
            
            point.pos =  triangleSoup[i+2].pos;
            point.normal = triangleSoup[i].normal;
            output1.push_back(point);
            
            point.pos = in1;
            point.normal = triangleSoup[i].normal;
            output1.push_back(point);
            
            point.pos =  triangleSoup[i+2].pos;
            point.normal = triangleSoup[i].normal;
            output1.push_back(point);
            
            point.pos = in3;
            point.normal = triangleSoup[i].normal;
            output1.push_back(point);
            
            point.pos = in1;
            point.normal = triangleSoup[i].normal;
            output1.push_back(point);
            
            point.pos = in1;
            point.normal = triangleSoup[i].normal;
            output2.push_back(point);
            
            point.pos = in3;
            point.normal = triangleSoup[i].normal;
            output2.push_back(point);
            
            point.pos =  triangleSoup[i].pos;
            point.normal = triangleSoup[i].normal;
            output2.push_back(point);
 
        }
        else if (i20 == 1 && i01 == 1 && count_n == 2 && zero == 0)
        {
            Vertex point;
            point.pos =  triangleSoup[i+1].pos;
            point.normal = triangleSoup[i].normal;
            output2.push_back(point);
            
            point.pos =  triangleSoup[i+2].pos;
            point.normal = triangleSoup[i].normal;
            output2.push_back(point);
            
            point.pos = in1;
            point.normal = triangleSoup[i].normal;
            output2.push_back(point);
            
            point.pos =  triangleSoup[i+2].pos;
            point.normal = triangleSoup[i].normal;
            output2.push_back(point);
            
            point.pos = in3;
            point.normal = triangleSoup[i].normal;
            output2.push_back(point);
            
            point.pos = in1;
            point.normal = triangleSoup[i].normal;
            output2.push_back(point);
            
            point.pos = in1;
            point.normal = triangleSoup[i].normal;
            output1.push_back(point);
            
            point.pos = in3;
            point.normal = triangleSoup[i].normal;
            output1.push_back(point);
            
            point.pos =  triangleSoup[i].pos;
            point.normal = triangleSoup[i].normal;
            output1.push_back(point);
            
        }
        
    }
  
    //computeNormals(output1);
    //computeNormals(output2);
    if(output1.empty() || output2.empty())
    {
        return 4;
    }
    //Vec a(1.0,0.0,0.0);
    // Vec b(0.0,3.0,0.0);
   // std::cout<<dot(a, b)<<std::endl;
    
    parser.write(output1, filepath1);
    parser.write(output2, filepath2);
   
    return 0;
}
