function [output] = XDoG(sigma,k,p,image)

image1 = spatial_filter(im2double(imread(image)),ant(sigma,1));
image2 = spatial_filter(im2double(imread(image)),ant(sigma,k));
output = ((1+p).*image1) - (p.*(image2));
end