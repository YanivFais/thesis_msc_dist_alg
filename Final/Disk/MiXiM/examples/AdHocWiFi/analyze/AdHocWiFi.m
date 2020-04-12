function  AdHocWiFi(fileName,eventFileName,varargin)

draw_all = 0;
full_stat =0;
subplot_num = 3;

if nargin >2
    graphFile = varargin{1};
    hasGraphData = 1;
else
    hasGraphData = 0;
end

mkdir jpg
mkdir eps
mkdir fig

fullscreen = get(0,'ScreenSize');
%fullscreen(4) = round(fullscreen(4)*1.5); % scale window size up
tic
'Loading statistics file'
dataStruct = mfcsvread(fileName);
toc
Field = fieldnames(dataStruct); 
secs = round(dataStruct.Rounds(1));
slots = dataStruct.Slots(1);
alpha = dataStruct.PathLossAlpha(1);
beta = dataStruct.PathLossBeta(1);
TX_Power_mW = dataStruct.TX_POWER_mW(1);
message_size = dataStruct.MsgSizeBits(1);
noise_dBm = dataStruct.noise_dBm(1);
sources = dataStruct.Sources;
destinations = dataStruct.Destinations;
positionsX = dataStruct.Initial_Positions_X;
positionsY = dataStruct.Initial_Positions_Y;
playground = dataStruct.PlaygroundSize(1);
roundTime = dataStruct.roundTime(1);

nodes = size(positionsX);

for i=1:size(positionsX)
    if (isnan(positionsX(i)))
      positionsY = positionsY(1:i-1);
      positionsX = positionsX(1:i-1);
      nodes = i-1;
      break;
    end
end

for i=1:nodes
    positionsY(i) = playground-positionsY(i);
end

streams_number = 0;
  for iField = 1:length(Field)
      if (regexp(Field{iField},'^Throughput'))
          streams_number=streams_number+1;
      end
  end
sources = sources(1:streams_number);
destinations = destinations(1:streams_number);
secs_range = linspace(1,secs,secs);
secs_range5 = linspace(0,secs,5*(secs)+1);
Streams = cell(1,streams_number);
streams_range = 1:1:streams_number;
throughput = zeros(streams_number,1);
stream_colors=varycolor(streams_number);
e2e_mean = zeros(1,streams_number);

tp_num = 1;
tp_fig = figure('Position',[0 -50 fullscreen(3) fullscreen(4)]);
hold on;
title('Throughput per stream','FontSize',14,'FontWeight','bold');
ylabel('Received in  round [Mbps]');
xlabel('Time [sec]');
hold off;

hops_colors=varycolor(8); 
hop_color_size = size(hops_colors);
q_len_legend = cell(streams_number,nodes);
q_len_edges_num = zeros(streams_number,1);


if (subplot_num>0)
  for i=1: ceil(streams_number/subplot_num)
    e2e_sp_fig(i) = figure('Position',[0 -50 fullscreen(3) fullscreen(4)]);      
    e2e_sp_hist_fig(i) = figure('Position',[0 -50 fullscreen(3) fullscreen(4)]);      
  end
end

'Processing queues length and end to end delays'
tic
for iField = 1:length(Field)
        vec = dataStruct.(Field{iField});
        name =  str_conv(Field{iField},1);
        name = strrep(name,'str','stream ');
      if (regexp(name,'^Throughput'))
          figure(tp_fig);
          hold on;
          plot(secs_range,vec(1:secs)/roundTime,'Color',stream_colors(tp_num,:),'Marker','*','LineWidth',2);
          throughput(tp_num) = vec(secs-1)/roundTime;
          name = strrep(name,'Throughput ','');
          name = strrep(name,'[Mbps]','');
          Streams(tp_num) = cellstr(name);
          tp_num = tp_num+1;
      end
      if (regexp(name,'End2End'))
              vec= vec(find(vec==vec & (vec>0))); % remove NaN and dropped,lost
              str_idx = strfind(name,' str');
              name_size = size(name);
              name_size = name_size(2);
              stream=sscanf(name(str_idx:name_size),' stream %d');
              e2e_mean(1,stream) = ceil(mean(vec));
              if (draw_all)
                e2e_fig = figure('Position',[0 -50 fullscreen(3) fullscreen(4)]);
                xlabel('Packet ID  []');
                ylabel('End to End delay [sec]');
                stem(vec,'LineStyle','none');
                title(sprintf('End To End Delay Stream #%d',stream),'FontSize',14,'FontWeight','bold');
                save_fig(e2e_fig,sprintf('e2e_delay_stream%d',stream),1);
                e2e_hist_fig = figure('Position',[0 -50 fullscreen(3) fullscreen(4)]);
                hist(vec); %line_spec(e2e_num,0),'LineWidth',2);
	            title(sprintf('End To End Delay Histogram Stream #%d',stream),'FontSize',14,'FontWeight','bold');
	            ylabel('# Packets []');
	            xlabel('End to End delay [sec]');
                save_fig(e2e_hist_fig,sprintf('e2e_delay_hist%d',stream),1);              
              end
              if (subplot_num>0)
                  figure(e2e_sp_fig(floor((stream-1)/subplot_num)+1));                  
                  subplot_range(subplot_num,stream,streams_number);
                  ylabel('End to End delay [sec]');
                  stem(vec,'LineStyle','none');
                  title(sprintf('End To End Delay Stream #%d',stream),'FontSize',14,'FontWeight','bold');
                  figure(e2e_sp_hist_fig(floor((stream-1)/subplot_num)+1));
                  subplot_range(subplot_num,stream,streams_number);
                  hist(vec); %line_spec(e2e_num,0),'LineWidth',2);
  	              title(sprintf('End To End Delay Histogram Stream #%d',stream),'FontSize',14,'FontWeight','bold');
	              ylabel('# Packets []');
              end
      end
  end
