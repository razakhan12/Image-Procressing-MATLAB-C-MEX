
// average_images
#include "mex.h"

void procress (int length, int width, int n,const unsigned char *input, unsigned char *output)
{
           for (int y = 0; y < length; y++)
        for (int x = 0; x < width; x++)
        {
            int ind = x*length + y;
            output[ind] = ((1/n)*input[ind]) + output[ind];
        }
}
void mexFunction (int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    unsigned char *input;
    //check if there are images
    if (nrhs < 1)
        mexErrMsgTxt("You have passed no images");
    if (nlhs != 1)
        mexErrMsgTxt("Aleast 1 outputs must be provided");
    //check if putput is not provided
    const mwSize *dims = mxGetDimensions(prhs[0]);
    int height = dims[0];
    int width = dims[1];
    // dimensions will be the same for all other pixels
     mxArray *output = mxCreateNumericArray(2, dims, mxGetClassID(prhs[0]), mxREAL);
     plhs[0] = output;
     unsigned char *out_ptr = (unsigned char *)mxGetData(output);
    //only one output array
for(int image = 1; image <= nrhs; image++)
{
    const mxArray *img = prhs[image];
    if (mxIsUint8(img))
    {
        unsigned char *img_ptr = (unsigned char *)mxGetData(img);
        procress(height, width, image, img_ptr, out_ptr);
    }
}
}