// Applies a spatial filter of arbitrary size to the input image

#include "mex.h"
#include "matrix.h"

void filter (int imgdim[], int fildim[], const double *input, const double *filmsk, double *output)
{
    int filHeight = fildim[0], filWidth = fildim[1];
    int imgCols = imgdim[0], imgRows = imgdim[1];
    int index, filindex, i, j, x, y, temp;
    int deriv = 0;
    double product, sum = 0.0, weight = 0.0;
    double in_img[imgCols][imgRows], filter[filHeight][filWidth];
    // check if derivative kernel
    for (i = 0; i < (filHeight * filWidth); i++)
        sum += filmsk[i];
    if (sum == 0.0)
        deriv = 1;
    else
        sum = 0.0;
    // fill the output array from the input if a blurring filter
    // (all but the border will be replaced)
    for (i = 0; i < imgRows; i++)
    {
        for (j = 0; j < imgCols; j++)
        {
            index = i*imgCols + j;
            if (deriv == 0)
                output[index] = input[index];
            in_img[j][i] = input[index];
            // also fill a 2-d array from the input for easier addressing
        }
    }
    
    for (i = filWidth/2; i < (imgRows - filWidth/2); i++)
    {
        for (j = filHeight/2; j < (imgCols - filHeight/2); j++)
        {
                // double for loop for moving through the filter window
                for (x = 0; x < filWidth; x++)
                {
                    for (y = 0; y < filHeight; y++)
                    {
                         // calculate index of filter value
                        filindex = x + (filHeight*y);
                        // multiply relavent pixel with filter value
                        product = in_img[j - filWidth/2 + y][i - filHeight/2 + x] * filmsk[filindex]; 
                        // keep a running total of the products
                        sum += product;
                    }
                }
                // place the new pixel value in the output image
                index = i*imgCols + j;
                output[index] = sum;
                //output[index] = input[index];  // for debugging
                // reset accumulator variables
                sum = 0.0;
                product = 0.0;
                //weight = 0.0;
        }
    }
    
}

void mexFunction (int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    int i, imgdim[2], fildim[2];
    double *input, *mask, *output;
    
    // I/O errors will be "detected and rejected"
    if (nrhs != 2)
        mexErrMsgTxt("Error:  two input arguements required.");
    if (nlhs != 1)
        mexErrMsgTxt("Error:  one output required.");
    for (i = 0; i < 2; i++)
    {
        if ( !mxIsDouble(prhs[i]) )
            mexErrMsgTxt("Error:  inputs must be in double-precision flaoting point format.");
    }
    
    // get dimensions of image and filter mask
    const mwSize *imgdims = mxGetDimensions(prhs[0]);
    const mwSize *fildims = mxGetDimensions(prhs[1]);
    
    // prepare integer dimension arrays
    for (i = 0; i <2; i++)
    {
        imgdim[i] = imgdims[i];
        fildim[i] = fildims[i];
    }
    
    // assign inputs to their own pointers
    input = mxGetPr(prhs[0]);
    mask = mxGetPr(prhs[1]);
    
    // prepare output array 
    mxArray *out = mxCreateNumericArray(2, imgdims, mxDOUBLE_CLASS, mxREAL);
    plhs[0] = out;
    output = (double *) mxGetData(out);
    
    // let the image processing begin!
    filter(imgdim, fildim, input, mask, output);
    
}