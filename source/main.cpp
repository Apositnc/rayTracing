#include<iostream>

#include "color.h"
#include "vec3.h"
#include "ray.h"

double hit_sphere(const point3& center,double radius,const ray& r){
    vec3 oc = center - r.origin();
    double a = dot(r.Dir(),r.Dir());
    //double b = -2.0*dot(r.Dir(),oc);
    double h = dot(r.Dir(),oc);
    double c = dot(oc,oc) -radius*radius;
    double d = 4*h*h-4*a*c;

    if (d < 0) {
        return -1.0;
    } else {
        return (h - std::sqrt(d) ) / a;//t是摄像机离与圆的交点有多远
    }
}

color ray_color(const ray& r){


    double t = hit_sphere(point3(0,0,-1),0.5,r);
    //相交才的球才有颜色
    if(t>0.0){
        vec3 n = unit_vector(r.at(t)-vec3(0,0,-1));//rat算出交点坐标 然后是与球心坐标算出法线
        //n 是一个在长度在球半径内的向量 画的颜色和设置的半径有关系
        return 0.5*color(n.x()+1,n.y()+1,n.z()+1);
    }

    //天空背景
    vec3 unitDir = unit_vector(r.Dir());
    //越往上 y 越大 蓝色越多
    //设置的天空盒是这样
    //unitDir刚刚归一化了 只看y来确定蓝色
    //加一之后再次归一化 a决定蓝色多少 1-a决定白色多少
    //白色 W = (1.0, 1.0, 1.0)
    //蓝色 B = (0.5, 0.7, 1.0)
    double a = 0.5*(unitDir.y()+1);
    //return (1-a)*color(1.0,1.0,1.0)+a*color(0.5, 0.7, 1.0);//下蓝上白
    return a*color(1.0,1.0,1.0)+(1-a)*color(0.5, 0.7, 1.0);
}


int main(){

    // int w=256;
    // int h=256;
    // std::cout<<"P3\n"<<w<<' '<<h<<"\n255\n";
    // for(int i = 0;i<h;i++){
    //     std::clog<<"\rscan remaing: " << ' ' <<h-i<<std::flush;//进度条
    //     for(int j=0;j<w;j++){
    //         auto pix_color = color(double(j)/(w-1),double(i)/(h-1),0.0);
    //         write_color(std::cout,pix_color);
    //     }
    // }
    // std::clog << "\rDone.                 \n";

    // 图片
    double aspect_ratio = 16.0/9.0;
    int image_w = 400;
    int image_h = int(image_w/aspect_ratio);
    if(image_h<1)
        image_h = 1;
    //相机
    point3 camera_center = point3(0,0,0);
    double focal_length = 1.0;//板子离相机垂直距离
    double viewport_h = 2;
    double viewport_w = viewport_h*(double(image_w)/image_h);
    //沿着板子宽高走的两个向量 长度和方向
    vec3 viewport_u = vec3(viewport_w,0,0);
    vec3 viewport_v = vec3(0,viewport_h,0);
    //用像素算 在视口上面一步走多长距离
    vec3 pixel_delta_u = viewport_u / image_w;
    vec3 pixel_delta_v = viewport_v / image_h;

    //左上角
    point3 viewportLeft = camera_center - vec3(0,0,focal_length)
                            -viewport_u/2 - viewport_v/2;
    point3 pix100Loc = viewportLeft+0.5*(pixel_delta_u+pixel_delta_v);
    
    std::cout << "P3\n" << image_w << " " << image_h << "\n255\n";

    for (int j = 0; j < image_h; j++) {
        std::clog << "\rScanlines remaining: " << (image_h - j) << ' ' << std::flush;
        for (int i = 0; i < image_w; i++) {
            // 处理这个格子。
            point3 pixelCenter = pix100Loc+(i*pixel_delta_u+j*pixel_delta_v);
            vec3 rayDir = pixelCenter - camera_center;
            ray r = ray(camera_center,rayDir);

            color pixelColor = ray_color(r);
            write_color(std::cout, pixelColor);
        }
    }

    std::clog << "\rDone.                 \n";
}
