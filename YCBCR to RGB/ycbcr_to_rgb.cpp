#include "mex.h"
#include<math.h>
#define PI 3.141592654

void convertDB (int width, int height, const double *input, double *output)
{
    int maxIndex = width * height, i;
    double red, green, blue;
    double Y, Cb, Cr;
    double temp;
    double *R = &output[0];
    double *G = &output[width*height];
    double *B = &output[2*width*height];
    const double *inY = &input[0];//Y
    const double *inCb = &input[width*height];//Cb
    const double *inCr = &input[2*width*height];//Cr
    for (int c = 0; c <3; c++){
    for(int i = 0 ; i<height; i++){
    for(int j = 0; j<width; j++){
                int ind = j*height + i;
                double Y = inY[ind];
                double Cb = inCb[ind];
                double Cr = inCr[ind];
                
                R[ind] = Y +  0*Cb + 1.402*(Cr);
                G[ind] = Y -0.344136*(Cb)  - 0.714136*(Cr);
                B[ind] = Y + 1.772*(Cb) - 0*Cr;
            } 
        }
    }
    
}

void convert8 (int width, int height, const unsigned char *input, unsigned char *output)
{
    int maxIndex = width * height, i;
    unsigned char red, green, blue;
    unsigned char Y, Cb, Cr;
    unsigned char temp;
    unsigned char *R = &output[0];
    unsigned char *G = &output[width*height];
    unsigned char *B = &output[2*width*height];
    
    const  unsigned char *inY = &input[0];//Y
    const  unsigned char *inCb = &input[width*height];//Cb
    const  unsigned char *inCr = &input[2*width*height];//Cr
    
    unsigned char c = 128;
    int a =0;
    for (int c = 0; c <3; c++){
    for(int i = 0 ; i<height; i++){
    for(int j = 0; j<width; j++){
                int ind = j*height + i;
                unsigned char Y = inY[ind];
                unsigned char Cb = inCb[ind];
                unsigned char Cr = inCr[ind];
                
               a = round(Y+1.402*(Cr-128));
               if(a>255)
               R[ind] = 255;
               if(a < 0)
               R[ind] = 0;
               R[ind] = (unsigned char) a;
                
               a = round(Y-(0.34414*(Cb-128))-(0.71414*(Cr-128)));
               if(a>255)
               G[ind] = 255;
               if(a < 0)
               G[ind] = 0;
               G[ind] = (unsigned char) a; 
               
               a = round(Y+1.772*(Cb-128));
               if(a>255)
               B[ind] = 255;
               if(a < 0)
               B[ind] = 0;
               B[ind] = (unsigned char) a;
            } 
        }
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