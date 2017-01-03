


plane = imread('7.2.01-small.png');
orca = imread('digital_orca_blurred.png');

plane_p = laplacian_sharpen(plane, 0.2);
orca_p = laplacian_sharpen(orca, 0.2);

figure
imshow(plane)
title('Original Image')

figure
imshow(orca)
title('Original Image')

figure
imshow(plane_p)
title('Sharpened Image')

figure
imshow(orca_p)
title('Sharpened Image')
