
input_src = 'dump_2_act.csv';

nodes = {'192.168.188.31' '192.168.188.32'};

f_mac = '00F4B96A32ED';

file = ['data/' input_src];
T = parse_csv(file);

minT = min(T.timestamp);
maxT = max(T.timestamp);
diffT = maxT - minT;
T.timestamp = T.timestamp - minT;

clear device;
device = get_device(T, f_mac, nodes, [0 diffT / 2]);

%% init plot
clf(figure(1))
figure(1)
hold all;

xlabel('Time [ms]');
ylabel('RSSI [dBm]');
%%

for n=1:length(nodes)
    plot(device.nodes{n}.timestamp, device.nodes{n}.rssi);
end

legend(nodes);

rssi_cal(device.nodes{2}.timestamp', device.nodes{2}.rssi');

% apply some filters

%rssi_f = sgolayfilt(rssi, 5, 9);

%alpha = 0.3;
%rssi_f = filter(alpha, [1 alpha-1], rssi);

%figure(2)
%hold all
%plot(timestamp, rssi_f);

%print(figure(1), [name '.eps'], '-depsc');
