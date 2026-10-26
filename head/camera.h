#pragma once

#include "hittable.h"
#include "material.h" // 调用 scatter() 需要材质类的完整定义。
#include "vec3.h"


class camera{
private:
    int image_h;
    // 设置相机坐标和视口的大小
    point3 camera_center;
    double focal_length;//板子离相机垂直距离

    vec3 pixel_delta_u;
    vec3 pixel_delta_v;
    point3 pix100Loc;
    
    double pixel_samples_scale;  // 像素样本总和的颜色缩放因子

    vec3 u,v,w;

    void initialize(){
        image_h = int(image_w/aspect_ratio);
        if(image_h<1)
            image_h = 1;
        camera_center = lookfrom;
        focal_length = (lookfrom-lookat).length();//板子离相机垂直距离

        w = unit_vector(lookfrom - lookat);
        u = unit_vector(cross(vup,w));
        v = cross(w,u);

        double theta = degrees_to_radians(vfov);
        double viewport_h = 2 * std::tan(theta/2) * focal_length;
        double viewport_w = viewport_h*(double(image_w)/image_h);

        //沿着板子宽高走的两个向量 长度和方向
        vec3 viewport_u = viewport_w*u;
        vec3 viewport_v = viewport_h* -v;//图像行号向下增加，对应世界坐标 y 减小

        //用之前确定的像素算 在视口上面一步走多长距离
        pixel_delta_u = viewport_u / image_w;
        pixel_delta_v = viewport_v / image_h;

        //视口左上角坐标 也就是最开始的第一个格子的左上角
        // point3 viewportLeft = camera_center - vec3(0,0,focal_length)
        //                     -viewport_u/2 - viewport_v/2;
        point3 viewportLeft = camera_center - (focal_length * w) - viewport_u/2-viewport_v/2; 
        //第一个格子加上半个格子得到格子中心
        pix100Loc = viewportLeft+0.5*(pixel_delta_u+pixel_delta_v);

        pixel_samples_scale = 1.0 / samples_per_pixel;
    }

    vec3 sample_square() const {
        // 返回单位正方形 [-.5, -.5]-[+.5, +.5] 内的随机点向量
        return vec3(random_double() - 0.5, random_double() - 0.5, 0);
    }

    ray get_ray(int i,int j)const{//当前像素是第几行第几列
        vec3 offset = sample_square();//offset这里表示这一格内偏移了多少
        point3 pixel_sample = pix100Loc//pixel_delta_u是像素大小 但是这里offset小于1 保证不会超出去
                            +((i+offset.x())*pixel_delta_u)
                            +((j+offset.y())*pixel_delta_v);
        point3 ray_origin = camera_center;
        vec3 ray_direction = pixel_sample - ray_origin;
        return ray(ray_origin,ray_direction);
    }

    //返回当前光线与世界交互后的最终颜色
    color ray_color(const ray& r,int depth,const hittable& world){
        if(depth<=0)
            return color(0,0,0);

        hit_record rec;

        if(world.hit(r,interval(0.001,infinity),rec)){//如果进入内层循环则证明已经发生了碰撞 rec已经记录上数据
            //vec3 direction = random_on_hemisphere(rec.normal);
            //vec3 direction = rec.normal + random_unit_vector();
            //return 0.5*ray_color(ray(rec.p,direction),depth-1,world);

            //初始化 颜色 和 反射之后的光线
            ray scattered;
            color attenuation;
            
            if(rec.mat->scatter(r,rec,attenuation,scattered)){
                return attenuation * ray_color(scattered,depth-1,world);
            }
            return color(0,0,0);
        }


        vec3 unit_direction = unit_vector(r.Dir());
        auto a = 0.5 * (unit_direction.y() + 1.0);
        //return a*color(1.0,1.0,1.0) + (1.0-a)*color(0.5,0.7,1.0);
        return (1.0-a)*color(1,1,1) + a*color(0.5,0.7,1);
    }


public:
    double aspect_ratio = 16.0/9.0;
    int image_w = 400;
    int samples_per_pixel = 10;   // 每个像素的随机采样数
    int max_depth = 10;   // 射线在场景中弹射次数上限


    double vfov = 90;
    point3 lookfrom = point3(0,0,0);
    point3 lookat = point3(0,0,-1);
    vec3 vup = vec3(0,1,0);


    void render(const hittable& world){
        initialize();
        // 设置图片属性
        std::cout << "P3\n" << image_w << " " << image_h << "\n255\n";
        
        for (int j = 0; j < image_h; j++) {
            std::clog << "\rScanlines remaining: " << (image_h - j) << ' ' << std::flush;
            for (int i = 0; i < image_w; i++) {

                // 处理这个格子。
                color pixel_color(0,0,0);
                for (int sample = 0; sample < samples_per_pixel; sample++) {
                    ray r = get_ray(i, j);
                    pixel_color += ray_color(r, max_depth, world);
                }
                write_color(std::cout, pixel_samples_scale * pixel_color);
            }
        }
        std::clog << "\rDone.                 \n";

    }
};
