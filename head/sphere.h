#pragma once

#include "hittable.h"
#include "vec3.h"

class sphere : public hittable{
public:
    sphere(const point3& center,double radius):center(center),radius(radius){}

    bool hit(const ray& r,double ray_tmin,double ray_tmax,hit_record& rec)const override{

        vec3 oc = center - r.origin();
        double a = dot(r.Dir(),r.Dir());
        //double b = -2.0*dot(r.Dir(),oc);
        double h = dot(r.Dir(),oc);
        double c = dot(oc,oc) -radius*radius;

        double d = 4*h*h-4*a*c;
        double tmin = (h - std::sqrt(d) ) / a;
        double tmax = (h + std::sqrt(d) ) / a;

        if (d < 0) {
            return false;
        } else {
            if(tmin<ray_tmax&&tmin>ray_tmin)
            {
                rec.t = tmin;//长度
                rec.p = r.at(rec.t);//交点坐标
                vec3 outward_normal = (rec.p - center)/radius;
                rec.set_face_normal(r,outward_normal);
                return true;
            }else if(tmax<ray_tmax&&tmax>ray_tmin){
                rec.t = tmax;//长度
                rec.p = r.at(rec.t);//交点坐标
                //rec.normal = (rec.p - center)/radius;
                vec3 outward_normal = (rec.p - center)/radius;
                rec.set_face_normal(r,outward_normal);
                return true;
            }
            return false;
        }
    }


private:
    point3 center;
    double radius;
};