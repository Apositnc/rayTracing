#pragma once

#include "vec3.h"
#include "interval.h"
#include <iostream>

using color = vec3;

inline double linear_to_gamma(double liner_component){
    if(liner_component>0){
        return std::sqrt(liner_component);
    }
    return 0;
}

void write_color(std::ostream& out,const color& c){
    auto r = c.x();
    auto g = c.y();
    auto b = c.z();

    r = linear_to_gamma(r);
    g = linear_to_gamma(g);
    b = linear_to_gamma(b);

    static const interval intensity(0.000,0.999);
    //传入的数保证在设置的区间上
    int rb = int(256*intensity.clamp(r));
    int gb = int(256*intensity.clamp(g));
    int bb = int(256*intensity.clamp(b));

    out << rb << ' '<< gb << ' ' << bb <<'\n';
}