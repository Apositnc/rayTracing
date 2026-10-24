#pragma once

#include "vec3.h"
#include "interval.h"
#include <iostream>

using color = vec3;

void write_color(std::ostream& out,const color& c){
    auto r = c.x();
    auto g = c.y();
    auto b = c.z();

    static const interval intensity(0.000,0.999);
    //传入的数保证在设置的区间上
    int rb = int(255*intensity.clamp(r));
    int gb = int(255*intensity.clamp(g));
    int bb = int(255*intensity.clamp(b));

    out << rb << ' '<< gb << ' ' << bb <<'\n';
}