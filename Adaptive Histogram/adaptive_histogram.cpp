#include "mex.h"
void adaptive_histogram(const int filwidth, const int filheight,const int imgcol,const int imgrow, const unsigned char *input, unsigned char *output)
{ 
    int img_size = filwidth * filheight;
    double cdf = 0;
    double counter[256];
    double test;
    unsigned char in_image[imgcol][imgrow];
    unsigned char out_image[imgcol][imgrow];
    int locH =imgcol-((int)((double)(filheight/2)));
    int locW = imgrow-((int)((double)(filwidth/2)));
    for(int i = 0 ; i<imgcol; i++){
        for(int j = 0; j<imgrow; j++){
            int ind = j*imgcol + i;
            in_image[i][j] = input[ind];
        } //converting to 2d array
    }
    for(int a = ((int)((double)(filheight/2))) ; a < locH; a++)
    {
        for(int b = ((int)((double)(filwidth/2))); b < locW; b++)
        {     
            for(int i = (a - ((int)((double)(filheight/2)))); i<=(filheight+(a - ((int)((double)(filheight/2)))));i++){
                for(int j =(b - ((int)((double)(filwidth/2)))); j<=(filwidth+(b - ((int)((double)(filwidth/2)))));j++)
                {
                    int location = in_image[i][j];
                    counter[location] += 1;

                }
            }
            for (int an = 0; an<256; an++){ // doing the cdf calculation
                                  cdf = cdf + counter[an]/(img_size);
                                   test  = cdf*255;
                                  if (test>255)
                                      test = 255;
                                  if (test<0)
                                      test = 0;
                                  counter[an] = test;
            }
            // 
            for(int i = (a - ((int)((double)(filheight/2)))); i<=(filheight+(a - ((int)((double)(filheight/2)))));i++){ 
                for(int j =(b - ((int)((double)(filwidth/2)))); j<=(filwidth+(b - ((int)((double)(filwidth/2)))));j++)
                {
                    int location = in_image[i][j];
                    out_image[i][j] = counter[location];
                  }
                }
                 cdf = 0;// set cdf to 0
                 for (int set = 0; set < 256; set++) //reset counters sum to all zeros
                     {
                       counter[set] = 0; 
                      }
            }
        }
        //conversion to normal image

    for (int y = 0; y < imgcol; y++){
        for (int x = 0; x < imgrow; x++){
            int ind = x*imgcol+ y;
            output[ind] = out_image[y][x];
        }
    }
}
void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    if (nlhs != 1)
        mexErrMsgTxt("adaptive_equalize can only accept one input argument");
    if (nrhs != 3)
        mexErrMsgTxt("adaptive_equalize requires one output argument");
    
    const mxArray *img = prhs[0];
    if (!mxIsUint8(img))
        mexErrMsgTxt("adaptive_equalize can only accept images of type 'double' or 'uint8'");
    
    mwSize ndims = mxGetNumberOfDimensions(img);
    const mwSize *dims = mxGetDimensions(img);
   const  int filheight   = mxGetScalar(prhs[1]);
    const int filwidth    = mxGetScalar(prhs[2]);
    const int imgCol   = dims[0];
    const int imgRow    = dims[1];
    mxClassID input_type = mxGetClassID(img);
    mxArray *output = mxCreateNumericArray(ndims, dims, input_type, mxREAL);
    plhs[0] = output;
    printf("Image Coloums: %d, Image Rows: %d",imgCol,imgRow);
    if (mxIsUint8(img))
    {
       const unsigned char *img_ptr = (unsigned char *)mxGetData(img);
        unsigned char *out_ptr = (unsigned char *)mxGetData(output);
        adaptive_histogram(filwidth, filheight, imgCol, imgRow,  img_ptr, out_ptr);
    }   
}