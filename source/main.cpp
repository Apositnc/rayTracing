
#include "rtweekend.h"

#include "hittable.h"
#include "hittable_list.h"
#include "sphere.h"

double hit_sphere(const point3& center,double radius,const ray& r){
    vec3 oc = center - r.origin();
    double a = dot(r.Dir(),r.Dir());
    //double b = -2.0*dot(r.Dir(),oc);
    double h = dot(r.Dir(),oc);
    double c = dot(oc,oc) -radius*radius;
    double d = h*h-a*c;

    if (d < 0) {
        return -1.0;
    } else {
        return (h - std::sqrt(d) ) / a;//t是摄像机离与圆的交点有多远
    }
}

color ray_color(const ray& r, const hittable& world){

    hit_record rec;
    if(world.hit(r,interval(0,infinity),rec)){
        return 0.5*(rec.normal+color(1,1,1));
    }
    vec3 unit_direction = unit_vector(r.Dir());
    auto a = 0.5 * (unit_direction.y() + 1.0);

    return (1.0-a)*color(1.0,1.0,1.0) + a*color(0.5,0.7,1.0);
    // double t = hit_sphere(point3(0,0,-1),0.5,r);
    // //相交才的球才有颜色
    // if(t>0.0){
    //     vec3 n = unit_vector(r.at(t)-vec3(0,0,-1));//rat算出交点坐标 然后是与球心坐标算出法线
    //     //n 是一个在长度在球半径内的向量 画的颜色和设置的半径有关系
    //     return 0.5*color(n.x()+1,n.y()+1,n.z()+1);
    // }
    // //天空背景
    // vec3 unitDir = unit_vector(r.Dir());
    // //越往上 y 越大 蓝色越多
    // //设置的天空盒是这样
    // //unitDir刚刚归一化了 只看y来确定蓝色
    // //加一之后再次归一化 a决定蓝色多少 1-a决定白色多少
    // //白色 W = (1.0, 1.0, 1.0)
    // //蓝色 B = (0.5, 0.7, 1.0)
    // double a = 0.5*(unitDir.y()+1);
    // //return (1-a)*color(1.0,1.0,1.0)+a*color(0.5, 0.7, 1.0);//下蓝上白
    // return a*color(1.0,1.0,1.0)+(1-a)*color(0.5, 0.7, 1.0);
}


int main(){

   
    //世界
    hittable_list world;

    auto material_ground = make_shared<lambertian>(color(0.8,0.8,0.0));

    auto material_center = make_shared<lambertian>(color(0.1,0.2,0.5));//对三种颜色的反射能力
    auto material_left = make_shared<metal>(color(0.8,0.8,0.8),0.3);
    auto material_right = make_shared<metal>(color(0.8,0.6,0.2),1.0);

    world.add(make_shared<sphere>(point3( 0.0, -100.5, -1.0), 100.0, material_ground));
    world.add(make_shared<sphere>(point3( 0.0,    0.0, -1.2),   0.5, material_center));
    world.add(make_shared<sphere>(point3(-1.0,    0.0, -1.0),   0.5, material_left));
    world.add(make_shared<sphere>(point3( 1.0,    0.0, -1.0),   0.5, material_right));
    
    //相机
    camera cam;
    cam.aspect_ratio = 16.0/9.0;
    cam.image_w = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth = 50;
    cam.render(world);
    
}
