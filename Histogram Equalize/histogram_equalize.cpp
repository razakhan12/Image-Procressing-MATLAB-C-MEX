
#include "mex.h"
void histogram_equalize(int width, int height, const unsigned char *input, unsigned char *output)
{ 
    int MN = width * height;
    double PDF[255] = {0};
    double CDF[255] ={0};
    double count[255] = {0};

    for(int current = 0; current <= 255; current++)
    {
        for (int y = 0; y < height; y++)
        {
            for (int x = 0; x < width; x++)
            {
                int ind = x*height + y; 
                if (input[ind] == current)
                {
                    count[current] = count[current] + 1; 
                }
            }
        }
    }
     for(int i = 0; i <= 255; i++)
    {
        for (int y = 0; y < height; y++)
        {
            for (int x = 0; x < width; x++)
            {
                int ind = x*height + y; 
                PDF[i] = count[i]/MN;
             output[ind] = CDF[(input[ind])]*255;
            }
        }
            CDF[i] = PDF[i] + CDF[i-1]; //we get the total CDF and add 
                                        //it
    }
}

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    if (nlhs != 1)
        mexErrMsgTxt("histogram_equalize can only accept one input argument");
    if (nrhs != 1)
        mexErrMsgTxt("histogram_equalize requires one output argument");
    
    const mxArray *img = prhs[0];
    if (!mxIsUint8(img))
        mexErrMsgTxt("histogram_equalize can only accept images of type 'unsigned char'");
    
    mwSize ndims = mxGetNumberOfDimensions(img);
    const mwSize *dims = mxGetDimensions(img);
    int height   = dims[0];
    int width    = dims[1];
    
    mxClassID input_type = mxGetClassID(img);
    mxArray *output = mxCreateNumericArray(ndims, dims, input_type, mxREAL);
    plhs[0] = output;
    
    if (mxIsUint8(img))
    {
        unsigned char *img_ptr = (unsigned char *)mxGetData(img);
        unsigned char *out_ptr = (unsigned char *)mxGetData(output);
        histogram_equalize(width, height, img_ptr, out_ptr);
    }   
}