toc

figure(tp_fig);
legend(Streams,'Location', 'NorthEastOutside');
save_fig(tp_fig,'throughput',1);              

for i=1:streams_number
  if (subplot_num>0)
      sp_num = floor((i-1)/subplot_num)+1;
      if (mod(i,subplot_num)==0 | i==streams_number)
        save_e2e(e2e_sp_fig(sp_num),e2e_sp_hist_fig(sp_num),i,streams_number,subplot_num);
      end
  end
end

q_len_initiator = zeros(1,streams_number);

if (draw_all>0)
  for i=1: streams_number
    q_len_max_fig(i) = figure('Position',[0 -50 fullscreen(3) fullscreen(4)]);  
    q_len_mean_fig(i) = figure('Position',[0 -50 fullscreen(3) fullscreen(4)]);  
  end
else
    q_len_max_fig = 0;
    q_len_mean_fig = 0;
end
if (subplot_num>0)
  for i=1: ceil(streams_number/subplot_num)
    q_len_max_sp_fig(i) = figure('Position',[0 -50 fullscreen(3) fullscreen(4)]);  
    q_len_mean_sp_fig(i) = figure('Position',[0 -50 fullscreen(3) fullscreen(4)]);  
  end
end

% iterating again since not enough memory
for iField = 1:length(Field)
        vec = dataStruct.(Field{iField});
        name =  str_conv(Field{iField},1);
        name = strrep(name,'str','stream ');
        name = strrep(name,'Q ','Queue ');
      if (regexp(name,':Queue length'))
              str_idx = strfind(name,'_str');
              name_size = size(name);
              name_size = name_size(2);
              stream=sscanf(name(str_idx:name_size),'_stream %d');
              col_idx = strfind(name,':');
              num = q_len_edges_num(stream);                  
              name2 = name(1:col_idx-1);
              node = sscanf(name2,'n%d');
              name2 = strrep(name2,'n','node ');
              if (node ~= destinations(stream))
                if (regexp(name,'length_max')) 
                    q_len_fig = q_len_max_fig;
                    q_len_sp_fig = q_len_max_sp_fig;
                    type = 'Max';
                    q_len_edges_num(stream) = num+1;
                    q_len_legend(stream,num+ 1) = cellstr(name2);
                 else
                   q_len_fig = q_len_mean_fig;
                   q_len_sp_fig = q_len_mean_sp_fig;
                   type = 'Mean';
                   if (node == sources(stream))
                        q_len_initiator(stream) = vec(secs-1);
                   end
                 end
                  style = line_spec(node,1);
                  style_len = size(style);
                  hop_color_idx = mod(num,hop_color_size(1))+1;
                  hop_color = hops_colors(hop_color_idx,:);
                  marker = style(style_len(2));
                  if (marker==0)
                      marker = '*';
                  end
                  if (draw_all>0)
                    figure(q_len_fig(stream));
                    hold on;
                    title(sprintf('Queue length %s stream %d',type,stream),'FontSize',14,'FontWeight','bold');
                    ylabel('# Packets []');
                    xlabel('Time [sec]');
                    plot(secs_range5,[0;vec(1:5*secs)],'Color',hop_color,'LineStyle',style(2:style_len(2)-1),'Marker',marker,'LineWidth',2);
                  end
                  if (subplot_num>0)
                    figure(q_len_sp_fig(floor((stream-1)/subplot_num)+1));
                    subplot_range(subplot_num,stream,streams_number);
                    hold on;                       
                    ylabel('# Packets []');
                    title(sprintf('Queue length %s stream %d',type,stream),'FontSize',14,'FontWeight','bold');                    
                    plot(secs_range5,[0;vec(1:5*secs)],'Color',hop_color,'LineStyle',style(2:style_len(2)-1),'Marker',marker,'LineWidth',2);
                  end
              end
      end
  end
