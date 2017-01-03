

#include "mex.h"

void ctr_stretch (int length, int height, const unsigned char *input, unsigned char *output)
{
    int maxIndex = length * height, i, max = 0, min = 255;
    double denom, Rmin, R, temp;
    
    //find the min and max values
    for (i = 0; i < maxIndex; i++)
    {
        if (input[i] > max)
            max = input[i];
        if (input[i] < min)
            min = input[i];
    }
    
    denom = (double)(max - min);
    Rmin = (double)min;
    
    for (i = 0; i < maxIndex; i++)
    {
        R = (double)input[i];
        temp = 255.0 * ((1.0/denom)*R - (Rmin/denom));
        output[i] = (unsigned char)temp;
    }
    
}

void mexFunction (int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    int width, height, i;
    unsigned char *input, *output;
    
    // I/O errors will be "detected and rejected"
    if (nrhs != 1)
        mexErrMsgTxt("Error:  one input image required.");
    if (nlhs != 1)
        mexErrMsgTxt("Error:  one output required.");
    if ( !mxIsUint8(prhs[0]) )
        mexErrMsgTxt("Error:  input image must be 8-bit.");
        
    // get dimensions of image
    const mwSize *dims = mxGetDimensions(prhs[0]);
    height = dims[0];
    width = dims[1];
    
    // assign input to its own pointer
    input = (unsigned char *) mxGetPr(prhs[0]);
    
    // prepare output array 
    mxArray *out = mxCreateNumericArray(2, dims, mxUINT8_CLASS, mxREAL);
    plhs[0] = out;
    output = (unsigned char *) mxGetData(out);
    
    // let the image processing begin!
    ctr_stretch(width, height, input, output);
    
}
