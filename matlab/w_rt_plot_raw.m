function [] = w_rt_plot_raw(t)

[figureHandle, plotHandle] = w_rt_plot_init();

%% Set the time span and interval for data collection
timeInterval = 0.001;

%% Collect data
count = 1;
timestamp = 0;
rssi = 0;

% MAC to filter out
f_mac = '00F4B96A32ED';

complete = true;
while complete
    [packet, complete] = parse_packet(t);
    if complete
        process(packet); 
    end
end

function [] = process(packet)
    if (strcmpi(f_mac, packet.mac)) 
        
    timestamp(count) = packet.timestamp;
    rssi(count) = packet.rssi;
    count = count + 1;    
    
    set(plotHandle,'YData', rssi, 'XData', timestamp);    
    set(figureHandle,'Visible','on');    
    
    title(packet.id,'FontSize',15,'Color',[1 1 0]);
    
    pause(timeInterval);
      
    end 
end

%% clean up the socket
fclose(t);
delete(t);
clear t

end