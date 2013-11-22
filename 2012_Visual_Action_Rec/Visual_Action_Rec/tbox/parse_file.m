%% Author      : Austin Horne
%% Date        : 7/22/13
%% Purpose     : Takes a comma seperated text file as input and gets all of the
%%             : columns containing x y z joint coordinates.
%%             : See also http://pr.cs.cornell.edu/humanactivities/data.php#format
%%
%%             : NOTE: This function is not at all generalized but fits the format of the
%%             : CAD-60 data set because the x y z coordinates are always found in 
%%             : these ranges. The function was written in haste in order to quickly
%%             : test the data set. Generalize this in the future.

function coordinates = parse_file(file)
    matr = csvread(file);
    
    coordinates = matr(:, [12:14, 26:28, 40:42, 54:56, 68:70, 82:84, 96:98, 110:112, 124:126, 138:140, 152:154, 156:158, 160:162, 164:166, 168:170]);
end
