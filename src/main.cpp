#include <sil/sil.hpp>
#include <complex>
#include "random.hpp"
#include <iostream>
#include <glm/gtx/matrix_transform_2d.hpp>
#include <vector>

glm::vec2 rotated(glm::vec2 point, glm::vec2 center_of_rotation, float angle)
{
    return glm::vec2{glm::rotate(glm::mat3{1.f}, angle) * glm::vec3{point - center_of_rotation, 0.f}} + center_of_rotation;
}

void only_green(sil::Image & img){
    for (int x {0}; x < img.width(); x++){
        for (int y {0}; y < img.height(); y++){
            img.pixel(x,y).r = 0.f;
            img.pixel(x,y).b = 0.f;
        }
    }
}

void swap_blue_red(sil::Image & img){
    float temp{0.f};
    for (glm::vec3& color : img.pixels())
    {   
        temp = color.b;
        color.b = color.r;
        color.r = temp;
    }
}

void black_and_white(sil::Image & img){
    float grey_scale {0.f};
    for (glm::vec3& color : img.pixels())
    {   
       grey_scale = color.r * 0.299f + color.g * 0.587f + color.b * 0.114f;
       color.r = grey_scale;
       color.g = grey_scale;
       color.b = grey_scale;
    }
}

void negative(sil::Image & img){
    for (glm::vec3& color : img.pixels())
    {   
       color.r = 1.f - color.r;
       color.g = 1.f - color.g;
       color.b = 1.f - color.b;
    }
}

void degrade(sil::Image & img){
    for (int x {0}; x < img.width(); x++){
        for (int y {0}; y < img.height(); y++){
            img.pixel(x,y).r =  (x*1.f)/(img.width() - 1);
            img.pixel(x,y).g =  (x*1.f)/(img.width() - 1);
            img.pixel(x,y).b =  (x*1.f)/(img.width() - 1);
        }
    }
}

void mirror(sil::Image & img){
    float temp_r {0.f};
    float temp_g {0.f};
    float temp_b {0.f};
    int half {img.width()/2};
    for (int x {0}; x < half; x++){
        for (int y {0}; y < img.height(); y++){
            temp_r = img.pixel(x,y).r;
            temp_g = img.pixel(x,y).g;
            temp_b = img.pixel(x,y).b;
            img.pixel(x,y).r = img.pixel((img.width()-1-x),y).r;
            img.pixel(x,y).g = img.pixel((img.width()-1-x),y).g;
            img.pixel(x,y).b = img.pixel((img.width()-1-x),y).b;
            img.pixel((img.width()-1-x),y).r = temp_r;
            img.pixel((img.width()-1-x),y).g = temp_g;
            img.pixel((img.width()-1-x),y).b = temp_b; 
        }
    }
}

void noise(sil::Image & img){
    for (int x {0}; x < img.width(); x++){
        for ( int y {0}; y < img.height(); y++){
            if (random_int(1,6) == 5){
                img.pixel(x,y).r = random_float(0.f,1.f);
                img.pixel(x,y).g = random_float(0.f,1.f);
                img.pixel(x,y).b = random_float(0.f,1.f);
            }
        }
    }
}

void rotate_90_toward_left(sil::Image & img, sil::Image const & img_const){
    for (int x {0}; x < img.width(); x++){
        for (int y {0}; y < img.height(); y++){
            img.pixel(x,y).r = img_const.pixel(y,x).r;
            img.pixel(x,y).g = img_const.pixel(y,x).g;
            img.pixel(x,y).b = img_const.pixel(y,x).b;
        }
    }
    mirror(img);
}

void split_rgb(sil::Image & img, sil::Image const & img_const){
    for (int x {0}; x < img.width(); x++){
        for (int y {0}; y < img.height(); y++){
            if ( x <= 25){
                img.pixel(x,y).r = 0.f;
                img.pixel(x,y).g = img_const.pixel(x,y).g;
                img.pixel(x,y).b = img_const.pixel(x+25,y).b;  
            }else if (x >= img.width()-25){
                img.pixel(x,y).r = img_const.pixel(x-25,y).r;
                img.pixel(x,y).g = img_const.pixel(x,y).g;
                img.pixel(x,y).b = 0.f;
            } else {
                img.pixel(x,y).r = img_const.pixel(x-25,y).r;
                img.pixel(x,y).g = img_const.pixel(x,y).g;
                img.pixel(x,y).b = img_const.pixel(x+25,y).b;
            }
        }
    }
}

void lighter_photo(sil::Image & img){
    for (int x {0}; x < img.width(); x++){
        for (int y {0}; y < img.height(); y++){
            img.pixel(x,y).r = sqrt(img.pixel(x,y).r);
            img.pixel(x,y).g = sqrt(img.pixel(x,y).g);
            img.pixel(x,y).b = sqrt(img.pixel(x,y).b);
        }
    }
}

void darker_photo(sil::Image & img){
    for (int x {0}; x < img.width(); x++){
        for (int y {0}; y < img.height(); y++){
            img.pixel(x,y).r = pow(img.pixel(x,y).r,2);
            img.pixel(x,y).g = pow(img.pixel(x,y).g,2);
            img.pixel(x,y).b = pow(img.pixel(x,y).b,2);
        }
    }
}

void draw_disc(sil::Image & img, int radius){
    int center_x {img.width()/2};
    int center_y {img.height()/2};
    for (int x {0}; x < img.width(); x++){
        for (int y {0}; y < img.height(); y++){
            double length { sqrt(pow(center_x-x,2) + pow(center_y-y,2))};
            if (length < radius){
                img.pixel(x,y).r = 1.f;
                img.pixel(x,y).g = 1.f;
                img.pixel(x,y).b = 1.f;
            }
        }
    }
}

void draw_circle(sil::Image & img,int radius,int thickness){
    int center_x {img.width()/2};
    int center_y {img.height()/2};
    draw_disc(img,radius);
    for (int x {0}; x < img.width(); x++){
        for (int y {0}; y < img.height(); y++){
            double length { sqrt(pow(center_x-x,2) + pow(center_y-y,2))};
            if (length < radius-thickness){
                img.pixel(x,y).r = 0.f;
                img.pixel(x,y).g = 0.f;
                img.pixel(x,y).b = 0.f;
            }
        }
    }
    
}

void shift_pixel_to_the_left(sil::Image & img, sil::Image const & model, int space){
    for (int x {0}; x < img.width(); x++){
        for (int y {0}; y < img.height(); y++){
            if ( x < img.width()-1-space){
                img.pixel(x,y).r = model.pixel(x+space,y).r;
                img.pixel(x,y).g = model.pixel(x+space,y).g;
                img.pixel(x,y).b = model.pixel(x+space,y).b;
            } else {
                img.pixel(x,y).r = model.pixel(space - (img.width()-1-x),y).r;
                img.pixel(x,y).g = model.pixel(space - (img.width()-1-x),y).g;
                img.pixel(x,y).b = model.pixel(space - (img.width()-1-x),y).b;
            }
        }
    }
}

void shift_pixel_to_the_right(sil::Image & img, sil::Image const & model, int space){
    for (int x {0}; x < img.width(); x++){
        for (int y {0}; y < img.height(); y++){
            if ( x > space){
                img.pixel(x,y).r = model.pixel(x-space,y).r;
                img.pixel(x,y).g = model.pixel(x-space,y).g;
                img.pixel(x,y).b = model.pixel(x-space,y).b;
            } else {
                img.pixel(x,y).r = model.pixel(img.width()-1-space + x,y).r;
                img.pixel(x,y).g = model.pixel(img.width()-1-space + x,y).g;
                img.pixel(x,y).b = model.pixel(img.width()-1-space + x,y).b;
            }
        }
    }
}

void draw_circle_select_center(sil::Image & img,int radius,int thickness, int center_x, int center_y){
    for (int x {0}; x < img.width(); x++){
        for (int y {0}; y < img.height(); y++){
            double length { sqrt(pow(center_x-x,2) + pow(center_y-y,2))};
            if (length < radius && length >= radius - thickness){
                img.pixel(x,y).r = 1.f;
                img.pixel(x,y).g = 1.f;
                img.pixel(x,y).b = 1.f;
            }
        }
    }
    
}

void draw_rosace(sil::Image & img,int radius,int thickness){
    draw_circle(img,radius,thickness);
    int center_x {img.width()/2};
    int center_y {img.height()/2};
    float approx_pi {3.1415f};
    int length { center_x-radius};
    draw_circle_select_center(img,radius,thickness,center_x-radius+5,center_y); // 0
    draw_circle_select_center(img,radius,thickness,center_x+radius-5,center_y); // pi
    
    draw_circle_select_center(img,radius,thickness, center_x + length * cos(approx_pi/3) -10 ,center_y + length * sin(approx_pi/3)-10); // pi/3
    draw_circle_select_center(img,radius,thickness, center_x + length * cos(2*approx_pi/3) +10 ,center_y + length * sin(2*approx_pi/3)-10); //2pi/3
    
    draw_circle_select_center(img,radius,thickness, center_x + length * cos(approx_pi/(-3)) -10 ,center_y + length * sin(approx_pi/(-3))+10);; // -pi/3
    draw_circle_select_center(img,radius,thickness, center_x + length * cos((-2)*approx_pi/3) +10 ,center_y + length * sin((-2)*approx_pi/3)+10); // -2pi/3    
}

