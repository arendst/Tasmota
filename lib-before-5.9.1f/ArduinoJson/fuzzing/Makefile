# CAUTION: this file is invoked by https://github.com/google/oss-fuzz

CXXFLAGS += -I../include

all: \
	$(OUT)/json_fuzzer \
	$(OUT)/json_fuzzer_seed_corpus.zip \
	$(OUT)/json_fuzzer.options

$(OUT)/json_fuzzer: fuzzer.cpp
	$(CXX) $(CXXFLAGS) $< -o$@ $(LIB_FUZZING_ENGINE)

$(OUT)/json_fuzzer_seed_corpus.zip: seed_corpus/*
	zip -j $@ $?

$(OUT)/json_fuzzer.options:
	@echo "[libfuzzer]" > $@
	@echo "max_len = 256" >> $@
	@echo "timeout = 10" >> $@
