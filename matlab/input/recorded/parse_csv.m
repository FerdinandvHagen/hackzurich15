function [ T ] = parse_csv( file )

    T = readtable(file, 'Delimiter', ';', 'headerlines', 0);

end

