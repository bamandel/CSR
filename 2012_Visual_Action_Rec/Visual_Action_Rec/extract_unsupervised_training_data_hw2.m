  
function [] = extract_unsupervised_training_data_hw2(train_video_dir, result_filename, params, keyphrase, dim3D)

spatial_size = params.spatial_size;
temporal_size = params.temporal_size;
num_patches = params.num_patches;

if dim3D
    train_filenames = directories(train_video_dir);     %gets the directories containing RGB images. Excludes . and .. from the directory listing
else
    dirlist = dir([train_video_dir, keyphrase, '*']);
    num_clips = length(dirlist);
    train_filenames = cell(num_clips, 1);

    for i = 1 : num_clips
        train_filenames{i} = dirlist(i).name;
    end
end

X = sample_video_blks(train_video_dir, train_filenames, spatial_size, temporal_size, num_patches, keyphrase, dim3D);

%writes the contents of X, spatial_size, and temporal_size to a file named
%by result_filename - AH 6/'13
save(result_filename, 'X', 'spatial_size', 'temporal_size', '-v7.3');
end

function X = sample_video_blks(path, filenames, sp_size, tp_size, num_perclip, keyphrase, dim3D)

num_clips = length(filenames);
if dim3D
    num_joints = 15;
    num_frames = 0;
    
    for i = 1 : num_clips
        framelist = dir([path, filenames{i}]);
        frames = size(framelist,1) - 2;
        num_frames = num_frames + frames;
    end
    
    X = zeros(sp_size^2*tp_size, num_frames*num_joints);
else
    X = zeros(sp_size^2*tp_size, num_perclip*num_clips);
end
margin = 5;
counter = 1;

for i = 1 : num_clips
    vidpath = [path, filenames{i}];
    fprintf('loading clip: %s\n', vidpath);
    
    if dim3D        
        skel_file_path = [vidpath, '.txt']; %path to the skeleton data file that corresponds to the currently loaded video
        [vidpath, '/'];    
        M = loadclip_3dm(vidpath, sp_size, 0, 0, keyphrase);    
        coords = parse_file(skel_file_path);                    %gets all the physical x y and z coordinates from the skeleton data
        [x_columns,y_columns,z_columns] = get_dim_columns(coords); %seperate x, y, and z values into individual matrices
        size_t = size(M,3);               
        for j = 1 : size_t
            for k = 1 : num_joints
                x_coord = x_columns(j,k);
                y_coord = y_columns(j,k);
                z_coord = z_columns(j,k);

                [x_pos, y_pos] = getXY(x_coord, y_coord, z_coord);
                x_pos = round(x_pos);
                y_pos = round(y_pos);  
                t_pos = randi([1, size_t-tp_size+1]);
                blk = M(x_pos: x_pos+sp_size-1, y_pos: y_pos+sp_size-1, t_pos: t_pos+tp_size-1); 
                X(:, counter) = reshape(blk, sp_size^2*tp_size, []);    %returns a single column representation of the matrix "blk" and places it in the   
                                                                        %column of X indexed by counter
                counter = counter + 1;
            end %for
         end %for
    else
        
        M = loadclip_2dm(vidpath, sp_size, 0, 0);
        [dimx, dimy, dimt] = size(M);   %Initialize dimx to the width of matrix M, dimy to the height of M, and dimt to the time dimension of M. - AH 6/13
    
        for j = 1 : num_perclip

            x_pos = randi([1+margin, dimx-margin-sp_size+1]);
            y_pos = randi([1+margin, dimy-margin-sp_size+1]);
            t_pos = randi([1, dimt-tp_size+1]);

            blk = M(x_pos: x_pos+sp_size-1, y_pos: y_pos+sp_size-1, t_pos: t_pos+tp_size-1);
            X(:, counter) = reshape(blk, sp_size^2*tp_size, []);    %returns a single column representation of the matrix "blk" and places it in the   
                                                                    %column of X indexed by counter
            counter = counter + 1;
        end    
 
    end %% End if
    
end %% End for 

end %% End sample_video_blks
