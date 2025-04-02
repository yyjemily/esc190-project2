//Last updated 9:56 AM 
//odd numbers
//The function will compute the dual-gradient energy function, and place it in the struct rgb_img *grad.
#include "c_img.h"
#include "c_img.c"
#include <stdio.h>
#include <math.h>


void calc_energy(struct rgb_img *im, struct rgb_img **grad){
    //change location that it writes to depending on who's testing
    //for each pixel get the rbg calculate gradient, set to the gradient matrix at that index 
    
    //malloc space for the gradient matrix 
    *grad = (struct rgb_img *)malloc(sizeof(struct rgb_img));
    (*grad)->height = im -> height;
    (*grad)->width = im -> width;
    (*grad)->raster = (uint8_t *)malloc(3 * im -> height * im -> width);
    
   for (int x = 0; x < im -> width; x++) {
    for (int y = 0; y < im -> height; y++ ){
        int r = get_pixel(im, y, x, 0);
        int b = get_pixel(im, y, x, 1);
        int g = get_pixel(im, y, x, 2);
    //get pixel of top - bot 

    //check boundary conditions for x 
        int r_diff_x = get_pixel(im, y, x+1, 0) - get_pixel(im, y, x-1, 0);
        int b_diff_x = get_pixel(im, y, x+1, 1) - get_pixel(im, y, x-1, 1);
        int g_diff_x = get_pixel(im, y, x+1, 2) - get_pixel(im, y, x-1, 2);

        printf("%d %d %d", r_diff_x, b_diff_x, g_diff_x);
        printf("\n x val above \n");  
        int r_diff_y = get_pixel(im, y+1, x, 0) - get_pixel(im, y-1, x, 0);
        int b_diff_y = get_pixel(im, y+1, x, 1) - get_pixel(im, y-1, x, 1);
        int g_diff_y = get_pixel(im, y+1, x, 2) - get_pixel(im, y-1, x, 2);

        printf("%d %d %d", r_diff_x, b_diff_x, g_diff_x);
        printf("\n y val above \n");  
    //calculate the gradient 
        int grad_x = (pow(r_diff_x, 2) + pow(g_diff_x, 2) + pow(b_diff_x, 2));
        int grad_y = (pow(r_diff_y, 2) + pow(g_diff_y, 2) + pow(b_diff_y, 2)); 
        int energy = sqrt(grad_x + grad_y); 

    //calc dual energy 
        int dual_energy = (uint8_t)energy/10; 
        
        set_pixel(*grad, y, x, dual_energy, dual_energy ,dual_energy); 
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

