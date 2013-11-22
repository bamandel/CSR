function train_filenames = test(train_video_dir, keyphrase)
dirlist = dir([train_video_dir, keyphrase, '*']);
    num_clips = length(dirlist);
    train_filenames = cell(num_clips, 1);

    for i = 1 : num_clips
        train_filenames{i} = dirlist(i).name;
    end
end