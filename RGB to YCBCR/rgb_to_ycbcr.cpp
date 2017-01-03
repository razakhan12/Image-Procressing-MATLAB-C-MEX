

// converts an RGB image to YCbCr
// accepts 8-bit or 64-bit floating point RGB images
// outputs 8-bit or 64-bit floating point YCbCr images

#include "mex.h"
#include<math.h>
#define PI 3.141592654

void convertDB (int length, int height, const double *input, double *output)
{
    int maxIndex = length * height, i;
    double red, green, blue;
    double Y, Cb, Cr;
    double temp;
    
    for (i = 0; i < maxIndex; i++)
    {
        // get pixel RGB values
        red = input[i];
        green = input[i+(maxIndex)];
        blue = input[i+(2*maxIndex)];
        
        // calculate YCbCr values
        Y = 0.299*red + 0.587*green + 0.114*blue;
        Cb = 0.5*blue - 0.3313*green - 0.1687*red;
        Cr = 0.5*red - 0.4187*green - 0.0813*blue;
        
        // place YCbCr values in output array
        output[i] = Y;
        output[i+maxIndex] = Cb;
        output[i+(2*maxIndex)] = Cr;
        
    }
    
}

void convert8 (int length, int height, const unsigned char *input, unsigned char *output)
{
    int maxIndex = length * height, i;
    unsigned char red, green, blue;
    unsigned char Y, Cb, Cr;
    int temp;
    
    for (i = 0; i < maxIndex; i++)
    {
        // get pixel RGB values
        red = input[i];
        green = input[i+(maxIndex)];
        blue = input[i+(2*maxIndex)];
        
        // calculate YCbCr values
        Y = round(0.299*red + 0.587*green + 0.114*blue);
        temp = round(0.5*blue - 0.3313*green - 0.1687*red);
        Cb = temp + 128;
        temp = round(0.5*red - 0.4187*green - 0.0813*blue);
        Cr = temp + 128;
        
        // place YCbCr values in output array
        output[i] = Y;
        output[i+maxIndex] = Cb;
        output[i+(2*maxIndex)] = Cr;
        
    }
    
}

void mexFunction (int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    int width, height, i;
    double *inputDB, *outputDB;
    unsigned char *input8, *output8;
    
    // I/O errors will be "detected and rejected"
    if (nrhs != 1)
        mexErrMsgTxt("Error:  one input image required.");
    if (nlhs != 1)
        mexErrMsgTxt("Error:  one output required.");
    if ( !( mxIsUint8(prhs[0]) || mxIsDouble(prhs[0])) )
        mexErrMsgTxt("Error:  input image must be 8-bit or 64-bit floating point");
        
    // get dimensions of image
    const mwSize *dims = mxGetDimensions(prhs[0]);
    height = dims[0];
    width = dims[1];
    
    
    if (mxIsDouble(prhs[0]))
    {
        // assign input to its own pointer
        inputDB = (double *) mxGetPr(prhs[0]);
        
        // prepare output array 
        mxArray *out = mxCreateNumericArray(3, dims, mxDOUBLE_CLASS, mxREAL);
        plhs[0] = out;
        outputDB = (double *) mxGetData(out);
    
        // let the image processing begin!
        convertDB(width, height, inputDB, outputDB);
    }
    else
    {
        // assign input to its own pointer
        input8 = (unsigned char *) mxGetPr(prhs[0]);
        
        // prepare output array 
        mxArray *out = mxCreateNumericArray(3, dims, mxUINT8_CLASS, mxREAL);
        plhs[0] = out;
        output8 = (unsigned char *) mxGetData(out);
    
        // let the image processing begin!
        convert8(width, height, input8, output8);
    }
    
    
    
}