void do_mosaic(sil::Image & img, sil::Image const & model){
    for (int x {0}; x < img.width(); x++){
        for (int y {0}; y < img.height(); y++){
            img.pixel(x,y).r = model.pixel((x*5)%300,(y*5)%345).r;
            img.pixel(x,y).g = model.pixel((x*5)%300,(y*5)%345).g;
            img.pixel(x,y).b = model.pixel((x*5)%300,(y*5)%345).b;  
        }
    }
}

void reflet(sil::Image & img, sil::Image const & model){
    for (int x {0}; x < img.width(); x++){
        for (int y {0}; y < img.height(); y++){
            
            img.pixel(x,y).r = model.pixel(x,img.height()-1-y).r;
            img.pixel(x,y).g = model.pixel(x,img.height()-1-y).g;
            img.pixel(x,y).b = model.pixel(x,img.height()-1-y).b;
             
        }
    }
}

void do_mirror_mosaic(sil::Image & img, sil::Image const & model, sil::Image const & model_mirror, sil::Image const & model_reflect, sil::Image const & model_reflect_mirror){
    for (int x {0}; x < img.width(); x++){
        for (int y {0}; y < img.height(); y++){
            if (x < 60 || (x >= 120 && x < 180) || x >= 240){
                if(y < 69 || (y >= 138 && y < 207) || (y >= 276) ){
                    img.pixel(x,y).r = model.pixel((x*5)%300,(y*5)%345).r;
                    img.pixel(x,y).g = model.pixel((x*5)%300,(y*5)%345).g;
                    img.pixel(x,y).b = model.pixel((x*5)%300,(y*5)%345).b; 
                } else if ( (y >= 69 && y < 138) || (y >= 207 && y < 276) ){
                    img.pixel(x,y).r = model_reflect.pixel((x*5)%300,(y*5)%345).r;
                    img.pixel(x,y).g = model_reflect.pixel((x*5)%300,(y*5)%345).g;
                    img.pixel(x,y).b = model_reflect.pixel((x*5)%300,(y*5)%345).b;
                }
            } else if ((x >= 60 && x < 120)|| (x >= 180 && x < 240)){
                if(y < 69 || (y >= 138 && y < 207) || (y >= 276) ){
                    img.pixel(x,y).r = model_mirror.pixel((x*5)%300,(y*5)%345).r;
                    img.pixel(x,y).g = model_mirror.pixel((x*5)%300,(y*5)%345).g;
                    img.pixel(x,y).b = model_mirror.pixel((x*5)%300,(y*5)%345).b; 
                } else if ( (y >= 69 && y < 138) || (y >= 207 && y < 276) ){
                    img.pixel(x,y).r = model_reflect_mirror.pixel((x*5)%300,(y*5)%345).r;
                    img.pixel(x,y).g = model_reflect_mirror.pixel((x*5)%300,(y*5)%345).g;
                    img.pixel(x,y).b = model_reflect_mirror.pixel((x*5)%300,(y*5)%345).b;
                }
            }
        }
    }
}

void add_glitch(sil::Image & img, sil::Image const & model){
    int number_of_glitch {random_int(30,50)};
    for (int i {0}; i < number_of_glitch; i++){
        int x_length {random_int(15,40)};
        int y_length {random_int(5,20)};
        int cible_1_x {random_int(0,img.width()-41)};
        int cible_1_y {random_int(0,img.height()-21)};
        int cible_2_x {random_int(0,img.width()-41)};
        int cible_2_y {random_int(0,img.height()-21)};
        for (int x {0}; x < x_length; x++){
            for (int y {0}; y < y_length; y++){
                img.pixel(cible_1_x+x,cible_1_y+y).r = model.pixel(cible_2_x+x,cible_2_y+y).r;
                img.pixel(cible_1_x+x,cible_1_y+y).g = model.pixel(cible_2_x+x,cible_2_y+y).g;
                img.pixel(cible_1_x+x,cible_1_y+y).b = model.pixel(cible_2_x+x,cible_2_y+y).b;
                img.pixel(cible_2_x+x,cible_2_y+y).r = model.pixel(cible_1_x+x,cible_1_y+y).r;
                img.pixel(cible_2_x+x,cible_2_y+y).g = model.pixel(cible_1_x+x,cible_1_y+y).g;
                img.pixel(cible_2_x+x,cible_2_y+y).b = model.pixel(cible_1_x+x,cible_1_y+y).b;
            }
        }
    }
}

float brightness(glm::vec3 const & color){
    return color.r * 0.299f + color.g * 0.587f + color.b * 0.114f;
}

void sort_pixels(sil::Image & img){
    float temp {0.f};
    int number_of_blocks {random_int(5,10)};
    int length_of_block {img.width()/number_of_blocks};
    for (int y {0}; y < img.height(); y++){
        if (y%3 == 0){
            number_of_blocks = random_int(5,10);
            length_of_block = img.width()/number_of_blocks;
        }
        for (int j {0}; j < number_of_blocks;j++){
            for (int i {j*length_of_block}; i < (j+1)*length_of_block; i++){
                int coord_x {i}; 
                float brightest {brightness(img.pixel(i,y))};
                for (int x {i+1}; x < (j+1)*length_of_block;x++){
                    if ( brightest < brightness(img.pixel(x,y))){
                        brightest = brightness(img.pixel(x,y));
                        coord_x = x;
                    }
                }

                if (coord_x != i){
                    temp = img.pixel(coord_x,y).r;
                    img.pixel(coord_x,y).r = img.pixel(i,y).r;
                    img.pixel(i,y).r = temp;
                    temp = img.pixel(coord_x,y).g;
                    img.pixel(coord_x,y).g = img.pixel(i,y).g;
                    img.pixel(i,y).g = temp;
                    temp = img.pixel(coord_x,y).b;
                    img.pixel(coord_x,y).b = img.pixel(i,y).b;
                    img.pixel(i,y).b = temp;
                } 
            }
        }
    }
}

void degrade_couleur(sil::Image & img){
    for (int x {0}; x < img.width(); x++){
        for (int y {0}; y < img.height(); y++){
             img.pixel(x,y) = glm::mix(glm::vec3{1.f, 1.f, 0.5f}, glm::vec3{0.f,0.f,0.3f}, (x*1.f)/(img.width() - 1));
            // img.pixel(x,y).r =  ((img.width() - 1 - x)*1.f)/(img.width() - 1);
            // img.pixel(x,y).g =  0.f;
            // img.pixel(x,y).b =  (x*1.f)/(img.width() - 1);
        }
    }
}

struct Lab {float L; float a; float b;};
struct RGB {float r; float g; float b;};

Lab linear_srgb_to_oklab(RGB c) 
{
    float l = 0.4122214708f * c.r + 0.5363325363f * c.g + 0.0514459929f * c.b;
	float m = 0.2119034982f * c.r + 0.6806995451f * c.g + 0.1073969566f * c.b;
	float s = 0.0883024619f * c.r + 0.2817188376f * c.g + 0.6299787005f * c.b;

    float l_ = cbrtf(l);
    float m_ = cbrtf(m);
    float s_ = cbrtf(s);

    return {
        0.2104542553f*l_ + 0.7936177850f*m_ - 0.0040720468f*s_,
        1.9779984951f*l_ - 2.4285922050f*m_ + 0.4505937099f*s_,
        0.0259040371f*l_ + 0.7827717662f*m_ - 0.8086757660f*s_,
    };
}

RGB oklab_to_linear_srgb(Lab c) 
{
    float l_ = c.L + 0.3963377774f * c.a + 0.2158037573f * c.b;
    float m_ = c.L - 0.1055613458f * c.a - 0.0638541728f * c.b;
    float s_ = c.L - 0.0894841775f * c.a - 1.2914855480f * c.b;

    float l = l_*l_*l_;
    float m = m_*m_*m_;
    float s = s_*s_*s_;

    return {
		+4.0767416621f * l - 3.3077115913f * m + 0.2309699292f * s,
		-1.2684380046f * l + 2.6097574011f * m - 0.3413193965f * s,
		-0.0041960863f * l - 0.7034186147f * m + 1.7076147010f * s,
    };
}

