%% Author      : Austin Horne
%% Date        : 7/22/13
%% Purpose     : This function is used for conversion of x,y,z coordinates listed
%%             : in the CAD-60 skeleton data to corresponding x,y coordinates in
%%             : the RGB images. Uses the formulas detailed in the CAD-60 
%%             : README file.

function [x_pos, y_pos] = getXY( x_coord, y_coord, z_coord )
    x_pos = 156.8584456124928 + 0.0976862095248 * x_coord - 0.0006444357104 * y_coord + 0.0015715946682 * z_coord;
    y_pos = 125.5357201011431 + 0.0002153447766 * x_coord - 0.1184874093530 * y_coord - 0.0022134485957 * z_coord;
end

