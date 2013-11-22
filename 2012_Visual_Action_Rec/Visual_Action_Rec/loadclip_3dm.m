%% Author      : Austin Horne
%% Date        : 7/13
%% Purpose     : Designed to work with the CAD-60 data set of 3D videos
%%             : captured from the Kinect. The videos are saved as directories
%%             : containing the depth and RGB images which compose the videos.
%%             : This function reads the RGB images from those directories and
%%             : creates a matrix of the color data in each of them.
%%            
%%             : NOTE: The use of imread seems to make this function
%%             : particularly slow. Finding a faster way of reading the
%%             : RGB images would be a considerable optimization.

function M = loadclip_3dm(file, spatial_size, fast, smooth, keyphrase)

    vidFrames = dir([file, '/', keyphrase, '*']);
    
    l = length(vidFrames);
    first_frame = fix_frame(imread([file, '/', vidFrames(1).name]), spatial_size);
    
    %size(first_frame, 1) returns the size of the first dimension of first_frame
    %size(first_frame, 2) returns the size of the second dimension
    % - AH 6/13
    if fast
        M = zeros(size(first_frame, 1), size(first_frame,2), floor(l/2), 'single'); %3D matrix of zeros of type single. 
                                                                                    %If fast option is chosen, they skip about half the frames,
                                                                                    %so the 3rd dimension only needs to be half size of # of frames. - AH 7/13
    else        
        M = zeros(size(first_frame, 1), size(first_frame,2), l, 'single');  
    end
    
    if fast
        for i = 1:2:floor(l/2)*2-1 %i starts at 1 and increments by 2 until floor(l/2)*2-1
            fixed_frame = fix_frame(imread([file, '/', vidFrames(i).name]), spatial_size);
            if smooth
                fixed_frame = smooth_frame(fixed_frame);
            end
            M(:, :, i) = fixed_frame;
        end
    else
        for i = 1:l            
            fixed_frame = fix_frame(imread([file, '/', vidFrames(i).name]), spatial_size);
            if smooth
                fixed_frame = smooth_frame(fixed_frame);                
            end
            M(:, :, i) = fixed_frame;
        end
    end

end
