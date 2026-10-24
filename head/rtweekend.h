#pragma once

#include<cmath>
#include<iostream>
#include<limits>
#include<memory>
#include<cstdlib>
using std::make_shared;
using std::shared_ptr;
//定义无穷大和π
const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

inline double degrees_to_radians(double degrees) {
    return degrees * pi / 180.0;
}

inline double random_double(){
    return std::rand()/(RAND_MAX + 1.0);// 返回一个在 [0,1) 上的随机实数
}
inline double random_double(double min, double max) {
    return min + (max - min) * random_double();// 返回一个在 [min,max) 上的随机实数
}

#include "color.h"
#include "ray.h"
#include "vec3.h"
#include "interval.h"
#include "camera.h"