port = 3004;

if exist('t2','var')
    if strcmp(t2.Status, 'open')
        fclose(t2);
    end
end

t2 = net_server(port);
fprintf('%d connected', port);
w_rt_plot_raw(t2);
disp('%d disconnected', port);