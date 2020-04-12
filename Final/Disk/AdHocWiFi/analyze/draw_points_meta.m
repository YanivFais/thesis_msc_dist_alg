function draw_points_meta( points_meta , playground,scenario)

data = csvread(points_meta);
[rows cols] = size(data);
fullscreen = get(0,'ScreenSize');

layout_fig = figure('Position',[0 0 fullscreen(3) fullscreen(4)]);
hold on;
title('Scenario layout','FontSize',14,'FontWeight','bold');
mode = 0;
node = 1;
for i=1:rows
    if (mode == 0)
        mode = data(i,3);
    else        
        x = data(i,3)*playground;
        y = data(i,4)*playground;
        text(x,y,sprintf('%d',node),'FontSize',10,'FontWeight','bold');
        node = node + 1;
        mode = mode - 1;
    end
end
axis([0 playground 0 playground]);
%hLegend = legend(Streams,'Location', 'NorthEastOutside');
xlabel('x [meter]');
ylabel('y [meter]');
save_fig(layout_fig,scenario,0); 

end

