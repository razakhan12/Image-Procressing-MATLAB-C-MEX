
#include "mex.h"

int findMin(int length, int width,const unsigned char *input)
{ int min = 256;
  
    for (int y = 0; y < length; y++)
        for (int x = 0; x < width; x++)
        {int ind = x*length + y;
            if(input[ind] < min)
            {
                min = input[ind];
            }
        }
  return min;
}
int findMax(int length, int width,const unsigned char *input)
{ int max = 0;
  
    for (int y = 0; y < length; y++)
        for (int x = 0; x < width; x++)
        {int ind = x*length + y;
            if(input[ind] > max)
            {
                max = input[ind];
            }
        }
  return max;
}
void procress (int length, int width, int s1,int s2,int r1, int r2, const unsigned char *input, unsigned char *output)
{
    int L = 256;//grey level for 8-bit image is 2^k=2^8=256
        double a1,a2,a3;
        //calculation of slopes
       if(s1 == 0)
       {
       a1= 0;
       }
       else
       {
       a1= (r1/s1);
       }
       a2 = (r2-r1)/(s2-s1);
       a3 = ((L-1)-r2)/((L-1)-s2);
   for (int y = 0; y < length; y++)
        for (int x = 0; x < width; x++)
        {
            int ind = x*length + y;
          
            if((input[ind] >= 0) && (input[ind] < r1))
            {
                output[ind] = a1 * input[ind];
            }
            else if((input[ind] >= r1) && (input[ind] < r2))
            {
                output[ind] = (a2 * (input[ind] - r1)) + s1;
            }
            else if((input[ind] >= r2) && (input[ind] <= (L-1)))
            {
                output[ind] = (a3 *(input[ind] - r2)) + s2;
            }
            else if (output[ind] >= 255){
            output[ind] = 0;
            }
            else if (output[ind] <= 0){
            output[ind] = 255;
            }
            else {
            output[ind] = output[ind];
            }
    }
}
void mexFunction (int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{    
  double *ar1, *ar2;
  int r1, r2, s1, s2;
    unsigned char *input;
    if (nrhs < 1)
        mexErrMsgTxt("You have passed no images");
    if (nlhs != 1)
        mexErrMsgTxt("Aleast 1 outputs must be provided");
  const mxArray *v1 = prhs[1];
  const mxArray *v2 = prhs[2];
  ar1 = (double *)mxGetData(v1);
  ar2  = (double *)mxGetData(v2);
  r1 = (int) ar1[0];
  s1 = (int) ar1[1];
  r2 = (int) ar2[0];
  s2 = (int) ar2[1];
  const mwSize *dims = mxGetDimensions(prhs[0]);
  int height = dims[0];
  int width = dims[1];
  mxArray *output = mxCreateNumericArray(2, dims, mxGetClassID(prhs[0]), mxREAL);
  plhs[0] = output;
  unsigned char *out_ptr = (unsigned char *)mxGetData(output);
  const mxArray *img = prhs[0];
  unsigned char *img_ptr = (unsigned char *)mxGetData(img);
  mexPrintf("r1 %d, r2 %d, s1 %d ,s2 %d, max: %d , min: %d",r1, r2 , s1, s2, findMax(height, width,img_ptr), findMin(height, width,img_ptr));
if (mxIsUint8(img))
 {
 procress(height, width,  s1, s2, r1, r2, img_ptr, out_ptr);
 }
}