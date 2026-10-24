
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

    // 设置图片属性
    double aspect_ratio = 16.0/9.0;
    int image_w = 400;
    int image_h = int(image_w/aspect_ratio);
    if(image_h<1)
        image_h = 1;

    // 设置相机坐标和视口的大小
    point3 camera_center = point3(0,0,0);
    double focal_length = 1.0;//板子离相机垂直距离
    double viewport_h = 2;
    double viewport_w = viewport_h*(double(image_w)/image_h);
    
    //沿着板子宽高走的两个向量 长度和方向
    vec3 viewport_u = vec3(viewport_w,0,0);
    vec3 viewport_v = vec3(0,-viewport_h,0);//图像行号向下增加，对应世界坐标 y 减小

    //用之前确定的像素算 在视口上面一步走多长距离
    vec3 pixel_delta_u = viewport_u / image_w;
    vec3 pixel_delta_v = viewport_v / image_h;

    //视口左上角坐标 也就是最开始的第一个格子的左上角
    point3 viewportLeft = camera_center - vec3(0,0,focal_length)
                            -viewport_u/2 - viewport_v/2;
    //第一个格子加上半个格子得到格子中心
    point3 pix100Loc = viewportLeft+0.5*(pixel_delta_u+pixel_delta_v);
    
    std::cout << "P3\n" << image_w << " " << image_h << "\n255\n";

    //世界
    hittable_list world;
    world.add(make_shared<sphere>(point3(0,0,-1),0.5));
    world.add(make_shared<sphere>(point3(0,-100.5,-1),100));

    for (int j = 0; j < image_h; j++) {
        std::clog << "\rScanlines remaining: " << (image_h - j) << ' ' << std::flush;
        for (int i = 0; i < image_w; i++) {

            // 处理这个格子。
            point3 pixelCenter = pix100Loc+(i*pixel_delta_u+j*pixel_delta_v);
            vec3 rayDir = pixelCenter - camera_center;
            ray r = ray(camera_center,rayDir);

            color pixel_color = ray_color(r,world);
            
            // color pixelColor = ray_color(r);
            write_color(std::cout, pixel_color);
        }
    }

    std::clog << "\rDone.                 \n";
}
