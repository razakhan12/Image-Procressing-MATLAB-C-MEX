

// Applies an unsharp mask to the input image
// (input parameters:  radius of blurring kernel, 
//  scaling factor for adjusting strenth of effect, 
//  input and output arrays)

#include "mex.h"
#include "matrix.h"
#include <math.h>
#define PI 3.141592654

void spatial_filter (int imgCols, int imgRows, int filRadius, const unsigned char input[], const double filmsk[], int out_img[]);

void sharpen(int imgCols, int imgRows, int radius, double scale, const unsigned char *input, unsigned char *output)
{
    int imgdim[2], filter_out[imgCols*imgRows];
    //double blur_fil[radius*radius];
    double blur_fil[(2*radius+1)*(2*radius+1)], sigma = 1.0, gaus_scale, gaus_exp;
    int i, j, index, temp, filindex;
    
    // generate blurring filter kernel
    // (simple box filter for now)
    /*
    for (i = 0; i < radius*radius; i++)
        blur_fil[i] = 1.0/(radius*radius);
    */ 
    gaus_scale = 1.0/(2*PI*sigma*sigma);
    for (i = 0-radius; i <= radius; i++)
    {
        for (j = 0-radius; j<= radius; j++)
        {
            filindex = (i+radius) + (2*radius + 1)*(j+radius);
            gaus_exp = (i*i + j*j)/(2*sigma*sigma);
            blur_fil[filindex] = gaus_scale*exp(-gaus_exp);
        }
    }
    
    // call the spatial filter to get the blurred image
    spatial_filter(imgCols, imgRows, (2*radius+1), input, blur_fil, filter_out);
    
    // calculate the unsharp mask and output pixel values
    for (i = 0; i < imgRows; i++)
    {
        for (j= 0; j < imgCols; j++)
        {
            index = i*imgCols + j;
            
            temp = (int) scale * (input[index] - filter_out[index]);
            temp += input[index];
            
            if (temp > 255)
                temp = 255;
            if (temp < 0)
                temp = 0;
            
            output[index] = temp;
        }
    }
    
}

void spatial_filter (int imgCols, int imgRows, int filRadius, const unsigned char input[], const double filmsk[], int out_img[])
{
    
    int index, filindex, i, j, x, y, temp;
    unsigned char in_img[imgCols][imgRows];
    double product, sum = 0.0, weight = 0.0;
    
    // fill the input and output arrays

    for (i = 0; i < imgRows; i++)
    {
        for (j = 0; j < imgCols; j++)
        {
            index = i*imgCols + j;
            in_img[j][i] = input[index];
            out_img[index] = input[index];
        }
    }
    
    for (i = filRadius/2; i < (imgRows - filRadius/2); i++)
    {
        for (j = filRadius/2; j < (imgCols - filRadius/2); j++)
        {
           
           
                
                // double for loop for moving through the filter window
                for (x = 0; x < filRadius; x++)
                {
                    for (y = 0; y < filRadius; y++)
                    {
                    
                        filindex = x + (filRadius*y);
                        
                        // multiply relavent pixel with filter value
                        product = in_img[j - filRadius/2 + y][i - filRadius/2 + x] * filmsk[filindex]; 
                    
                        // keep a running total of the products
                        sum += product;
                        
                    }
                    
                }
                // place the new pixel value in the output image
                index = i*imgCols + j;
                out_img[index] = (int) sum;
                
                // reset accumulator variables
                sum = 0.0;
                product = 0.0;
                //weight = 0.0;
            
        }
    }
    
}

void mexFunction (int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    int i, height, width, rad;
    double k;
    unsigned char *input, *output;
    
    // I/O errors will be "detected and rejected"
    if (nrhs != 3)
        mexErrMsgTxt("Error:  three input arguements required.");
    if (nlhs != 1)
        mexErrMsgTxt("Error:  one output required.");
    
    if ( !mxIsUint8(prhs[0]) )
        mexErrMsgTxt("Error:  input image must be 8-bit.");
    if ( !mxIsScalar(prhs[1]) )
        mexErrMsgTxt("Error:  radius must be scalar");
    if ( !mxIsScalar(prhs[2]) )
        mexErrMsgTxt("Error:  sharpening strenth parameter must be scalar");
    
    
    // get dimensions of image
    const mwSize *imgdims = mxGetDimensions(prhs[0]);
    width = imgdims[0];
    height = imgdims[1];
    
    // assign input image to its own pointer
    input = (unsigned char *) mxGetPr(prhs[0]);
    
    // get the other input parameters
    rad = (int) mxGetScalar(prhs[1]);
    k = mxGetScalar(prhs[2]);
    
    // prepare output array 
    mxArray *out = mxCreateNumericArray(2, imgdims, mxUINT8_CLASS, mxREAL);
    plhs[0] = out;
    output = (unsigned char *) mxGetData(out);
    
    // let the image processing begin!
    sharpen(width, height, rad, k, input, output);
    
}
