# Makefile for generating waveform header data

#### WAVEFORM HEADERS TO GENERATE: ########
all: include/epdiy_ED097TC2.h include/eink_ED097TC2.h include/eink_ED047TC2.h

# Generate 16 grascale update waveforms
EXPORT_EINK_MODES ?= 1,2,5,16,17
# Generate waveforms in room temperature range
EXPORT_TEMPERATURE_RANGE ?= 15,35

HRDGEN ?= ../../scripts/waveform_hdrgen.py
WAVEFORM_GEN ?= ../../scripts/epdiy_waveform_gen.py

include/eink_%.h: waveforms/eink_%.json
	python3 $(HRDGEN) --export-modes $(EXPORT_EINK_MODES) --temperature-range $(EXPORT_TEMPERATURE_RANGE) $* < $< > $@

include/epdiy_%.h: waveforms/epdiy_%.json
	python3 $(HRDGEN) --export-modes $(EXPORT_EINK_MODES) --temperature-range $(EXPORT_TEMPERATURE_RANGE) $* < $< > $@

waveforms/epdiy_%.json:
	python3 $(WAVEFORM_GEN) $* > $@

.PHONY: all
