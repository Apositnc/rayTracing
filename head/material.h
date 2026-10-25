#pragma once

#include "hittable.h"

//材质基类 
class material {

public:
    virtual ~material() = default;
    //提供发生反射的时候应该怎么做的接口 判断是否继续反射 接收 入射光线 命中点信息 颜色 反射后的光线
    virtual bool scatter(const ray& r_in,const hit_record& rec,color& attenuation,ray& scattered)const{
        return false;
    }

};

//理想的漫反射表面
class lambertian : public material{

public:
    lambertian(const color& albedo):albedo(albedo){}
    //把传入的光线反射后的方向随机取 并且规定大小不能为0
    bool scatter(const ray& r_in,const hit_record& rec,color& attenuation,ray& scattered) const override{
        vec3 scatter_direction = rec.normal + random_unit_vector();
        
        if(scatter_direction.near_zero())
            scatter_direction = rec.normal;
        
        scattered = ray(rec.p,scatter_direction);
        attenuation = albedo;
        return true;
    }
    
private:
    color albedo;//反射率
};

//金属表面
class metal : public material{
public:
    metal(const color& albedo,double fuzz) : albedo(albedo) ,fuzz(fuzz<1?fuzz:1){}
    
    bool scatter(const ray& r_in,const hit_record& rec,color& attenuation,ray& scattered) const override{
        vec3 reflected = reflect(r_in.Dir(),rec.normal);//折射光线计算方式
        
        reflected = unit_vector(reflected) + (fuzz*random_unit_vector());

        scattered = ray(rec.p,reflected);
        attenuation = albedo;

        return (dot(scattered.Dir(),rec.normal)>0);//如果极端情况 散射到下方就直接忽略
    }

private:
//albedo 控制颜色衰减，fuzz 控制倒影模糊
    color albedo;

    double fuzz;
};