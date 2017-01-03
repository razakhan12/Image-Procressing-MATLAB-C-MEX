% Mex function tests

% Spatial filter test (Gaussian filter)

crash = imread('head_crash_bw_sm.jpg');
crash_db = im2double(crash);

gaus_unscaled = [1 4 7 4 1; 4 20 33 20 4; 7 33 55 33 7; 4 20 33 20 4; 1 4 7 4 1];
gaus = (1/331).*gaus_unscaled;
central = [-1 0 1];
sobel = [1 0 -1; 2 0 -2; 1 0 -1];

out1 = spatial_filter(crash_db, gaus);

out2 = spatial_filter(crash_db, central);
out3 = spatial_filter(crash_db, sobel);

% unmodified test image for spatial filter
figure(1)
imshow(crash_db)

% spatial filter output (Gaussian filter)
figure(2)
imshow(out1)

% spatial filter output (central difference)
figure(3)
imshow(out2)
% spatial filter output (sobel filter)
figure(4)
imshow(out3)
% non-maximum filter test 
%nmtest = imread('nms-test.png');
%nmtest_db = im2double(nmtest);
%out4 = non_max_suppress(nmtest_db, 5, 5);
%figure
%imshow(out4)
% threshold test
thresh = imread('threshold-test.png');
thresh_db = im2double(thresh);
out5 = image_threshold(thresh_db, 0.25);
out6 = image_threshold(thresh_db, 0.5);
out7 = image_threshold(thresh_db, 0.75);

figure
imshow(out5)
title('Threshold = 0.25')

figure
imshow(out6)
title('Threshold = 0.5')

figure
imshow(out7)
title('Threshold = 0.75')