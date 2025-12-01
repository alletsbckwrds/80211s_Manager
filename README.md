## How to build
- First compile: ```make english``` (Spanish is also supported.)<br/>
- Then install: ```sudo make install```
- Use it: ```mmcli -h```
### Warning
DO NOT build multiple languages before installing! Run ```make clean``` if you need to rebuild.
### Note
- Not all NICs support "mesh point" mode. Modern ones should though. See if yours does with ```mmcli -d```
- This program uses ```sudo```, if you use ```doas``` or ```su -c```, i trust you to know how to edit this program.
- To uninstall: ```sudo make uninstall```
- Depends on ```iw``` and ```wpa_supplicant```
- Not all builds of ```wpa_supplicant``` support mesh mode. if you get weird errors after wpa_supplicant initialisation, maybe your build does not support mesh.
- More usage instructions with: ```mmcli -h```
## Issues
japanese language does not work, Password field correctly handles special characters, but mesh name field does not.
## Todo
- Implement all the languages listed in the makefile.
- Handle IPv6 addresses.
- ~~Handle multiple configurations.~~
- Improve the code (help is appreciated, this code is so bad!).
- Make it do everything in C++, no system() calls.