toc


for i=1:streams_number
  if (draw_all)
      figure(q_len_max_fig(i));
      legend(q_len_legend(i,1:q_len_edges_num(i)),'Location', 'NorthEastOutside');
      save_fig(q_len_max_fig(i),sprintf('q_len_max_%d',i),1);              
      figure(q_len_mean_fig(i));
      legend(q_len_legend(i,1:q_len_edges_num(i)),'Location', 'NorthEastOutside');
      save_fig(q_len_mean_fig(i),sprintf('q_len_mean_%d',i),1);              
  end
  if (subplot_num>0)
      sp_num = floor((i-1)/subplot_num)+1;
     figure(q_len_max_sp_fig(sp_num));
     subplot_range(subplot_num,i,streams_number);
     orient tall;
     legend(q_len_legend(i,1:q_len_edges_num(i)),'Location', 'NorthEastOutside');
     if (mod(i,subplot_num)==0 | i==streams_number)
        xlabel('Time [sec]');
     end
     figure(q_len_mean_sp_fig(sp_num));
     subplot_range(subplot_num,i,streams_number);
     orient tall;
     legend(q_len_legend(i,1:q_len_edges_num(i)),'Location', 'NorthEastOutside');
     if (mod(i,subplot_num)==0 | i==streams_number)
        xlabel('Time [sec]');
        save_fig(q_len_max_sp_fig(sp_num),sprintf('q_len_max_multi_%d',sp_num),0);      
        save_fig(q_len_mean_sp_fig(sp_num),sprintf('q_len_mean_multi_%d',sp_num),1);      
     end    
  end
end

hops_num_fig = figure('Position',[0 -50 fullscreen(3) fullscreen(4)]);
hold on;

function [ max_hops_num ] = path_walk( node,from,to,stream,stream_number,edges_num,nodes_num )
    travelled_nodes = zeros(nodes_num,1);
    max_hops_num = 0;
    for i=1:edges_num
        if ((from(i)==node) && (stream(i)==stream_number) && (travelled_nodes(to(i))==0))
            next = to(i);
            hops_c = 1 + path_walk( next,from,to,stream,stream_number,edges_num , nodes_num);
            sprintf('stream-%d,walk from node=%d -> %d, length=%d',stream_number,node,to(i),hops_c);
            travelled_nodes(next)=1;
            max_hops_num = max(max_hops_num,hops_c);
        end
    end
end


tableFrom = dataStruct.Table_From;
tableTo = dataStruct.Table_To;
tableSlot = dataStruct.Table_Slot;
tableChannel = dataStruct.Table_Channel;
tableStream = dataStruct.Table_Stream;
tableData = dataStruct.Table_Data;
tableMCS = dataStruct.Table_MCS;

tic;
'Calculating hops number'
hops = zeros(streams_number,1);
[ edges_num dummy] = size(tableFrom);
for stream_num=1:streams_number
    hops(stream_num) = path_walk(sources(stream_num),tableFrom,tableTo,tableStream,stream_num,edges_num,nodes);
end
toc

bar(streams_range,hops);
title('Maximal number of hops per stream','FontSize',14,'FontWeight','bold');

xlabel('Stream number []');
ylabel('Hops number []');
ylimits = ylim;
 set(gca,'YTick',ylimits(1):1:ylimits(2))
save_fig(hops_num_fig,'hops_number',1);              

if (hasGraphData==1)
    graphData = csvread(graphFile);
    graphFrom=graphData(:,1);
    graphTo=graphData(:,2);
    graphPER=graphData(:,9);
  	%	mDebugFile << pFrom->GetVertexNum() << "," << pTo->GetVertexNum()
  	%	<< ","  << PFrom.x << "," << PFrom.y << "," << PTo.x << "," << PTo.y << ","
	%		<< dist << "," << communicationParams.SNR << "," << communicationParams.PER 
	%		<< "," << communicationParams.MCS_INDEX << endl;  
end
tic
'Loading events log'
eventsStruct = mfcsvread(eventFileName);
toc

eventsFields = fieldnames(eventsStruct);
msgID = eventsStruct.MessageSPCID;
msgFrom = eventsStruct.from;
msgTo = eventsStruct.to;
msgStream = eventsStruct.stream;
msgSlot = eventsStruct.slot;
msgChannel = eventsStruct.channel;
msgSINR = 10*log10(eventsStruct.SINR);
msgEventType = eventsStruct.Event__0_Recv__1_Send__2_Drop__3_Lost__4_Rate; 
msgTime = eventsStruct.Time;

typeRecv = 0;
typeSend = 1;
typeDrop = 2;
typeLost = 3;
typeRate = 4;

