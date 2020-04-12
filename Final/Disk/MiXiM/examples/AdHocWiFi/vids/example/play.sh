#!/bin/sh
PATH="/cygdrive/c/Program Files (x86)/VideoLAN/VLC:/cygdrive/c/Program Files/VideoLAN/VLC:$PATH"
#echo $PATH
echo "Playing original"
#vlc --help -H  --advanced
vlc "../wildlife.mpg" &
echo "Loading VLC"
vlc "rtp://239.255.42.42:8888" --noaudio & 
sleep 2s
echo "Streaming"
../../rtpreplay/bin/rtpreplay.exe  -f ../wildlife -t node1_str12.txt 239.255.42.42/8888 &
#../../rtpreplay/bin/rtpreplay.exe  -f ../wildlife -t node1_str12.txt 127.0.0.1/6666
