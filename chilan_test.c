#include "emily_test.c"
#include <stdio.h>
#include <math.h>


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

    printf("\n");
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
                (*best_arr)[i*width + j] = (double)min + (*best_arr)[(i-1)*width + j];
            }
        }
    }
}

void recover_path(double *best, int height, int width, int **path) {
    (*path) = (int *)malloc(sizeof(int)*height);

    // start from bottom and find min and then find min up
    int minn = best[(height - 1)*width]; // first one
    int minin = 0; // index of the min

    for (int i = 1; i < width; i++) {
        if (best[(height - 1)*width + i] < minn) {
            minn = best[(height - 1)*width + i];
            minin = i;
        }
    }
    (*path)[height - 1] = minn; // last element is last row min
    for (int i = height - 1; i >= 0; i--) { // reverse upwards to the three values around it
        int minline, minlinein;

        // min of the nearest elements
        for (int j = (int)fmax(0, minin - 1); j < (int)fmin(width - 1, minin + 1); j++) {
            if (j == (int)fmax(0, minin - 1)) {
                minline = best[i*width + j];
            }
            else {
                if (best[i*width + j] < minline) {
                    minline = best[i*width + j];
                }
            }
        }
        (*path)[i - 1] = minn;
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
    read_in_img(&im, "C:/Users/PC/esc190-project2/3x4.bin");
    calc_energy(im,  &grad);
    print_grad(grad);
    dynamic_seam(grad, &best_array);
    print_best_array(best_array, (int)grad->width, (int)grad->height);
    recover_path(best_array, (int)grad->width, (int)grad->height, &path);
    print_recovered_path(path, (int)grad->height);
}