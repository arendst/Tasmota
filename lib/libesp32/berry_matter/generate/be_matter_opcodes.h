/*********************************************************************************\
* Compact form for attributes and clusters
*
* Generated content, do not edit
\*********************************************************************************/
#include <stddef.h>
#include <stdint.h>

typedef struct {
  uint16_t id;
  const char* name;
} matter_opcode_t;

const matter_opcode_t matter_OpCodes[] = {
  { 0x0000, "MsgCounterSyncReq" },
  { 0x0001, "MsgCounterSyncRsp" },
  { 0x0010, "MRP_Standalone_Acknowledgement" },
  { 0x0020, "PBKDFParamRequest" },
  { 0x0021, "PBKDFParamResponse" },
  { 0x0022, "PASE_Pake1" },
  { 0x0023, "PASE_Pake2" },
  { 0x0024, "PASE_Pake3" },
  { 0x0030, "CASE_Sigma1" },
  { 0x0031, "CASE_Sigma2" },
  { 0x0032, "CASE_Sigma3" },
  { 0x0033, "CASE_Sigma2_Resume" },
  { 0x0040, "StatusReport" },
  { 0xFFFF, NULL },
};