if (full_stat==1)
    for stream=1:streams_number
        fc_fig = figure('Position',[0 0 fullscreen(3) fullscreen(4)]);
        hold on;
        title(sprintf('Flow control per node - stream %d',stream),'FontSize',14,'FontWeight','bold');
        fc_legend = cell(1,nodes);
        num = 0;
        for node =1:nodes
            indices=find(msgEventType==typeRate & msgFrom==node & msgStream==stream);
            if (size(indices)>0)
                style = line_spec(node,1);
                style_len = size(style);
                hop_color_idx = mod(num,hop_color_size(1))+1;
                hop_color = hops_colors(hop_color_idx,:);
                num = num + 1;
                vals= zeros(1,secs);
                for s=0:secs-1
                   vals(s+1) = msgChannel(indices(s+1));
                end
                fc_legend(num) = cellstr(sprintf('node %d',node));
                plot(0:1:secs-1,vals,'Color',hop_color,'LineStyle',style(2:style_len(2)-1),'Marker',style(style_len(2)),'LineWidth',2);
            end
        end
        xlabel('Time [sec]');
        ylabel('Out rate [bits]');
        legend(fc_legend(1:num),'Location', 'NorthEastOutside');
         save_fig(fc_fig,sprintf('fc_str%d',stream),1);         
    end
end


layout_fig = figure('Position',[0 0 fullscreen(3) fullscreen(4)]);
hold on;
title('Scenario layout','FontSize',14,'FontWeight','bold');
for stream=1:streams_number
     switch (mod(stream,3)) 
    case 0  
        line_style = '--';
    case 1  
        line_style = ':';
    case 2  
        line_style = '-.';
     end
     quiver([0],[0],[0],[0],0.9,'Color',stream_colors(stream,:),'LineWidth',2,'LineStyle',line_style); % get the legend correctly but don't do anything
end
arrows(1) = struct('from',0,'to',0,'stream',0);
drawn_nodes = zeros(1,nodes);
collision_slots = zeros(3,slots);

'Processing schedule table and SNR'
tic
for i=1:size(tableStream)
  if (isnan(tableStream(i)))
    break;
  end
  from = tableFrom(i);
  to = tableTo(i);
  stream = tableStream(i);
  skip_draw = 0;
  arrows_size = size(arrows);
  arrows_size = arrows_size(2);
  for ar=1:arrows_size
	if (arrows(ar).from==from & arrows(ar).to==to & arrows(ar).stream==stream)
		skip_draw = 1;
		break;
	end
  end
	
  if (skip_draw==0)
	 arrows(arrows_size+1) = struct('from',from,'to',to,'stream',stream);
 	 figure(layout_fig);
	 hold on;
     drawn_nodes(1,from) = 1;
     drawn_nodes(1,to) = 1;
	 pFrom = [positionsX(from),positionsY(from)];
	 pTo= [positionsX(to),positionsY(to)];
     switch (mod(stream,3)) 
    case 0  
        line_style = '--';
    case 1  
        line_style = ':';
    case 2  
        line_style = '-.';
     end
	 %sprintf('Drawing line with %s style from %d->%d',style(1),from,to)
     quiver([positionsX(from)],[positionsY(from)],[positionsX(to)-positionsX(from)],[positionsY(to)-positionsY(from)],0.9,'Color',stream_colors(stream,:),'LineWidth',2,'LineStyle',line_style);
	 %vectarrow(pFrom,pTo,style,2);
	 %circle(pFrom,10,50,'+');
	 text(pFrom(1),pFrom(2),sprintf('%d',from),'FontSize',12,'FontWeight','bold');
	 %circle(pTo,10,50,'+');
	 text(pTo(1),pTo(2),sprintf('%d',to),'FontSize',12,'FontWeight','bold');
