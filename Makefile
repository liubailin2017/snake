INSTALL_PATH = ~/bin
INSTALL_NAME = snake
install: build
	mkdir -p $(INSTALL_PATH)
	cp -r --preserve=timestamps bin/* $(INSTALL_PATH)/
	cp -r SDL_font.ttf $(INSTALL_PATH)/
	cp -r hscore.sav $(INSTALL_PATH)/

	rm -rf bin 
	echo Installed in $(INSTALL_PATH)/
build: dependent
	mkdir -p obj
	mkdir -p bin
	gcc -c main.c -o obj/main.o
	gcc -c file.c -o obj/file.o
	gcc -c ver.c -o obj/ver.o
	gcc obj/main.o file.o ver.o -o bin/snake  -lSDL2  -lSDL2_ttf -lpthread
dependent: 
	echo 'install sdl2 and other library that about Sdl2.'
	sudo apt install libsdl2-dev
	sudo apt install libsdl2-ttf-dev
	echo 'Installed finish'
