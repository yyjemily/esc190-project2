//Last updated 9:56 AM 
//odd numbers
//The function will compute the dual-gradient energy function, and place it in the struct rgb_img *grad.
#include "c_img.h"
#include "c_img.c"
#include <stdio.h>
#include <math.h>

int wrap_index(int x, int sz){
    //enforces wrapping around method for images at the edge 
    // printf("current index: %d\n ", x); 
    // printf("sz: %d, x+sz: %d \n", sz, (x+sz));
    // printf("wrap ind: %d \n", (x + sz)% sz);
    return (x + sz)% sz;

}

void calc_energy(struct rgb_img *im, struct rgb_img **grad){
    //change location that it writes to depending on who's testing
    //for each pixel get the rbg calculate gradient, set to the gradient matrix at that index 
    
    //malloc space for the gradient matrix 
    *grad = (struct rgb_img *)malloc(sizeof(struct rgb_img));
    (*grad)->height = im -> height;
    (*grad)->width = im -> width;
    (*grad)->raster = (uint8_t *)malloc(3 * im -> height * im -> width);

   for (int y = 0; y < im -> width; y++) {
    for (int x = 0; x < im -> height; x++ ){
        
        printf("(%d %d)\n", y, x); 
        int r = get_pixel(im, y, x, 0);
        int b = get_pixel(im, y, x, 1);
        int g = get_pixel(im, y, x, 2);
    //get pixel of top - bot 
        printf(" r: %d, b: %d, g: %d\n", r, b, g); 

        //wrap around for pixels at the edge of the image 
    //check boundary conditions for x 
        //increment columns
        int r_diff_x = get_pixel(im, y, wrap_index(x+1, im -> width), 0) - get_pixel(im, y, wrap_index(x-1, im -> width), 0);
        int b_diff_x = get_pixel(im, y, wrap_index(x+1, im -> width), 1) - get_pixel(im, y, wrap_index(x-1, im -> width), 1);
        int g_diff_x = get_pixel(im, y, wrap_index(x+1, im -> width), 2) - get_pixel(im, y, wrap_index(x-1, im -> width), 2);
    
        printf("x: %d %d %d\n", r_diff_x, b_diff_x, g_diff_x);
        printf("r_diff: %d - %d ", get_pixel(im, y, wrap_index(x+1, im -> width), 0),get_pixel(im, y, wrap_index(x-1, im -> width), 0));
        printf("b_diff: %d - %d ", get_pixel(im, y, wrap_index(x+1, im -> width), 1),get_pixel(im, y, wrap_index(x-1, im -> width), 1));
        printf("g_diff: %d - %d ", get_pixel(im, y, wrap_index(x+1, im -> width), 2),get_pixel(im, y, wrap_index(x-1, im -> width), 2));
        printf("\n");
        
        //increments rows 
        int r_diff_y = get_pixel(im, wrap_index(y+1, im -> height), x, 0) - get_pixel(im, wrap_index(y-1, im -> height), x, 0);
        int b_diff_y = get_pixel(im, wrap_index(y+1, im -> height), x, 1) - get_pixel(im, wrap_index(y-1, im -> height), x, 1);
        int g_diff_y = get_pixel(im, wrap_index(y+1, im -> height), x, 2) - get_pixel(im, wrap_index(y-1, im -> height), x, 2);
        
        printf("r_diff: %d - %d ", get_pixel(im, wrap_index(y+1, im -> width), x, 0),get_pixel(im, wrap_index(y-1, im -> width), x, 0));
        //printf("check here, x = %d\n", x);
        //printf("(2,1): %d", get_pixel(im, 2, 1, 1)); 
        //printf("wrap_index: %d",  wrap_index(y-1, im -> width));
        printf("b_diff: %d - %d ", get_pixel(im, wrap_index(y+1, im -> width), x, 1),get_pixel(im, wrap_index(y-1, im -> width), x, 1));
        //printf("check stop\n");
        printf("g_diff: %d - %d \n", get_pixel(im, wrap_index(y+1, im -> width), x, 2),get_pixel(im, wrap_index(y-1, im -> width), x, 2));
        
        printf("y: %d %d %d\n", r_diff_y, b_diff_y, g_diff_y);
        
    //calculate the gradient 
        int grad_x = (pow(r_diff_x, 2) + pow(g_diff_x, 2) + pow(b_diff_x, 2));
        int grad_y = (pow(r_diff_y, 2) + pow(g_diff_y, 2) + pow(b_diff_y, 2)); 
        int energy = (int)sqrt(grad_x + grad_y); 

    //calc dual energy 
        int dual_energy = (uint8_t)(energy/10); 
        
        set_pixel(*grad, y, x, dual_energy, dual_energy ,dual_energy); 
    printf("\n");
    }

    printf("\n");  
   }

}

int main(){
    struct rgb_img *grad;
    struct rgb_img *im; 
    read_in_img(&im, "/Users/emiliemui/coding/ESC190/esc190/esc190-project2/3x4.bin");
    calc_energy(im,  &grad);
    print_grad(grad);

}

