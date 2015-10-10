function [] = rssi_cal(timestamp, rssi)
%% trying to calibrate RSSI

f = fit(timestamp, rssi, 'exp1');
plot(f);

end