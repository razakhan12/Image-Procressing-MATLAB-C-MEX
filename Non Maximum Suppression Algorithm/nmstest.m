img_db = im2double(imread('2(256).jpg'));
o = non_max_suppress(img_db,20.0,1.0);
o = non_max_suppress(o,1.0,20.0);
imshow(o);