%  else
%   sprintf('skipping draw %d,%d,stream',from,to,stream)
  end
 
  collision_slots(tableChannel(i),tableSlot(i)) = collision_slots(tableChannel(i),tableSlot(i))+1;
  indicesTx = find(msgFrom==from & msgTo==to & msgEventType==typeSend & msgSlot==tableSlot(i) & msgChannel==tableChannel(i) & (msgTime>e2e_mean(stream)));
  TxSize = size(indicesTx);
  indicesRx = find(  (msgTime>=(secs-1)) & msgFrom==from & msgTo==to & msgEventType==typeRecv & msgSlot==tableSlot(i) & msgChannel==tableChannel(i)); % over last second
  RxSize = size(indicesRx);
  indicesErr = find(msgFrom==from & msgTo==to & msgEventType==typeLost & msgSlot==tableSlot(i) & msgChannel==tableChannel(i) & (msgTime>e2e_mean(stream)));
  ErrSize = size(indicesErr);
  indices = find(msgFrom==from & msgTo==to & (msgEventType==typeRecv | msgEventType==typeLost) & msgSlot==tableSlot(i) & msgChannel==tableChannel(i) & (msgTime>e2e_mean(stream)));
  SINR = msgSINR(indices);
  SINR = SINR(find(SINR~=-Inf));
  name = cellstr(sprintf('chan=%d,slot=%d,%d->%d',tableChannel(i),tableSlot(i),from,to));
  if (size(SINR)>0)
    dist = realsqrt((positionsX(from)-positionsX(to))*(positionsX(from)-positionsX(to))+(positionsY(from)-positionsY(to))*(positionsY(from)-positionsY(to)));
    pathloss_dist = beta + alpha*log10(dist);
    rxPwr_mW = 10*log10(TX_Power_mW) - pathloss_dist;
    SNR = rxPwr_mW - noise_dBm;
    sortedSINR = sort(SINR);
    minSINR = sortedSINR(1);
    %sprintf('dist=%g,pl_dist=%g,rx=%g,SINR=%g,SNR=%g,noise=%g,loss=%g\n',dist,pathloss_dist,rxPwr_mW,SINR(i),SNR,noise_dBm,SNR_loss_per_slot)
    table_entry(i) = struct('minSINR',minSINR,'SNR',SNR,'label',name,'RX',RxSize(1),'TX',TxSize(1),'Err',ErrSize(1)); 
  end
end

%collision_slots_fig = figure('Position',[0 0 fullscreen(3) fullscreen(4)]);
%bar3(collision_slots);
%ylabel('channel');
%xlabel('slot');
%zlabel('number of transmitters in slot x channel');
%save_fig(collision_slots_fig,'transmissions',1); 


figure(layout_fig);
hold on;
for i=1:nodes
    if (drawn_nodes(1,i)==0)
	   text(positionsX(i),positionsY(i),sprintf('%d',i),'FontSize',10);        
    end
end


tic
    % expected rate per MCS
    actual = [6,9,12,18,24,36,48,54];
    %function  rate = expected_rate(mcs)
    %    bitrate = actual(mcs)*1E6;
    %   rate = (message_size/((656+message_size)/(bitrate/1E6)+305))*1E6;
    % end