void degrade_couleur_oklab(sil::Image & img){
        for (int x {0}; x < img.width(); x++){
        for (int y {0}; y < img.height(); y++){
            //img.pixel(x,y) = static_cast<glm::vec3>( oklab_to_linear_srgb(glm::mix(linear_srgb_to_oklab(glm::vec3{1.f, 1.f, 0.5f}), linear_srgb_to_oklab(glm::vec3{0.f,0.f,0.3f}), (x*1.f)/(img.width() - 1))));
            // img.pixel(x,y).r =  ((img.width() - 1 - x)*1.f)/(img.width() - 1);
            // img.pixel(x,y).g =  0.f;
            // img.pixel(x,y).b =  (x*1.f)/(img.width() - 1);
        }
    }
    // for (int x {0}; x < img.width(); x++){
    //     for (int y {0}; y < img.height(); y++){
    //         RGB color {img.pixel(x,y).r,img.pixel(x,y).g,img.pixel(x,y).b};
    //         Lab ok_color {linear_srgb_to_oklab(color)};
    //         color = oklab_to_linear_srgb(ok_color);
    //         img.pixel(x,y).r =  color.r;
    //         img.pixel(x,y).g =  color.g;
    //         img.pixel(x,y).b =  color.b;
    //     }
    // }
}

void draw_mandelbrot(sil::Image & img){
    int ratio {0};
    for (int x {0}; x < img.width(); x++){
        for (int y {0}; y < img.height(); y++){
            std::complex<float> c {static_cast<float>(x-250)/125.f,static_cast<float>(y-250)/125.f};
            std::complex<float> z {0.f,0.f};
            img.pixel(x,y).r = 1.f;
            img.pixel(x,y).g = 1.f;
            img.pixel(x,y).b = 1.f;
            for (int i {0}; i < 100;i++){
                if (std::abs(z) > 2){
                    img.pixel(x,y).r = 0.05f*i;
                    img.pixel(x,y).g = 0.05f*i;
                    img.pixel(x,y).b = 0.05f*i;
                    break;
                } else {
                    z = z * z + c;
                }
            }
            
        }
    }
}

void white_noise(sil::Image & img){
    for (int x {0}; x < img.width(); x++){
        for ( int y {0}; y < img.height(); y++){
            float random_grey {random_float(0.3f,0.8f)};
            if (random_int(1,3) == 3){
                img.pixel(x,y).r = random_grey;
                img.pixel(x,y).g = random_grey;
                img.pixel(x,y).b = random_grey;
            }
        }
    }
}

void tramage(sil::Image & img){
    const int bayer_n = 4;
    float bayer_matrix_4x4[][bayer_n] = {
        {    -0.5,       0,  -0.375,   0.125 },
        {    0.25,   -0.25,   0.375, - 0.125 },
        { -0.3125,  0.1875, -0.4375,  0.0625 },
        {  0.4375, -0.0625,  0.3125, -0.1875 },
    };

    for (int x {0}; x < img.width(); x++){
        for (int y {0}; y < img.height(); y++){
            img.pixel(x,y).r += 1.f*bayer_matrix_4x4[x%bayer_n][y%bayer_n];
            img.pixel(x,y).g += 1.f*bayer_matrix_4x4[x%bayer_n][y%bayer_n];
            img.pixel(x,y).b += 1.f*bayer_matrix_4x4[x%bayer_n][y%bayer_n];

            if (img.pixel(x,y).r < 0.5f){
                img.pixel(x,y).r = 0.f;
                img.pixel(x,y).g = 0.f;
                img.pixel(x,y).b = 0.f;
            } else { 
                // pour eclaircir 
                img.pixel(x,y).r += 0.8f;
                img.pixel(x,y).g += 0.8f;
                img.pixel(x,y).b += 0.8f;
            }
        }

    }
}

float normalisation (float val,float val_max, float val_min, float new_val_max, float new_val_min){
    float value {(val-val_min) * (new_val_max-new_val_min) / (val_max-val_min) - new_val_min};
    return value;
}

void normalisation_histogramme(sil::Image & img){
    int brightest_x {0};
    int brightest_y {0};
    int darkest_x {0};
    int darkest_y {0};
    for (int x {0}; x < img.width(); x++){
        for (int y {0}; y < img.height(); y++){
            if (brightness(img.pixel(x,y)) > brightness(img.pixel(brightest_x,brightest_y))){
                brightest_x = x;
                brightest_y = y;
            } 
            if (brightness(img.pixel(x,y)) < brightness(img.pixel(darkest_x,darkest_y))){
                darkest_x = x;
                darkest_y = y;
            } 
        }
    }
    float brightest_r {img.pixel(brightest_x,brightest_y).r};
    float brightest_g {img.pixel(brightest_x,brightest_y).g};
    float brightest_b {img.pixel(brightest_x,brightest_y).b};
    float darkest_r {img.pixel(darkest_x,darkest_y).r};
    float darkest_g {img.pixel(darkest_x,darkest_y).g};
    float darkest_b {img.pixel(darkest_x,darkest_y).b};

    for (int x {0}; x < img.width(); x++){
        for (int y {0}; y < img.height(); y++){ 
            img.pixel(x,y).r = normalisation(img.pixel(x,y).r,brightest_r,darkest_r,1.f,0.f); 
            img.pixel(x,y).g = normalisation(img.pixel(x,y).g,brightest_g,darkest_g,1.f,0.f); 
            img.pixel(x,y).b = normalisation(img.pixel(x,y).b,brightest_b,darkest_b,1.f,0.f);  
        }
    }

}

void ygo_fusion_vortex(sil::Image & img, sil::Image const & model, float custom_spin){
    int center_x {img.width()/2};
    int center_y {img.height()/2};
    float approx_pi {3.1415f};
    glm::vec2 center_of_rotation {center_x,center_y};
    float max_distance {glm::distance(center_of_rotation, {0.f,0.f})};
    for (int x {0}; x < img.width(); x++){
        for (int y {0}; y < img.height(); y++){
            glm::vec2 point {x,y};
            float distance {glm::distance(center_of_rotation,point)};
            glm::vec2 rotated_point {rotated(point,center_of_rotation,distance*custom_spin*approx_pi/max_distance)};
            if (!(rotated_point.x < 0 || rotated_point.x >= img.width() || rotated_point.y < 0 || rotated_point.y >= img.height())){
                img.pixel(rotated_point.x,rotated_point.y).r = model.pixel(x,y).r;
                img.pixel(rotated_point.x,rotated_point.y).g = model.pixel(x,y).g;
                img.pixel(rotated_point.x,rotated_point.y).b = model.pixel(x,y).b;
            }
        }
    }
}

void customisable_convolution_3_by_3_kernel (sil::Image & img , sil::Image const & model, float const matrix[3][3]){
    for (int x {1}; x < img.width()-1; x++ ){
        for (int y {1}; y < img.height()-1;y++){
            img.pixel(x,y).r = 
            model.pixel(x-1,y+1).r * matrix[0][0] + model.pixel(x,y+1).r * matrix[0][1] + model.pixel(x+1,y+1).r * matrix[0][2] +
            model.pixel(x-1,y).r * matrix[1][0] + model.pixel(x,y).r * matrix[1][1] + model.pixel(x+1,y).r * matrix[1][2] +
            model.pixel(x-1,y-1).r * matrix[2][0] + model.pixel(x,y-1).r * matrix[2][1] + model.pixel(x+1,y-1).r * matrix[2][2];
            
            img.pixel(x,y).g = 
            model.pixel(x-1,y+1).g * matrix[0][0] + model.pixel(x,y+1).g * matrix[0][1] + model.pixel(x+1,y+1).g * matrix[0][2] +
            model.pixel(x-1,y).g * matrix[1][0] + model.pixel(x,y).g * matrix[1][1] + model.pixel(x+1,y).g * matrix[1][2] +
            model.pixel(x-1,y-1).g * matrix[2][0] + model.pixel(x,y-1).g * matrix[2][1] + model.pixel(x+1,y-1).g * matrix[2][2];
            
            img.pixel(x,y).b = 
            model.pixel(x-1,y+1).b * matrix[0][0] + model.pixel(x,y+1).b * matrix[0][1] + model.pixel(x+1,y+1).b * matrix[0][2] +
            model.pixel(x-1,y).b * matrix[1][0] + model.pixel(x,y).b * matrix[1][1] + model.pixel(x+1,y).b * matrix[1][2] +
            model.pixel(x-1,y-1).b * matrix[2][0] + model.pixel(x,y-1).b * matrix[2][1] + model.pixel(x+1,y-1).b * matrix[2][2];
        }
    }
}

