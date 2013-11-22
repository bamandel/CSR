function network = build_network(network_params, load_layer_weights, bases_path)

%function to build network structure
%load_layer_weights: 1 load layer 1 only; 2 load layers 1+2, 3 load 1+2+3,

network.num_isa_layers = network_params.num_isa_layers;     %INITIALIZED TO 2 IN set_network_params.m - AH 6/13

%each ISA layer of the neural network (beginning layer 1) - AH 6/13
for i = 1:network_params.num_isa_layers 
   network.isa{i}.fovea = network_params.fovea{i}; 
   network.isa{i}.bases_id = network_params.bases_id{i};
end

network = set_strides(network, network_params);

network = load_network_bases(network, network_params, load_layer_weights, bases_path);

end