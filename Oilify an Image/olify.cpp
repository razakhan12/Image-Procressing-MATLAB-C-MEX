#include "mex.h"
#include "math.h"
void adaptive_histogram(const int filwidth, const int filheight,const int imgcol,const int N, const int gamma, const int imgrow, double *input, double *output)
{ 
    int img_size = filwidth * filheight;
    double cdf = 0;
    double counter[256];
    double acc[256];
    double imax = 0.0;
    int hmax = 0;
    double test;
    double A = 0;
    double B = 0;
    double w =0.0;
    unsigned char in_image[imgcol][imgrow];
    unsigned char out_image[imgcol][imgrow];
    int locH =imgcol-((int)((double)(filheight/2)));
    int locW = imgrow-((int)((double)(filwidth/2)));
    for(int i = 0 ; i<imgcol; i++){
        for(int j = 0; j<imgrow; j++){
            int ind = j*imgcol + i;
            in_image[i][j] = input[ind];
            if(imax < in_image[i][j])
            {
                    imax = in_image[i][j];
            }
        } //converting to 2d array
    }
    for(int a = ((int)((double)(filheight/2))) ; a < locH; a++)
    {
        for(int b = ((int)((double)(filwidth/2))); b < locW; b++)
        {     
            for(int i = (a - ((int)((double)(filheight/2)))); i<=(filheight+(a - ((int)((double)(filheight/2)))));i++){
                for(int j =(b - ((int)((double)(filwidth/2)))); j<=(filwidth+(b - ((int)((double)(filwidth/2)))));j++)
                {
                    int location = (int)(floor(in_image[i][j]/imax)*(N-1));
                    counter[location] += 1;
                    acc[location] = acc[location] + in_image[i][j];

                }
            }
            for (int an = 0; an<256; an++)
            { // doing the cdf calculation
                if(hmax < counter[an])
                {
                    hmax = counter[an];
                    printf("%d", hmax);
                }
            }
            A = 0;
            B = 0;
             for (int c = 0; c < 256; c++)
            { // doing the cdf calculation
                for(int i = (a - ((int)((double)(filheight/2)))); i<=(filheight+(a - ((int)((double)(filheight/2)))));i++)
                { 
                for(int j =(b - ((int)((double)(filwidth/2)))); j<=(filwidth+(b - ((int)((double)(filwidth/2)))));j++)
                {
                   
                    w =  pow((counter[c]/hmax),gamma);
                    B =  B + w;
                    A = A + w * (acc[c]/counter[c]);

                  }
                }
            }
out_image[a][b] = 0.5;
int ind = a*imgcol+ b;
output[ind] = out_image[a][b];
printf("%lf",(A/B));
            }
        }
        //conversion to normal image

    // for (int y = 0; y < imgcol; y++){
    //     for (int x = 0; x < imgrow; x++){
    //         int ind = x*imgcol+ y;
    //         output[ind] = out_image[y][x];
    //     }
    // }
}
void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    if (nlhs != 1)
        mexErrMsgTxt("adaptive_equalize can only accept one input argument");
    if (nrhs != 5)
        mexErrMsgTxt("adaptive_equalize requires one output argument");
    const mxArray *img = prhs[0];
    if (!mxIsDouble(img))
        mexErrMsgTxt("adaptive_equalize can only accept images of type 'double' or 'uint8'");
    mwSize ndims = mxGetNumberOfDimensions(img);
    const mwSize *dims    =         mxGetDimensions(img);
   const  int filheight   =         mxGetScalar(prhs[1]);
    const int filwidth    =         mxGetScalar(prhs[2]);
    const int N           =         mxGetScalar(prhs[2]);
    const int gamma       =         mxGetScalar(prhs[2]);
    const int imgCol      =         dims[0];
    const int imgRow      =         dims[1];
    mxClassID input_type  =         mxGetClassID(img);
    mxArray *output = mxCreateNumericArray(ndims, dims, input_type, mxREAL);
    plhs[0] = output;
    printf("Image Coloums: %d, Image Rows: %d",imgCol,imgRow);
    if (mxIsUint8(img))
    {
       double *img_ptr = (double *)mxGetData(img);
        double *out_ptr = (double *)mxGetData(output);
        adaptive_histogram(filwidth, filheight, imgCol,  N,  gamma, imgRow,  img_ptr, out_ptr);
    }   
}