## How to run
Either run the script directly: ```./11s_manager.sh``` or link/add it to the path.
## Expected output
![Expected output](https://github.com/alletsbckwrds/80211s_Manager/blob/main/readme/out.png?raw=true)
## Config ~~file~~ script example
11sconfig.sh:
```
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
This is really just a small draft of a script, it's really just meant to save some time.<br/>
One issue is that it saves the password in clear text, another is that the password cannot contain certain characters (due to my decision of scripting in bash)
## Todo
I honestly should've used Python or some other language.. so i might re-write it in the future.