void separate_convolution_100_by_100(sil::Image & img, sil::Image & intermediate, sil::Image const & model){
    float total_r {0.f};
    float total_g {0.f};
    float total_b {0.f};
    
    for (int x {0}; x < img.width(); x++){
        for (int y {0}; y < img.height();y++){
            if (x < 49){
                for (int z {0}; z < x+51; z++){
                    total_r += model.pixel(z,y).r;
                    total_g += model.pixel(z,y).g;
                    total_b += model.pixel(z,y).b;
                }
                intermediate.pixel(x,y).r = total_r/(x+50.f);
                intermediate.pixel(x,y).g = total_g/(x+50.f);
                intermediate.pixel(x,y).b = total_b/(x+50.f);
            } else if (x >= img.width()-51){
                for (int z {x-49}; z < img.width(); z++){
                    total_r += model.pixel(z,y).r;
                    total_g += model.pixel(z,y).g;
                    total_b += model.pixel(z,y).b;
                }
                intermediate.pixel(x,y).r = total_r/(img.width()-x+50.f);
                intermediate.pixel(x,y).g = total_g/(img.width()-x+50.f);
                intermediate.pixel(x,y).b = total_b/(img.width()-x+50.f);
            } else {
                for(int z {x-49}; z < x+51; z++){
                    total_r += model.pixel(z,y).r;
                    total_g += model.pixel(z,y).g;
                    total_b += model.pixel(z,y).b;
                } 
                intermediate.pixel(x,y).r = total_r/100.f;
                intermediate.pixel(x,y).g = total_g/100.f;
                intermediate.pixel(x,y).b = total_b/100.f;
            }
            total_r = 0.f;
            total_g = 0.f;
            total_b = 0.f; 
        }
    }

    for (int x {0}; x < img.width(); x++){
        for (int y {0}; y < img.height();y++){
            if (y < 49){
                for (int z {0}; z < y+51; z++){
                    total_r += intermediate.pixel(x,z).r;
                    total_g += intermediate.pixel(x,z).g;
                    total_b += intermediate.pixel(x,z).b;
                }
                img.pixel(x,y).r = total_r/(y+50.f);
                img.pixel(x,y).g = total_g/(y+50.f);
                img.pixel(x,y).b = total_b/(y+50.f);
            } else if (y >= img.height()-51){
                for (int z {y-49}; z < img.height(); z++){
                    total_r += intermediate.pixel(x,z).r;
                    total_g += intermediate.pixel(x,z).g;
                    total_b += intermediate.pixel(x,z).b;
                }
                img.pixel(x,y).r = total_r/(img.height()-y+50.f);
                img.pixel(x,y).g = total_g/(img.height()-y+50.f);
                img.pixel(x,y).b = total_b/(img.height()-y+50.f);
            } else {
                for(int z {y-49}; z < y+51; z++){
                    total_r += intermediate.pixel(x,z).r;
                    total_g += intermediate.pixel(x,z).g;
                    total_b += intermediate.pixel(x,z).b;
                } 
                img.pixel(x,y).r = total_r/100.f;
                img.pixel(x,y).g = total_g/100.f;
                img.pixel(x,y).b = total_b/100.f;
            }
            total_r = 0.f;
            total_g = 0.f;
            total_b = 0.f; 
        }
    }
    
}

void separate_convolution_50_by_50(sil::Image & img, sil::Image & intermediate, sil::Image const & model){
    float total_r {0.f};
    float total_g {0.f};
    float total_b {0.f};
    
    for (int x {0}; x < img.width(); x++){
        for (int y {0}; y < img.height();y++){
            if (x < 24){
                for (int z {0}; z < x+26; z++){
                    total_r += model.pixel(z,y).r;
                    total_g += model.pixel(z,y).g;
                    total_b += model.pixel(z,y).b;
                }
                intermediate.pixel(x,y).r = total_r/(x+25.f);
                intermediate.pixel(x,y).g = total_g/(x+25.f);
                intermediate.pixel(x,y).b = total_b/(x+25.f);
            } else if (x >= img.width()-26){
                for (int z {x-24}; z < img.width(); z++){
                    total_r += model.pixel(z,y).r;
                    total_g += model.pixel(z,y).g;
                    total_b += model.pixel(z,y).b;
                }
                intermediate.pixel(x,y).r = total_r/(img.width()-x+25.f);
                intermediate.pixel(x,y).g = total_g/(img.width()-x+25.f);
                intermediate.pixel(x,y).b = total_b/(img.width()-x+25.f);
            } else {
                for(int z {x-24}; z < x+26; z++){
                    total_r += model.pixel(z,y).r;
                    total_g += model.pixel(z,y).g;
                    total_b += model.pixel(z,y).b;
                } 
                intermediate.pixel(x,y).r = total_r/50.f;
                intermediate.pixel(x,y).g = total_g/50.f;
                intermediate.pixel(x,y).b = total_b/50.f;
            }
            total_r = 0.f;
            total_g = 0.f;
            total_b = 0.f; 
        }
    }

    for (int x {0}; x < img.width(); x++){
        for (int y {0}; y < img.height();y++){
            if (y < 24){
                for (int z {0}; z < y+26; z++){
                    total_r += intermediate.pixel(x,z).r;
                    total_g += intermediate.pixel(x,z).g;
                    total_b += intermediate.pixel(x,z).b;
                }
                img.pixel(x,y).r = total_r/(y+25.f);
                img.pixel(x,y).g = total_g/(y+25.f);
                img.pixel(x,y).b = total_b/(y+25.f);
            } else if (y >= img.height()-26){
                for (int z {y-24}; z < img.height(); z++){
                    total_r += intermediate.pixel(x,z).r;
                    total_g += intermediate.pixel(x,z).g;
                    total_b += intermediate.pixel(x,z).b;
                }
                img.pixel(x,y).r = total_r/(img.height()-y+25.f);
                img.pixel(x,y).g = total_g/(img.height()-y+25.f);
                img.pixel(x,y).b = total_b/(img.height()-y+25.f);
            } else {
                for(int z {y-24}; z < y+26; z++){
                    total_r += intermediate.pixel(x,z).r;
                    total_g += intermediate.pixel(x,z).g;
                    total_b += intermediate.pixel(x,z).b;
                } 
                img.pixel(x,y).r = total_r/50.f;
                img.pixel(x,y).g = total_g/50.f;
                img.pixel(x,y).b = total_b/50.f;
            }
            total_r = 0.f;
            total_g = 0.f;
            total_b = 0.f; 
        }
    }
    
}

void separate_convolution_20_by_20(sil::Image & img, sil::Image & intermediate, sil::Image const & model){
    float total_r {0.f};
    float total_g {0.f};
    float total_b {0.f};
    
    for (int x {0}; x < img.width(); x++){
        for (int y {0}; y < img.height();y++){
            if (x < 9){
                for (int z {0}; z < x+11; z++){
                    total_r += model.pixel(z,y).r;
                    total_g += model.pixel(z,y).g;
                    total_b += model.pixel(z,y).b;
                }
                intermediate.pixel(x,y).r = total_r/(x+10.f);
                intermediate.pixel(x,y).g = total_g/(x+10.f);
                intermediate.pixel(x,y).b = total_b/(x+10.f);
            } else if (x >= img.width()-11){
                for (int z {x-9}; z < img.width(); z++){
                    total_r += model.pixel(z,y).r;
                    total_g += model.pixel(z,y).g;
                    total_b += model.pixel(z,y).b;
                }
                intermediate.pixel(x,y).r = total_r/(img.width()-x+10.f);
                intermediate.pixel(x,y).g = total_g/(img.width()-x+10.f);
                intermediate.pixel(x,y).b = total_b/(img.width()-x+10.f);
            } else {
                for(int z {x-9}; z < x+11; z++){
                    total_r += model.pixel(z,y).r;
                    total_g += model.pixel(z,y).g;
                    total_b += model.pixel(z,y).b;
                } 
                intermediate.pixel(x,y).r = total_r/20.f;
                intermediate.pixel(x,y).g = total_g/20.f;
                intermediate.pixel(x,y).b = total_b/20.f;
            }
            total_r = 0.f;
            total_g = 0.f;
            total_b = 0.f; 
        }
    }
}

void separate_convolution_10_by_10(sil::Image & img, sil::Image & intermediate, sil::Image const & model){
    float total_r {0.f};
    float total_g {0.f};
    float total_b {0.f};
    
    for (int x {0}; x < img.width(); x++){
        for (int y {0}; y < img.height();y++){
            if (x < 4){
                for (int z {0}; z < x+6; z++){
                    total_r += model.pixel(z,y).r;
                    total_g += model.pixel(z,y).g;
                    total_b += model.pixel(z,y).b;
                }
                intermediate.pixel(x,y).r = total_r/(x+5.f);
                intermediate.pixel(x,y).g = total_g/(x+5.f);
                intermediate.pixel(x,y).b = total_b/(x+5.f);
            } else if (x >= img.width()-6){
                for (int z {x-4}; z < img.width(); z++){
                    total_r += model.pixel(z,y).r;
                    total_g += model.pixel(z,y).g;
                    total_b += model.pixel(z,y).b;
                }
                intermediate.pixel(x,y).r = total_r/(img.width()-x+5.f);
                intermediate.pixel(x,y).g = total_g/(img.width()-x+5.f);
                intermediate.pixel(x,y).b = total_b/(img.width()-x+5.f);
            } else {
                for(int z {x-4}; z < x+6; z++){
                    total_r += model.pixel(z,y).r;
                    total_g += model.pixel(z,y).g;
                    total_b += model.pixel(z,y).b;
                } 
                intermediate.pixel(x,y).r = total_r/10.f;
                intermediate.pixel(x,y).g = total_g/10.f;
                intermediate.pixel(x,y).b = total_b/10.f;
            }
            total_r = 0.f;
            total_g = 0.f;
            total_b = 0.f; 
        }
    }

    for (int x {0}; x < img.width(); x++){
        for (int y {0}; y < img.height();y++){
            if (y < 4){
                for (int z {0}; z < y+6; z++){
                    total_r += intermediate.pixel(x,z).r;
                    total_g += intermediate.pixel(x,z).g;
                    total_b += intermediate.pixel(x,z).b;
                }
                img.pixel(x,y).r = total_r/(y+5.f);
                img.pixel(x,y).g = total_g/(y+5.f);
                img.pixel(x,y).b = total_b/(y+5.f);
            } else if (y >= img.height()-6){
                for (int z {y-4}; z < img.height(); z++){
                    total_r += intermediate.pixel(x,z).r;
                    total_g += intermediate.pixel(x,z).g;
                    total_b += intermediate.pixel(x,z).b;
                }
                img.pixel(x,y).r = total_r/(img.height()-y+5.f);
                img.pixel(x,y).g = total_g/(img.height()-y+5.f);
                img.pixel(x,y).b = total_b/(img.height()-y+5.f);
            } else {
                for(int z {y-4}; z < y+6; z++){
                    total_r += intermediate.pixel(x,z).r;
                    total_g += intermediate.pixel(x,z).g;
                    total_b += intermediate.pixel(x,z).b;
                } 
                img.pixel(x,y).r = total_r/10.f;
                img.pixel(x,y).g = total_g/10.f;
                img.pixel(x,y).b = total_b/10.f;
            }
            total_r = 0.f;
            total_g = 0.f;
            total_b = 0.f; 
        }
    }
}

