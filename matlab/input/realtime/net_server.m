function t = net_server(port)

    t = tcpip('0.0.0.0', port, 'NetworkRole', 'server');
    t.InputBufferSize = 128;
    t.Terminator = 0;
    fopen(t);

end