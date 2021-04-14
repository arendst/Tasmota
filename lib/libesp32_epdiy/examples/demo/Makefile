FONT_DIR ?= /usr/share/fonts/TTF
FONTCONVERT ?= ../../scripts/fontconvert.py
IMGCONVERT ?= ../../scripts/imgconvert.py

.PHONY: fonts
fonts: \
	main/firasans_20.h \
	main/firasans_12.h \
	main/img_zebra.h \
	main/img_board.h \
	main/img_giraffe.h \
	main/img_beach.h

main/firasans_%.h: $(FONTCONVERT)
	python3 $(FONTCONVERT) --compress FiraSans_$* $* $(FONT_DIR)/FiraSans-Regular.ttf $(FONT_DIR)/Symbola.ttf  > $@

main/img_%.h: files/%.jpg
	python3 $(IMGCONVERT) -i $< -o $@ -n img_$*

clean:
	rm main/*.h
