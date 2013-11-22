%% Author      : Austin Horne
%% Date        : 7/16/13
%% Purpose     : Returns a cell array of all the directories contained under a 
%%             : specified path, excluding . and .. directory shortcuts.
%%                  
function nameFolds = directories(directory)
    d = dir(directory);
    isub = [d(:).isdir]; 
    nameFolds = {d(isub).name}';
  
    nameFolds(ismember(nameFolds,{'.','..'})) = []; %deletes the directory shortcuts from the cell array
end

