function outputStuff = per2snr(fileName,eventFileName)

outputStuff = 1;

tic
dataStruct = mfcsvread(fileName);
toc
Field = fieldnames(dataStruct); 
secs = round(dataStruct.Rounds(1));
slots = dataStruct.Slots(1);
alpha = dataStruct.PathLossAlpha(1);
beta = dataStruct.PathLossBeta(1);
TX_Power_mW = dataStruct.TX_POWER_mW(1);
noise_dBm = dataStruct.noise_dBm(1);
sources = dataStruct.Sources;
destinations = dataStruct.Destinations;
positionsX = dataStruct.Initial_Positions_X;
positionsY = dataStruct.Initial_Positions_Y;

for i=1:size(positionsX)
    if (isnan(positionsX(i)) || (positionsY(i)==0 && positionsX(i)==0))
      positionsY = positionsY(1:i-1);
      positionsX = positionsX(1:i-1);
      break;
    end
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
Streams = cell(1,streams_number);

tp_fig = figure;
hold on;
title('Throughput per MCS','FontSize',14,'FontWeight','bold');
ylabel('[Mbps]');
xlabel('MCS  []');
throughputs = zeros(streams_number,2);
throughputs(1,1) = 5.2;
throughputs(2,1) = 7.45;
throughputs(3,1) = 9.45;
throughputs(4,1) = 13;
throughputs(5,1) = 16;
throughputs(6,1) = 20.84;
throughputs(7,1) = 24.54;
throughputs(8,1) = 26.08;
tp_num = 1;
for iField = 1:length(Field)
      vec = dataStruct.(Field{iField});
      if (regexp(Field{iField},'^Throughput'))
	throughputs(tp_num,2) = vec(1)*10;
        tp_num = tp_num+1;
      end
end
bar(throughputs);
legend({'Calculated','Simulated'});
print(tp_fig,'-djpeg','mcs_throughputs.jpg');


eventsStruct = mfcsvread(eventFileName);

eventsFields = fieldnames(eventsStruct);

msgID = eventsStruct.(eventsFields(1));
msgFrom = eventsStruct.from;
msgTo = eventsStruct.to;
msgStream = eventsStruct.stream;
msgSlot = eventsStruct.slot;
msgChannel = eventsStruct.channel;
msgSINR = 10*log10(eventsStruct.SINR);
msgEventType = eventsStruct.Event__0_Recv__1_Send__2_Drop__3_Lost__4_Rate;
msgTime = eventsStruct.Time;
per = zeros(streams_number,secs+1);
SNR = zeros(1,secs+1);

for stream=1:streams_number
  for round=0:secs
    
    indices = find(msgEventType==1 & msgStream==stream & msgTime>=round & msgTime<=round+1); % count send
    send_count = size(indices);
    send_count = send_count(1);

    indices = find(msgEventType==0 & msgStream==stream & msgTime>round & msgTime<=round+1);
    SINR = msgSINR(indices);
    count = size(SINR);
    count = count(1);
    if (stream==1)
      mSINR = mean(SINR);
      if (isnan(mSINR))
        mSINR = 0;
      end
      SNR(1,round+1) = mSINR;
    end
      per(stream,round+1) = max(0,1-abs(count/send_count));
  end
end


%smooth graph
%for stream=1:streams_number
%  for round=0:secs
%	if (per(stream,round+1)>0.8 & round+1<secs)
%		if (per(stream,round)<0.5)
%			per(stream,round+1) = mean([per(stream,round),per(stream,round+2)]);
%		end
%	end
%  end
%end

per2snr = figure;
plot(SNR,per);
legend({'1(6Mbps)','2(9Mbps)','3(12Mbps)','4(18Mbps)','5(24Mbps)','6(36Mbps)','7(48Mbps)','8(54Mbps)'});

title('PER to SNR for different MCS ');
xlabel('SNR [dB]');
ylabel('PER []');
print(per2snr,'-djpeg','per2snr.jpg');
save per2snr.mat
