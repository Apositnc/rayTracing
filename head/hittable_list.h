#pragma once

#include"hittable.h"
#include<vector>
#include<memory>

using std::make_shared;
using std::shared_ptr;

class hittable_list : public hittable{
public:
    std::vector<shared_ptr<hittable>> objects;

    hittable_list(){}
    hittable_list(shared_ptr<hittable> obj){add(obj);}
    
    void clear(){objects.clear();}
    void add(shared_ptr<hittable> obj){
        objects.push_back(obj);
    }

    bool hit(const ray& r,interval ray_t,hit_record& rec)
    const override{
        hit_record temp_rec;
        bool hit_anything = false;
        double cloosest_so_far = ray_t.max;

        for(const auto& obj : objects){//遍历列表内每一个物体 看看是否撞上
            if(obj->hit(r,interval(ray_t.min,cloosest_so_far),temp_rec)){//调用各个物体自己的碰撞安全范围 不断缩小max
                hit_anything = 1;
                cloosest_so_far = temp_rec.t;
                rec = temp_rec;
            }
        }
        return hit_anything;
    }
};
