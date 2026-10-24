#pragma once

#include "hittable.h"
#include "vec3.h"

class sphere : public hittable{
public:
    sphere(const point3& center,double radius):center(center),radius(radius){}

    bool hit(const ray& r,interval ray_t,hit_record& rec)const override{

        vec3 oc = center - r.origin();
        double a = dot(r.Dir(),r.Dir());
        //double b = -2.0*dot(r.Dir(),oc);
        double h = dot(r.Dir(),oc);
        double c = dot(oc,oc) -radius*radius;

        // 使用 h 后，根为 (h ± sqrt(h*h - a*c)) / a。
        double d = h*h-a*c;

        if (d < 0) {
            return false;
        } else {
            double sqrtd = std::sqrt(d);
            double tmin = (h - sqrtd) / a;
            double tmax = (h + sqrtd) / a;
            if(tmin<ray_t.max&&tmin>ray_t.min)
            {
                rec.t = tmin;//长度
                rec.p = r.at(rec.t);//交点坐标
                vec3 outward_normal = (rec.p - center)/radius;
                rec.set_face_normal(r,outward_normal);
                return true;
            }else if(tmax<ray_t.max&&tmax>ray_t.min){
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
