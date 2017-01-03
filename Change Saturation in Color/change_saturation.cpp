#include "mex.h"
#include<math.h>
#define PI 3.141592654
void convert (int length, int height, double val ,const double *input, double *output)
{
    int maxIndex = length * height, i;
    double red, green, blue;
    double hue, sat, intense;
    double top, bottom, theta, cmin, temp;
    for (i = 0; i < maxIndex; i++)
    {
        // get pixel RGB values
        red = input[i];
        green = input[i+(maxIndex)];
        blue = input[i+(2*maxIndex)];
        // find theta
        top = 0.5*((red-green)+(red-blue));
        bottom = sqrt((red-green)*(red-green) + (red-blue)*(green-blue));
        theta = acos(top/bottom);
        // find cmin, the minimum subpixel value 
        cmin = red;
        if (cmin > green)
            cmin = green;
        if (cmin > blue)
            cmin = blue;
        // pixel hue
        if (blue > green)
            hue = 2*PI - theta;
        else
            hue = theta;
        // pixel saturation
        temp = 3.0/(red+green+blue);
        sat = 1 - (temp*cmin);
        sat = (sat + val);
        if(sat > 1)
        {
            sat = 1;
        }
        if(sat < 0) 
        {
            sat = 0;
        }
        // pixel intensity
        intense = (red+green+blue)/3.0;
        // place HSI values in output array
        output[i] = hue;
        output[i+maxIndex] = sat;
        output[i+(2*maxIndex)] = intense;
    }
}

void mexFunction (int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    int width, height, i;
    double *input, *output;
    unsigned char *img;
    // I/O errors will be "detected and rejected"
    if (nrhs != 2)
        mexErrMsgTxt("Error:  one parameter is missing.");
    if (nlhs != 1)
        mexErrMsgTxt("Error:  one output required.");
    if ( !( mxIsUint8(prhs[0]) || mxIsDouble(prhs[0])) )
        mexErrMsgTxt("Error:  input image must be 8-bit or 64-bit floating point");
    // get dimensions of image
    const mwSize *dims = mxGetDimensions(prhs[0]);
    height = dims[0];
    width = dims[1];
    // assign input to its own pointer
    if (mxIsDouble(prhs[0]))
        input = (double *) mxGetPr(prhs[0]);
    else
    {
        img = (unsigned char *) mxGetPr(prhs[0]);
        double temp[width*height*3];
        for (i = 0; i < width*height*3; i++)
            temp[i] = (double) img[i]/255.0;
        input = temp;
    }
      double value = mxGetScalar(prhs[1]);
    // prepare output array 
    mxArray *out = mxCreateNumericArray(3, dims, mxDOUBLE_CLASS, mxREAL);
    plhs[0] = out;
    output = (double *) mxGetData(out);
    // let the image processing begin!
    convert(width, height, value, input, output);
    
}