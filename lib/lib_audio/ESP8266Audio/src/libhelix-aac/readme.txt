Fixed-point HE-AAC decoder
Developed by RealNetworks, 2005
===============================

Overview
--------
This module contains a high-performance HE-AAC decoder for 32-bit fixed-point 
processors. The following is a summary of what is and is not supported:

Supported:
 - MPEG2, MPEG4 low complexity decoding (intensity stereo, M-S, TNS, PNS)
 - spectral band replication (SBR), high-quality mode
 - mono, stereo, and multichannel modes
 - ADTS, ADIF, and raw data block file formats

Not currently supported:
 - main or SSR profile, LTP
 - coupling channel elements (CCE)
 - 960/1920-sample frame size
 - low-power mode SBR
 - downsampled (single-rate) SBR
 - parametric stereo

Highlights
----------
 - highly optimized for ARM processors (details in docs/ subdirectory)
 - reference x86 implementation
 - C and assembly code only (C++ not required for codec library)
 - reentrant, statically linkable
 - low memory (details in docs/ subdirectory)
 - option to use Intel Integrated Performance Primitives (details below)

Supported platforms and toolchains
----------------------------------
This codec should run on any 32-bit fixed-point processor which can perform a full 32x32-bit 
multiply (providing a 64-bit result). The following processors and toolchains are supported:
 - x86, Microsoft Visual C++
 - x86, GNU toolchain (gcc)
 - ARM, ARM Developer Suite (ADS)
 - ARM, Microsoft Embedded Visual C++
 - ARM, GNU toolchain (gcc)

ARM refers to any processor supporting ARM architecture v.4 or above. Thumb is not required.

Generally ADS produces the fastest code. EVC 3 does not support inline assembly code for
ARM targets, so calls to MULSHIFT32 (smull on ARM) are left as function calls. This incurs
a significant performance penalty. For the fastest code on targets which do not normally use 
ADS consider compiling with ADS, using the -S option to output assembly code, and 
feeding this assembly code to the assembler of your choice. This might require some 
syntax changes in the .S file.

Adding support for a new processor is fairly simple. Simply add a new block to the file 
real/assembly.h which implements the required inline assembly functions for your processor. 
Something like

...
#elif defined NEW_PROCESSOR

/* you implement MULSHIFT32() and so forth */

#else
#error Unsupported platform in assembly.h
#endif

Optionally you can rewrite or add assembly language files optimized for your platform. Note 
that many of the algorithms are designed for an ARM-type processor, so performance of the
unmodified C code might be noticeably worse on other architectures. 

Adding support for a new toolchain is straightforward. Use the sample projects or the
Helix makefiles as a template for which source files to include.

Multichannel
------------
For multichannel, just set AAC_MAX_NCHANS in pub/aacdec.h to the desired max number 
of channels (default = 2) and recompile. This increases RAM usage since more memory 
is required to save state for multiple channels. See docs/memory.xls for details.
  
Directory structure
-------------------
fixpt/           platform-independent code and tables, public API
fixpt/docs       memory and CPU usage figures, callgraphs
fixpt/hxwrap     Helix wrapper code and makefiles
fixpt/ipp        source code which uses IPP for decoding (see the "IPP" section below)
fixpt/pub        public header files
fixpt/real       source code for RealNetworks' AAC decoder
fixpt/testwrap   sample code to build a command-line test application

Code organization
-----------------
fixpt/
  aacdec.c       main decode functions, exports C-only API
  aactabs.c      common tables used by all implementations
fixpt/pub/
  aaccommon.h    low-level codec API which aacdec.c calls
  aacdec.h       high-level codec API which applications call
  statname.h     symbols which get name-mangled by C preprocessor to allow static linking
fixpt/ipp        source code for wrapper files which link in IPP libraries
fixpt/real       full source code for RealNetworks AAC decoder, including SBR
fixpt/real/asm   optimized assembly code files for certain platforms

To build an AAC decoder library, you'll need to compile the top-level files and EITHER 
real/*.c OR ipp/*.c and the appropriate IPP library. 

Decoder using Real code: aacdec.c + aactabs.c + real/*.c + real/asm/[platform]/*.s (if necessary)
Decoder using IPP code:  aacdec.c + aactabs.c + ipp/*.c + ippac*.lib

IPP 
--- 
For certain platforms Intel® has created highly-optimized object code libraries of DSP 
routines. These are called the Intel® Integrated Performance Primitives (IPP). If IPP 
libraries are available for a platform, this AAC decoder can link them in and use them 
instead of the RealNetworks source code. To use IPP, you still need to build the top-level 
files (aacdec.c, aactabs.c). You also build the files in ipp/*.c. These are just thin 
wrappers which provide the glue logic between the top-level decode functions in 
aacdec.c and the optimized DSP primitives in the IPP libraries. IPP libraries are not 
included in this module. You must obtain them WITH A LICENSE directly from Intel. 
Further info on the latest versions of IPP (as of the date of this readme) is available 
from the URL below:

http://www.intel.com/software/products/ipp/

This site explains how to obtain IPP and the terms under which IPP libraries may be used.
The code in this module is merely wrapper code which calls IPP functions. You are fully 
responsible for adhering to the license agreement under which you obtain the IPP 
libraries from Intel.

readme.txt last updated 02/25/05
