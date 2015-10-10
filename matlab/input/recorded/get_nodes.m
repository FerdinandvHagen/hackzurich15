function [ nodes ] = get_nodes( T, max_nodes )

if (nargin < 2)
    max_nodes = -1;
end

nodes = {};

for i=1:height(T)
    if (~ismember(nodes, T.receiver(i)))
        nodes{end + 1} = T.receiver;
        if (max_nodes >= 0)
            if (max_nodes >= length(nodes))
                break;
            end
        end
    end
end

end