void gaussian_difference(sil::Image & img, sil::Image & intermediate_1, sil::Image & intermediate_2, sil::Image const & model, float scalar,float threshold){
    separate_convolution_10_by_10(intermediate_1,intermediate_2,model);
    float temp {0.f};
    float phi {0.7f};
    for (int x {0}; x < img.width(); x++){
        for (int y {0}; y < img.height(); y++){
            img.pixel(x,y).r = (1+scalar) * model.pixel(x,y).r - scalar * intermediate_1.pixel(x,y).r;
            img.pixel(x,y).g = (1+scalar) * model.pixel(x,y).g - scalar * intermediate_1.pixel(x,y).g;
            img.pixel(x,y).b = (1+scalar) * model.pixel(x,y).b - scalar * intermediate_1.pixel(x,y).b;
            temp = brightness(img.pixel(x,y));
            if (temp < threshold){
                img.pixel(x,y).r = tanh((temp - threshold)* phi);
                img.pixel(x,y).g = tanh((temp - threshold)* phi);
                img.pixel(x,y).b = tanh((temp - threshold)* phi);
            } else {
                img.pixel(x,y).r = 1.f;
                img.pixel(x,y).g = 1.f;
                img.pixel(x,y).b = 1.f;
            }
        }
    }
}

void k_means(sil::Image & img, sil::Image const & model, int k, int precision){
    std::vector<glm::vec2> list_of_k {};
    std::vector<int> number_per_group {};
    std::vector<float> total_color_r_per_group {};
    std::vector<float> total_color_g_per_group {};
    std::vector<float> total_color_b_per_group {};
    std::vector<glm::vec3> color_averaged_per_group {};

    for (int i {0}; i < k; i++){ //inititalisation of k centers
        glm::vec2 coord {random_int(0,img.width()),random_int(0,img.height())};
        list_of_k.push_back(coord);
        number_per_group.push_back(1);
        total_color_r_per_group.push_back(model.pixel(coord.x,coord.y).r);
        total_color_g_per_group.push_back(model.pixel(coord.x,coord.y).g);
        total_color_b_per_group.push_back(model.pixel(coord.x,coord.y).b);
    }

    int closest_index{0};
    float closest_distance {0.f};
    
    for (int x {0}; x < img.width(); x++){ // group making
        for (int y {0}; y < img.height(); y++){
            closest_index = 0;
            closest_distance = glm::distance(model.pixel(x,y),model.pixel(list_of_k[0].x,list_of_k[0].y));
            for (int z {1}; z < list_of_k.size(); z++){
                if (glm::distance(model.pixel(x,y),model.pixel(list_of_k[z].x,list_of_k[z].y)) < closest_distance){
                    closest_index = z;
                    closest_distance = glm::distance(model.pixel(x,y),model.pixel(list_of_k[z].x,list_of_k[z].y));
                }
            }
            number_per_group[closest_index] += 1;
            total_color_r_per_group[closest_index] += model.pixel(x,y).r;
            total_color_g_per_group[closest_index] += model.pixel(x,y).g;
            total_color_b_per_group[closest_index] += model.pixel(x,y).b;
        }
    }

    for (int j {0}; j < list_of_k.size(); j++){ // color averaging
        glm::vec3 color {
            total_color_r_per_group[j]/static_cast<float>(number_per_group[j]),
            total_color_g_per_group[j]/static_cast<float>(number_per_group[j]),
            total_color_b_per_group[j]/static_cast<float>(number_per_group[j]),
            };
        color_averaged_per_group.push_back(color);
    }

    for (int x {0}; x < img.width(); x++){ //picture coloring
        for (int y {0}; y < img.height(); y++){
            closest_index = 0;
            closest_distance = glm::distance(model.pixel(x,y),model.pixel(list_of_k[0].x,list_of_k[0].y));
            for (int z {1}; z < list_of_k.size(); z++){
                if (glm::distance(model.pixel(x,y),model.pixel(list_of_k[z].x,list_of_k[z].y)) < closest_distance){
                    closest_index = z;
                    closest_distance = glm::distance(model.pixel(x,y),model.pixel(list_of_k[z].x,list_of_k[z].y));
                }
            }
            img.pixel(x,y).r = color_averaged_per_group[closest_index].r;
            img.pixel(x,y).g = color_averaged_per_group[closest_index].g;
            img.pixel(x,y).b = color_averaged_per_group[closest_index].b;
        }
    }

    for (int p {1}; p < precision; p++){
        for (int i {0}; i < k; i++){ //reinitialization
            for (int x {0}; x < img.width(); x++){
                if (img.pixel(list_of_k[i].x,list_of_k[i].y).r == color_averaged_per_group[i].r 
                    && img.pixel(list_of_k[i].x,list_of_k[i].y).g == color_averaged_per_group[i].g
                    && img.pixel(list_of_k[i].x,list_of_k[i].y).b == color_averaged_per_group[i].b){
                    break;
                }
                for (int y {0}; y < img.height(); y++){
                    if (img.pixel(x,y).r == color_averaged_per_group[i].r 
                    && img.pixel(x,y).g == color_averaged_per_group[i].g
                    && img.pixel(x,y).b == color_averaged_per_group[i].b){
                        list_of_k[i].x = x;
                        list_of_k[i].y = y;
                        break;
                    }
                }
                
            }
            number_per_group[i] = 1;
            total_color_r_per_group[i] = img.pixel(list_of_k[i].x,list_of_k[i].y).r;
            total_color_g_per_group[i] = img.pixel(list_of_k[i].x,list_of_k[i].y).g;
            total_color_b_per_group[i] = img.pixel(list_of_k[i].x,list_of_k[i].y).b;
        }

        for (int x {0}; x < img.width(); x++){ //group making
            for (int y {0}; y < img.height(); y++){
                closest_index = 0;
                closest_distance = glm::distance(model.pixel(x,y),img.pixel(list_of_k[0].x,list_of_k[0].y));
                for (int z {1}; z < list_of_k.size(); z++){
                    if (glm::distance(model.pixel(x,y),img.pixel(list_of_k[z].x,list_of_k[z].y)) < closest_distance){
                    closest_index = z;
                    closest_distance = glm::distance(model.pixel(x,y),img.pixel(list_of_k[z].x,list_of_k[z].y));
                    }
                }
                number_per_group[closest_index] += 1;
                total_color_r_per_group[closest_index] += model.pixel(x,y).r;
                total_color_g_per_group[closest_index] += model.pixel(x,y).g;
                total_color_b_per_group[closest_index] += model.pixel(x,y).b;
            }
        }

        for (int j {0}; j < list_of_k.size(); j++){ // color averaging
            glm::vec3 color {
                total_color_r_per_group[j]/static_cast<float>(number_per_group[j]),
                total_color_g_per_group[j]/static_cast<float>(number_per_group[j]),
                total_color_b_per_group[j]/static_cast<float>(number_per_group[j]),
                };
            color_averaged_per_group[j] = color;
        }

        for (int x {0}; x < img.width(); x++){ //picture coloring
            for (int y {0}; y < img.height(); y++){
                closest_index = 0;
                closest_distance = glm::distance(model.pixel(x,y),img.pixel(list_of_k[0].x,list_of_k[0].y));
                for (int z {1}; z < list_of_k.size(); z++){
                    if (glm::distance(model.pixel(x,y),img.pixel(list_of_k[z].x,list_of_k[z].y)) < closest_distance){
                        closest_index = z;
                        closest_distance = glm::distance(model.pixel(x,y),img.pixel(list_of_k[z].x,list_of_k[z].y));
                    }
                }
                img.pixel(x,y).r = color_averaged_per_group[closest_index].r;
                img.pixel(x,y).g = color_averaged_per_group[closest_index].g;
                img.pixel(x,y).b = color_averaged_per_group[closest_index].b;
            }
        }
    }
}

