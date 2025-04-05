// #include "emily_test.c"
#include "c_img.h"
#include "c_img.c"
#include <stdio.h>
#include <math.h>

int wrap_index(int x, int sz){
    //enforces wrapping around method for images at the edge 
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

   for (int x = 0; x < im -> width; x++) {
    for (int y = 0; y < im -> height; y++ ){
        
        int r = get_pixel(im, y, x, 0);
        int b = get_pixel(im, y, x, 1);
        int g = get_pixel(im, y, x, 2);

    //check boundary conditions for x 
        //increment columns
        int r_diff_x = get_pixel(im, y, wrap_index(x+1, im -> width), 0) - get_pixel(im, y, wrap_index(x-1, im -> width), 0);
        int b_diff_x = get_pixel(im, y, wrap_index(x+1, im -> width), 1) - get_pixel(im, y, wrap_index(x-1, im -> width), 1);
        int g_diff_x = get_pixel(im, y, wrap_index(x+1, im -> width), 2) - get_pixel(im, y, wrap_index(x-1, im -> width), 2);

        //increments rows 
        int r_diff_y = get_pixel(im, wrap_index(y+1, im -> height), x, 0) - get_pixel(im, wrap_index(y-1, im -> height), x, 0);
        int b_diff_y = get_pixel(im, wrap_index(y+1, im -> height), x, 1) - get_pixel(im, wrap_index(y-1, im -> height), x, 1);
        int g_diff_y = get_pixel(im, wrap_index(y+1, im -> height), x, 2) - get_pixel(im, wrap_index(y-1, im -> height), x, 2);
        

    //calculate the gradient 
        int grad_x = (pow(r_diff_x, 2) + pow(g_diff_x, 2) + pow(b_diff_x, 2));
        int grad_y = (pow(r_diff_y, 2) + pow(g_diff_y, 2) + pow(b_diff_y, 2)); 
        int energy = (int)sqrt(grad_x + grad_y); 

    //calc dual energy 
        int dual_energy = (uint8_t)(energy/10); 
        
        set_pixel(*grad, y, x, dual_energy, dual_energy ,dual_energy); 
    }
   }

}


void print_best_array(double *best_array, int width, int height) {
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            printf("%f\t", best_array[i*width + j]);
        }
    printf("\n");    
    }
}
void dynamic_seam(struct rgb_img *grad, double **best_arr) {
    // set height and width
    int width, height;
    width = (int)grad->width;
    height = (int)grad->height;

    // malloc space for best_arr
    *best_arr = (double *)malloc(sizeof(double)*height*width);
    if (*best_arr == NULL) {
        printf("Memory allocation failed!\n");
        return;
    }

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (i == 0) {
                // initialize first row
                (*best_arr)[i*width + j] = get_pixel(grad, i, j, 0);
                
            }
            else {
                // get min of the edges
                
                int min; 
                for (int k = (int)fmax(0, j - 1); k <= fmin(width - 1, j + 1); k++) {
                    if (k == (int)fmax(0, j - 1)) {
                        min = (*best_arr)[(i-1)*width + k];
                    }
                    else if ((*best_arr)[(i-1)*width + k] < min) {
                        min = (*best_arr)[(i-1)*width + k];
                    }
                }
                (*best_arr)[i*width + j] = (double)min + (double)get_pixel(grad, i, j, 0);
            }
        }
    }
} 

void recover_path(double *best, int height, int width, int **path) {
    (*path) = (int *)malloc(sizeof(int)*height);

    // start from bottom and find min and then find min up
    double minn = best[(height - 1)*width]; // first one
    int minin = 0; // index of the min

    for (int i = 1; i < width; i++) {
        if (best[(height - 1)*width + i] < minn) {
            minn = best[(height - 1)*width + i];
            minin = i;
        }
    }
    (*path)[height - 1] = minin; // last element is last row min
    for (int i = height - 2; i >= 0; i--) { // reverse upwards to the three values around it
        double minline;
        int minlinein;
        int a = (*path)[i+1];
        // min of the nearest elements
        for (int j = (int)fmax(0, a - 1); j <= (int)fmin(width - 1, a + 1); j++) {
            if (j == (int)fmax(0, a - 1)) {
                minline = best[i*width + j];
                minlinein = j;
            }
            else {
                if (best[i*width + j] < minline) {
                    minline = best[i*width + j];
                    minlinein = j;
                }
            }
        }
        (*path)[i] = minlinein;
    }
}

void print_recovered_path(int *path, int length) {
    printf("\n");
    for (int i = 0; i < length; i++) {
        printf("%d\t", path[i]);
    }
}

int main()
{
    struct rgb_img *grad;
    struct rgb_img *im; 
    double *best_array;
    int *path;
    read_in_img(&im, "/Users/emiliemui/coding/ESC190/esc190/esc190-project2/6x5.bin");
    calc_energy(im,  &grad);
    print_grad(grad);
    dynamic_seam(grad, &best_array);
    print_best_array(best_array, (int)grad->width, (int)grad->height);
    recover_path(best_array, (int)grad->height, (int)grad->width, &path);
    print_recovered_path(path, (int)grad->height);
}