function [ figureHandle, plotHandle ] = w_rt_plot_init( )

figureHandle = figure('NumberTitle','off',...
    'Name','RSSI characterisics',...
    'Color',[0 0 0],'Visible','off');

% Set axes
axesHandle = axes('Parent',figureHandle,...
    'YGrid','on',...
    'YColor',[0.9725 0.9725 0.9725],...
    'YLim', [-80,-20],...
    'XGrid','on',...
    'XColor',[0.9725 0.9725 0.9725],...
    'Color',[0 0 0]);

hold on;

plotHandle = plot(axesHandle,0 ,0,'Marker','.','LineWidth',1,'Color',[0 1 0]);

% Create xlabel
xlabel('Time [ms]','FontWeight','bold','FontSize',14,'Color',[1 1 0]);

% Create ylabel
ylabel('RSSI [dBm]','FontWeight','bold','FontSize',14,'Color',[1 1 0]);

end

