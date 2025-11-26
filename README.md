## How to build
- First compile: ```make english``` (Spanish is also supported. Japanese has issues, so it has been disabled.)<br/>
- Then install: ```sudo make install```
- Use it: ```mmcli```
### Warning
DO NOT build multiple languages before installing! Run ```make clean``` if you need to rebuild.
### Note
- Not all NICs support "mesh point" mode. Modern ones should though.
- This program uses ```sudo```, if you use ```doas``` or ```su -c```, i trust you to know how to edit this program.
- To uninstall: ```sudo make uninstall```
- Depends on ```iw```
## Expected output
![Expected output](https://github.com/alletsbckwrds/80211s_Manager/blob/main/readme/out.png?raw=true)
## Config ~~file~~ script example
~/.mesh_config:
```
#!/bin/bash

if [ $(id -u) != "0" ]; then
echo "Error, run as root."
exit
fi

##Stability options:
#systemctl stop wpa_supplicant
#systemctl stop NetworkManager

echo "Setting mesh mode..."
ip link set wlan0 down
iw dev wlan0 set type mp
ip link set wlan0 up
iw dev wlan0 mesh join mymesh 2412 HT20 key_mgmt TF-PSK psk 'password123'
echo "Setting IPv4..."
ip addr add 10.0.0.1/8 dev wlan0

##Stability options:
#iw dev wlan0 set power_save off
```
## Issues
japanese language does not work, Password field correctly handles special characters, but mesh name field does not.
## Todo
- Implement all the languages listed in the makefile
- Handle flags: "mmcli -a" to apply config instantly, "mmcli -V" for version, and "mmcli -*" for help.