void kuwahara_filter(sil::Image & img, sil::Image const & model){
    glm::vec3 color_upper_left {}; // 0
    glm::vec3 color_upper_right {}; //1
    glm::vec3 color_lower_left {}; // 2
    glm::vec3 color_lower_right {}; //3
    float closest {0.f};
    int index {0};

    for (int x {0}; x < img.width(); x++){
        for (int y {0}; y <img.height(); y++ ){
            if (x < 2 && y < 2 ){
                img.pixel(x,y).r = 
                (model.pixel(x+1,y+2).r + model.pixel(x+2,y+2).r +
                model.pixel(x+1,y+1).r + model.pixel(x+2,y+1).r) /4.f;
                img.pixel(x,y).g = 
                (model.pixel(x+1,y+2).g + model.pixel(x+2,y+2).g +
                model.pixel(x+1,y+1).g + model.pixel(x+2,y+1).g) /4.f;
                img.pixel(x,y).b = 
                (model.pixel(x+1,y+2).b + model.pixel(x+2,y+2).b +
                model.pixel(x+1,y+1).b + model.pixel(x+2,y+1).b) /4.f;
            }else if (x < 2 && y > img.height()-3){
                img.pixel(x,y).r = 
                (model.pixel(x+1,y-1).r + model.pixel(x+2,y-1).r +
                model.pixel(x+1,y-2).r + model.pixel(x+2,y-2).r) /4.f;
                img.pixel(x,y).g = 
                (model.pixel(x+1,y-1).g + model.pixel(x+2,y-1).g +
                model.pixel(x+1,y-2).g + model.pixel(x+2,y-2).g) /4.f;
                img.pixel(x,y).b = 
                (model.pixel(x+1,y-1).b + model.pixel(x+2,y-1).b +
                model.pixel(x+1,y-2).b + model.pixel(x+2,y-2).b) /4.f;
            } else if (x > img.width()-3 && y < 2) {
                img.pixel(x,y).r = 
                (model.pixel(x-2,y+2).r + model.pixel(x-1,y+2).r +
                model.pixel(x-2,y+1).r + model.pixel(x-1,y+1).r) /4.f;
                img.pixel(x,y).g = 
                (model.pixel(x-2,y+2).g + model.pixel(x-1,y+2).g +
                model.pixel(x-2,y+1).g + model.pixel(x-1,y+1).g) /4.f;
                img.pixel(x,y).b = 
                (model.pixel(x-2,y+2).b + model.pixel(x-1,y+2).b +
                model.pixel(x-2,y+1).b + model.pixel(x-1,y+1).b) /4.f;
            } else if (x > img.width()-3 && y > img.height()-3){
                img.pixel(x,y).r = 
                (model.pixel(x-2,y-1).r + model.pixel(x-1,y-1).r +
                model.pixel(x-2,y-2).r + model.pixel(x-1,y-2).r) /4.f;
                img.pixel(x,y).g = 
                (model.pixel(x-2,y-1).g + model.pixel(x-1,y-1).g +
                model.pixel(x-2,y-2).g + model.pixel(x-1,y-2).g) /4.f;
                img.pixel(x,y).b = 
                (model.pixel(x-2,y-1).b + model.pixel(x-1,y-1).b +
                model.pixel(x-2,y-2).b + model.pixel(x-1,y-2).b) /4.f; 
            }else if (y < 2){
                color_upper_left.r = 
                (model.pixel(x-2,y+2).r + model.pixel(x-1,y+2).r +
                model.pixel(x-2,y+1).r + model.pixel(x-1,y+1).r) /4.f;
                color_upper_left.g = 
                (model.pixel(x-2,y+2).g + model.pixel(x-1,y+2).g +
                model.pixel(x-2,y+1).g + model.pixel(x-1,y+1).g) /4.f;
                color_upper_left.b = 
                (model.pixel(x-2,y+2).b + model.pixel(x-1,y+2).b +
                model.pixel(x-2,y+1).b + model.pixel(x-1,y+1).b) /4.f;

                color_upper_right.r = 
                (model.pixel(x+1,y+2).r + model.pixel(x+2,y+2).r +
                model.pixel(x+1,y+1).r + model.pixel(x+2,y+1).r) /4.f;
                color_upper_right.g = 
                (model.pixel(x+1,y+2).g + model.pixel(x+2,y+2).g +
                model.pixel(x+1,y+1).g + model.pixel(x+2,y+1).g) /4.f;
                color_upper_right.b = 
                (model.pixel(x+1,y+2).b + model.pixel(x+2,y+2).b +
                model.pixel(x+1,y+1).b + model.pixel(x+2,y+1).b) /4.f;

                if (glm::distance(color_upper_left,model.pixel(x,y)) > glm::distance(color_upper_right,model.pixel(x,y))){
                    img.pixel(x,y).r = color_upper_right.r;
                    img.pixel(x,y).g = color_upper_right.g;
                    img.pixel(x,y).b = color_upper_right.b;
                } else {
                    img.pixel(x,y).r = color_upper_left.r;
                    img.pixel(x,y).g = color_upper_left.g;
                    img.pixel(x,y).b = color_upper_left.b;
                }
            } else if (y > img.height()-3){
                color_lower_left.r = 
                (model.pixel(x-2,y-1).r + model.pixel(x-1,y-1).r +
                model.pixel(x-2,y-2).r + model.pixel(x-1,y-2).r) /4.f;
                color_lower_left.g = 
                (model.pixel(x-2,y-1).g + model.pixel(x-1,y-1).g +
                model.pixel(x-2,y-2).g + model.pixel(x-1,y-2).g) /4.f;
                color_lower_left.b = 
                (model.pixel(x-2,y-1).b + model.pixel(x-1,y-1).b +
                model.pixel(x-2,y-2).b + model.pixel(x-1,y-2).b) /4.f; 

                color_lower_right.r = 
                (model.pixel(x+1,y-1).r + model.pixel(x+2,y-1).r +
                model.pixel(x+1,y-2).r + model.pixel(x+2,y-2).r) /4.f;
                color_lower_right.g = 
                (model.pixel(x+1,y-1).g + model.pixel(x+2,y-1).g +
                model.pixel(x+1,y-2).g + model.pixel(x+2,y-2).g) /4.f;
                color_lower_right.b = 
                (model.pixel(x+1,y-1).b + model.pixel(x+2,y-1).b +
                model.pixel(x+1,y-2).b + model.pixel(x+2,y-2).b) /4.f;

                if (glm::distance(color_lower_left,model.pixel(x,y)) > glm::distance(color_lower_right,model.pixel(x,y))){
                    img.pixel(x,y).r = color_lower_right.r;
                    img.pixel(x,y).g = color_lower_right.g;
                    img.pixel(x,y).b = color_lower_right.b;
                } else {
                    img.pixel(x,y).r = color_lower_left.r;
                    img.pixel(x,y).g = color_lower_left.g;
                    img.pixel(x,y).b = color_lower_left.b;
                }
            } else if (x < 2){
                color_upper_right.r = 
                (model.pixel(x+1,y+2).r + model.pixel(x+2,y+2).r +
                model.pixel(x+1,y+1).r + model.pixel(x+2,y+1).r) /4.f;
                color_upper_right.g = 
                (model.pixel(x+1,y+2).g + model.pixel(x+2,y+2).g +
                model.pixel(x+1,y+1).g + model.pixel(x+2,y+1).g) /4.f;
                color_upper_right.b = 
                (model.pixel(x+1,y+2).b + model.pixel(x+2,y+2).b +
                model.pixel(x+1,y+1).b + model.pixel(x+2,y+1).b) /4.f;

                color_lower_right.r = 
                (model.pixel(x+1,y-1).r + model.pixel(x+2,y-1).r +
                model.pixel(x+1,y-2).r + model.pixel(x+2,y-2).r) /4.f;
                color_lower_right.g = 
                (model.pixel(x+1,y-1).g + model.pixel(x+2,y-1).g +
                model.pixel(x+1,y-2).g + model.pixel(x+2,y-2).g) /4.f;
                color_lower_right.b = 
                (model.pixel(x+1,y-1).b + model.pixel(x+2,y-1).b +
                model.pixel(x+1,y-2).b + model.pixel(x+2,y-2).b) /4.f;
            
                if (glm::distance(color_upper_right,model.pixel(x,y)) > glm::distance(color_lower_right,model.pixel(x,y))){
                    img.pixel(x,y).r = color_lower_right.r;
                    img.pixel(x,y).g = color_lower_right.g;
                    img.pixel(x,y).b = color_lower_right.b;
                } else {
                    img.pixel(x,y).r = color_upper_right.r;
                    img.pixel(x,y).g = color_upper_right.g;
                    img.pixel(x,y).b = color_upper_right.b;
                }
            } else if ( x > img.width()-3){
                color_upper_left.r = 
                (model.pixel(x-2,y+2).r + model.pixel(x-1,y+2).r +
                model.pixel(x-2,y+1).r + model.pixel(x-1,y+1).r) /4.f;
                color_upper_left.g = 
                (model.pixel(x-2,y+2).g + model.pixel(x-1,y+2).g +
                model.pixel(x-2,y+1).g + model.pixel(x-1,y+1).g) /4.f;
                color_upper_left.b = 
                (model.pixel(x-2,y+2).b + model.pixel(x-1,y+2).b +
                model.pixel(x-2,y+1).b + model.pixel(x-1,y+1).b) /4.f;

                color_lower_left.r = 
                (model.pixel(x-2,y-1).r + model.pixel(x-1,y-1).r +
                model.pixel(x-2,y-2).r + model.pixel(x-1,y-2).r) /4.f;
                color_lower_left.g = 
                (model.pixel(x-2,y-1).g + model.pixel(x-1,y-1).g +
                model.pixel(x-2,y-2).g + model.pixel(x-1,y-2).g) /4.f;
                color_lower_left.b = 
                (model.pixel(x-2,y-1).b + model.pixel(x-1,y-1).b +
                model.pixel(x-2,y-2).b + model.pixel(x-1,y-2).b) /4.f; 

                if (glm::distance(color_upper_left,model.pixel(x,y)) > glm::distance(color_lower_left,model.pixel(x,y))){
                    img.pixel(x,y).r = color_lower_left.r;
                    img.pixel(x,y).g = color_lower_left.g;
                    img.pixel(x,y).b = color_lower_left.b;
                } else{
                    img.pixel(x,y).r = color_upper_left.r;
                    img.pixel(x,y).g = color_upper_left.g;
                    img.pixel(x,y).b = color_upper_left.b;
                } 
            } else {
                closest = 0.f;
                color_upper_left.r = 
                (model.pixel(x-2,y+2).r + model.pixel(x-1,y+2).r +
                model.pixel(x-2,y+1).r + model.pixel(x-1,y+1).r) /4.f;
                color_upper_left.g = 
                (model.pixel(x-2,y+2).g + model.pixel(x-1,y+2).g +
                model.pixel(x-2,y+1).g + model.pixel(x-1,y+1).g) /4.f;
                color_upper_left.b = 
                (model.pixel(x-2,y+2).b + model.pixel(x-1,y+2).b +
                model.pixel(x-2,y+1).b + model.pixel(x-1,y+1).b) /4.f;

                color_upper_right.r = 
                (model.pixel(x+1,y+2).r + model.pixel(x+2,y+2).r +
                model.pixel(x+1,y+1).r + model.pixel(x+2,y+1).r) /4.f;
                color_upper_right.g = 
                (model.pixel(x+1,y+2).g + model.pixel(x+2,y+2).g +
                model.pixel(x+1,y+1).g + model.pixel(x+2,y+1).g) /4.f;
                color_upper_right.b = 
                (model.pixel(x+1,y+2).b + model.pixel(x+2,y+2).b +
                model.pixel(x+1,y+1).b + model.pixel(x+2,y+1).b) /4.f; 

                color_lower_left.r = 
                (model.pixel(x-2,y-1).r + model.pixel(x-1,y-1).r +
                model.pixel(x-2,y-2).r + model.pixel(x-1,y-2).r) /4.f;
                color_lower_left.g = 
                (model.pixel(x-2,y-1).g + model.pixel(x-1,y-1).g +
                model.pixel(x-2,y-2).g + model.pixel(x-1,y-2).g) /4.f;
                color_lower_left.b = 
                (model.pixel(x-2,y-1).b + model.pixel(x-1,y-1).b +
                model.pixel(x-2,y-2).b + model.pixel(x-1,y-2).b) /4.f; 

                color_lower_right.r = 
                (model.pixel(x+1,y-1).r + model.pixel(x+2,y-1).r +
                model.pixel(x+1,y-2).r + model.pixel(x+2,y-2).r) /4.f;
                color_lower_right.g = 
                (model.pixel(x+1,y-1).g + model.pixel(x+2,y-1).g +
                model.pixel(x+1,y-2).g + model.pixel(x+2,y-2).g) /4.f;
                color_lower_right.b = 
                (model.pixel(x+1,y-1).b + model.pixel(x+2,y-1).b +
                model.pixel(x+1,y-2).b + model.pixel(x+2,y-2).b) /4.f;

                closest = glm::distance(color_upper_left,model.pixel(x,y));
                index = 0;
                if (closest > glm::distance(color_upper_right,model.pixel(x,y))){
                    closest = glm::distance(color_upper_right,model.pixel(x,y));
                    index = 1;
                }  
                if (closest > glm::distance(color_lower_left,model.pixel(x,y))){
                    closest = glm::distance(color_lower_left,model.pixel(x,y));
                    index = 2;
                } 
                if (closest > glm::distance(color_lower_right,model.pixel(x,y))){
                    closest = glm::distance(color_lower_right,model.pixel(x,y));
                    index = 3;
                }  

                switch(index){
                    case 0:
                        img.pixel(x,y).r = color_upper_left.r;
                        img.pixel(x,y).g = color_upper_left.g;
                        img.pixel(x,y).b = color_upper_left.b;
                        break;
                    case 1:
                        img.pixel(x,y).r = color_upper_right.r;
                        img.pixel(x,y).g = color_upper_right.g;
                        img.pixel(x,y).b = color_upper_right.b;
                        break;
                    case 2:
                        img.pixel(x,y).r = color_lower_left.r;
                        img.pixel(x,y).g = color_lower_left.g;
                        img.pixel(x,y).b = color_lower_left.b;
                        break;
                    case 3:
                        img.pixel(x,y).r = color_lower_right.r;
                        img.pixel(x,y).g = color_lower_right.g;
                        img.pixel(x,y).b = color_lower_right.b;
                        break;
                    default:
                        break;
                }    
            }
        }
    }
}

