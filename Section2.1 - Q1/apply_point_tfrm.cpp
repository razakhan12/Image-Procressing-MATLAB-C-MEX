// Raza Khan Mir Ali
// 500519680
// ELE882
// Assignment 1, Part 2, Question 1
// highlights a range of gray levels
#include "mex.h"

void procress (int length, int width, double c, int b, const unsigned char *input, unsigned char *output)
{
    for (int y = 0; y < length; y++)
        for (int x = 0; x < width; x++)
        {
            int ind = x*length + y;
            int temp = (input[ind] * c) + b;
            if(temp > 255)
            {
                temp = 255;
            }
            else if(temp < 0)
            {
                temp = 0;
            }
            output[ind] = temp;
        }
}

void mexFunction (int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    int i = 0;
    int c, b;
    unsigned char *input;
    if (nrhs != 3)
        mexErrMsgTxt("Aleast 3 outputs must be provided");
    if (nlhs != 1)
        mexErrMsgTxt("Aleast 1 outputs must be provided");
    const mwSize *dims = mxGetDimensions(prhs[0]);
    int height = dims[0];
    int width = dims[1];
    const mxArray *img = prhs[0];
     c = (unsigned char) mxGetScalar(prhs[1]);
    b = (unsigned char) mxGetScalar(prhs[2]);
    mxClassID input_type = mxGetClassID(img);
    mxArray *output = mxCreateNumericArray(2, dims, input_type, mxREAL);
    plhs[0] = output;
    if (mxIsUint8(img))
    {
        unsigned char *img_ptr = (unsigned char *)mxGetData(img);
        unsigned char *out_ptr = (unsigned char *)mxGetData(output);
        procress(height, width, c, b, img_ptr, out_ptr);
    }
}