if (exist('table_entry')==1)
   'Processing SINR'
    entries = size(table_entry);
    entries = entries(2);
    SNR_val = zeros(1,entries);
    SINR_val = zeros(1,entries);
    PER_val = zeros(1,entries);
     tp_ratio_val = zeros(1,entries);
    entries_labels = cell(1,entries);
    expected_PER_val = zeros(1,entries);

    j=1;
    for i=1:entries
        if (size(table_entry(1,i).SNR)>0)
            SNR_val(1,j)  =  table_entry(i).SNR;
            SINR_val(1,j)  =  min(table_entry(i).SNR,table_entry(i).minSINR);
            entries_labels(1,j) = cellstr(table_entry(i).label);
            PER_val(1,j) = table_entry(i).Err*100/(table_entry(i).Err+table_entry(i).RX);
            tp_ratio_val(1,j) = table_entry(i).RX*message_size/((tableData(i))*1E6);
            if (hasGraphData == 1)
                indices =  find(graphFrom==tableFrom(j) & graphTo==tableTo(j));
                if (size(indices)>0)
                    expected_PER_val(1,j) = graphPER(indices)*100;
                else
                    expected_PER_val(1,j) = 0;
                end
            end
            j=j+1;
        end
    end

    if (full_stat==1)
        entries_labels_tp_ratio = entries_labels;
        [sorted_tp_ratio_val,indices] = sort(tp_ratio_val);
        entries_labels_tp_ratio = entries_labels_tp_ratio(indices);
        tp_ratio_table_fig = figure('Position',[0 -50 fullscreen(3) fullscreen(4)]);
        orient tall;
        hold on;
        bar(sorted_tp_ratio_val); 
        title(sprintf('Throughput ratio (actual/expected) by Edge x Slot x Channel'),'FontSize',14,'FontWeight','bold');
        ylabel('Actual/Expected []');
        xlabel('Edge x Slot x Channel ID []');
        save_fig(tp_ratio_table_fig,'throughput_ratio',1); 

        tp_ratio_table_low_fig = figure('Position',[0 -50 fullscreen(3) fullscreen(4)]);
        orient tall;
        hold on;
        bar(sorted_tp_ratio_val(1:10)); 
        title(sprintf('Lowest throughput ratio (actual/expected) by Edge x Slot x Channel '),'FontSize',14,'FontWeight','bold');
        ylabel('Actual/Expected []');
        set(gca,'xticklabel',[1:10]); % needed in octave
        xticklabel_rotate([1:10],90,entries_labels_tp_ratio(1:10),'FontSize',6);
        save_fig(tp_ratio_table_low_fig,'lowest_throughput_ratio',1); 
    end


    entries_labels_PER = entries_labels;
    [sorted_PER,indices] = sort(PER_val);
    entries_labels_PER = entries_labels_PER(indices);
    per_table_fig = figure('Position',[0 -50 fullscreen(3) fullscreen(4)]);
    orient tall;
    hold on;
    bar(sorted_PER); 
    title(sprintf('PER by Edge x Slot x Channel'),'FontSize',14,'FontWeight','bold');
    ylabel('Packet error rate [%]');
    xlabel('Edge x Slot x Channel ID []');
    save_fig(per_table_fig,'table_PER',1); 
    sorted_PER_size = size(sorted_PER);
    sorted_PER_size = sorted_PER_size(2);

    if (sorted_PER(sorted_PER_size)>0) % has some per
      per_table_high_fig = figure('Position',[0 -50 fullscreen(3) fullscreen(4)]);
      orient tall;
      hold on;
      if (hasGraphData == 1)
         sorted_exp_PER = expected_PER_val(indices);
         sp(:,1) = sorted_PER(sorted_PER_size-10:sorted_PER_size);
         sp(:,2) = sorted_exp_PER(sorted_PER_size-10:sorted_PER_size);
         bar(sp);    
         legend('PER','Scheudler PER');
      else   
        bar(sorted_PER(sorted_PER_size-10:sorted_PER_size)); 
      end
      title(sprintf('Highest PER by Edge x Slot x Channel '),'FontSize',14,'FontWeight','bold');
      ylabel('Packet error rate [%]');
      set(gca,'xticklabel',[1:11]); % needed in octave
      xticklabel_rotate([1:11],90,entries_labels_PER(sorted_PER_size-10:sorted_PER_size),'FontSize',6);
      save_fig(per_table_high_fig,'high_table_PER',1); 
    end

    SNR_loss_val(1,:) = SNR_val(1,:) - SINR_val(1,:);
    [sorted_SNR_loss,indices] = sort(SNR_loss_val);
    SNR_val = SNR_val(indices);
    SINR_val = SINR_val(indices);
    entries_labels  = entries_labels(indices);

    x_size = 10; % size of x labels
    SNR_losses_size = size(sorted_SNR_loss);
    SNR_losses_size = SNR_losses_size(2);

    % draw upper 
    indices = find(SNR_loss_val>=sorted_SNR_loss(max(SNR_losses_size-x_size,1)) & SNR_val>0);
    isize = size(indices);
    isize=isize(2);
    x_size = min(isize,x_size);
    if (x_size>0)
        sinru_fig = figure('Position',[0 -50 fullscreen(3) fullscreen(4)]);
        orient tall;
        indices=indices(1,1:x_size);
        power_val(:,1) =  SNR_val(indices)';
        power_val(:,2) =  SINR_val(indices)';
        upperLabels = entries_labels(indices);
        hold on;
        bar(power_val); 
        title(sprintf('SNR and SINR per table entries- Highest loss'),'FontSize',14,'FontWeight','bold');
        ylabel('[dB]');
        set(gca,'xticklabel',[1:x_size]); % needed in octave
        xticklabel_rotate([1:x_size],90,upperLabels,'FontSize',6);
        legend({'SNR','SINR'});
        save_fig(sinru_fig,'sinr_loss_high',1); 
    end

    % draw middle
    x_size = 10; % size of x labels
    val_bot = sorted_SNR_loss(round(max((entries-x_size)/2,1)));
    indices = find((SNR_loss_val>=val_bot) & SNR_val>0);
    isize = size(indices);
    isize=isize(2);
    x_size = min(isize,x_size);
    if (x_size>0)
        sinrm_fig = figure('Position',[0 -50 fullscreen(3) fullscreen(4)]);
        orient tall;
        indices=indices(1:x_size);
        clear power_val
        power_val(:,1) =  SNR_val(indices)';
        power_val(:,2) =  SINR_val(indices)';
        medianLabels = entries_labels(indices);
        hold on;
        bar(power_val); 
        legend({'SNR','SINR'});
        title(sprintf('SNR and SINR per table entries-  Median loss'),'FontSize',14,'FontWeight','bold');
        ylabel('[dB]');
        set(gca,'xticklabel',[1:x_size]); % needed in octave
        xticklabel_rotate([1:x_size],90,medianLabels,'FontSize',6);
        save_fig(sinrm_fig,'sinr_loss_median',1); 
    end

    % draw lower
    x_size = 10; % size of x labels
    indices = find(SNR_loss_val<=sorted_SNR_loss(min(entries,x_size)) & SNR_val>0);
    isize = size(indices);
    isize=isize(2);
    x_size = min(isize,x_size);
    if (x_size>0)
        sinrl_fig = figure('Position',[0 -50 fullscreen(3) fullscreen(4)]);
        orient tall;
        indices=indices(1,1:x_size);
        clear power_val
        power_val(:,1) =  SNR_val(indices)';
        power_val(:,2) =  SINR_val(indices)';
        lowerLabels = entries_labels(indices);
        hold on;
        bar(power_val);
        legend({'SNR','SINR'});
        title(sprintf('SNR and SINR per table entries- Lower loss'),'FontSize',14,'FontWeight','bold');
        ylabel(' [dB]');
        set(gca,'xticklabel',[1:x_size]); % needed in octave
        xticklabel_rotate([1:x_size],90,lowerLabels,'FontSize',6,'FontWeight','Bold');
        save_fig(sinrl_fig,'sinr_loss_lower',1); 
    end
    toc
