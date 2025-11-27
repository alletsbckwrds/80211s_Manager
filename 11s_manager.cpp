#include <iostream>
#include <stdlib.h> //for getenv()
#include <fstream>
#include <sys/stat.h>
#include <boost/algorithm/string.hpp>
#include <languagestr.hpp>

bool fexists (const std::string& name) {
	struct stat buffer;
	return (stat (name.c_str(), &buffer) == 0);
}

std::string sanitise (std::string& in) {
	//there is a much cleaner way to do it (for cycle with replace(find(...),...)
	boost::replace_all(in,"\x5C","\x5C\x5C"); //must be first.
	boost::replace_all(in,"\"","\x5C\"");
	boost::replace_all(in,"$","\x5C$");
	return in;
}

void flagcheck(char flag) {
	switch(flag){
		case 'a':
			system("sudo ~/.config/.mesh_config");
			break;
		case 'd':
			system("iw list | grep -e \" phy\" -e \"* mesh point$\"");
			break;
		case 'V':
			std::cout<<"Version 0.1.0\n";
			break;
		default:
			std::cout<<"Usage: mmcli [-a|-d|-V|-h]\n\n\t-a\tInstantly apply configuration.\n\t-d\tDiagnose NICs for 802.11s support.\n\t-V\tPrint version.\n\t-h\tPrint this help message\n";		
	}
}

int main (int argc, char *argv[]) {
	if(argc > 1){
		flagcheck(argv[1][1]);
		return 0;
	}

	char choice = 'm';
	std::string iwdevice,meshnm,frqcy,bwht,ipass;
	std::string homedir = getenv("HOME");

	if(fexists(homedir+"/.config/.mesh_config")){
		STRONE;
		std::cin>>choice;
		if(choice != 'n'){system("sudo ~/.config/.mesh_config");}
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
	std::string psk,keymgmt="";
	switch(choice){
		case '2':
			wpaver="TF-PSK psk ";
			break;
		case '3':
			wpaver="SAE psk ";
	}
	if(wpaver != ""){
		STRSEVEN;
		std::cin>>psk;
		wpaver += ("\""+sanitise(psk)+"\"");
		keymgmt = "key_mgmt ";
	}

	STREIGHT;
	std::cin>>ipass;

	//if(ipass == ""){std::cout<<"Invalid IP."; return 10;}
	//if(meshnm == ""){std::cout<<"Invalid mesh."; return 11;}
	//if(iwdevice == ""){std::cout<<"Invalid device."; return 12;}
	//if(psk == "" && wpaver != ""){std::cout<<"Error, password empty."; return 13;}

	system("touch ~/.config/.mesh_config 2>/dev/null"); //will fail if file already exists
	system("sudo chown root:root ~/.config/.mesh_config && sudo chmod 722 ~/.mesh_config"); //rwx-w--w-

	//reusing variable psk to output to file
	psk = "#!/bin/bash\n\nif [ $(id -u) != \"0\" ]; then\necho \"Error, run as root.\"\nexit\nfi\n\n";
	psk += "##Stability options:\n#systemctl stop wpa_supplicant\n#systemctl stop NetworkManager\n\n";
	psk += ("echo \"Setting mesh mode...\"\nip link set "+iwdevice+" down\niw dev "+iwdevice+" set type mp\nip link set "+iwdevice+" up\n");
	psk += ("iw dev "+iwdevice+" mesh join "+meshnm+" "+frqcy+" "+bwht+" "+keymgmt+wpaver+"\necho \"Setting IPv4...\"\n");
	psk += ("ip addr add "+ipass+" dev "+iwdevice+"\n\n##Stability options:\n#iw dev "+iwdevice+" set power_save off\n");

	std::ofstream ConfigFile(homedir+"/.config/.mesh_config");
	ConfigFile<<psk;
	ConfigFile.close();

	system("sudo chmod 710 ~/.config/.mesh_config"); //rwx--x---

	STRNINE;
	std::cin>>choice;
	if(choice == 'n'){
		return 0;
	}else{
		system("sudo ~/.config/.mesh_config");
	}

	return 0;
}
