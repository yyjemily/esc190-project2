//The function will compute the dual-gradient energy function, and place it in the struct rgb_img *grad.
#include <stdio.h>
#include <math.h>
#include "chilan_test.c"

void remove_seam(struct rgb_img *src, struct rgb_img **dest, int *path){
    //make a new image with the address dest
    create_img(dest, src -> height, src-> width -1); 

    //iterate through the path array 
    for (int remove_index = 0 ; remove_index < sizeof(*path)/sizeof(path[0]); remove_index ++){
        for (int row = 0; row < src -> height; row ++){

            for (int col = 0; col < src -> width; col ++){
                //if the current index is equal to the remove index 
                if (col != remove_index) {
                    uint8_t r = get_pixel(*dest, row, col, 0);
                    uint8_t g = get_pixel(*dest, row, col, 1);
                    uint8_t b = get_pixel(*dest, row, col, 2);
                    set_pixel(*dest, row, col, r, g, b); 
                }

            }

        }
        
    }
    write_img(*dest,"/Users/emiliemui/coding/ESC190/esc190/esc190-project2/6x5remove_seam.bin"); 
}

int main(){
    struct rgb_img *grad;
    struct rgb_img *im; 
    read_in_img(&im, "/Users/emiliemui/coding/ESC190/esc190/esc190-project2/6x5.bin");
    calc_energy(im,  &grad);
    print_grad(grad);

    //best array test 
    double *best_array;
    int *path;
    dynamic_seam(grad, &best_array);
    print_best_array(best_array, (int)grad->width, (int)grad->height);
    recover_path(best_array, (int)grad->width, (int)grad->height, &path);
    print_recovered_path(path, (int)grad->height);


}

