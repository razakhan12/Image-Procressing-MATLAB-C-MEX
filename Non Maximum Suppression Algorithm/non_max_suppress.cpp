//Raza Khan Mir Ali
//500519680
//create windows such as 1X(5 or more) and (5 or more) x 1   

#include "mex.h"
#include <math.h>
void non_max_suppress(double *img , double *output, int height_img, int width_img, double height_window, double width_window )
{
 int counter = 0;
int inc = 0;
double te =0; 
// double *array;
//location of the pixels
 //use 1d windows
if((height_window == 1) && (width_window > 1) )//horizontal
{
    for (int y = 0; y < height_img; y++)
    
   {
         for (int x = 0; x < width_img; x++)
        {
            int ind = x*height_img + y;
            if( inc == 0 )
            {
                for(int in = 0; in < (int)(width_window-1); in++)
                   {
                   // printf("%f", (x+inc)*height_img + y);
                     if(te < img[(x+inc)*height_img + y])
                         {
                          te = img[(x+inc)*height_img + y];

                         }
                           inc++;
                                 
                       if (inc == (int)(width_window-1.00))
                           {
                            inc = 0;
                           // printf("inc");
                           }
                         }
            }
            if(img[ind] == te)
            {
                output[ind] = te;
            }
            else
            {
                output[ind] = 0;
            }
            }
        }
}
 if((width_window == 1) && (height_window > 1) )//vertical
{
      for (int x = 0; x < width_img; x++)
                                            {
       for (int y = 0; y < height_img; y++)
        {
            int ind = x*height_img + y;
            if( inc == 0 )
            {
                for(int in = 0; in < (int)(height_window-1); in++)
                   {
                   // printf("%f", (x+inc)*height_img + y);
                     if(te < img[(x)*height_img + (y+inc)])
                         {
                          te = img[(x+inc)*height_img + (y+inc)];
                         // printf("%f", te);
                        //  printf("hello!");
                         }
                           inc++;
                                 
                       if (inc == (int)(height_window-1.00))
                           {
                            inc = 0;
                           // printf("inc");
                           }
                         }
            }
            if(img[ind] == te)
            {
                output[ind] = te;
            }
            else
            {
                output[ind] = 0;
            }
            }
        }
}
    }
/* Main mex function */
void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    mxArray *img_m;
    mxArray *output_m;
    double *img;
    double *output;
    double height_window;
    double width_window;
    /* Duplicate input image and kernel */
    img_m = mxDuplicateArray(prhs[0]);
    height_window = mxGetScalar(prhs[1]);
    printf("\n Height: %f", height_window);
    width_window = mxGetScalar(prhs[2]);
    printf("\n Width: %f", height_window);
    /* Get Dimensions of input image */
    const mwSize *image_dims = mxGetDimensions(prhs[0]);
    int height_img = image_dims[0];
    int width_img = image_dims[1];
    /* Create matrix for output image*/
    output_m = plhs[0] = mxCreateNumericArray (2, image_dims, mxDOUBLE_CLASS, mxREAL );
    /* Link pointers for images */
    img = (double*)mxGetData(img_m);
    output = (double*)mxGetData(output_m);
    non_max_suppress(img, output, height_img, width_img, height_window, width_window);
    return;
}