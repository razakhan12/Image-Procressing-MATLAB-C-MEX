

plane = imread('7.2.01-small.png');
orca = imread('digital_orca_blurred.png');

plane_p = unsharp_mask(plane, 3, 1.5);
orca_p = unsharp_mask(orca, 3, 1.5);

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
