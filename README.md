## How to run
Either run the script directly: ```./11s_manager.sh``` or add it to the path without the extension and with a shebang... if you need instructions on how to operate scripts, you're in the wrong place.
## Expected output
![Expected output](https://github.com/alletsbckwrds/80211s_Manager/edit/main/readme/output.jpg?raw=true)
## Config file example
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
