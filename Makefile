CXX = g++

english: 11s_manager_eng

#chinese: 11s_manager_zh

#japanese: 11s_manager_jp

#italian: 11s_manager_it

#esperanto: 11s_manager_esper

spanish: 11s_manager_es

#all: english japanese spanish

clean:
	rm ./11s_manager_*

11s_manager_eng: 11s_manager.cpp ./headers/english/languagestr.hpp
	${CXX} 11s_manager.cpp -I./headers/english -o 11s_manager_eng

#11s_manager_zh: 11s_manager.cpp ./headers/chinese/languagestr.hpp
#	${CXX} 11s_manager.cpp -I./headers/chinese -o 11s_manager_zh

#11s_manager_jp: 11s_manager.cpp ./headers/japanese/languagestr.hpp
#	${CXX} 11s_manager.cpp -I./headers/japanese -o 11s_manager_jp

#11s_manager_esper: 11s_manager.cpp ./headers/esperanto/languagestr.hpp
#	${CXX} 11s_manager.cpp -I./headers/esperanto -o 11s_manager_esper

11s_manager_es: 11s_manager.cpp ./headers/spanish/languagestr.hpp
	${CXX} 11s_manager.cpp -I./headers/spanish -o 11s_manager_es

#11s_manager_it: 11s_manager.cpp ./headers/italian/languagestr.hpp
#	${CXX} 11s_manager.cpp -I./headers/italian -o 11s_manager_it

install:
	cp ./11s_manager_* /usr/local/bin/mmcli

uninstall:
	rm /usr/local/bin/mmcli
	echo "Remember the config files in ~/.config/mmcli_conf/*"
