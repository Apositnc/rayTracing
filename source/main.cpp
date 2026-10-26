
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


int main() {
    hittable_list world;

    // diff-add-start
    auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
    world.add(make_shared<sphere>(point3(0,-1000,0), 1000, ground_material));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = random_double();
            point3 center(a + 0.9*random_double(), 0.2, b + 0.9*random_double());

            if ((center - point3(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<material> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = color::random() * color::random();
                    sphere_material = make_shared<lambertian>(albedo);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                } else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = color::random(0.5, 1);
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = make_shared<metal>(albedo, fuzz);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                } else {
                    // glass
                    sphere_material = make_shared<dielectric>(1.5);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 = make_shared<dielectric>(1.5);
    world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
    world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));
    // diff-add-end

    camera cam;

    // diff-add-start
    cam.aspect_ratio      = 16.0 / 9.0;
    cam.image_w     = 1200;
    cam.samples_per_pixel = 10;
    cam.max_depth         = 50;

    cam.vfov     = 20;
    cam.lookfrom = point3(13,2,3);
    cam.lookat   = point3(0,0,0);
    cam.vup      = vec3(0,1,0);
    // diff-add-end

    cam.render(world);
}