end

figure(layout_fig);
hold on;
axis([0 playground 0 playground]);
set(gcf,'PaperType','E');
hLegend = legend(Streams,'Location', 'NorthEastOutside');
xlabel('x [meter]');
ylabel('y [meter]');
save_fig(layout_fig,'scenario',0); 


'Processing PER and dropped'
per=zeros(streams_number,1);
sched_flow=zeros(streams_number,1);
dropped=zeros(streams_number,1);

per_fig = figure('Position',[0 -50 fullscreen(3) fullscreen(4)]);
hold on;
title('Packet Error Rate per stream','FontSize',14,'FontWeight','bold');
ylabel('lost/transmitted [%]');
xlabel('Stream number []');

tp_vs_sched_fig = figure('Position',[0 -50 fullscreen(3) fullscreen(4)]);
hold on;
title('Throughput vs. scheduled','FontSize',14,'FontWeight','bold');
ylabel('Mbps received/Mbps scheduled []');
xlabel('Stream number []');

dropped_fig = figure('Position',[0 -50 fullscreen(3) fullscreen(4)]);
hold on;
title('Packet Dropped Rate per stream','FontSize',14,'FontWeight','bold');
ylabel('dropped/transmitted [%]');
xlabel('Stream number []');

    %function num = count_different(vector);
     %   sorted = sort(vector);
      %  [siz b] = size(sorted);
      %  num = 0;
      %  for i=1:siz-1
       %     if (sorted(i)~=sorted(i+1))
        %        num = num + 1;
         %   end
     %   end
  %  end

%tp_msg = zeros(streams_number,secs);
for stream=1:streams_number
  indices = find(msgEventType==typeLost & msgStream==stream & (msgTime>e2e_mean(stream))); % lost
  [  err_size dummy] = size(msgID(indices));
  indices = find(msgEventType==typeRecv & msgStream==stream & (msgTime>e2e_mean(stream)));  % recv
   [  recv_size dummy] = size(msgID(indices));
  indices = find(msgEventType==typeSend & msgStream==stream & (msgTime>e2e_mean(stream)));  % send
   [  good_size dummy] = size(msgID(indices));
  indices = find(msgEventType==typeDrop & msgStream==stream & (msgTime>e2e_mean(stream))); % dropped
  [  dropped_size dummy ] = size(msgID(indices));
  per(stream) = err_size*100/(recv_size+err_size);
  dropped(stream) = dropped_size*100/(good_size+dropped_size+q_len_initiator(stream));
  if (q_len_initiator(stream)~=q_len_initiator(stream))
      q_len_initiator(stream)=0;
  end
  indices = find(tableFrom==sources(stream) & tableStream==stream);
  data = tableData(indices);
  scheduled = sum(data);
  if (scheduled==0)
    sched_flow(stream) = 1;
  else
    sched_flow(stream) = (throughput(stream)*roundTime)/scheduled;
  end
   
  if (full_stat==1)
      for node=1:nodes
          indices = find(msgStream==stream & (msgTo==node | msgFrom==node));
          isize = size(indices);
          if (isize(1)>0) % node in stream
            tp_in_node_str = zeros(1,secs);
            tp_out_node_str = zeros(1,secs);
            per_node_str = zeros(1,secs);
            drop_node_str = zeros(1,secs);
            for sec=1:secs
               indices = find(msgEventType==typeSend & msgStream==stream  & (msgTime>=sec-1) & (msgTime<sec) & msgFrom==node);  % send
               [tx_size dummy ] = size(indices);
               tp_out_node_str(sec) = (tx_size*message_size/1E6)/roundTime;
               indices = find(msgEventType==typeRecv & msgStream==stream  & (msgTime>=sec-1) & (msgTime<sec) & msgTo==node);  % recv
               [recv_size dummy ] = size(indices);
               tp_in_node_str(sec) = (recv_size*message_size/1E6)/roundTime;
               indices = find(msgEventType==typeDrop & msgStream==stream  & (msgTime>=sec-1) & (msgTime<sec) & msgTo==node);  % dropped
               [dropped_size dummy] = size(indices);
               drop_node_str(sec) = dropped_size;
               indices = find(msgEventType==typeLost & msgStream==stream & (msgTime>=sec-1) & (msgTime<sec) & msgFrom==node); % lost               
               [err_size dummy] = size(msgID(indices));
               per_node_str(sec) = err_size*100/(err_size+recv_size);
            end
            tp_out_node_str_fig = figure('Position',[0 -50 fullscreen(3) fullscreen(4)]);
            hold on;
            title(sprintf('Throughput Out node %d,stream %d',node,stream),'FontSize',14,'FontWeight','bold');
            ylabel('Mbps sent in round [Mbps]');
            xlabel('Time [sec]');
            bar(tp_out_node_str);
            save_fig(tp_out_node_str_fig,sprintf('tp_out_str%d_node%d',stream,node),1); 

            tp_in_node_str_fig = figure('Position',[0 -50 fullscreen(3) fullscreen(4)]);
            hold on;
            title(sprintf('Throughput In node %d,stream %d',node,stream),'FontSize',14,'FontWeight','bold');
            ylabel('Mbps received in round [Mbps]');
            xlabel('Time [sec]');
            bar(tp_in_node_str);
            save_fig(tp_in_node_str_fig,sprintf('tp_in_str%d_node%d',stream,node),1); 

            tp_in_node_str_fig = figure('Position',[0 -50 fullscreen(3) fullscreen(4)]);
            hold on;
            title(sprintf('PER node %d,stream %d',node,stream),'FontSize',14,'FontWeight','bold');
            ylabel('PER (out) [%]');
            xlabel('Time [sec]');
            bar(per_node_str);
            save_fig(tp_in_node_str_fig,sprintf('per_str%d_node%d',stream,node),1); 

            drop_node_str_fig = figure('Position',[0 -50 fullscreen(3) fullscreen(4)]);
            hold on;
            title(sprintf('Packets dropped node %d,stream %d',node,stream),'FontSize',14,'FontWeight','bold');
            ylabel('Dropped  [#msg]');
            xlabel('Time [sec]');
            bar(drop_node_str);
            save_fig(drop_node_str_fig,sprintf('drop_str%d_node%d',stream,node),1); 
          end
      end
  end
