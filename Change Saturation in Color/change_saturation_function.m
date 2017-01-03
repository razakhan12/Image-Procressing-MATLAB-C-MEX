function y = change_saturation_function(x)
imgout = imread('lenna.tiff');
%Error checking is already in C file
satImg = (change_saturation(im2double(imgout),x));
figure(1);title('Changed Saturated Image'); imshow(satImg);
final = hsi_to_rgb(satImg);
figure(2);title('Final RGB Saturated Image'); imshow(final);
y = 0;
end 