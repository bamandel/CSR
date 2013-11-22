%This function uses a third-party library called mmread to read in a video
%clip and get the individual frames from the video. The code then takes the
%actual color data from each frame and sends it to the fix_frame function
%to crop and grayscale the frame. The function returns a 3D matrix composed
%of all the color data in the frames. So if, for example, a video had 3
%frames, then a very small scale version of this function's return value
%would look like the following which is supposed to represent a 3D matrix
%of numeric values:

%        #####       #####       #####
%   y    #####       #####       #####
%        #####       #####       #####
%          x
%
% - AH 7/13

function M = loadclip_2dm(file, spatial_size, fast, smooth)

    %returns a struct
    readerobj = mmread(file, [], [], false, true); %MATLAB FUNCTION. Read all frames, disable audio

    vidFrames = readerobj.frames;   %struct array with the fields, cdata and colormap. cdata is the only field of significance
    l = length(vidFrames);
    
    first_frame = fix_frame(vidFrames(1).cdata, spatial_size);
    
    %size(X, dim) returns the size of the dimension of X specified by scalar dim.    
    if fast
        M = zeros(size(first_frame, 1), size(first_frame,2), floor(l/2), 'single'); %3D matrix of zeros of type single. 
                                                                                    %If fast option is chosen, they skip about half the frames,
                                                                                    %so the 3rd dimension only needs to be half as big. - AH 7/13
    else        
        M = zeros(size(first_frame, 1), size(first_frame,2), l, 'single');  
    end
    
    if fast
        for i = 1:2:floor(l/2)*2-1 %i starts at 1 and increments by 2 until floor(l/2)*2-1
            fixed_frame = fix_frame(vidFrames(i).cdata, spatial_size);
            if smooth
                fixed_frame = smooth_frame(fixed_frame);
            end
            M(:, :, i) = fixed_frame;   
        end
    else
        for i = 1:l
            fixed_frame = fix_frame(vidFrames(i).cdata, spatial_size);

            if smooth
                fixed_frame = smooth_frame(fixed_frame);                
            end

            M(:, :, i) = fixed_frame;
        end
    end

end