end

[max_drop_val,max_drop_str] = max(dropped);

dropped_time = zeros(1,secs*roundTime);
for sec=roundTime:roundTime:secs*roundTime
  indices = find(msgEventType==typeSend & (msgTime>sec-roundTime) & (msgTime<=sec)  & msgStream==max_drop_str);  % send
   [  good_size dummy] = size(msgID(indices));
  indices = find(msgEventType==typeDrop & (msgTime>sec-roundTime) & (msgTime<=sec) & msgStream==max_drop_str); % dropped
  [  dropped_size dummy ] = size(msgID(indices));
  indices = find(msgEventType==typeDrop & (msgTime>sec-roundTime) & (msgTime<=sec) & msgFrom==msgTo & msgStream==max_drop_str); % dropped in source
  [  dropped_size_source dummy ] = size(msgID(indices));
  dropped_time(sec/roundTime) = dropped_size*100/(good_size+dropped_size_source);
   % indicesRx = find(  (msgTime>=(sec-1)) & (msgTime<sec) & msgTo==destinations(stream) & msgStream==stream & msgEventType==typeRecv); % received in second
    % rx_size = size(indicesRx);
   % rx_size = rx_size(1);
   % tp_msg(stream,sec) = rx_size*message_size/1E6; % Mbits
end
drop_time_fig = figure('Position',[0 -50 fullscreen(3) fullscreen(4)]);
hold on;
title(sprintf('Maximal packet drop in time (stream #%d)',max_drop_str),'FontSize',14,'FontWeight','bold');
ylabel('Dropped/Transmitted[%]');
xlabel('Time [sec]');
bar(dropped_time);
save_fig(drop_time_fig,'max_drop_in_time',1); 

%tp_msg_fig = figure('Position',[0 -50 fullscreen(3) fullscreen(4)]);
%hold on;
%title('Throughput','FontSize',14,'FontWeight','bold');
%ylabel('Mbps received in round [Mbps]');
%xlabel('Stream number []');
%bar(streams_range,tp_msg);
%legend(Streams,'Location', 'NorthEastOutside');
%save_fig(tp_msg_fig,'tp_msg',1); 

axis normal;
figure(per_fig);
bar(streams_range,per);
set(gca,'XTick',streams_range);
save_fig(per_fig,'per',0); 
figure(tp_vs_sched_fig);
bar(streams_range,sched_flow);
set(gca,'XTick',streams_range);
save_fig(tp_vs_sched_fig,'tp_vs_sched',0); 
figure(dropped_fig);
bar(streams_range,dropped);
set(gca,'XTick',streams_range);
save_fig(dropped_fig,'dropped',0); 

toc

if (full_stat==1)
    save vars.mat
end
hold off;
end

