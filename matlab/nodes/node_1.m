port = 3003;

if exist('t1','var')
    if strcmp(t1.Status, 'open')
        fclose(t1);
    end
end

t1 = net_server(port);
fprintf('%d connected', port);
w_rt_plot_raw(t1);
disp('%d disconnected', port);