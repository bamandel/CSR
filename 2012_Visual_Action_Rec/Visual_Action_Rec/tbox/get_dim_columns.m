%% Author      : Austin Horne
%% Date        : 7/22/13
%% Purpose     : Intended to be used on a matrix returned by parse_file.m
%%             : This function returns individual matrices of all x, y, and
%%             : z values from the input matrix. This can only be used in
%%             : conjunction with parse_file.m which relies on the CAD-60
%%             : data set.
function [ x_columns, y_columns, z_columns ] = get_dim_columns( mat )

    [rows, columns] = size(mat);
    columns_per_dim = columns / 3;

    x_columns = zeros(rows, columns_per_dim);
    y_columns = zeros(rows, columns_per_dim);
    z_columns = zeros(rows, columns_per_dim);
    
    j = 1;
    k = 2;
    l = 3;
    
    for i = 1 : columns_per_dim
        x_columns(:,i) = mat(:,j);
        y_columns(:,i) = mat(:,k);
        z_columns(:,i) = mat(:,l);
        j = j + 3;
        k = k + 3;
        l = l + 3;
    end       
end

