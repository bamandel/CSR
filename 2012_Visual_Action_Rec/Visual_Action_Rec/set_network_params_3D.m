function params = set_network_params_3D()

%% number of layers
params.num_isa_layers = 2;  %params = new struct - AH 6/13

%% fovea sizes
% (fovea centralis is the part of the human eye responsible for sharp central vision - AH 6/13)
% Matlab cell arrays can contain data of varying type and size - AH 6/13
fovea{1}.spatial_size = 16;
fovea{1}.temporal_size = 10;

fovea{2}.spatial_size = 20; %fovea = new 1x2 cell array - AH 6/13
fovea{2}.temporal_size = 14;

params.fovea = fovea;

%% convolutional strides
stride{1}.spatial_stride = 4;
stride{1}.temporal_stride = 4;

params.stride = stride;

%% network feature dimension and group size
% these are the default settings

params.pca_dim_l1 = 50; % number of feature dimensions network layer 1
params.group_size_l1 = 1; % group size

params.pca_dim_l2 = 75; 
params.group_size_l2 = 2; 

% results reported in the paper were obtained using group_size_l2 = 4 to 
% save memory on our clusters, but this does not affect results by a lot

params.bases_id{1} = sprintf('isa1layer_%d_%d_%d_%d', ...
              fovea{1}.spatial_size, fovea{1}.temporal_size, params.pca_dim_l1, params.group_size_l1);

params.bases_id{2} = sprintf('isa2layer_%dt%d_ts%dt%d_nf%d_gs%d_st%dt%d_l1_%s', ...
              fovea{1}.spatial_size, fovea{2}.spatial_size, ...
			  fovea{1}.temporal_size, fovea{2}.temporal_size, ...
			  params.pca_dim_l2, params.group_size_l2, stride{1}.spatial_stride, stride{1}.temporal_stride, params.bases_id{1});

end
