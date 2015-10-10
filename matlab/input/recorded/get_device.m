function [device] = get_device(T, device_mac, nodes, time_range)
    
    if nargin < 4
        time_range = [];
    end
       
    %% init device structure    

    device.mac = device_mac;
    device.nodes = cell(length(nodes),1);
    for i=1:length(nodes)
        device.nodes{i}.id = nodes{i};
        device.nodes{i}.timestamp = [];
        device.nodes{i}.rssi = [];
    end
    %%

    min = 1;
    max = height(T);        
    
    if (length(time_range) == 2)
        if (time_range(1) > time_range(2))
            t = time_range(1);
            time_range(1) = time_range(2);
            time_range(2) = t;
        end
        
        min_found = false;                
                
        for d=1:height(T)
            if (~min_found)
                if (T.timestamp(d) >= time_range(1))
                    min = d;
                    min_found = true;
                end
            else                
                if (T.timestamp(d) >= time_range(2))
                    max = d;                        
                    break;
                end
            end
        end
    end
    
    for d=min:max
        if (strcmpi(T.mac(d), device.mac))        
            node = find(ismember(nodes,T.receiver(d)));    
            if (~isempty(node))
                device.nodes{node}.timestamp = [device.nodes{node}.timestamp T.timestamp(d)];
                device.nodes{node}.rssi = [device.nodes{node}.rssi T.rssi(d)];            
            end
        end                
    end   
    
    for node=1:length(nodes)
        device.nodes{node}.data_length = length(device.nodes{i}.timestamp);
    end
end