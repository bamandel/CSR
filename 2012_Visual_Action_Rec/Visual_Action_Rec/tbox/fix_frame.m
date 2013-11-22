function I = fix_frame(I, spatial_size)

% =========
% fix_frame
% =========
% Crops the given image such that the size of the frame is a multiple of
% the spatial size.
%
% Input arguments
% ---------------
% I           : image in 2D array 
% spatial_size: spatial size of the filter. (Spatial size is 16 for the
% first layer, 20 for the second layer - AH 7/13)
%
% Return values
% -------------
% I: cropped image

I = rgb2gray(I);        %convert the image from rgb to grayscale image.
I = double(I) / 255.0;  %convert the image to 2D Matrix of type double.
[w, h] = size(I);
I = I(1:floor(w/spatial_size)*spatial_size, 1:floor(h/spatial_size)*spatial_size); 

