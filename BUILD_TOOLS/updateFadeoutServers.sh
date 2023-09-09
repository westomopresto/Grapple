#! /bin/bash
pkill -u weston GrappleServer

steamcmd +login anonymous +app_update 1380950 +quit
nohup sh ~/.steam/steamapps/common/Fadeout\ Dedicated/GrappleServer.sh > /dev/null &