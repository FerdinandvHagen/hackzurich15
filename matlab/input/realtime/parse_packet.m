function [ packet, complete ] = parse_packet( t )

    complete = false;
    n_packet = zeros(32,1);
    c_pos = 1;

    while strcmp(t.Status, 'open') && ~complete     
        data = fread(t, 128);

        for i=1:length(data)
            if (data(i) ~= 0)
                n_packet(c_pos) = data(i);
                c_pos = c_pos + 1;
                if (c_pos > 32)
                    c_pos = 1;
                end
            else   
                try
                    if (length(n_packet) == 31)
                        packet.id = n_packet(3);
                        packet.timestamp = hex2dec(char(n_packet(4:11)));
                        packet.rssi = hex2dec(char(n_packet(14:15)));
                        if (packet.rssi > 127) 
                            packet.rssi = packet.rssi - 256;
                        end
                        packet.mac = char(n_packet(16:27));              
                        complete = true;                   
                    end
                catch e
                    disp(e);
                end            

                n_packet = [];
            end

        end
    end

end

