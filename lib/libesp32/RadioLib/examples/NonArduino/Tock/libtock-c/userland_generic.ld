/* Userland Generic Layout
 *
 * This linker script is designed for Tock apps where the end microcontroller
 * is not known. Therefore, this script over provisions space on some platforms.
 */

/* Memory Spaces Definitions, 448K flash, 64K ram */
PROG_LENGTH = 0x00070000;
RAM_LENGTH  = 0x00010000;

ENTRY(_start)

/* Note: On platforms where apps are position-independent and relocatable, the
 * FLASH address here acts as a sentinel value for relocation fixup routines.
 * The application loader will select the actual location in flash where the app
 * is placed. On platforms where apps are compiled for fixed addresses, these
 * addresses will be changed automatically before the linking step.
 */
MEMORY {
    FLASH (rx) : ORIGIN = 0x80000000, LENGTH = PROG_LENGTH
    SRAM (RWX) : ORIGIN = 0x00000000, LENGTH = RAM_LENGTH
}

SECTIONS {
    /* Section for just the app crt0 header.
     * This must be first so that the app can find it.
     */
    .crt0_header :
    {
        /**
         * Populate the header expected by `crt0`:
         *
         *  struct hdr {
         *    uint32_t got_sym_start;
         *    uint32_t got_start;
         *    uint32_t got_size;
         *    uint32_t data_sym_start;
         *    uint32_t data_start;
         *    uint32_t data_size;
         *    uint32_t bss_start;
         *    uint32_t bss_size;
         *    uint32_t reldata_start;
         *    uint32_t stack_size;
         *  };
         */
        /* Offset of GOT symbols in flash from the start of the application
         * binary. */
        LONG(LOADADDR(.got) - ORIGIN(FLASH));
        /* Offset of where the GOT section will be placed in memory from the
         * beginning of the app's assigned memory. */
        LONG(_got - ORIGIN(SRAM));
        /* Size of GOT section. */
        LONG(SIZEOF(.got));
        /* Offset of data symbols in flash from the start of the application
         * binary. */
        LONG(LOADADDR(.data) - ORIGIN(FLASH));
        /* Offset of where the data section will be placed in memory from the
         * beginning of the app's assigned memory. */
        LONG(_data - ORIGIN(SRAM));
        /* Size of data section. */
        LONG(SIZEOF(.data));
        /* Offset of where the BSS section will be placed in memory from the
         * beginning of the app's assigned memory. */
        LONG(_bss - ORIGIN(SRAM));
        /* Size of BSS section */
        LONG(SIZEOF(.bss));
        /* First address offset after program flash, where elf2tab places
         * .rel.data section */
        LONG(LOADADDR(.endflash) - ORIGIN(FLASH));
        /* The size of the stack requested by this application */
        LONG(STACK_SIZE);
    } > FLASH =0xFF

    /* App state section. Used for persistent app data.
     * We put this first so that if the app code changes but the persistent
     * data doesn't, the app_state can be preserved.
     */
    .wfr.app_state :
    {
        KEEP (*(.app_state))
        . = ALIGN(4); /* Make sure we're word-aligned here */
    } > FLASH =0xFF

    /* Text section, Code! */
    .text :
    {
        . = ALIGN(4);
        _text = .;
        KEEP (*(.start))
        *(.text*)
        *(.rodata*)
        *(.srodata*) /* for RISC-V */
        KEEP (*(.syscalls))
        _etext = .;
        *(.ARM.extab*)
        . = ALIGN(4); /* Make sure we're word-aligned here */
    } > FLASH =0xFF

    /* Need to reserve room for the stack in the linker file. This makes the
     * _got addresses used by the compiler match what they will be when the
     * app is loaded into memory. This is not necessary for full PIC supported
     * platforms (like Cortex-M), but is needed when an app is compiled for a
     * fixed address.
     */
    .stack :
    {
        /* elf2tab requires that the `_SRAM_ORIGIN` symbol be present to
		 * mark the first address in the SRAM memory. Since ELF files do
		 * not really need to specify this address as they only care about
		 * loading into flash, we need to manually mark this address for
		 * elf2tab. elf2tab will use it to add a fixed address header in the
		 * TBF header if needed.
		 */
	    _sram_origin = .;

        /* Be conservative about our alignment for the stack. Different
         * architectures require different values (8 for ARM, 16 for RISC-V),
         * so we choose the largest value. In practice, this likely will not
         * matter since the start of SRAM is unlikely to be at a very peculiar
         * address.
         */
        . = ALIGN(16);
        _stack = .;
        . = _stack + STACK_SIZE;
        . = ALIGN(16);
    } > SRAM


    /* Global Offset Table */
    .got :
    {
        . = ALIGN(4); /* Make sure we're word-aligned here */
        _got = .;
        *(.got*)
        *(.got.plt*)
        . = ALIGN(4);
    } > SRAM AT > FLASH

    /* Data section, static initialized variables
     *  Note: This is placed in Flash after the text section, but needs to be
     *  moved to SRAM at runtime
     */
    .data :
    {
        . = ALIGN(4); /* Make sure we're word-aligned here */
        _data = .;
        KEEP(*(.data*))
        /* Include the "small data" in the data section. Otherwise it will be
         * dropped when the TBF is created.
         */
        KEEP(*(.sdata*))
        . = ALIGN(4); /* Make sure we're word-aligned at the end of flash */
    } > SRAM AT > FLASH

    /* BSS section, static uninitialized variables */
    .bss :
    {
        . = ALIGN(4); /* Make sure we're word-aligned here */
        _bss = .;
        KEEP(*(.bss*))
        KEEP(*(.sbss*)) /* for RISC-V */
        *(COMMON)
        . = ALIGN(4);
    } > SRAM

    /* End of flash. */
    .endflash :
    {
    } > FLASH

    /* ARM Exception support
     *
     * This contains compiler-generated support for unwinding the stack,
     * consisting of key-value pairs of function addresses and information on
     * how to unwind stack frames.
     * https://wiki-archive.linaro.org/KenWerner/Sandbox/libunwind
     * (See also https://github.com/tock/libtock-c/issues/48)
     *
     *
     * .ARM.exidx is sorted, so has to go in its own output section.
     *
     * __NOTE__: It's at the end because we currently don't actually serialize
     * it to the binary in elf2tab. If it was before the RAM sections, it would
     * through off our calculations of the header.
     *
     * The section is marked as NOLOAD, to ensure elf2tab does not serialize it
     * https://github.com/tock/elf2tab/pull/66
     */
    PROVIDE_HIDDEN (__exidx_start = .);
    .ARM.exidx (NOLOAD) :
    {
      /* (C++) Index entries for section unwinding */
      *(.ARM.exidx* .gnu.linkonce.armexidx.*)
    } > FLASH
    PROVIDE_HIDDEN (__exidx_end = .);
}

ASSERT(_got <= _bss, "
The GOT section must be before the BSS section for crt0 setup to be correct.");
ASSERT(_data <= _bss, "
The data section must be before the BSS section for crt0 setup to be correct.");
