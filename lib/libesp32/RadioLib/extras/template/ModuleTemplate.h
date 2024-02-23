/*
  RadioLib Module Template header file

  Before opening pull request, please make sure that:
  1. All files MUST be compiled without errors using default Arduino IDE settings.
  2. All files SHOULD be compiled without warnings with compiler warnings set to "All".
  3. Example sketches MUST be working correctly and MUST be stable enough to run for prolonged periods of time.
  4. Writing style SHOULD be consistent.
  5. Comments SHOULD be in place for the most important chunks of code and SHOULD be free of typos.
  6. To indent, 2 spaces MUST be used.

  If at any point you are unsure about the required style, please refer to the rest of the modules.
*/

#if !defined(_RADIOLIB_<module_name>_H) && !defined(RADIOLIB_EXCLUDE_<module_name>)
#if !defined(_RADIOLIB_<module_name>_H)
#define _RADIOLIB_<module_name>_H

/*
  Header file for each module MUST include Module.h and TypeDef.h in the src folder.
  The header file MAY include additional header files.
*/
#include "../../Module.h"
#include "../../TypeDef.h"

/*
  Only use the following include if the module implements methods for OSI physical layer control.
  This concerns only modules similar to SX127x/RF69/CC1101 etc.

  In this case, your class MUST implement all virtual methods of PhysicalLayer class.
*/
//#include "../../protocols/PhysicalLayer/PhysicalLayer.h"

/*
  Register map
  Definition of SPI register map SHOULD be placed here. The register map SHOULD have two parts:

  1 - Address map: only defines register names and addresses. Register names MUST match names in
      official documentation (datasheets etc.).
  2 - Variable map: defines variables inside register. This functions as a bit range map for a specific register.
      Bit range (MSB and LSB) as well as short description for each variable MUST be provided in a comment.

  See RF69 and SX127x header files for examples of register maps.
*/
// <module_name> register map                                   | spaces up to this point
#define RADIOLIB_<module_name>_REG_<register_name>              0x00

// <module_name>_REG_<register_name>                                            MSB   LSB   DESCRIPTION
#define RADIOLIB_<module_name>_<register_variable>              0b00000000  //  7     0     <description>


/*
  Module class definition

  The module class MAY inherit from the following classes:

  1 - PhysicalLayer: In case the module implements methods for OSI physical layer control (e.g. SX127x).
  2 - Common class: In case the module further specifies some more generic class (e.g. SX127x/SX1278)
*/
class <module_name> {
  public:
    /*
      Constructor MUST have only one parameter "Module* mod".
      The class MAY implement additional overloaded constructors.
    */
    // constructor
    <module_name>(Module* mod);

    /*
      The class MUST implement at least one basic method called "begin".
      The "begin" method MUST initialize the module and return the status as int16_t type.
    */
    // basic methods
    int16_t begin();

    /*
      The class MAY implement additional methods.
      All implemented methods SHOULD return the status as int16_t type.
    */

#if !defined(RADIOLIB_GODMODE)
  private:
#endif
    /*
      The class MUST contain private member "Module* _mod"
    */
    Module* _mod;

    /*
      The class MAY contain additional private variables and/or methods.
      Private member variables MUST have a name prefixed with "_" (underscore, ASCII 0x5F)

      Usually, these are variables for saving module configuration, or methods that do not have to be exposed to the end user.
    */
};

#endif

#endif
