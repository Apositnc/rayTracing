#pragma once

#include "ray.h"

class material; // 这里只保存材质指针，前置声明即可，避免与 material.h 循环包含。

//一个碰撞记录
class hit_record{
public:
    point3 p;
    vec3 normal;//法向量 但是要区分是在圆内部碰到还是球外部 这两种法向量朝向虽然算出来都是朝外
    //但是实际上有可能需要朝内的法向量
    double t;   

    bool front_face;//1 正面命中 2 内部命中

    shared_ptr<material> mat;//一个指向材质基类的共享指针

    void set_face_normal(const ray& r,const vec3& outward_normal){
        front_face = dot(r.Dir(),outward_normal)<0;//小于0证明是光线目前和算出来的是反方向，直接赋值就行
        //大于0证明 光线此时和法向量同向 需要把法向量变负数才能得出正确的

        normal = front_face?outward_normal:-outward_normal;
    }
};

//可碰撞物体的抽象类
class hittable{
public:
    virtual ~hittable() = default;

    //bool确定是否命中 rec用来保存命中的信息
    //virtual bool hit(const ray& r,double ray_tmin,double ray_tmax,hit_record& rec) const = 0;
    virtual bool hit(const ray& r,
        interval ray_t,hit_record& rec) const = 0;
};
