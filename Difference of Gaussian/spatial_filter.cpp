

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
    // check if derivative kernel
    for (i = 0; i < (filHeight * filWidth); i++)
        sum += filmsk[i];
    if (sum == 0.0)
        deriv = 1;
    else
        sum = 0.0;
    for (i = 0; i < imgRows; i++)
    {
        for (j = 0; j < imgCols; j++)
        {
            // deal with pixels on the perimeter of the image
            // far left and far right are ignored (output image padded)
            // wrap-around on the top and bottom are allowed
            if ( j < filWidth/2 || j >= (imgCols - (filWidth/2)) )
            {
                index = j * imgRows + i;
                
                // zero-pad if passed a derivative kernel, 
                // pass-through original image pixels otherwise
                if (deriv == 1)
                    output[index] = 0.0;
                else
                    output[index] = input[index];
                    //output[index] = 1.0;  // for debugging
            }
            else
            {
                // double for loop for moving through the filter window
                for (x = 0; x < filHeight; x++)
                {
                    for (y = 0; y < filWidth; y++)
                    {
                        // calcualte index of one of the input pixels under the filter window
                        //index = (j + x - filHeight/2)*imgRows + (i + y - filWidth/2);  // output split along horiz centre
                        index = j*(imgRows) + (x - filHeight/2) + (i + y - filWidth/2);  // output looks right
                        //index = j*imgRows + (i + y - filWidth/2);  // only works with 1-d arrays
                        
                        // calculate index of corrosponding filter value
                        filindex = y + (filWidth*x);
                    
                        // multiply relavent pixel with filter value
                        product = input[index] * filmsk[filindex]; 
                    
                        // keep a running total of the products
                        sum += product;
                        //weight +=filmsk[filindex];
                    }
                }
                // place the new pixel value in the output image
                index = j * imgRows + i;
                //output[index] = sum/weight;
                output[index] = sum;
                //output[index] = input[index];  // for debugging
                // reset accumulator variables
                sum = 0.0;
                product = 0.0;
                //weight = 0.0;
            }
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
