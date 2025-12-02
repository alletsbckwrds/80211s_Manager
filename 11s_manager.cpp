#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <sys/stat.h>
#include <string.h>
#include <languagestr.hpp>

int system(const std::string& command) {
	return system(command.c_str());
}

bool fexists (const std::string& name) {
	struct stat buffer;
	return (stat (name.c_str(), &buffer) == 0);
}

std::string sanitise (std::string& in) { //yes, it modifies the string, and then also returns it. this is on purpose.
	int pos = 0;
	while((pos=in.find('\x5C',pos)) != std::string::npos){
		in.replace(pos,1,"\x5C\x5C");
		pos += 2; //cuz i just added a new character
	}
	pos=0;
	while((pos=in.find('\"',pos)) != std::string::npos){ //there is surely a way to put everything into one while() but i am lazy
		in.replace(pos,1,"\x5C\"");
		pos += 2;
	}
	pos=0;
	while((pos=in.find('$',pos)) != std::string::npos){
		in.replace(pos,1,"\x5C$");
		pos += 2;
	}
	return in;
}

void check_safety_of_str(std::string& in) {
	//i prefer not to deal with these characters...
	if(in.find(' ') != std::string::npos || in.find('\'') != std::string::npos || in.find('$') != std::string::npos || in.find('(') != std::string::npos || in.find('[') != std::string::npos || in.find('&') != std::string::npos || in.find(';') != std::string::npos) {
		std::cout<<"Error: Bad character detected. Exiting.\n";
		exit(EXIT_FAILURE);
	}
}

void apply_conf(const char *profilename) {
	std::string cpp_profilename = profilename;
	check_safety_of_str(cpp_profilename);
	std::string command = "sudo ";
	command += getenv("HOME");
	command += "/.config/mmcli_conf/.mesh_config";
	command += cpp_profilename;
	system(command);
}

void new_config() {
	char choice = 'm';
	std::string iwdevice,meshnm,frqcy,bwht,ipass;
	std::string homedir = getenv("HOME");
	std::string profilename;
	STRTEN;
	std::cin>>profilename;
	check_safety_of_str(profilename);
	
	if(fexists(homedir+"/.config/mmcli_conf/.mesh_config"+profilename)){
		STRELEVEN;
	}

	STRTWO;
	std::cin>>iwdevice;

	STRTHREE;
	std::cin>>meshnm;

	frqcy="";
	STRFOUR;
	//std::cin>>frqcy;
	std::cin.ignore(); //clears previous newline
	std::getline(std::cin, frqcy);
	if(frqcy == "") frqcy = "2412";

	bwht="";
	STRFIVE;
	//std::cin>>bwht;
	std::getline(std::cin, bwht);
	if(bwht == "") bwht = "HT20";

	choice = '1';
	STRSIX;
	std::cin>>choice;
	std::string wpaver="";
	std::string psk;
	if (choice == '2'){
		wpaver="ctrl_interface=/var/run/wpa_supplicant\nuser_mpm=1\n\nnetwork={\n\tssid=\""+meshnm+"\"\n\tmode=5\n\tfrequency="+frqcy+"\n\tkey_mgmt=SAE\n\tpsk=";
	}

	if(wpaver != ""){
		STRSEVEN;
		std::cin>>psk;
		wpaver += ("\""+sanitise(psk)+"\"\n}");
	}

	STREIGHT;
	std::cin>>ipass;


	system("mkdir "+homedir+"/.config 2>/dev/null");
	system("mkdir "+homedir+"/.config/mmcli_conf 2>/dev/null");
	system("touch "+homedir+"/.config/mmcli_conf/.mesh_config"+profilename+" 2>/dev/null"); //will fail if file already exists
	system("sudo chown root:root "+homedir+"/.config/mmcli_conf/.mesh_config"+profilename+" && sudo chmod 722 "+homedir+"/.config/mmcli_conf/.mesh_config"+profilename); //rwx-w--w-

	//reusing variable psk to output to file
	psk = "#!/bin/bash\n\nif [ $(id -u) != \"0\" ]; then\necho \"Error, run as root.\"\nexit\nfi\n\n";
	psk += "systemctl stop wpa_supplicant\nsystemctl stop NetworkManager\n\n";
	if(wpaver == ""){ //if no encryption -> use the reliable "iw"
		psk += ("echo \"Setting mesh mode...\"\nip link set "+iwdevice+" down\niw dev "+iwdevice+" set type mp\nip link set "+iwdevice+" up\n");
		psk += ("iw dev "+iwdevice+" mesh join "+meshnm+" freq "+frqcy+" "+bwht+"\necho \"Setting IPv4...\"\n"); //+" "+keymgmt+wpaver+"\necho \"Setting IPv4...\"\n");
	}else{ //if user wants encryption -> use the less-supported wpa_supplicant
		psk += "wpa_supplicant -D nl80211 -i "+iwdevice+" -c "+homedir+"/.config/mmcli_conf/.wpa_config"+profilename+" -B\n";
	}
	psk += ("ip addr add "+ipass+" dev "+iwdevice+"\n\n##Stability options:\n#iw dev "+iwdevice+" set power_save off\n");
	
	std::ofstream ConfigFile(homedir+"/.config/mmcli_conf/.mesh_config"+profilename);
	ConfigFile<<psk;
	ConfigFile.close();

	system("sudo chmod 710 "+homedir+"/.config/mmcli_conf/.mesh_config"+profilename); //rwx--x---
	
	if(wpaver != ""){ //if user selected encryption, create the wpa_config file
		system("touch "+homedir+"/.config/mmcli_conf/.wpa_config"+profilename+" 2>/dev/null"); //will fail if file already exists
		system("sudo chown root:root "+homedir+"/.config/mmcli_conf/.wpa_config"+profilename+" && sudo chmod 722 "+homedir+"/.config/mmcli_conf/.wpa_config"+profilename); //rwx-w--w-
	
		std::ofstream WpaFile(homedir+"/.config/mmcli_conf/.wpa_config"+profilename);
		WpaFile<<wpaver;
		WpaFile.close();
		
		system("sudo chmod 710 "+homedir+"/.config/mmcli_conf/.wpa_config"+profilename); //rwx--x---
	}

	STRNINE;
	std::cin>>choice;
	if(choice == 'n'){
		return;
	}else{
		system("sudo "+homedir+"/.config/mmcli_conf/.mesh_config"+profilename);
	}
}

void flagcheck(char flag, const char *arg) {
	switch(flag){
		case 'a':
			apply_conf(arg);
			break;
		case 'd':
			system("iw list | grep -e \" phy\" -e \"* mesh point$\"");
			std::cout<<"-----wpa_supplicant version:-----\n";
			system("wpa_supplicant -v");
			break;
		case 'k':
			system("sudo systemctl stop NetworkManager && sudo systemctl stop wpa_supplicant");
			break;
		case 'n':
			new_config();
			break;
		case 'V':
			std::cout<<"Version 0.2.1\n";
			break;
		default:
			std::cout<<"Mesh network Manager Command Line Interface\nUsage: mmcli [-a|-d|-V|-n|-k|-h]\n\n\t-a PROFILE\tInstantly apply configuration.\n\t-d\t\tDiagnose NICs for 802.11s support.\n\t-V\t\tPrint version.\n\t-h\t\tPrint this help message.\n\t-k\t\tKill often-conflicting services.\n\t-n\t\tCreate new config (default).\n";		
	}
}

int main (int argc, char *argv[]) {
	if(argc > 1){
		flagcheck(argv[1][1], argc>2 ? argv[2] : "" ); //this is so good to look at lol
		return 0;
	}

	new_config();

	return 0;
}
