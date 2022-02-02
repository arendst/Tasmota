
================================================================================

                     TinyCrypt Cryptographic Library

================================================================================

          Copyright (c) 2017, Intel Corporation. All rights reserved.         

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

  - Redistributions of source code must retain the above copyright notice, this 
      list of conditions and the following disclaimer.
      
  - Redistributions in binary form must reproduce the above copyright notice, 
      this list of conditions and the following disclaimer in the documentation 
      and/or other materials provided with the distribution.
      
  - Neither the name of the Intel Corporation nor the names of its contributors 
      may be used to endorse or promote products derived from this software 
      without specific prior written permission. 


THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND 
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR 
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES 
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; 
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON 
ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS 
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

================================================================================

Overview:

The TinyCrypt Library provides an implementation for constrained devices of a 
minimal set of standard cryptography primitives. 

Please, ***SEE THE DOCUMENTATION*** folder for more information on the supported 
cryptographic primitives and the limitations of TinyCrypt library. For usage,
security and technicalities, please see the corresponding header file of each 
cryptographic primitive. 

================================================================================

Organization:

/lib: C source code of the cryptographic primitives.
/lib/include/tinycrypt: C header files of the cryptographic primitives.
/tests: Test vectors of the cryptographic primitives.
/doc: Documentation of TinyCrypt. 

================================================================================

Building:

1) In Makefile.conf set: 
    - CFLAGS for compiler flags.
    - CC for compiler.
    - ENABLE_TESTS for enabling (true) or disabling (false) tests compilation.
2) In lib/Makefile select the primitives required by your project.
3) In tests/Makefile select the corresponding tests of the selected primitives.
4) make 
5) run tests in tests/

================================================================================