void diamond_square(sil::Image & img){
    int height = img.height();
    std::vector<std::vector<int>> height_map {}; 
    for (int i {0}; i < height; i++){
        height_map.push_back({});
        for (int j {0}; j < height; j++){
            height_map[i].push_back(0);
        }
    }

    height_map[0][0] = random_int(-height, height+1); // coin bas gauche
    height_map[0][height-1] = random_int(-height, height+1); // coin bas droite
    height_map[height-1][0] = random_int(-height, height+1); // coin haut gauche
    height_map[height-1][height-1] = random_int(-height, height+1); // coin haut droite
    int half {height};
    int pas {half/2};
    
    while (pas > 0){ 
        //diamond step
        for (int y {pas}; y < height; y += half ){
            for (int x {pas}; x < height ; x += half){
                height_map[y][x] = (
                    height_map[y-pas][x-pas] + //bas gauche
                    height_map[y+pas][x-pas] + //haut gauche
                    height_map[y-pas][x+pas] +  // bas droite
                    height_map[y+pas][x+pas] // haut droite
                    )/4 + random_int(-20,21);
            }
        }

        //square step
        for (int y {pas}; y < height; y += half ){
            for (int x {0}; x < height ; x += half){
                if (x == 0){
                    height_map[y][x] = (height_map[y-pas][x] + height_map[y][x+pas] + height_map[y+pas][x])/3 + random_int(-20,21);
                } else if ( x == height-1){
                    height_map[y][x] = (height_map[y-pas][x] + height_map[y][x-pas] + height_map[y+pas][x])/3 + random_int(-20,21);
                } else {
                    height_map[y][x] = (height_map[y-pas][x] + height_map[y][x+pas] + height_map[y+pas][x] + height_map[y][x-pas])/4 + random_int(-20,21);
                }
            }
        }
        for (int y {0}; y < height; y += half ){
            for (int x {pas}; x < height ; x += half){
                if (y == 0){
                    height_map[y][x] = (height_map[y][x-pas] + height_map[y][x+pas] + height_map[y+pas][x])/3 + random_int(-20,21);
                } else if ( y == height-1){
                    height_map[y][x] = (height_map[y-pas][x] + height_map[y][x-pas] + height_map[y][x+pas])/3 + random_int(-20,21);
                } else {
                    height_map[y][x] = (height_map[y-pas][x] + height_map[y][x+pas] + height_map[y+pas][x] + height_map[y][x-pas])/4 + random_int(-20,21);
                }
            }
        }
        half = pas;
        pas /= 2;
    }
    

    for (int x {0}; x < height;x++){
        for (int y {0}; y < height; y++){
            img.pixel(x,y).r = static_cast<float>(height_map[x][y]+height)/(height*2.f);
            img.pixel(x,y).g = static_cast<float>(height_map[x][y]+height)/(height*2.f);
            img.pixel(x,y).b = static_cast<float>(height_map[x][y]+height)/(height*2.f); 
        }
    }
}

