#include "mex.h"

void histogram_equalize(int width, int height, int channels, const unsigned char *input, unsigned char *output)
{
    for (int c = 0; c < 1; c++){
        int colour = c*width*height;
        double cdf = 0;
        int L = 256;
        double histogram[256]={0};
        int a[256]={0};
        int size = height*width;
        const unsigned char *input_img = &input[colour];
        unsigned char *out = &output[colour];
        
        for (int y = 0; y < height; y++){
            for (int x = 0; x < width; x++){
                int ind = x*height + y;
                histogram[input_img[ind]] +=  1;
            }
        }
        for (int i = 0; i < L; i++) {
            cdf +=  histogram[i]/size;
            a[i] = 255 * cdf;
        }
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width;j++) {
                int ind = j*height + i;
                out[ind] = a[input_img[ind]];
            }
        }
    }
}
void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    const mxArray *img = prhs[0];
    mwSize ndims = mxGetNumberOfDimensions(img);
    const mwSize *dims = mxGetDimensions(img);
    int height   = dims[0];
    int width    = dims[1];
    int channels = (ndims == 2) ? 1 : dims[2];
    
    mxClassID input_type = mxGetClassID(img);
    
    mxArray *output = mxCreateNumericArray(ndims, dims, input_type, mxREAL);
    plhs[0] = output;

    const unsigned char *img_ptr = (unsigned char *)mxGetData(img);
    unsigned char *out_ptr = (unsigned char *)mxGetData(output);
    
    histogram_equalize(width, height, channels, img_ptr, out_ptr); 
}
