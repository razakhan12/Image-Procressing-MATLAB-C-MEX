
%%Global histogram
img1 = imread('7.2.01-small.png');%plane
img2 = imread('207056.jpg');%goat
a  = histogram_equalize(img1);figure(1); imshow(a);title('Global Plane Image');
b  = histogram_equalize(img2);figure(2);imshow(b);title('Global goat Image');
%%local histogram with 40,40 window size
f  = adaptive_histogram(img1,40,40);figure(3);  imshow(f); title('local Plane Image');
g  = adaptive_histogram(img2,40,40);figure(4); imshow(g);title('local goat Image');