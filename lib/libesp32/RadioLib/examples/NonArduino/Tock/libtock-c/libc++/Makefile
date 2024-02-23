GCC_VERSION ?= 11.2.0

all: rebuild-gcc

gcc-$(GCC_VERSION).tar.xz:
	@echo "Downloading gcc source $(@F)"
	@wget -q -O $@ 'https://bigsearcher.com/mirrors/gcc/releases/gcc-$(GCC_VERSION)/gcc-$(GCC_VERSION).tar.xz'

gcc-$(GCC_VERSION): gcc-$(GCC_VERSION).tar.xz
	@echo "Extracting $(<F)"
	@tar -xf $<
	@touch $@ # Touch so directory appears newer than tarball

rebuild-gcc: gcc-$(GCC_VERSION)
	@rm -rf gcc-$(GCC_VERSION)-out
	@mkdir -p gcc-$(GCC_VERSION)-out
	@echo "Entering directory gcc-$(GCC_VERSION)-out"
	cd gcc-$(GCC_VERSION)-out; sh ../build.sh ../$<
	cp gcc-$(GCC_VERSION)-out/arm-none-eabi/libgcc/libgcc.a .
	cp gcc-$(GCC_VERSION)-out/arm-none-eabi/libstdc++-v3/src/.libs/libstdc++.a .
	cp gcc-$(GCC_VERSION)-out/arm-none-eabi/libstdc++-v3/libsupc++/.libs/libsupc++.a .

clean:
	rm -f gcc-$(GCC_VERSION).tar.xz
	rm -rf gcc-$(GCC_VERSION)
	rm -rf gcc-$(GCC_VERSION)-out

.PHONY: clean rebuild-gcc

