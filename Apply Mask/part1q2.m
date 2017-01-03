% part1, q 2

img1 = imread('fish.png');
img2 = imread('bridge.png');
msk = imread('mask.png');

out = apply_mask(img1, img2, msk);

imshow(out)