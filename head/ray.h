#pragma once

#include<vec3.h>
//定义光线是两个参数 起点和方向 一个坐标 一个向量
class ray
{
private:
    point3 orig;
    vec3 dir;
public:
    ray(){}
    ray(const point3& origin,const point3& direction) : orig(origin),dir(direction){}

    const point3& origin() const{return orig;}
    const vec3& Dir() const{return dir;}

    point3 at(double t) const{
        return orig + t*dir;
    }
};

