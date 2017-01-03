
clear
a = [3,0];
b = [250,123];
testImg = imread('7.2.01.tiff');
figure(1); 
imshow(testImg);
figure(2);
imshow(contrast_piecewise(imread('7.2.01.tiff'),a,b));