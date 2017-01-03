#include "mex.h"
void histogram_equalize(int width, int height, const unsigned char *input, unsigned char *output)
{
    int MN = width * height;
    double PDF[255] = {0};
    double CDF[255] ={0};
    double count[255] = {0};
    const unsigned char *inr = &input[0];//Y
    const unsigned char *ing = &input[width*height];//Cb
    const unsigned char *inb = &input[2*width*height];//Cr
    
    //The outputs of RGB
    unsigned char *R = &output[0];
    unsigned char *G = &output[width*height];
    unsigned char *B = &output[2*width*height];
for(int c = 0; c<3;c++)
{
    
//--------------------------------------------------------------------------
    if(c == 0) //red
    {
         for(int current = 0; current <= 255; current++)
    {
        for (int y = 0; y < height; y++)
        {
            for (int x = 0; x < width; x++)
            {
                int ind = x*height + y; 
                if (inr[ind] == current)
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
             int a = CDF[(inr[ind])]*255;
               if(a>255)
               R[ind] = 255;
               if(a < 0)
               R[ind] = 0;
               R[ind] = (unsigned char) a;
            }
        }
            CDF[i] = PDF[i] + CDF[i-1]; //we get the total CDF and add 
                                        //it
    }
         
    }
    for(int a =0; a <256; a++)
    {
        PDF[a] =0;
        CDF[a] =0;
        count[a] =0;
    }
    //--------------------------------------------------------------------------
     if(c == 1) //green
    {
          for(int current = 0; current <= 255; current++)
    {
        for (int y = 0; y < height; y++)
        {
            for (int x = 0; x < width; x++)
            {
                int ind = x*height + y; 
                if (ing[ind] == current)
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
             int a = CDF[(ing[ind])]*255;
             if(a>255)
               G[ind] = 255;
               if(a < 0)
               G[ind] = 0;
               G[ind] = (unsigned char) a;
            }
        }
            CDF[i] = PDF[i] + CDF[i-1]; //we get the total CDF and add 
                                        //it
    }
    }
      for(int a =0; a <256; a++)
    {
        PDF[a] =0;
        CDF[a] =0;
        count[a] =0;
    }
//--------------------------------------------------------------------------

     if(c == 2) //blue
    {
          for(int current = 0; current <= 255; current++)
    {
        for (int y = 0; y < height; y++)
        {
            for (int x = 0; x < width; x++)
            {
                int ind = x*height + y; 
                if (inb[ind] == current)
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
             int a = CDF[(inb[ind])]*255;
             if(a>255)
               B[ind] = 255;
               if(a < 0)
               B[ind] = 0;
               B[ind] = (unsigned char) a;
            }
        }
            CDF[i] = PDF[i] + CDF[i-1]; //we get the total CDF and add 
                                        //it
    }
    }
      for(int a =0; a <256; a++)
    {
        PDF[a] =0;
        CDF[a] =0;
        count[a] =0;
    }
}
//--------------------------------------------------------------------------

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