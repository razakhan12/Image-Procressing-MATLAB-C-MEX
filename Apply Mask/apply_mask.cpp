

#include "mex.h"
#include "matrix.h"

void appMask (int width, int height, const unsigned char *imgA, const unsigned char *imgB, 
              const unsigned char *mask, unsigned char *output)
{
    int maxIndex = width * height , i;
    
    for (i = 0; i < maxIndex; i++)
    {
        if (mask[i] > 0)
            output[i] = imgA[i];
        else
            output[i] = imgB[i];
    }
}

void mexFunction (int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    int width, height, i;
    unsigned char *imgA, *imgB, *mask, *output;
    
    // I/O errors will be "detected and rejected"
    if (nrhs != 3)
        mexErrMsgTxt("Error:  three input images required.");
    if (nlhs != 1)
        mexErrMsgTxt("Error:  one output required.");
    for (i = 0; i < 2; i++)
    {
        if ( !mxIsUint8(prhs[i]) )
            mexErrMsgTxt("Error:  input images must be 8-bit.");
    }
    if ( !mxIsLogical(prhs[2]) )
        mexErrMsgTxt("Error:  mask should be boolean.");
    
    // get dimensions of image A (and assume the others are the same)
    const mwSize *dims = mxGetDimensions(prhs[0]);
    height = dims[0];
    width = dims[1];
    
    // assign inputs to their own pointers
    imgA = (unsigned char *) mxGetPr(prhs[0]);
    imgB = (unsigned char *) mxGetPr(prhs[1]);
    mask = (unsigned char *) mxGetPr(prhs[2]);
    
    // prepare output array 
    mxArray *out = mxCreateNumericArray(2, dims, mxUINT8_CLASS, mxREAL);
    plhs[0] = out;
    output = (unsigned char *) mxGetData(out);
    
    // let the image processing begin!
    appMask(width, height, imgA, imgB, mask, output);
    
}
