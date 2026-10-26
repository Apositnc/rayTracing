#pragma once
#include<cmath>
#include<iostream>
#include "rtweekend.h"

class vec3 {
private:
    double e[3];
public:
    vec3() : e{0,0,0} {}
    vec3(double e0, double e1, double e2) : e{e0, e1, e2} {}

    double x() const {return e[0];}
    double y() const {return e[1];}
    double z() const {return e[2];}

    vec3 operator-() const {return vec3(-e[0],-e[1],-e[2]);}

    double operator[](int i) const {return e[i];}
    double& operator[](int i) {return e[i];}

    vec3& operator+=(const vec3 &v){
        e[0] += v.e[0];
        e[1] += v.e[1];
        e[2] += v.e[2];
        return *this;
    }

    vec3& operator*=(const vec3 &v){
        e[0] *= v.e[0];
        e[1] *= v.e[1];
        e[2] *= v.e[2];
        return *this;
    }

    vec3& operator*=(const double t){
        e[0] *= t;
        e[1] *= t;
        e[2] *= t;
        return *this;
    }

    vec3& operator/=(const double t){
        return *this *= 1/t;
    }

    double length_squared() const {
        return e[0]*e[0] + e[1]*e[1] + e[2]*e[2];
    }

    bool near_zero() const{
        double s = 1e-8;//全部都小于最小值则为真
        return (std::fabs(e[0])<s)&&(std::fabs(e[1])<s)&&(std::fabs(e[2])<s);
    }
    double length() const {
        return std::sqrt(length_squared());
    }

    static vec3 random(){
        return vec3(random_double(),random_double(),random_double());
    }
    static vec3 random(double min, double max){
        return vec3(random_double(min,max),random_double(min,max),random_double(min,max));
    }

};

using point3 = vec3;   // 3D point

inline std::ostream& operator<<(std::ostream &out,const vec3& v){
    return out << v.x() << ' ' << v.y() << ' ' << v.z();
}

inline vec3 operator+(const vec3& v1,const vec3& v2){
    return vec3(v1.x()+v2.x(),v1.y()+v2.y(),v1.z()+v2.z());

}

inline vec3 operator-(const vec3& v1,const vec3& v2){
    return vec3(v1.x()-v2.x(),v1.y()-v2.y(),v1.z()-v2.z());
}

inline vec3 operator*(const vec3& v1,const vec3& v2){
    return vec3(v1.x()*v2.x(),v1.y()*v2.y(),v1.z()*v2.z());
}

inline vec3 operator*(const vec3& v,double n){
    return vec3(v.x()*n,v.y()*n,v.z()*n);
}

inline vec3 operator*(double n,const vec3& v){
    return vec3(v.x()*n,v.y()*n,v.z()*n);
}

inline vec3 operator/(const vec3& v,double n){
    return vec3(v.x()/n,v.y()/n,v.z()/n);
}

inline double dot(const vec3& v1,const vec3& v2){
    return v1.x()*v2.x()+v1.y()*v2.y()+v1.z()*v2.z();
}

inline vec3 cross(const vec3& v1,const vec3& v2){
    return vec3(
        v1.y()*v2.z() - v1.z()*v2.y(),
        v1.z()*v2.x() - v1.x()*v2.z(),
        v1.x()*v2.y() - v1.y()*v2.x()
    );
}

inline vec3 unit_vector(const vec3& v) {
        return v / v.length();
}

inline vec3 random_unit_vector(){
    while(true){
        point3 p = vec3::random(-1,1);
        double lens = p.length_squared();
        if(lens<=1&&lens>1e-160){
            return p/sqrt(lens);//生成的是随机单位方向
        }
    }
}

inline vec3 random_on_hemisphere(const vec3& normal){
    vec3 on_unit_sphere = random_unit_vector();
    if(dot(on_unit_sphere,normal)>0.0){
        return on_unit_sphere;
    }else{
        return -on_unit_sphere;
    }
}

inline vec3 reflect(const vec3& v,const vec3& n){
    return v-2*dot(v,n)*n;
}

inline vec3 refract(const vec3& uv,const vec3& n,double etai_over_etat){
    double cos_theta = std::fmin(dot(-uv,n),1.0);
    vec3 r_out_perp = etai_over_etat * (uv+cos_theta*n);
    vec3 r_out_parallel = -std::sqrt(std::fabs(1.0-r_out_perp.length_squared()))*n;
    return r_out_parallel+r_out_perp;
}