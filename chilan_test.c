#include "c_img.h"
#include "c_img.c"
#include "emily_test.c"
#include <stdio.h>
#include <math.h>

void dynamic_seam(struct rgb_img *grad, double **best_arr) {
    // set height and width
    int width, height;
    width = (int)grad->width;
    height = (int)grad->height;

    // malloc space for best_arr
    *best_arr = (double *)malloc(sizeof(double));


    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int energy = get_pixel(grad, i, j, 0);

            if (i == 0) {
                (*best_arr)[j] = (double)energy;
            }
            else {
                // mins from previous row
                double a, b;
                if (j > 0) {
                    a = min((*best_arr)[i*(width-1) + j - 1], (*best_arr)[i*(width-1) + j]);
                }
                if (j < width - 1) {
                    b = min((*best_arr)[i*(width+1) + j - 1], (*best_arr)[i*(width-1) + j]);
                }

                double prev;
                if (j == 0) {
                    prev = a;
                }
                else if (j == width - 1) {
                    prev = b;
                }
                else {
                    prev = min(a, b);
                }
                
                (*best_arr)[i*width + j] = (double)energy + prev;
            }
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

int main()
{
    struct rgb_img *grad;
    struct rgb_img *im; 
    double *best_array;
    read_in_img(&im, "C:/Us/PC/esc190-project2/6x5.bin");
    calc_energy(im,  &grad);
    print_grad(grad);
    dynamic_seam(grad, &best_array);
    print_best_array(best_array, (int)grad->width, (int)grad->height);


}