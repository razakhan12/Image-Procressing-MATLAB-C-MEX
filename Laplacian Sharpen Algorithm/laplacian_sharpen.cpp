

// Applies a laplacian sharpening operation to the input image
// (input parameters:  input and output arrays, 
//  scaling factor for adjusting strenth of effect)

#include "mex.h"
#include "matrix.h"

void spatial_filter (int imgCols, int imgRows, int filSize, const unsigned char input[], const int filmsk[], int out_img[]);

void sharpen(int imgCols, int imgRows, double scale, const unsigned char *input, unsigned char *output)
{
    int filter_out[imgCols*imgRows];
    int laplace[] = {-1, -1, -1, -1, 8, -1, -1, -1, -1};
    int i, j, index, temp;
    
    // call the spatial filter to get the sharpening mask
    spatial_filter(imgCols, imgRows, 3, input, laplace, filter_out);
    
    // calculate the output pixel values
    for (i = 0; i < imgRows; i++)
    {
        for (j= 0; j < imgCols; j++)
        {
            index = i*imgCols + j;
            
            temp = (int) input[index] + scale*filter_out[index];
            
            if (temp > 255)
                temp = 255;
            if (temp < 0)
                temp = 0;
            
            output[index] = temp;
        }
    }
    
}

void spatial_filter (int imgCols, int imgRows, int filSize, const unsigned char input[], const int filmsk[], int out_img[])
{
    
    unsigned char in_img[imgCols][imgRows];
    int i, j, x, y, temp;
    int index, filindex;
    int product, sum = 0;
    
    // fill the input and output arrays

    for (i = 0; i < imgRows; i++)
    {
        for (j = 0; j < imgCols; j++)
        {
            index = i*imgCols + j;
            in_img[j][i] = input[index];
            out_img[index] = 0;
        }
    }    
    
    for (i = filSize/2; i < (imgRows - filSize/2); i++)
    {
        for (j = filSize/2; j < (imgCols - filSize/2); j++)
        {
           
           
                // double for loop for moving through the filter window
                for (x = 0; x < filSize; x++)
                {
                    for (y = 0; y < filSize; y++)
                    {
                    
                        filindex = x + (filSize*y);
                        // multiply relavent pixel with filter value
                        product = in_img[j - filSize/2 + y][i - filSize/2 + x] * filmsk[filindex]; 
                    
                        // keep a running total of the products
                        sum += product;
                        
                    }
                    
                }
                // place the new pixel value in the output image
                index = i*imgCols + j;
                out_img[index] = sum;
                
                // reset accumulator variables
                sum = 0;
                product = 0;
            
        }
    }
    
}

void mexFunction (int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    int i, height, width, rad;
    double k;
    unsigned char *input, *output;
    
    // I/O errors will be "detected and rejected"
    if (nrhs != 2)
        mexErrMsgTxt("Error:  two input arguements required.");
    if (nlhs != 1)
        mexErrMsgTxt("Error:  one output required.");
    
    if ( !mxIsUint8(prhs[0]) )
        mexErrMsgTxt("Error:  input image must be 8-bit.");
    if ( !mxIsScalar(prhs[1]) )
        mexErrMsgTxt("Error:  sharpening strenth parameter must be scalar");
    
    
    // get dimensions of image
    const mwSize *imgdims = mxGetDimensions(prhs[0]);
    width = imgdims[0];
    height = imgdims[1];
    
    // assign input image to its own pointer
    input = (unsigned char *) mxGetPr(prhs[0]);
    
    // get the other input parameters
    k = mxGetScalar(prhs[1]);
    
    // prepare output array 
    mxArray *out = mxCreateNumericArray(2, imgdims, mxUINT8_CLASS, mxREAL);
    plhs[0] = out;
    output = (unsigned char *) mxGetData(out);
    
    // let the image processing begin!
    sharpen(width, height, k, input, output);
    
}
