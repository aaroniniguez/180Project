#!/bin/bash
rfkill unblock bluetooth
echo -ne "power on\npairable on\ndiscoverable on\npair 20:16:08:04:80:56\ntrust 20:16:08:04:80:56\nconnect 20:16:08:04:80:56\nquit" | bluetoothctl

rfcomm connect 0 20:16:08:04:80:56 10> /dev/null &
hcitool rssi 20:16:08:04:80:56
watch -n 1 hcitool rssi 20:16:08:04:80:56

#| bluetoothctl
#echo -ne "agent KeyboardDisplay\n default-agent\nkkkkpower on\n\npairable on\ndiscoverable on\nscan on\npair 20:16:08:04:80:56"
