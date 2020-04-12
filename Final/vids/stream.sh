#!/bin/sh
vlc $1 --sout '#transcode{vcodec=h264,deinterlace}:rtp{dst=239.255.42.42,port=8888,mux=ts}' --h264-fps=$2 --sout-transcode-fps=$2 --no-sout-rtp-sap --no-sout-standard-sap --sout-all --sout-keep 

#--sout '#transcode{vcodec=h264,vb=800,ab=128,deinterlace}:rtp{mux=ts,dst=239.255.42.42,port=8888,sdp=sap,name="WildLife"}'

# --h264-fps=$2 --sout-transcode-fps=$2
