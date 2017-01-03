#include "mex.h"
#include<math.h>
#define PI 3.141592654

void convert (int length, int height, const double *input, double *output)
{
    int maxIndex = length * height, i;
    double twopi = ((2.0/3.0)*PI);
    double fourpi = ((4.0/3.0)*PI);
    double red, green, blue;
    double hue, sat, intense;
    double top, bottom, theta, cmin, temp;
    
    for (i = 0; i < maxIndex; i++)
    {
     // get pixel RGB values
     hue = input[i];
     sat = input[i+(maxIndex)];
     intense = input[i+(2*maxIndex)];
     
     if((0 <= hue) && (hue < twopi))
     {
      blue = intense*(1.0 - sat);
      temp = ((sat * cos(hue))/cos((PI/3.0)-hue));
      red = intense*(1.0 + temp);
      green = (3.0 *intense) - (red+blue);
     }
     if((twopi <= hue) && (hue < fourpi))
     {
          red = intense*(1.0 + sat);
          temp = ((sat * cos(hue-twopi))/cos((PI/3.0)-(hue-twopi)));
          green = intense*(1.0+temp);
          blue = (3.0*intense) - (red+green);
     }
      if(( fourpi <= hue) && (hue <= (2.0*PI)))
     {
              green = intense*(1.0 -sat);
              temp = ((sat * cos(hue - fourpi))/cos((PI/3.0)-(hue - fourpi)));
                
              blue = intense*(1.0 + temp);
              red = (3.0*intense) - (green+blue);
     }
     // place HSI values in output array
     output[i] = red;
     output[i+maxIndex] = green;
     output[i+(2*maxIndex)] = blue;
    }
    
}

void mexFunction (int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    int width, height, i;
    double *input, *output;
    unsigned char *img;
    
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
    
    // assign input to its own pointer
    if (mxIsDouble(prhs[0]))
        input = (double *) mxGetPr(prhs[0]);

    
    // prepare output array 
    mxArray *out = mxCreateNumericArray(3, dims, mxDOUBLE_CLASS, mxREAL);
    plhs[0] = out;
    output = (double *) mxGetData(out);
    
    // let the image processing begin!
    convert(width, height, input, output);
    
}