FONT_SIZE ?= 8
FONT_DIR ?= /usr/share/fonts/TTF
FONTCONVERT ?= ../../scripts/fontconvert.py

ADDITIONAL_INTERVALS ?= 0xE0A0,0xE0A2 0xE0B0,0xE0B3 0x1F600,0x1F680 0x2500,0x259F 0x2200,0x22FF

.PHONY: fonts main/*
fonts: main/firacode.h main/firacode_bold.h main/firasans.h main/firasans_bold.h

main/firacode.h:
	 python3 $(FONTCONVERT) \
		 $(addprefix --additional-intervals ,$(ADDITIONAL_INTERVALS)) \
		 FiraCode \
		 $(FONT_SIZE) \
		 $(FONT_DIR)/FiraCode-Regular.ttf $(FONT_DIR)/Symbola.ttf > $@

main/firacode_bold.h:
	 python3 $(FONTCONVERT) \
		 $(addprefix --additional-intervals ,$(ADDITIONAL_INTERVALS)) \
		 FiraCode_Bold \
		 $(FONT_SIZE) \
		 $(FONT_DIR)/FiraCode-Bold.ttf $(FONT_DIR)/Symbola.ttf > $@

main/firasans.h:
	 python3 $(FONTCONVERT) \
		 $(addprefix --additional-intervals ,$(ADDITIONAL_INTERVALS)) \
		 FiraSans \
		 $(FONT_SIZE) \
		 $(FONT_DIR)/FiraSans-Regular.ttf $(FONT_DIR)/Symbola.ttf $(FONT_DIR)/FiraCode-Regular.ttf > $@

main/firasans_bold.h:
	 python3 $(FONTCONVERT) \
		 $(addprefix --additional-intervals ,$(ADDITIONAL_INTERVALS)) \
		 FiraSans_Bold \
		 $(FONT_SIZE) \
		 $(FONT_DIR)/FiraSans-Bold.ttf $(FONT_DIR)/Symbola.ttf $(FONT_DIR)/FiraCode-Bold.ttf > $@
