

// Applies a thresholding operation on the input image, using the 
// parameter provided.  All I/O is 64-bit floating point.  Threshold 
// value between 0 and 1 enforced.

#include "mex.h"
#include "matrix.h"

void threshold(int height, int width, double thresh, const double *input, double *output)
{
    int imgSize = height * width;
    int i;
    
    if (thresh > 1.0)
        thresh = 1.0;
    if (thresh < 0.0)
        thresh = 0.0;
    
    for (i = 0; i < imgSize; i++)
    {
        if (input[i] >= thresh)
            output[i] = 1.0;
        else
            output[i] = 0.0;
    }
}

void mexFunction (int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    int width, height;
    double *input, *output, thresh;
    //double scalars[3];
    
    // I/O errors will be "detected and rejected"
    if (nrhs != 2)
        mexErrMsgTxt("Error:  two input arguements required.");
    if (nlhs != 1)
        mexErrMsgTxt("Error:  one output required.");
    if ( !mxIsDouble(prhs[0]) )
        mexErrMsgTxt("Error:  input image must be 64-bit floating point.");
    if ( !mxIsScalar(prhs[1]) )
            mexErrMsgTxt("Error:  threshold value must be a scalar number.");
            
    // get dimensions of image
    const mwSize *dims = mxGetDimensions(prhs[0]);
    height = dims[0];
    width = dims[1];
    
    // assign input image to its own pointer
    input = (double *) mxGetPr(prhs[0]);
    // get scalar values and assign them to variables
    thresh = mxGetScalar(prhs[1]);
    
    // prepare output array 
    mxArray *out = mxCreateNumericArray(2, dims, mxDOUBLE_CLASS, mxREAL);
    plhs[0] = out;
    output = (double *) mxGetData(out);
    
    // let the image processing begin!
    threshold(width, height, thresh, input, output);
    
}
