if [ -f ./11sconfig.sh ]; then
	printf "Configuration file found. Apply now [A] or create new? [n] "
	read choice
	if [ $choice = "n" ]; then
		echo ""
	else
		sudo ./11sconfig.sh
		exit
	fi
fi

printf "Device: "
read iwdevic

printf "Mesh name: "
read meshnm

printf "Frequency (default: 2412): "
read frqcy
if [ -z "$frqcy" ]; then
	frqcy="2412"
fi

printf "Bandwidth (default: \"HT20\"): "
read bwht
if [ -z "$bwht" ]; then
	bwht="HT20"
fi

printf "Unencrypted [1] (def.), WPA2-PSK [2], WPA3-SAE [3]: "
read choice

wpaver=""
keymgmt=""
case "$choice" in
	"2")
		wpaver="TF-PSK psk " ;;
	"3")
		wpaver="SAE psk " ;;
esac

if [ -n "$wpaver" ]; then
	printf "Password: "
	read psk
	wpaver_="${wpaver}\'${psk}\'"
	wpaver=${wpaver_}
	keymgmt="key_mgmt "
fi

printf "IPv4 in xxx.xxx.xxx.xxx/mask format: "
read ipass

if [ -z "$ipass" ]; then
	echo "Invalid IP."
	exit
fi
if [ -z "$meshnm" ]; then
	echo "Invalid mesh."
	exit
fi
if [ -z "$iwdevic" ]; then
	echo "Invalid device."
	exit
fi
if [ -z "$psk" ]; then
	echo "Error, password empty."
	exit
fi



printf "if [ \$(id -u) != \"0\" ]; then
echo \"Error, run as root.\"
exit\nfi\n\n" >./11sconfig.sh
printf "##Stability options:
#systemctl stop wpa_supplicant
#systemctl stop NetworkManager\n\n" >>./11sconfig.sh
printf "echo \"Setting mesh mode...\"
ip link set ${iwdevic} down
iw dev ${iwdevic} set type mp
ip link set ${iwdevic} up
iw dev ${iwdevic} mesh join ${meshnm} ${frqcy} ${bwht} ${keymgmt}${wpaver}
echo \"Setting IPv4...\"
ip addr add ${ipass} dev ${iwdevic}\n\n" >>./11sconfig.sh
printf "##Stability options:
#iw dev ${iwdevic} set power_save off\n" >>./11sconfig.sh



chmod +x ./11sconfig.sh
echo "Config file created!"
printf "Apply now? [Y/n]"
read choice
if [ "$choice" = "n" ]; then
	exit
else
	sudo ./11sconfig.sh
fi