void color_diamond_square(sil::Image & img){
    for (int x {0}; x < img.width();x++){
        for (int y {0}; y < img.height(); y++){
            if(brightness(img.pixel(x,y)) < 0.5f){
                img.pixel(x,y) = glm::mix(glm::vec3{0.f,0.f,1.f},glm::vec3{0.4f,0.4f,0.90f},brightness(img.pixel(x,y))/0.5f);
            } else {
                img.pixel(x,y) = glm::mix(glm::vec3{0.f,0.5f,0.f},glm::vec3{0.55f,0.27f,0.07f},(brightness(img.pixel(x,y))-0.5f)/0.5f);
            }
        }
    }
}

int main()
{
    { //green only
        sil::Image image{"images/logo.png"};
        only_green(image);
        image.save("output/green_only.png");
    }

    { // red and blue swapped
        sil::Image image{"images/logo.png"};
        swap_blue_red(image);
        image.save("output/blue_red_swap.png");
    }
    
    { // turning a colored picture into a black and white
        sil::Image image{"images/logo.png"};
        black_and_white(image);
        image.save("output/black_white.png");
    }

    { // negative image
        sil::Image image{"images/logo.png"};
        negative(image);
        image.save("output/negative.png");
    }

    { //degrade
        sil::Image image{300, 200};
        degrade(image);
        image.save("output/degrade.png");
    }

    { //mirror
        sil::Image image{"images/logo.png"};
        mirror(image);
        image.save("output/mirror.png");
    }

    { //noise
        sil::Image image{"images/logo.png"};
        noise(image);
        image.save("output/noise.png");
    }

    { //rotation to the left by 90°
        sil::Image image{"images/logo.png"};
        sil::Image img_rotate {image.height(),image.width()};
        rotate_90_toward_left(img_rotate,image);
        img_rotate.save("output/rotate_toward_left_by_90.png");
    }

    { //splitting rgb
        sil::Image image{"images/logo.png"};
        sil::Image img_split {image.width(),image.height()};
        split_rgb(img_split,image);
        img_split.save("output/split_rgb.png");
    }

    { // more light
        sil::Image image{"images/photo.jpg"};
        lighter_photo(image);
        image.save("output/lighter_photo.jpg");
    }

    { // less light
        sil::Image image{"images/photo.jpg"};
        darker_photo(image);
        image.save("output/darker_photo.jpg");
    }

    { // a disc
        sil::Image image{500,500};
        draw_disc(image,120);
        image.save("output/disque.png");
    }

    { // a circle
        sil::Image image{500,500};
        draw_circle(image,120,20);
        image.save("output/cercle.png");
    }

    { // what was needed to make the gif
        sil::Image image{500,500};
        sil::Image model{"images/disque.png"};
        shift_pixel_to_the_left(image,model,250);
        image.save("output/disque_1.png");
        shift_pixel_to_the_left(image,model,200);
        image.save("output/disque_2.png");
        shift_pixel_to_the_left(image,model,150);
        image.save("output/disque_3.png");
        shift_pixel_to_the_left(image,model,100);
        image.save("output/disque_4.png");
        shift_pixel_to_the_left(image,model,50);
        image.save("output/disque_5.png");
        shift_pixel_to_the_right(image,model,50);
        image.save("output/disque_6.png");
        shift_pixel_to_the_right(image,model,100);
        image.save("output/disque_7.png");
        shift_pixel_to_the_right(image,model,150);
        image.save("output/disque_8.png");
        shift_pixel_to_the_right(image,model,200);
        image.save("output/disque_9.png");
    }

    { // the rosace
        sil::Image image{500,500};
        draw_rosace(image,120,5);
        image.save("output/rosace.png");
    }

    { // the mosaic
        sil::Image image{"images/logo.png"};
        sil::Image model{"images/logo.png"};
        do_mosaic(image,model);
        image.save("output/mosaique.png");
    }

    { // the complicated mosaic
        sil::Image image{"images/logo.png"};
        sil::Image model{"images/logo.png"};
        reflet(image,model);
        image.save("output/reflet.png");
        sil::Image refl3t{"images/reflet.png"};
        mirror(refl3t);
        refl3t.save("output/reflet_mirror.png");
        //deplacement des output dans images
        sil::Image model_mirror{"images/mirror.png"};
        sil::Image reflet_mirror{"images/reflet_mirror.png"};
        sil::Image refl3t_model{"images/reflet.png"};
        do_mirror_mosaic(image,model,model_mirror,refl3t_model,reflet_mirror);
        image.save("output/mirror_mosaic.png");  
    }

    { // add a glitch effect
        sil::Image image{"images/logo.png"};
        sil::Image model{"images/logo.png"};
        add_glitch(image,model);
        image.save("output/glitch.png");
    }

    { // sort the pixel
        sil::Image image{"images/logo.png"};
        sort_pixels(image);
        image.save("output/sorted_logo.png");
    }

    { // color degrade
        sil::Image image{300,200};
        degrade_couleur(image);
        image.save("output/degrade_couleur.png");
        degrade_couleur_oklab(image);
        image.save("output/oklab_degrade.png");
    }

    { //mandelbrot
        sil::Image image{500,500};
        draw_mandelbrot(image);
        image.save("output/mandelbrot.png");
    }

    { // tramage
        sil::Image image{"images/photo.jpg"};
        black_and_white(image);
        tramage(image);
        image.save("output/tramage.jpg");
    }

    { //normalisation de l'histogramme
        sil::Image image {"images/photo_faible_contraste.jpg"};
        normalisation_histogramme(image);
        image.save("output/normaliser.png");
    }

    { //vortex
        sil::Image model{"images/logo.png"};
        sil::Image image{model.width(),model.height()};
        float custom_spin {10.f}; // plus grand est le nombre , plus tout semble se melanger
        ygo_fusion_vortex(image,model,custom_spin);
        image.save("output/vortex.png");
    }

    { //convolution first box blur
        sil::Image model {"images/logo.png"};
        sil::Image image{model.width(),model.height()};
        float first_box_blur_matrix [][3]{
        {0.0625f, 0.125f, 0.0625f},
        {0.125f, 0.25f, 0.125f},
        {0.0625f, 0.125f, 0.0625f},
        };
        customisable_convolution_3_by_3_kernel(image,model,first_box_blur_matrix);
        image.save("output/first_box_blur.png");
    }

    { //convolution emboss
        sil::Image model {"images/logo.png"};
        sil::Image image{model.width(),model.height()};
        float emboss_matrix [][3]{
        {-2.f, -1.f, 0.f},
        {-1.f, 1.f, 1.f},
        {0.f, 1.f, 2.f},
        };
        customisable_convolution_3_by_3_kernel(image,model,emboss_matrix);
        image.save("output/emboss_logo.png");
    }

    { // convolution outline
        sil::Image model {"images/logo.png"};
        sil::Image image{model.width(),model.height()};
        float outline_matrix [][3]{
        {-1.f, -1.f, -1.f},
        {-1.f, 8.f, -1.f},
        {-1.f, -1.f, -1.f},
        };
        customisable_convolution_3_by_3_kernel(image,model,outline_matrix);
        image.save("output/outline_logo.png");
    }

    { // convolution sharpen
        sil::Image model {"images/logo.png"};
        sil::Image image{model.width(),model.height()};
        float sharpen_matrix [][3]{
        {0.f, -1.f, 0.f},
        {-1.f, 5.f, -1.f},
        {0.f, -1.f, 0.f},
        };
        customisable_convolution_3_by_3_kernel(image,model,sharpen_matrix);
        image.save("output/sharpen_logo.png");
    }

    { //big blur convolution
        sil::Image model {"images/logo.png"};
        sil::Image image{model.width(),model.height()};
        sil::Image intermediate{model.width(),model.height()};
        separate_convolution_100_by_100(image,intermediate,model);
        image.save("output/blur_100_by_100_logo.png");
    }

    { //gaussian difference
        sil::Image model{"images/photo.jpg"};
        sil::Image image {model.width(),model.height()};
        sil::Image intermediate_1{model.width(),model.height()};
        sil::Image intermediate_2{model.width(),model.height()};
        float scalar {0.9f};
        float threshold {0.25f};
        gaussian_difference(image,intermediate_1,intermediate_2,model,scalar,threshold);
        image.save("output/gaussian_difference.jpg");
    }

    { //k-means
        sil::Image model{"images/photo.jpg"};
        sil::Image image{model.width(),model.height()};
        int k {2};
        int precision {1}; // number of times the algorithm will be executed
        k_means(image,model,k,precision);
        // for k = 2, a precision of 1 is enough
        // for k = 3, a precision of 4 is enough 
        // for k = 16, a precision of 1 is enough 
        image.save("output/k_means_2_precision_1.jpg");
    }

    { // kuwahara filter
        sil::Image model{"images/photo.jpg"};
        sil::Image image{model.width(),model.height()};
        kuwahara_filter(image,model);
        image.save("output/kuwahara_photo.jpg");
    }

    { // diamond square
        sil::Image image {257,257};
        diamond_square(image);
        image.save("output/diamond_square.png");
        color_diamond_square(image);
        image.save("output/colored_diamond_square.png");
    }
}