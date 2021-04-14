FONT_DIR ?= /usr/share/fonts/TTF
FONTCONVERT ?= ../../scripts/fontconvert.py
IMGCONVERT ?= ../../scripts/imgconvert.py

.PHONY: fonts main/*
fonts: main/default_album.h main/firasans_20.h main/firasans_16_bold.h main/firasans_16.h main/firasans_24.h 


main/default_album.h:
	python3 $(IMGCONVERT) -n DefaultAlbum -i default_album.png -o $@
	
main/firasans_%.h:
	 python3 $(FONTCONVERT) FiraSans$* $* $(FONT_DIR)/FiraSans-Light.ttf $(FONT_DIR)/Symbola.ttf $(FONT_DIR)/FiraCode-Regular.ttf --compress > $@

main/firasans_%_bold.h:
	 python3 $(FONTCONVERT) FiraSans$*_Bold $* $(FONT_DIR)/FiraSans-Bold.ttf $(FONT_DIR)/Symbola.ttf $(FONT_DIR)/FiraCode-Bold.ttf --compress > $@

clean:
	rm main/firasans_*.h
	rm main/default_album.h
