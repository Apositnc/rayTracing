#pragma once

#include "vec3.h"

#include <iostream>

using color = vec3;

void write_color(std::ostream& out,const color& c){
    auto r = c.x();
    auto g = c.y();
    auto b = c.z();

    int rb = int(255.999*r);
    int gb = int(255.999*g);
    int bb = int(255.999*b);

    out << rb << ' '<< gb << ' ' << bb <<'\n';
}