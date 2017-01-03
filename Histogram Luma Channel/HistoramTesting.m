n = modified_histogram_equ(imread('lenna.tiff'));
imshow(n);

c = histogram_equalize('lenna.tiff');
imshow(c);