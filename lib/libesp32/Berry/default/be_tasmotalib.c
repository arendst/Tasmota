/********************************************************************
 * Tasmota lib
 *
 * To use: `import tasmota`
 *******************************************************************/
#include "be_constobj.h"

extern int l_getFreeHeap(bvm *vm);
extern int l_publish(bvm *vm);
extern int l_cmd(bvm *vm);
extern int l_get_cb(bvm *vm);
extern int l_getoption(bvm *vm);
extern int l_millis(bvm *vm);
extern int l_timereached(bvm *vm);
extern int l_rtc(bvm *vm);
extern int l_time_dump(bvm *vm);
extern int l_memory(bvm *vm);
extern int l_yield(bvm *vm);
extern int l_delay(bvm *vm);
extern int l_scaleuint(bvm *vm);
extern int l_logInfo(bvm *vm);
extern int l_save(bvm *vm);

extern int l_respCmnd(bvm *vm);
extern int l_respCmndStr(bvm *vm);
extern int l_respCmndDone(bvm *vm);
extern int l_respCmndError(bvm *vm);
extern int l_respCmndFailed(bvm *vm);
extern int l_resolveCmnd(bvm *vm);

extern int l_respAppend(bvm *vm);
extern int l_webSend(bvm *vm);
extern int l_webSendDecimal(bvm *vm);

extern int l_getlight(bvm *vm);
extern int l_getpower(bvm *vm);
extern int l_setlight(bvm *vm);
extern int l_setpower(bvm *vm);

extern int l_i2cenabled(bvm *vm);


/********************************************************************
    // add `chars_in_string(s:string,c:string) -> int``
    // looks for any char in c, and return the position of the first char
    // or -1 if not found
    // inv is optional and inverses the behavior, i.e. look for chars not in the list
    "def chars_in_string(s,c,inv) "
      "var inverted = inv ? true : false "
      "for i:0..size(s)-1 "
        "var found = false "
        "for j:0..size(c)-1 "
          "if s[i] == c[j] found = true end "
        "end "
        "if inverted != found return i end "
      "end "
      "return -1 "
    "end "
********************************************************************/
/********************************************************************
** Solidified function: chars_in_string
********************************************************************/

be_define_local_const_str(chars_in_string_str_name, "chars_in_string", -1146182164, 15);
be_define_local_const_str(chars_in_string_str_source, "string", 398550328, 6);
be_define_local_const_str(chars_in_string_str_2, "stop_iteration", -121173395, 14);

static const bvalue chars_in_string_ktab[3] = {
  { { .i=0 }, BE_INT},
  { { .i=1 }, BE_INT},
  { { .s=be_local_const_str(chars_in_string_str_2) }, BE_STRING},
};

static const uint32_t chars_in_string_code[44] = {
  0x780E0001,  //  0000  JMPF	R3	#0003
  0x50100200,  //  0001  LDBOOL	R4	1	0
  0x70020000,  //  0002  JMP		#0004
  0x50100000,  //  0003  LDBOOL	R4	0	0
  0x60140000,  //  0004  GETGBL	R5	G0
  0x60180012,  //  0005  GETGBL	R6	G18
  0x5C1C0200,  //  0006  MOVE	R7	R1
  0x7C180200,  //  0007  CALL	R6	1
  0x4180D01,  //  0008  SUB	R6	R6	R257
  0x401A0006,  //  0009  CONNECT	R6	R256	R6
  0x7C140200,  //  000A  CALL	R5	1
  0xA802001A,  //  000B  EXBLK	0	#0027
  0x5C180A00,  //  000C  MOVE	R6	R5
  0x7C180000,  //  000D  CALL	R6	0
  0x501C0000,  //  000E  LDBOOL	R7	0	0
  0x60200000,  //  000F  GETGBL	R8	G0
  0x60240012,  //  0010  GETGBL	R9	G18
  0x5C280400,  //  0011  MOVE	R10	R2
  0x7C240200,  //  0012  CALL	R9	1
  0x4241301,  //  0013  SUB	R9	R9	R257
  0x40260009,  //  0014  CONNECT	R9	R256	R9
  0x7C200200,  //  0015  CALL	R8	1
  0xA8020007,  //  0016  EXBLK	0	#001F
  0x5C241000,  //  0017  MOVE	R9	R8
  0x7C240000,  //  0018  CALL	R9	0
  0x94280206,  //  0019  GETIDX	R10	R1	R6
  0x942C0409,  //  001A  GETIDX	R11	R2	R9
  0x1C28140B,  //  001B  EQ	R10	R10	R11
  0x782A0000,  //  001C  JMPF	R10	#001E
  0x501C0200,  //  001D  LDBOOL	R7	1	0
  0x7001FFF7,  //  001E  JMP		#0017
  0x58200002,  //  001F  LDCONST	R8	K2
  0xAC200200,  //  0020  CATCH	R8	1	0
  0xB0080000,  //  0021  RAISE	2	R0	R0
  0x20200807,  //  0022  NE	R8	R4	R7
  0x78220001,  //  0023  JMPF	R8	#0026
  0xA8040001,  //  0024  EXBLK	1	1
  0x80040C00,  //  0025  RET	1	R6
  0x7001FFE4,  //  0026  JMP		#000C
  0x58140002,  //  0027  LDCONST	R5	K2
  0xAC140200,  //  0028  CATCH	R5	1	0
  0xB0080000,  //  0029  RAISE	2	R0	R0
  0x5415FFFE,  //  002A  LDINT	R5	-1
  0x80040A00,  //  002B  RET	1	R5
};

static const bproto chars_in_string_proto = {
  NULL,     // bgcobject *next
  8,       // type
  0x08,    // marked 0x08
  12,       // nstack
  0,       // nupvals
  4,       // argc
  0,       // varg
  NULL,     // bgcobject *gray
  NULL,     // bupvaldesc *upvals
  (bvalue*) &chars_in_string_ktab,     // ktab
  NULL,     // bproto **ptab
  (binstruction*) &chars_in_string_code,     // code
  be_local_const_str(chars_in_string_str_name),       // name
  44,       // codesize
  3,       // nconst
  0,       // nproto
  be_local_const_str(chars_in_string_str_source),     // source
#if BE_DEBUG_RUNTIME_INFO /* debug information */
  NULL,     // lineinfo
  0,        // nlineinfo
#endif
#if BE_DEBUG_VAR_INFO
  NULL,     // varinfo
  0,        // nvarinfo
#endif
};

const bclosure chars_in_string_closure = {
  NULL,     // bgcobject *next
  36,       // type
  0x08,        // marked
  0,       // nupvals
  NULL,     // bgcobject *gray
  (bproto*) &chars_in_string_proto,     // proto
  { NULL }     // upvals
};

/*******************************************************************/


/********************************************************************
// find a key in map, case insensitive, return actual key or nil if not found
def find_key_i(m,keyi)
  import string
  var keyu = string.toupper(keyi)
  if classof(m) == map
    for k:m.keys()
      if string.toupper(k)==keyu || keyi=='?'
        return k
      end
    end
  end
end
********************************************************************/
/********************************************************************
** Solidified function: find_key_i
********************************************************************/

be_define_local_const_str(find_key_i_str_name, "find_key_i", 850136726, 10);
be_define_local_const_str(find_key_i_str_source, "string", 398550328, 6);
be_define_local_const_str(find_key_i_str_0, "string", 398550328, 6);
be_define_local_const_str(find_key_i_str_1, "toupper", -602983720, 7);
be_define_local_const_str(find_key_i_str_2, "keys", -112588595, 4);
be_define_local_const_str(find_key_i_str_3, "?", 973910158, 1);
be_define_local_const_str(find_key_i_str_4, "stop_iteration", -121173395, 14);

static const bvalue find_key_i_ktab[5] = {
  { { .s=be_local_const_str(find_key_i_str_0) }, BE_STRING},
  { { .s=be_local_const_str(find_key_i_str_1) }, BE_STRING},
  { { .s=be_local_const_str(find_key_i_str_2) }, BE_STRING},
  { { .s=be_local_const_str(find_key_i_str_3) }, BE_STRING},
  { { .s=be_local_const_str(find_key_i_str_4) }, BE_STRING},
};

static const uint32_t find_key_i_code[31] = {
  0xA40E0000,  //  0000  IMPORT R3  R256
  0x8C100701,  //  0001  GETMET R4  R3  R257
  0x5C180400,  //  0002  MOVE R6  R2
  0x7C100400,  //  0003  CALL R4  2
  0x60140004,  //  0004  GETGBL R5  G4
  0x5C180200,  //  0005  MOVE R6  R1
  0x7C140200,  //  0006  CALL R5  1
  0x6018000B,  //  0007  GETGBL R6  G11
  0x1C140A06,  //  0008  EQ R5  R5  R6
  0x78160013,  //  0009  JMPF R5  #001E
  0x60140000,  //  000A  GETGBL R5  G0
  0x8C180302,  //  000B  GETMET R6  R1  R258
  0x7C180200,  //  000C  CALL R6  1
  0x7C140200,  //  000D  CALL R5  1
  0xA802000B,  //  000E  EXBLK  0 #001B
  0x5C180A00,  //  000F  MOVE R6  R5
  0x7C180000,  //  0010  CALL R6  0
  0x8C1C0701,  //  0011  GETMET R7  R3  R257
  0x5C240C00,  //  0012  MOVE R9  R6
  0x7C1C0400,  //  0013  CALL R7  2
  0x1C1C0E04,  //  0014  EQ R7  R7  R4
  0x741E0001,  //  0015  JMPT R7  #0018
  0x1C1C0503,  //  0016  EQ R7  R2  R259
  0x781E0001,  //  0017  JMPF R7  #001A
  0xA8040001,  //  0018  EXBLK  1 1
  0x80040C00,  //  0019  RET  1 R6
  0x7001FFF3,  //  001A  JMP    #000F
  0x58140004,  //  001B  LDCONST  R5  K4
  0xAC140200,  //  001C  CATCH  R5  1 0
  0xB0080000,  //  001D  RAISE  2 R0  R0
  0x80000000,  //  001E  RET  0 R0
};

static const bproto find_key_i_proto = {
  NULL,     // bgcobject *next
  8,       // type
  0x08,        // marked
  10,       // nstack
  0,       // nupvals
  3,       // argc
  0,       // varg
  NULL,     // bgcobject *gray
  NULL,     // bupvaldesc *upvals
  (bvalue*) &find_key_i_ktab,     // ktab
  NULL,     // bproto **ptab
  (binstruction*) &find_key_i_code,     // code
  be_local_const_str(find_key_i_str_name),       // name
  31,       // codesize
  5,       // nconst
  0,       // nproto
  be_local_const_str(find_key_i_str_source),     // source
#if BE_DEBUG_RUNTIME_INFO /* debug information */
  NULL,     // lineinfo
  0,        // nlineinfo
#endif
#if BE_DEBUG_VAR_INFO
  NULL,     // varinfo
  0,        // nvarinfo
#endif
};

const bclosure find_key_i_closure = {
  NULL,     // bgcobject *next
  36,       // type
  0x08,        // marked
  0,       // nupvals
  NULL,     // bgcobject *gray
  (bproto*) &find_key_i_proto,     // proto
  { NULL }     // upvals
};

/*******************************************************************/



/********************************************************************
    // # split the item when there is an operator, returns a list of (left,op,right)
    // # ex: "Dimmer>50" -> ["Dimmer",tasmota_gt,"50"]
    "def find_op(item) "
      "import string "
      "var op_chars = '=<>!' "
      "var pos = self.chars_in_string(item, op_chars) "
      "if pos >= 0 "
        "var op_split = string.split(item,pos) "
        "var op_left = op_split[0] "
        "var op_rest = op_split[1] "
        "pos = self.chars_in_string(op_rest, op_chars, true) "
        "if pos >= 0 "
          "var op_split2 = string.split(op_rest,pos) "
          "var op_middle = op_split2[0] "
          "var op_right = op_split2[1] "
          "return [op_left,op_middle,op_right] "
        "end "
      "end "
      "return [item, nil, nil] "
    "end "
********************************************************************/
/********************************************************************
** Solidified function: find_op
********************************************************************/

be_define_local_const_str(find_op_str_name, "find_op", -528253920, 7);
be_define_local_const_str(find_op_str_source, "string", 398550328, 6);
be_define_local_const_str(find_op_str_0, "string", 398550328, 6);
be_define_local_const_str(find_op_str_1, "=<>!", -1630497019, 4);
be_define_local_const_str(find_op_str_2, "chars_in_string", -1146182164, 15);
be_define_local_const_str(find_op_str_4, "split", -2017972765, 5);

static const bvalue find_op_ktab[6] = {
  { { .s=be_local_const_str(find_op_str_0) }, BE_STRING},
  { { .s=be_local_const_str(find_op_str_1) }, BE_STRING},
  { { .s=be_local_const_str(find_op_str_2) }, BE_STRING},
  { { .i=0 }, BE_INT},
  { { .s=be_local_const_str(find_op_str_4) }, BE_STRING},
  { { .i=1 }, BE_INT},
};

static const uint32_t find_op_code[42] = {
  0xA40A0000,  //  0000  IMPORT	R2	R256
  0x580C0001,  //  0001  LDCONST	R3	K1
  0x8C100102,  //  0002  GETMET	R4	R0	R258
  0x5C180200,  //  0003  MOVE	R6	R1
  0x5C1C0600,  //  0004  MOVE	R7	R3
  0x7C100600,  //  0005  CALL	R4	3
  0x28140903,  //  0006  GE	R5	R4	R259
  0x78160019,  //  0007  JMPF	R5	#0022
  0x8C140504,  //  0008  GETMET	R5	R2	R260
  0x5C1C0200,  //  0009  MOVE	R7	R1
  0x5C200800,  //  000A  MOVE	R8	R4
  0x7C140600,  //  000B  CALL	R5	3
  0x94180B03,  //  000C  GETIDX	R6	R5	R259
  0x941C0B05,  //  000D  GETIDX	R7	R5	R261
  0x8C200102,  //  000E  GETMET	R8	R0	R258
  0x5C280E00,  //  000F  MOVE	R10	R7
  0x5C2C0600,  //  0010  MOVE	R11	R3
  0x50300200,  //  0011  LDBOOL	R12	1	0
  0x7C200800,  //  0012  CALL	R8	4
  0x5C101000,  //  0013  MOVE	R4	R8
  0x28200903,  //  0014  GE	R8	R4	R259
  0x7822000B,  //  0015  JMPF	R8	#0022
  0x8C200504,  //  0016  GETMET	R8	R2	R260
  0x5C280E00,  //  0017  MOVE	R10	R7
  0x5C2C0800,  //  0018  MOVE	R11	R4
  0x7C200600,  //  0019  CALL	R8	3
  0x94241103,  //  001A  GETIDX	R9	R8	R259
  0x94281105,  //  001B  GETIDX	R10	R8	R261
  0x602C000A,  //  001C  GETGBL	R11	G10
  0x7C2C0000,  //  001D  CALL	R11	0
  0x40301606,  //  001E  CONNECT	R12	R11	R6
  0x40301609,  //  001F  CONNECT	R12	R11	R9
  0x4030160A,  //  0020  CONNECT	R12	R11	R10
  0x80041600,  //  0021  RET	1	R11
  0x6014000A,  //  0022  GETGBL	R5	G10
  0x7C140000,  //  0023  CALL	R5	0
  0x40180A01,  //  0024  CONNECT	R6	R5	R1
  0x4C180000,  //  0025  LDNIL	6
  0x40180A06,  //  0026  CONNECT	R6	R5	R6
  0x4C180000,  //  0027  LDNIL	6
  0x40180A06,  //  0028  CONNECT	R6	R5	R6
  0x80040A00,  //  0029  RET	1	R5
};

static const bproto find_op_proto = {
  NULL,     // bgcobject *next
  8,       // type
  0x08,        // marked
  13,       // nstack
  0,       // nupvals
  2,       // argc
  0,       // varg
  NULL,     // bgcobject *gray
  NULL,     // bupvaldesc *upvals
  (bvalue*) &find_op_ktab,     // ktab
  NULL,     // bproto **ptab
  (binstruction*) &find_op_code,     // code
  be_local_const_str(find_op_str_name),       // name
  42,       // codesize
  6,       // nconst
  0,       // nproto
  be_local_const_str(find_op_str_source),     // source
#if BE_DEBUG_RUNTIME_INFO /* debug information */
  NULL,     // lineinfo
  0,        // nlineinfo
#endif
#if BE_DEBUG_VAR_INFO
  NULL,     // varinfo
  0,        // nvarinfo
#endif
};

const bclosure find_op_closure = {
  NULL,     // bgcobject *next
  36,       // type
  0x08,        // marked
  0,       // nupvals
  NULL,     // bgcobject *gray
  (bproto*) &find_op_proto,     // proto
  { NULL }     // upvals
};

/*******************************************************************/


/********************************************************************
    // Rules
    "def add_rule(pat,f) "
      "if !self._rules "
        "self._rules={} "
      "end "
      "if type(f) == 'function' "
        "self._rules[pat] = f "
      "else "
        "raise 'value_error', 'the second argument is not a function' "
      "end "
    "end "
********************************************************************/

/********************************************************************
** Solidified function: add_rule
********************************************************************/

be_define_local_const_str(add_rule_str_name, "add_rule", 596540743, 8);
be_define_local_const_str(add_rule_str_source, "string", 398550328, 6);
be_define_local_const_str(add_rule_str_0, "_rules", -28750191, 6);
be_define_local_const_str(add_rule_str_1, "function", -1630125495, 8);
be_define_local_const_str(add_rule_str_2, "value_error", 773297791, 11);
be_define_local_const_str(add_rule_str_3, "the second argument is not a function", -340392827, 37);

static const bvalue add_rule_ktab[4] = {
  { { .s=be_local_const_str(add_rule_str_0) }, BE_STRING},
  { { .s=be_local_const_str(add_rule_str_1) }, BE_STRING},
  { { .s=be_local_const_str(add_rule_str_2) }, BE_STRING},
  { { .s=be_local_const_str(add_rule_str_3) }, BE_STRING},
};

static const uint32_t add_rule_code[15] = {
  0x880C0100,  //  0000  GETMBR R3  R0  R256
  0x740E0002,  //  0001  JMPT R3  #0005
  0x600C000B,  //  0002  GETGBL R3  G11
  0x7C0C0000,  //  0003  CALL R3  0
  0x90020003,  //  0004  SETMBR R0  R256  R3
  0x600C0015,  //  0005  GETGBL R3  G21
  0x5C100400,  //  0006  MOVE R4  R2
  0x7C0C0200,  //  0007  CALL R3  1
  0x1C0C0701,  //  0008  EQ R3  R3  R257
  0x780E0002,  //  0009  JMPF R3  #000D
  0x880C0100,  //  000A  GETMBR R3  R0  R256
  0x980C0202,  //  000B  SETIDX R3  R1  R2
  0x70020000,  //  000C  JMP    #000E
  0xB0060503,  //  000D  RAISE  1 R258  R259
  0x80000000,  //  000E  RET  0 R0
};

static const bproto add_rule_proto = {
  NULL,     // bgcobject *next
  8,       // type
  0x08,        // marked
  5,       // nstack
  0,       // nupvals
  3,       // argc
  0,       // varg
  NULL,     // bgcobject *gray
  NULL,     // bupvaldesc *upvals
  (bvalue*) &add_rule_ktab,     // ktab
  NULL,     // bproto **ptab
  (binstruction*) &add_rule_code,     // code
  be_local_const_str(add_rule_str_name),       // name
  15,       // codesize
  4,       // nconst
  0,       // nproto
  be_local_const_str(add_rule_str_source),     // source
#if BE_DEBUG_RUNTIME_INFO /* debug information */
  NULL,     // lineinfo
  0,        // nlineinfo
#endif
#if BE_DEBUG_VAR_INFO
  NULL,     // varinfo
  0,        // nvarinfo
#endif
};

static const bclosure add_rule_closure = {
  NULL,     // bgcobject *next
  36,       // type
  0x08,        // marked
  0,       // nupvals
  NULL,     // bgcobject *gray
  (bproto*) &add_rule_proto,     // proto
  { NULL }     // upvals
};

/*******************************************************************/

/********************************************************************
    "def remove_rule(pat) "
      "if self._rules "
        "self._rules.remove(pat) "
      "end "
    "end "
********************************************************************/

/********************************************************************
** Solidified function: remove_rule
********************************************************************/

be_define_local_const_str(remove_rule_str_name, "remove_rule", -838755968, 11);
be_define_local_const_str(remove_rule_str_source, "string", 398550328, 6);
be_define_local_const_str(remove_rule_str_0, "_rules", -28750191, 6);
be_define_local_const_str(remove_rule_str_1, "remove", -611183107, 6);

static const bvalue remove_rule_ktab[2] = {
  { { .s=be_local_const_str(remove_rule_str_0) }, BE_STRING},
  { { .s=be_local_const_str(remove_rule_str_1) }, BE_STRING},
};

static const uint32_t remove_rule_code[7] = {
  0x88080100,  //  0000  GETMBR R2  R0  R256
  0x780A0003,  //  0001  JMPF R2  #0006
  0x88080100,  //  0002  GETMBR R2  R0  R256
  0x8C080501,  //  0003  GETMET R2  R2  R257
  0x5C100200,  //  0004  MOVE R4  R1
  0x7C080400,  //  0005  CALL R2  2
  0x80000000,  //  0006  RET  0 R0
};

static const bproto remove_rule_proto = {
  NULL,     // bgcobject *next
  8,       // type
  0x08,        // marked
  5,       // nstack
  0,       // nupvals
  2,       // argc
  0,       // varg
  NULL,     // bgcobject *gray
  NULL,     // bupvaldesc *upvals
  (bvalue*) &remove_rule_ktab,     // ktab
  NULL,     // bproto **ptab
  (binstruction*) &remove_rule_code,     // code
  be_local_const_str(remove_rule_str_name),       // name
  7,       // codesize
  2,       // nconst
  0,       // nproto
  be_local_const_str(remove_rule_str_source),     // source
#if BE_DEBUG_RUNTIME_INFO /* debug information */
  NULL,     // lineinfo
  0,        // nlineinfo
#endif
#if BE_DEBUG_VAR_INFO
  NULL,     // varinfo
  0,        // nvarinfo
#endif
};

static const bclosure remove_rule_closure = {
  NULL,     // bgcobject *next
  36,       // type
  0x08,        // marked
  0,       // nupvals
  NULL,     // bgcobject *gray
  (bproto*) &remove_rule_proto,     // proto
  { NULL }     // upvals
};

/*******************************************************************/


/********************************************************************
    // Rules trigger if match. return true if match, false if not
    "def try_rule(event, rule, f) "
      "import string "
      "var rl_list = self.find_op(rule) "
      "var sub_event = event "
      "var rl = string.split(rl_list[0],'#') "
      "for it:rl "
        "found=self.find_key_i(sub_event,it) "
        "if found == nil return false end "
        "sub_event = sub_event[found] "
      "end "
      "var op=rl_list[1]"
      "var op2=rl_list[2]"
      "if op "
        "if   op=='==' "
          "if str(sub_event) != str(op2)   return false end "
        "elif op=='!==' "
          "if str(sub_event) == str(op2)   return false end "
        "elif op=='=' "
          "if real(sub_event) != real(op2) return false end "
        "elif op=='!=' "
          "if real(sub_event) == real(op2) return false end "
        "elif op=='>' "
          "if real(sub_event) <= real(op2) return false end "
        "elif op=='>=' "
          "if real(sub_event) < real(op2)  return false end "
        "elif op=='<' "
          "if real(sub_event) >= real(op2) return false end "
        "elif op=='<=' "
          "if real(sub_event) > real(op2)  return false end "
        "end "
      "end "
      "f(sub_event, rl_list[0], event) "
      "return true "
    "end "
********************************************************************/
/********************************************************************
** Solidified function: try_rule
********************************************************************/

be_define_local_const_str(try_rule_str_name, "try_rule", 1986449405, 8);
be_define_local_const_str(try_rule_str_source, "string", 398550328, 6);
be_define_local_const_str(try_rule_str_0, "string", 398550328, 6);
be_define_local_const_str(try_rule_str_1, "find_op", -528253920, 7);
be_define_local_const_str(try_rule_str_2, "split", -2017972765, 5);
be_define_local_const_str(try_rule_str_4, "#", 638357778, 1);
be_define_local_const_str(try_rule_str_5, "find_key_i", 850136726, 10);
be_define_local_const_str(try_rule_str_6, "stop_iteration", -121173395, 14);
be_define_local_const_str(try_rule_str_9, "==", -1863000881, 2);
be_define_local_const_str(try_rule_str_10, "!==", 559817114, 3);
be_define_local_const_str(try_rule_str_11, "=", 940354920, 1);
be_define_local_const_str(try_rule_str_12, "!=", -1866252285, 2);
be_define_local_const_str(try_rule_str_13, ">", 990687777, 1);
be_define_local_const_str(try_rule_str_14, ">=", 284975636, 2);
be_define_local_const_str(try_rule_str_15, "<", 957132539, 1);
be_define_local_const_str(try_rule_str_16, "<=", -1795743310, 2);

static const bvalue try_rule_ktab[17] = {
  { { .s=be_local_const_str(try_rule_str_0) }, BE_STRING},
  { { .s=be_local_const_str(try_rule_str_1) }, BE_STRING},
  { { .s=be_local_const_str(try_rule_str_2) }, BE_STRING},
  { { .i=0 }, BE_INT},
  { { .s=be_local_const_str(try_rule_str_4) }, BE_STRING},
  { { .s=be_local_const_str(try_rule_str_5) }, BE_STRING},
  { { .s=be_local_const_str(try_rule_str_6) }, BE_STRING},
  { { .i=1 }, BE_INT},
  { { .i=2 }, BE_INT},
  { { .s=be_local_const_str(try_rule_str_9) }, BE_STRING},
  { { .s=be_local_const_str(try_rule_str_10) }, BE_STRING},
  { { .s=be_local_const_str(try_rule_str_11) }, BE_STRING},
  { { .s=be_local_const_str(try_rule_str_12) }, BE_STRING},
  { { .s=be_local_const_str(try_rule_str_13) }, BE_STRING},
  { { .s=be_local_const_str(try_rule_str_14) }, BE_STRING},
  { { .s=be_local_const_str(try_rule_str_15) }, BE_STRING},
  { { .s=be_local_const_str(try_rule_str_16) }, BE_STRING},
};

static const uint32_t try_rule_code[143] = {
  0xA4120000,  //  0000  IMPORT R4  R256
  0x8C140101,  //  0001  GETMET R5  R0  R257
  0x5C1C0400,  //  0002  MOVE R7  R2
  0x7C140400,  //  0003  CALL R5  2
  0x5C180200,  //  0004  MOVE R6  R1
  0x8C1C0902,  //  0005  GETMET R7  R4  R258
  0x94240B03,  //  0006  GETIDX R9  R5  R259
  0x58280004,  //  0007  LDCONST  R10 K4
  0x7C1C0600,  //  0008  CALL R7  3
  0x60200000,  //  0009  GETGBL R8  G0
  0x5C240E00,  //  000A  MOVE R9  R7
  0x7C200200,  //  000B  CALL R8  1
  0xA802000D,  //  000C  EXBLK  0 #001B
  0x5C241000,  //  000D  MOVE R9  R8
  0x7C240000,  //  000E  CALL R9  0
  0x8C280105,  //  000F  GETMET R10 R0  R261
  0x5C300C00,  //  0010  MOVE R12 R6
  0x5C341200,  //  0011  MOVE R13 R9
  0x7C280600,  //  0012  CALL R10 3
  0x4C2C0000,  //  0013  LDNIL  11
  0x1C2C140B,  //  0014  EQ R11 R10 R11
  0x782E0002,  //  0015  JMPF R11 #0019
  0x502C0000,  //  0016  LDBOOL R11 0 0
  0xA8040001,  //  0017  EXBLK  1 1
  0x80041600,  //  0018  RET  1 R11
  0x94180C0A,  //  0019  GETIDX R6  R6  R10
  0x7001FFF1,  //  001A  JMP    #000D
  0x58200006,  //  001B  LDCONST  R8  K6
  0xAC200200,  //  001C  CATCH  R8  1 0
  0xB0080000,  //  001D  RAISE  2 R0  R0
  0x94200B07,  //  001E  GETIDX R8  R5  R263
  0x94240B08,  //  001F  GETIDX R9  R5  R264
  0x78220066,  //  0020  JMPF R8  #0088
  0x1C281109,  //  0021  EQ R10 R8  R265
  0x782A000A,  //  0022  JMPF R10 #002E
  0x60280013,  //  0023  GETGBL R10 G19
  0x5C2C0C00,  //  0024  MOVE R11 R6
  0x7C280200,  //  0025  CALL R10 1
  0x602C0013,  //  0026  GETGBL R11 G19
  0x5C301200,  //  0027  MOVE R12 R9
  0x7C2C0200,  //  0028  CALL R11 1
  0x2028140B,  //  0029  NE R10 R10 R11
  0x782A0001,  //  002A  JMPF R10 #002D
  0x50280000,  //  002B  LDBOOL R10 0 0
  0x80041400,  //  002C  RET  1 R10
  0x70020059,  //  002D  JMP    #0088
  0x1C28110A,  //  002E  EQ R10 R8  R266
  0x782A000A,  //  002F  JMPF R10 #003B
  0x60280013,  //  0030  GETGBL R10 G19
  0x5C2C0C00,  //  0031  MOVE R11 R6
  0x7C280200,  //  0032  CALL R10 1
  0x602C0013,  //  0033  GETGBL R11 G19
  0x5C301200,  //  0034  MOVE R12 R9
  0x7C2C0200,  //  0035  CALL R11 1
  0x1C28140B,  //  0036  EQ R10 R10 R11
  0x782A0001,  //  0037  JMPF R10 #003A
  0x50280000,  //  0038  LDBOOL R10 0 0
  0x80041400,  //  0039  RET  1 R10
  0x7002004C,  //  003A  JMP    #0088
  0x1C28110B,  //  003B  EQ R10 R8  R267
  0x782A000A,  //  003C  JMPF R10 #0048
  0x60280011,  //  003D  GETGBL R10 G17
  0x5C2C0C00,  //  003E  MOVE R11 R6
  0x7C280200,  //  003F  CALL R10 1
  0x602C0011,  //  0040  GETGBL R11 G17
  0x5C301200,  //  0041  MOVE R12 R9
  0x7C2C0200,  //  0042  CALL R11 1
  0x2028140B,  //  0043  NE R10 R10 R11
  0x782A0001,  //  0044  JMPF R10 #0047
  0x50280000,  //  0045  LDBOOL R10 0 0
  0x80041400,  //  0046  RET  1 R10
  0x7002003F,  //  0047  JMP    #0088
  0x1C28110C,  //  0048  EQ R10 R8  R268
  0x782A000A,  //  0049  JMPF R10 #0055
  0x60280011,  //  004A  GETGBL R10 G17
  0x5C2C0C00,  //  004B  MOVE R11 R6
  0x7C280200,  //  004C  CALL R10 1
  0x602C0011,  //  004D  GETGBL R11 G17
  0x5C301200,  //  004E  MOVE R12 R9
  0x7C2C0200,  //  004F  CALL R11 1
  0x1C28140B,  //  0050  EQ R10 R10 R11
  0x782A0001,  //  0051  JMPF R10 #0054
  0x50280000,  //  0052  LDBOOL R10 0 0
  0x80041400,  //  0053  RET  1 R10
  0x70020032,  //  0054  JMP    #0088
  0x1C28110D,  //  0055  EQ R10 R8  R269
  0x782A000A,  //  0056  JMPF R10 #0062
  0x60280011,  //  0057  GETGBL R10 G17
  0x5C2C0C00,  //  0058  MOVE R11 R6
  0x7C280200,  //  0059  CALL R10 1
  0x602C0011,  //  005A  GETGBL R11 G17
  0x5C301200,  //  005B  MOVE R12 R9
  0x7C2C0200,  //  005C  CALL R11 1
  0x1828140B,  //  005D  LE R10 R10 R11
  0x782A0001,  //  005E  JMPF R10 #0061
  0x50280000,  //  005F  LDBOOL R10 0 0
  0x80041400,  //  0060  RET  1 R10
  0x70020025,  //  0061  JMP    #0088
  0x1C28110E,  //  0062  EQ R10 R8  R270
  0x782A000A,  //  0063  JMPF R10 #006F
  0x60280011,  //  0064  GETGBL R10 G17
  0x5C2C0C00,  //  0065  MOVE R11 R6
  0x7C280200,  //  0066  CALL R10 1
  0x602C0011,  //  0067  GETGBL R11 G17
  0x5C301200,  //  0068  MOVE R12 R9
  0x7C2C0200,  //  0069  CALL R11 1
  0x1428140B,  //  006A  LT R10 R10 R11
  0x782A0001,  //  006B  JMPF R10 #006E
  0x50280000,  //  006C  LDBOOL R10 0 0
  0x80041400,  //  006D  RET  1 R10
  0x70020018,  //  006E  JMP    #0088
  0x1C28110F,  //  006F  EQ R10 R8  R271
  0x782A000A,  //  0070  JMPF R10 #007C
  0x60280011,  //  0071  GETGBL R10 G17
  0x5C2C0C00,  //  0072  MOVE R11 R6
  0x7C280200,  //  0073  CALL R10 1
  0x602C0011,  //  0074  GETGBL R11 G17
  0x5C301200,  //  0075  MOVE R12 R9
  0x7C2C0200,  //  0076  CALL R11 1
  0x2828140B,  //  0077  GE R10 R10 R11
  0x782A0001,  //  0078  JMPF R10 #007B
  0x50280000,  //  0079  LDBOOL R10 0 0
  0x80041400,  //  007A  RET  1 R10
  0x7002000B,  //  007B  JMP    #0088
  0x1C281110,  //  007C  EQ R10 R8  R272
  0x782A0009,  //  007D  JMPF R10 #0088
  0x60280011,  //  007E  GETGBL R10 G17
  0x5C2C0C00,  //  007F  MOVE R11 R6
  0x7C280200,  //  0080  CALL R10 1
  0x602C0011,  //  0081  GETGBL R11 G17
  0x5C301200,  //  0082  MOVE R12 R9
  0x7C2C0200,  //  0083  CALL R11 1
  0x2428140B,  //  0084  GT R10 R10 R11
  0x782A0001,  //  0085  JMPF R10 #0088
  0x50280000,  //  0086  LDBOOL R10 0 0
  0x80041400,  //  0087  RET  1 R10
  0x5C280600,  //  0088  MOVE R10 R3
  0x5C2C0C00,  //  0089  MOVE R11 R6
  0x94300B03,  //  008A  GETIDX R12 R5  R259
  0x5C340200,  //  008B  MOVE R13 R1
  0x7C280600,  //  008C  CALL R10 3
  0x50280200,  //  008D  LDBOOL R10 1 0
  0x80041400,  //  008E  RET  1 R10
};

static const bproto try_rule_proto = {
  NULL,     // bgcobject *next
  8,       // type
  0x08,        // marked
  14,       // nstack
  0,       // nupvals
  4,       // argc
  0,       // varg
  NULL,     // bgcobject *gray
  NULL,     // bupvaldesc *upvals
  (bvalue*) &try_rule_ktab,     // ktab
  NULL,     // bproto **ptab
  (binstruction*) &try_rule_code,     // code
  be_local_const_str(try_rule_str_name),       // name
  143,       // codesize
  17,       // nconst
  0,       // nproto
  be_local_const_str(try_rule_str_source),     // source
#if BE_DEBUG_RUNTIME_INFO /* debug information */
  NULL,     // lineinfo
  0,        // nlineinfo
#endif
#if BE_DEBUG_VAR_INFO
  NULL,     // varinfo
  0,        // nvarinfo
#endif
};

static const bclosure try_rule_closure = {
  NULL,     // bgcobject *next
  36,       // type
  0x08,        // marked
  0,       // nupvals
  NULL,     // bgcobject *gray
  (bproto*) &try_rule_proto,     // proto
  { NULL }     // upvals
};

/*******************************************************************/


/********************************************************************
    // Run rules, i.e. check each individual rule
    // Returns true if at least one rule matched, false if none
    "def exec_rules(ev_json) "
      "if self._rules "
        "import json "
        "var ev = json.load(ev_json) "
        "var ret = false "
        "if ev == nil "
          "print('BRY: ERROR, bad json: '+ev_json, 3) "
        "else "
          "for r: self._rules.keys() "
            "ret = self.try_rule(ev,r,self._rules[r]) || ret "
          "end "
        "end "
        "return ret "
      "end "
      "return false "
    "end "

********************************************************************/

/********************************************************************
** Solidified function: exec_rules
********************************************************************/

be_define_local_const_str(exec_rules_str_name, "exec_rules", 1445221092, 10);
be_define_local_const_str(exec_rules_str_source, "string", 398550328, 6);
be_define_local_const_str(exec_rules_str_0, "_rules", -28750191, 6);
be_define_local_const_str(exec_rules_str_1, "json", 916562499, 4);
be_define_local_const_str(exec_rules_str_2, "load", -435725847, 4);
be_define_local_const_str(exec_rules_str_3, "BRY: ERROR, bad json: ", -1579831487, 22);
be_define_local_const_str(exec_rules_str_5, "keys", -112588595, 4);
be_define_local_const_str(exec_rules_str_6, "try_rule", 1986449405, 8);
be_define_local_const_str(exec_rules_str_7, "stop_iteration", -121173395, 14);

static const bvalue exec_rules_ktab[8] = {
  { { .s=be_local_const_str(exec_rules_str_0) }, BE_STRING},
  { { .s=be_local_const_str(exec_rules_str_1) }, BE_STRING},
  { { .s=be_local_const_str(exec_rules_str_2) }, BE_STRING},
  { { .s=be_local_const_str(exec_rules_str_3) }, BE_STRING},
  { { .i=3 }, BE_INT},
  { { .s=be_local_const_str(exec_rules_str_5) }, BE_STRING},
  { { .s=be_local_const_str(exec_rules_str_6) }, BE_STRING},
  { { .s=be_local_const_str(exec_rules_str_7) }, BE_STRING},
};

static const uint32_t exec_rules_code[40] = {
  0x88080100,  //  0000  GETMBR R2  R0  R256
  0x780A0023,  //  0001  JMPF R2  #0026
  0xA40A0200,  //  0002  IMPORT R2  R257
  0x8C0C0502,  //  0003  GETMET R3  R2  R258
  0x5C140200,  //  0004  MOVE R5  R1
  0x7C0C0400,  //  0005  CALL R3  2
  0x50100000,  //  0006  LDBOOL R4  0 0
  0x4C140000,  //  0007  LDNIL  5
  0x1C140605,  //  0008  EQ R5  R3  R5
  0x78160004,  //  0009  JMPF R5  #000F
  0x6014000F,  //  000A  GETGBL R5  G15
  0x1A0601,  //  000B  ADD  R6  R259  R1
  0x581C0004,  //  000C  LDCONST  R7  K4
  0x7C140400,  //  000D  CALL R5  2
  0x70020015,  //  000E  JMP    #0025
  0x60140000,  //  000F  GETGBL R5  G0
  0x88180100,  //  0010  GETMBR R6  R0  R256
  0x8C180D05,  //  0011  GETMET R6  R6  R261
  0x7C180200,  //  0012  CALL R6  1
  0x7C140200,  //  0013  CALL R5  1
  0xA802000C,  //  0014  EXBLK  0 #0022
  0x5C180A00,  //  0015  MOVE R6  R5
  0x7C180000,  //  0016  CALL R6  0
  0x8C1C0106,  //  0017  GETMET R7  R0  R262
  0x5C240600,  //  0018  MOVE R9  R3
  0x5C280C00,  //  0019  MOVE R10 R6
  0x882C0100,  //  001A  GETMBR R11 R0  R256
  0x942C1606,  //  001B  GETIDX R11 R11 R6
  0x7C1C0800,  //  001C  CALL R7  4
  0x741E0001,  //  001D  JMPT R7  #0020
  0x74120000,  //  001E  JMPT R4  #0020
  0x50100001,  //  001F  LDBOOL R4  0 1
  0x50100200,  //  0020  LDBOOL R4  1 0
  0x7001FFF2,  //  0021  JMP    #0015
  0x58140007,  //  0022  LDCONST  R5  K7
  0xAC140200,  //  0023  CATCH  R5  1 0
  0xB0080000,  //  0024  RAISE  2 R0  R0
  0x80040800,  //  0025  RET  1 R4
  0x50080000,  //  0026  LDBOOL R2  0 0
  0x80040400,  //  0027  RET  1 R2
};

static const bproto exec_rules_proto = {
  NULL,     // bgcobject *next
  8,       // type
  0x08,        // marked
  12,       // nstack
  0,       // nupvals
  2,       // argc
  0,       // varg
  NULL,     // bgcobject *gray
  NULL,     // bupvaldesc *upvals
  (bvalue*) &exec_rules_ktab,     // ktab
  NULL,     // bproto **ptab
  (binstruction*) &exec_rules_code,     // code
  be_local_const_str(exec_rules_str_name),       // name
  40,       // codesize
  8,       // nconst
  0,       // nproto
  be_local_const_str(exec_rules_str_source),     // source
#if BE_DEBUG_RUNTIME_INFO /* debug information */
  NULL,     // lineinfo
  0,        // nlineinfo
#endif
#if BE_DEBUG_VAR_INFO
  NULL,     // varinfo
  0,        // nvarinfo
#endif
};

const bclosure exec_rules_closure = {
  NULL,     // bgcobject *next
  36,       // type
  0x08,        // marked
  0,       // nupvals
  NULL,     // bgcobject *gray
  (bproto*) &exec_rules_proto,     // proto
  { NULL }     // upvals
};

/*******************************************************************/


/********************************************************************
    "def set_timer(delay,f) "
      "if !self._timers self._timers=[] end "
      "self._timers.push([self.millis(delay),f]) "
    "end "
********************************************************************/
/********************************************************************
** Solidified function: set_timer
********************************************************************/

be_define_local_const_str(set_timer_str_name, "set_timer", 2135414533, 9);
be_define_local_const_str(set_timer_str_source, "string", 398550328, 6);
be_define_local_const_str(set_timer_str_0, "_timers", -1694866380, 7);
be_define_local_const_str(set_timer_str_1, "push", -2022703139, 4);
be_define_local_const_str(set_timer_str_2, "millis", 1214679063, 6);

static const bvalue set_timer_ktab[3] = {
  { { .s=be_local_const_str(set_timer_str_0) }, BE_STRING},
  { { .s=be_local_const_str(set_timer_str_1) }, BE_STRING},
  { { .s=be_local_const_str(set_timer_str_2) }, BE_STRING},
};

static const uint32_t set_timer_code[16] = {
  0x880C0100,  //  0000  GETMBR R3  R0  R256
  0x740E0002,  //  0001  JMPT R3  #0005
  0x600C000A,  //  0002  GETGBL R3  G10
  0x7C0C0000,  //  0003  CALL R3  0
  0x90020003,  //  0004  SETMBR R0  R256  R3
  0x880C0100,  //  0005  GETMBR R3  R0  R256
  0x8C0C0701,  //  0006  GETMET R3  R3  R257
  0x6014000A,  //  0007  GETGBL R5  G10
  0x7C140000,  //  0008  CALL R5  0
  0x8C180102,  //  0009  GETMET R6  R0  R258
  0x5C200200,  //  000A  MOVE R8  R1
  0x7C180400,  //  000B  CALL R6  2
  0x40180A06,  //  000C  CONNECT  R6  R5  R6
  0x40180A02,  //  000D  CONNECT  R6  R5  R2
  0x7C0C0400,  //  000E  CALL R3  2
  0x80000000,  //  000F  RET  0 R0
};

static const bproto set_timer_proto = {
  NULL,     // bgcobject *next
  8,       // type
  0x08,        // marked
  9,       // nstack
  0,       // nupvals
  3,       // argc
  0,       // varg
  NULL,     // bgcobject *gray
  NULL,     // bupvaldesc *upvals
  (bvalue*) &set_timer_ktab,     // ktab
  NULL,     // bproto **ptab
  (binstruction*) &set_timer_code,     // code
  be_local_const_str(set_timer_str_name),       // name
  16,       // codesize
  3,       // nconst
  0,       // nproto
  be_local_const_str(set_timer_str_source),     // source
#if BE_DEBUG_RUNTIME_INFO /* debug information */
  NULL,     // lineinfo
  0,        // nlineinfo
#endif
#if BE_DEBUG_VAR_INFO
  NULL,     // varinfo
  0,        // nvarinfo
#endif
};

const bclosure set_timer_closure = {
  NULL,     // bgcobject *next
  36,       // type
  0x08,        // marked
  0,       // nupvals
  NULL,     // bgcobject *gray
  (bproto*) &set_timer_proto,     // proto
  { NULL }     // upvals
};

/*******************************************************************/


/********************************************************************
    // run every 50ms tick
    "def run_deferred() "
      "if self._timers "
        "var i=0 "
        "while i<self._timers.size() "
          "if self.time_reached(self._timers[i][0]) "
            "f=self._timers[i][1] "
            "self._timers.remove(i) "
            "f() "
          "else "
            "i=i+1 "
          "end "
        "end "
      "end "
    "end "
********************************************************************/
/********************************************************************
** Solidified function: run_deferred
********************************************************************/

be_define_local_const_str(run_deferred_str_name, "run_deferred", 371594696, 12);
be_define_local_const_str(run_deferred_str_source, "string", 398550328, 6);
be_define_local_const_str(run_deferred_str_0, "_timers", -1694866380, 7);
be_define_local_const_str(run_deferred_str_2, "size", 597743964, 4);
be_define_local_const_str(run_deferred_str_3, "time_reached", 2075136773, 12);
be_define_local_const_str(run_deferred_str_5, "remove", -611183107, 6);

static const bvalue run_deferred_ktab[6] = {
  { { .s=be_local_const_str(run_deferred_str_0) }, BE_STRING},
  { { .i=0 }, BE_INT},
  { { .s=be_local_const_str(run_deferred_str_2) }, BE_STRING},
  { { .s=be_local_const_str(run_deferred_str_3) }, BE_STRING},
  { { .i=1 }, BE_INT},
  { { .s=be_local_const_str(run_deferred_str_5) }, BE_STRING},
};

static const uint32_t run_deferred_code[27] = {
  0x88040100,  //  0000  GETMBR R1  R0  R256
  0x78060017,  //  0001  JMPF R1  #001A
  0x58040001,  //  0002  LDCONST  R1  K1
  0x88080100,  //  0003  GETMBR R2  R0  R256
  0x8C080502,  //  0004  GETMET R2  R2  R258
  0x7C080200,  //  0005  CALL R2  1
  0x14080202,  //  0006  LT R2  R1  R2
  0x780A0011,  //  0007  JMPF R2  #001A
  0x8C080103,  //  0008  GETMET R2  R0  R259
  0x88100100,  //  0009  GETMBR R4  R0  R256
  0x94100801,  //  000A  GETIDX R4  R4  R1
  0x94100901,  //  000B  GETIDX R4  R4  R257
  0x7C080400,  //  000C  CALL R2  2
  0x780A0009,  //  000D  JMPF R2  #0018
  0x88080100,  //  000E  GETMBR R2  R0  R256
  0x94080401,  //  000F  GETIDX R2  R2  R1
  0x94080504,  //  0010  GETIDX R2  R2  R260
  0x880C0100,  //  0011  GETMBR R3  R0  R256
  0x8C0C0705,  //  0012  GETMET R3  R3  R261
  0x5C140200,  //  0013  MOVE R5  R1
  0x7C0C0400,  //  0014  CALL R3  2
  0x5C0C0400,  //  0015  MOVE R3  R2
  0x7C0C0000,  //  0016  CALL R3  0
  0x70020000,  //  0017  JMP    #0019
  0x40304,  //  0018  ADD R1  R1  R260
  0x7001FFE8,  //  0019  JMP    #0003
  0x80000000,  //  001A  RET  0 R0
};

static const bproto run_deferred_proto = {
  NULL,     // bgcobject *next
  8,       // type
  0x08,        // marked
  6,       // nstack
  0,       // nupvals
  1,       // argc
  0,       // varg
  NULL,     // bgcobject *gray
  NULL,     // bupvaldesc *upvals
  (bvalue*) &run_deferred_ktab,     // ktab
  NULL,     // bproto **ptab
  (binstruction*) &run_deferred_code,     // code
  be_local_const_str(run_deferred_str_name),       // name
  27,       // codesize
  6,       // nconst
  0,       // nproto
  be_local_const_str(run_deferred_str_source),     // source
#if BE_DEBUG_RUNTIME_INFO /* debug information */
  NULL,     // lineinfo
  0,        // nlineinfo
#endif
#if BE_DEBUG_VAR_INFO
  NULL,     // varinfo
  0,        // nvarinfo
#endif
};

const bclosure run_deferred_closure = {
  NULL,     // bgcobject *next
  36,       // type
  0x08,        // marked
  0,       // nupvals
  NULL,     // bgcobject *gray
  (bproto*) &run_deferred_proto,     // proto
  { NULL }     // upvals
};

/*******************************************************************/


/********************************************************************
    // Add command to list
    "def add_cmd(c,f) "
      "if !self._ccmd "
        "self._ccmd={} "
      "end "
      "if type(f) == 'function' "
        "self._ccmd[c]=f "
      "else "
        "raise 'value_error', 'the second argument is not a function' "
      "end "
    "end "
********************************************************************/
/********************************************************************
** Solidified function: add_cmd
********************************************************************/

be_define_local_const_str(add_cmd_str_name, "add_cmd", -933336417, 7);
be_define_local_const_str(add_cmd_str_source, "string", 398550328, 6);
be_define_local_const_str(add_cmd_str_0, "_ccmd", -2131545883, 5);
be_define_local_const_str(add_cmd_str_1, "function", -1630125495, 8);
be_define_local_const_str(add_cmd_str_2, "value_error", 773297791, 11);
be_define_local_const_str(add_cmd_str_3, "the second argument is not a function", -340392827, 37);

static const bvalue add_cmd_ktab[4] = {
  { { .s=be_local_const_str(add_cmd_str_0) }, BE_STRING},
  { { .s=be_local_const_str(add_cmd_str_1) }, BE_STRING},
  { { .s=be_local_const_str(add_cmd_str_2) }, BE_STRING},
  { { .s=be_local_const_str(add_cmd_str_3) }, BE_STRING},
};

static const uint32_t add_cmd_code[15] = {
  0x880C0100,  //  0000  GETMBR	R3	R0	R256
  0x740E0002,  //  0001  JMPT	R3	#0005
  0x600C000B,  //  0002  GETGBL	R3	G11
  0x7C0C0000,  //  0003  CALL	R3	0
  0x90020003,  //  0004  SETMBR	R0	R256	R3
  0x600C0015,  //  0005  GETGBL	R3	G21
  0x5C100400,  //  0006  MOVE	R4	R2
  0x7C0C0200,  //  0007  CALL	R3	1
  0x1C0C0701,  //  0008  EQ	R3	R3	R257
  0x780E0002,  //  0009  JMPF	R3	#000D
  0x880C0100,  //  000A  GETMBR	R3	R0	R256
  0x980C0202,  //  000B  SETIDX	R3	R1	R2
  0x70020000,  //  000C  JMP		#000E
  0xB0060503,  //  000D  RAISE	1	R258	R259
  0x80000000,  //  000E  RET	0	R0
};

static const bproto add_cmd_proto = {
  NULL,     // bgcobject *next
  8,       // type
  0x08,        // marked
  5,       // nstack
  0,       // nupvals
  3,       // argc
  0,       // varg
  NULL,     // bgcobject *gray
  NULL,     // bupvaldesc *upvals
  (bvalue*) &add_cmd_ktab,     // ktab
  NULL,     // bproto **ptab
  (binstruction*) &add_cmd_code,     // code
  be_local_const_str(add_cmd_str_name),       // name
  15,       // codesize
  4,       // nconst
  0,       // nproto
  be_local_const_str(add_cmd_str_source),     // source
#if BE_DEBUG_RUNTIME_INFO /* debug information */
  NULL,     // lineinfo
  0,        // nlineinfo
#endif
#if BE_DEBUG_VAR_INFO
  NULL,     // varinfo
  0,        // nvarinfo
#endif
};

static const bclosure add_cmd_closure = {
  NULL,     // bgcobject *next
  36,       // type
  0x08,        // marked
  0,       // nupvals
  NULL,     // bgcobject *gray
  (bproto*) &add_cmd_proto,     // proto
  { NULL }     // upvals
};

/*******************************************************************/


/********************************************************************
    // Remove command from list
    "def remove_cmd(c) "
      "if self._ccmd "
        "self._ccmd.remove(c) "
      "end "
    "end "
********************************************************************/
/********************************************************************
** Solidified function: remove_cmd
********************************************************************/

be_define_local_const_str(remove_cmd_str_name, "remove_cmd", -462651594, 10);
be_define_local_const_str(remove_cmd_str_source, "string", 398550328, 6);
be_define_local_const_str(remove_cmd_str_0, "_ccmd", -2131545883, 5);
be_define_local_const_str(remove_cmd_str_1, "remove", -611183107, 6);

static const bvalue remove_cmd_ktab[2] = {
  { { .s=be_local_const_str(remove_cmd_str_0) }, BE_STRING},
  { { .s=be_local_const_str(remove_cmd_str_1) }, BE_STRING},
};

static const uint32_t remove_cmd_code[7] = {
  0x88080100,  //  0000  GETMBR	R2	R0	R256
  0x780A0003,  //  0001  JMPF	R2	#0006
  0x88080100,  //  0002  GETMBR	R2	R0	R256
  0x8C080501,  //  0003  GETMET	R2	R2	R257
  0x5C100200,  //  0004  MOVE	R4	R1
  0x7C080400,  //  0005  CALL	R2	2
  0x80000000,  //  0006  RET	0	R0
};

static const bproto remove_cmd_proto = {
  NULL,     // bgcobject *next
  8,       // type
  0x08,        // marked
  5,       // nstack
  0,       // nupvals
  2,       // argc
  0,       // varg
  NULL,     // bgcobject *gray
  NULL,     // bupvaldesc *upvals
  (bvalue*) &remove_cmd_ktab,     // ktab
  NULL,     // bproto **ptab
  (binstruction*) &remove_cmd_code,     // code
  be_local_const_str(remove_cmd_str_name),       // name
  7,       // codesize
  2,       // nconst
  0,       // nproto
  be_local_const_str(remove_cmd_str_source),     // source
#if BE_DEBUG_RUNTIME_INFO /* debug information */
  NULL,     // lineinfo
  0,        // nlineinfo
#endif
#if BE_DEBUG_VAR_INFO
  NULL,     // varinfo
  0,        // nvarinfo
#endif
};

static const bclosure remove_cmd_closure = {
  NULL,     // bgcobject *next
  36,       // type
  0x08,        // marked
  0,       // nupvals
  NULL,     // bgcobject *gray
  (bproto*) &remove_cmd_proto,     // proto
  { NULL }     // upvals
};

/*******************************************************************/

/********************************************************************
    // Execute custom command
    "def exec_cmd(cmd, idx, payload) "
      "if self._ccmd "
        "import json "
        "var payload_json = json.load(payload) "
        "var cmd_found = self.find_key_i(self._ccmd, cmd) "
        "if cmd_found != nil "
          "self.resolvecmnd(cmd_found) "  // set the command name in XdrvMailbox.command
          "self._ccmd[cmd_found](cmd_found, idx, payload, payload_json) "
          "return true "
        "end "
      "end "
      "return false "
    "end "
********************************************************************/
/********************************************************************
** Solidified function: exec_cmd
********************************************************************/

be_define_local_const_str(exec_cmd_str_name, "exec_cmd", 493567399, 8);
be_define_local_const_str(exec_cmd_str_source, "string", 398550328, 6);
be_define_local_const_str(exec_cmd_str_0, "_ccmd", -2131545883, 5);
be_define_local_const_str(exec_cmd_str_1, "json", 916562499, 4);
be_define_local_const_str(exec_cmd_str_2, "load", -435725847, 4);
be_define_local_const_str(exec_cmd_str_3, "find_key_i", 850136726, 10);
be_define_local_const_str(exec_cmd_str_4, "resolvecmnd", 993361485, 11);

static const bvalue exec_cmd_ktab[5] = {
  { { .s=be_local_const_str(exec_cmd_str_0) }, BE_STRING},
  { { .s=be_local_const_str(exec_cmd_str_1) }, BE_STRING},
  { { .s=be_local_const_str(exec_cmd_str_2) }, BE_STRING},
  { { .s=be_local_const_str(exec_cmd_str_3) }, BE_STRING},
  { { .s=be_local_const_str(exec_cmd_str_4) }, BE_STRING},
};

static const uint32_t exec_cmd_code[27] = {
  0x88100100,  //  0000  GETMBR	R4	R0	R256
  0x78120016,  //  0001  JMPF	R4	#0019
  0xA4120200,  //  0002  IMPORT	R4	R257
  0x8C140902,  //  0003  GETMET	R5	R4	R258
  0x5C1C0600,  //  0004  MOVE	R7	R3
  0x7C140400,  //  0005  CALL	R5	2
  0x8C180103,  //  0006  GETMET	R6	R0	R259
  0x88200100,  //  0007  GETMBR	R8	R0	R256
  0x5C240200,  //  0008  MOVE	R9	R1
  0x7C180600,  //  0009  CALL	R6	3
  0x4C1C0000,  //  000A  LDNIL	7
  0x201C0C07,  //  000B  NE	R7	R6	R7
  0x781E000B,  //  000C  JMPF	R7	#0019
  0x8C1C0104,  //  000D  GETMET	R7	R0	R260
  0x5C240C00,  //  000E  MOVE	R9	R6
  0x7C1C0400,  //  000F  CALL	R7	2
  0x881C0100,  //  0010  GETMBR	R7	R0	R256
  0x941C0E06,  //  0011  GETIDX	R7	R7	R6
  0x5C200C00,  //  0012  MOVE	R8	R6
  0x5C240400,  //  0013  MOVE	R9	R2
  0x5C280600,  //  0014  MOVE	R10	R3
  0x5C2C0A00,  //  0015  MOVE	R11	R5
  0x7C1C0800,  //  0016  CALL	R7	4
  0x501C0200,  //  0017  LDBOOL	R7	1	0
  0x80040E00,  //  0018  RET	1	R7
  0x50100000,  //  0019  LDBOOL	R4	0	0
  0x80040800,  //  001A  RET	1	R4
};

static const bproto exec_cmd_proto = {
  NULL,     // bgcobject *next
  8,       // type
  0x08,        // marked
  12,       // nstack
  0,       // nupvals
  4,       // argc
  0,       // varg
  NULL,     // bgcobject *gray
  NULL,     // bupvaldesc *upvals
  (bvalue*) &exec_cmd_ktab,     // ktab
  NULL,     // bproto **ptab
  (binstruction*) &exec_cmd_code,     // code
  be_local_const_str(exec_cmd_str_name),       // name
  27,       // codesize
  5,       // nconst
  0,       // nproto
  be_local_const_str(exec_cmd_str_source),     // source
#if BE_DEBUG_RUNTIME_INFO /* debug information */
  NULL,     // lineinfo
  0,        // nlineinfo
#endif
#if BE_DEBUG_VAR_INFO
  NULL,     // varinfo
  0,        // nvarinfo
#endif
};

static const bclosure exec_cmd_closure = {
  NULL,     // bgcobject *next
  36,       // type
  0x08,        // marked
  0,       // nupvals
  NULL,     // bgcobject *gray
  (bproto*) &exec_cmd_proto,     // proto
  { NULL }     // upvals
};

/*******************************************************************/


/********************************************************************
    // Force gc and return allocated memory
    "def gc() "
      "import gc "
      "gc.collect() "
      "return gc.allocated() "
    "end "
********************************************************************/
/********************************************************************
** Solidified function: gc
********************************************************************/

/********** Solidified proto: gc */
be_define_local_const_str(gc_str_name, "gc", 1042313471, 2);
be_define_local_const_str(gc_str_source, "string", 398550328, 6);
be_define_local_const_str(gc_str_0, "gc", 1042313471, 2);
be_define_local_const_str(gc_str_1, "collect", -1895928271, 7);
be_define_local_const_str(gc_str_2, "allocated", 429986098, 9);

static const bvalue gc_ktab[3] = {
  { { .s=be_local_const_str(gc_str_0) }, BE_STRING},
  { { .s=be_local_const_str(gc_str_1) }, BE_STRING},
  { { .s=be_local_const_str(gc_str_2) }, BE_STRING},
};

static const uint32_t gc_code[6] = {
  0xA4060000,  //  0000  IMPORT	R1	R256
  0x8C080301,  //  0001  GETMET	R2	R1	R257
  0x7C080200,  //  0002  CALL	R2	1
  0x8C080302,  //  0003  GETMET	R2	R1	R258
  0x7C080200,  //  0004  CALL	R2	1
  0x80040400,  //  0005  RET	1	R2
};

be_define_local_proto(gc, 4, 1, 1, 0, 0);
be_define_local_closure(gc);

/*******************************************************************/

/********************************************************************
** Solidified function: event
********************************************************************/

/********** Solidified proto: event */
be_define_local_const_str(event_str_name, "event", -30355297, 5);
be_define_local_const_str(event_str_source, "input", -103256197, 5);
be_define_local_const_str(event_str_0, "cmd", -158181397, 3);
be_define_local_const_str(event_str_1, "exec_cmd", 493567399, 8);
be_define_local_const_str(event_str_2, "rule", -64077613, 4);
be_define_local_const_str(event_str_3, "exec_rules", 1445221092, 10);
be_define_local_const_str(event_str_4, "mqtt_data", -1756753932, 9);
be_define_local_const_str(event_str_5, "gc", 1042313471, 2);
be_define_local_const_str(event_str_6, "every_50ms", -1911083288, 10);
be_define_local_const_str(event_str_7, "run_deferred", 371594696, 12);
be_define_local_const_str(event_str_8, "_drivers", -1034638311, 8);
be_define_local_const_str(event_str_9, "every_second", 2075451465, 12);
be_define_local_const_str(event_str_10, "every_100ms", 1546407804, 11);
be_define_local_const_str(event_str_11, "web_add_button", -757092238, 14);
be_define_local_const_str(event_str_12, "web_add_main_button", -334599632, 19);
be_define_local_const_str(event_str_13, "save_before_restart", 1253239338, 19);
be_define_local_const_str(event_str_14, "web_add_handler", -304792334, 15);
be_define_local_const_str(event_str_15, "web_sensor", -1394870324, 10);
be_define_local_const_str(event_str_16, "json_append", -1292948012, 11);
be_define_local_const_str(event_str_17, "button_pressed", 1694209616, 14);
be_define_local_const_str(event_str_18, "display", 1164572437, 7);
be_define_local_const_str(event_str_19, "string", 398550328, 6);
be_define_local_const_str(event_str_20, "log", 1062293841, 3);
be_define_local_const_str(event_str_21, "format", -1180859054, 6);
be_define_local_const_str(event_str_22, "BRY: exception %s - %m", -1290966132, 22);
be_define_local_const_str(event_str_24, "stop_iteration", -121173395, 14);

static const bvalue event_ktab[25] = {
  { { .s=be_local_const_str(event_str_0) }, BE_STRING},
  { { .s=be_local_const_str(event_str_1) }, BE_STRING},
  { { .s=be_local_const_str(event_str_2) }, BE_STRING},
  { { .s=be_local_const_str(event_str_3) }, BE_STRING},
  { { .s=be_local_const_str(event_str_4) }, BE_STRING},
  { { .s=be_local_const_str(event_str_5) }, BE_STRING},
  { { .s=be_local_const_str(event_str_6) }, BE_STRING},
  { { .s=be_local_const_str(event_str_7) }, BE_STRING},
  { { .s=be_local_const_str(event_str_8) }, BE_STRING},
  { { .s=be_local_const_str(event_str_9) }, BE_STRING},
  { { .s=be_local_const_str(event_str_10) }, BE_STRING},
  { { .s=be_local_const_str(event_str_11) }, BE_STRING},
  { { .s=be_local_const_str(event_str_12) }, BE_STRING},
  { { .s=be_local_const_str(event_str_13) }, BE_STRING},
  { { .s=be_local_const_str(event_str_14) }, BE_STRING},
  { { .s=be_local_const_str(event_str_15) }, BE_STRING},
  { { .s=be_local_const_str(event_str_16) }, BE_STRING},
  { { .s=be_local_const_str(event_str_17) }, BE_STRING},
  { { .s=be_local_const_str(event_str_18) }, BE_STRING},
  { { .s=be_local_const_str(event_str_19) }, BE_STRING},
  { { .s=be_local_const_str(event_str_20) }, BE_STRING},
  { { .s=be_local_const_str(event_str_21) }, BE_STRING},
  { { .s=be_local_const_str(event_str_22) }, BE_STRING},
  { { .i=3 }, BE_INT},
  { { .s=be_local_const_str(event_str_24) }, BE_STRING},
};

static const uint32_t event_code[136] = {
  0x1C140300,  //  0000  EQ	R5	R1	R256
  0x78160006,  //  0001  JMPF	R5	#0009
  0x8C140101,  //  0002  GETMET	R5	R0	R257
  0x5C1C0400,  //  0003  MOVE	R7	R2
  0x5C200600,  //  0004  MOVE	R8	R3
  0x5C240800,  //  0005  MOVE	R9	R4
  0x7C140800,  //  0006  CALL	R5	4
  0x80040A00,  //  0007  RET	1	R5
  0x7002007D,  //  0008  JMP		#0087
  0x1C140302,  //  0009  EQ	R5	R1	R258
  0x78160004,  //  000A  JMPF	R5	#0010
  0x8C140103,  //  000B  GETMET	R5	R0	R259
  0x5C1C0800,  //  000C  MOVE	R7	R4
  0x7C140400,  //  000D  CALL	R5	2
  0x80040A00,  //  000E  RET	1	R5
  0x70020076,  //  000F  JMP		#0087
  0x1C140304,  //  0010  EQ	R5	R1	R260
  0x78160002,  //  0011  JMPF	R5	#0015
  0x4C140000,  //  0012  LDNIL	5
  0x80040A00,  //  0013  RET	1	R5
  0x70020071,  //  0014  JMP		#0087
  0x1C140305,  //  0015  EQ	R5	R1	R261
  0x78160003,  //  0016  JMPF	R5	#001B
  0x8C140105,  //  0017  GETMET	R5	R0	R261
  0x7C140200,  //  0018  CALL	R5	1
  0x80040A00,  //  0019  RET	1	R5
  0x7002006B,  //  001A  JMP		#0087
  0x1C140306,  //  001B  EQ	R5	R1	R262
  0x78160003,  //  001C  JMPF	R5	#0021
  0x8C140107,  //  001D  GETMET	R5	R0	R263
  0x7C140200,  //  001E  CALL	R5	1
  0x80040A00,  //  001F  RET	1	R5
  0x70020065,  //  0020  JMP		#0087
  0x88140108,  //  0021  GETMBR	R5	R0	R264
  0x78160063,  //  0022  JMPF	R5	#0087
  0x60140000,  //  0023  GETGBL	R5	G0
  0x88180108,  //  0024  GETMBR	R6	R0	R264
  0x7C140200,  //  0025  CALL	R5	1
  0xA802005C,  //  0026  EXBLK	0	#0084
  0x5C180A00,  //  0027  MOVE	R6	R5
  0x7C180000,  //  0028  CALL	R6	0
  0xA802004D,  //  0029  EXBLK	0	#0078
  0x1C1C0309,  //  002A  EQ	R7	R1	R265
  0x781E0004,  //  002B  JMPF	R7	#0031
  0x881C0D09,  //  002C  GETMBR	R7	R6	R265
  0x781E0002,  //  002D  JMPF	R7	#0031
  0x8C1C0D09,  //  002E  GETMET	R7	R6	R265
  0x7C1C0200,  //  002F  CALL	R7	1
  0x70020044,  //  0030  JMP		#0076
  0x1C1C030A,  //  0031  EQ	R7	R1	R266
  0x781E0004,  //  0032  JMPF	R7	#0038
  0x881C0D0A,  //  0033  GETMBR	R7	R6	R266
  0x781E0002,  //  0034  JMPF	R7	#0038
  0x8C1C0D0A,  //  0035  GETMET	R7	R6	R266
  0x7C1C0200,  //  0036  CALL	R7	1
  0x7002003D,  //  0037  JMP		#0076
  0x1C1C030B,  //  0038  EQ	R7	R1	R267
  0x781E0004,  //  0039  JMPF	R7	#003F
  0x881C0D0B,  //  003A  GETMBR	R7	R6	R267
  0x781E0002,  //  003B  JMPF	R7	#003F
  0x8C1C0D0B,  //  003C  GETMET	R7	R6	R267
  0x7C1C0200,  //  003D  CALL	R7	1
  0x70020036,  //  003E  JMP		#0076
  0x1C1C030C,  //  003F  EQ	R7	R1	R268
  0x781E0004,  //  0040  JMPF	R7	#0046
  0x881C0D0C,  //  0041  GETMBR	R7	R6	R268
  0x781E0002,  //  0042  JMPF	R7	#0046
  0x8C1C0D0C,  //  0043  GETMET	R7	R6	R268
  0x7C1C0200,  //  0044  CALL	R7	1
  0x7002002F,  //  0045  JMP		#0076
  0x1C1C030D,  //  0046  EQ	R7	R1	R269
  0x781E0004,  //  0047  JMPF	R7	#004D
  0x881C0D0D,  //  0048  GETMBR	R7	R6	R269
  0x781E0002,  //  0049  JMPF	R7	#004D
  0x8C1C0D0D,  //  004A  GETMET	R7	R6	R269
  0x7C1C0200,  //  004B  CALL	R7	1
  0x70020028,  //  004C  JMP		#0076
  0x1C1C030E,  //  004D  EQ	R7	R1	R270
  0x781E0004,  //  004E  JMPF	R7	#0054
  0x881C0D0E,  //  004F  GETMBR	R7	R6	R270
  0x781E0002,  //  0050  JMPF	R7	#0054
  0x8C1C0D0E,  //  0051  GETMET	R7	R6	R270
  0x7C1C0200,  //  0052  CALL	R7	1
  0x70020021,  //  0053  JMP		#0076
  0x1C1C030F,  //  0054  EQ	R7	R1	R271
  0x781E0004,  //  0055  JMPF	R7	#005B
  0x881C0D0F,  //  0056  GETMBR	R7	R6	R271
  0x781E0002,  //  0057  JMPF	R7	#005B
  0x8C1C0D0F,  //  0058  GETMET	R7	R6	R271
  0x7C1C0200,  //  0059  CALL	R7	1
  0x7002001A,  //  005A  JMP		#0076
  0x1C1C0310,  //  005B  EQ	R7	R1	R272
  0x781E0004,  //  005C  JMPF	R7	#0062
  0x881C0D10,  //  005D  GETMBR	R7	R6	R272
  0x781E0002,  //  005E  JMPF	R7	#0062
  0x8C1C0D10,  //  005F  GETMET	R7	R6	R272
  0x7C1C0200,  //  0060  CALL	R7	1
  0x70020013,  //  0061  JMP		#0076
  0x1C1C0311,  //  0062  EQ	R7	R1	R273
  0x781E0004,  //  0063  JMPF	R7	#0069
  0x881C0D11,  //  0064  GETMBR	R7	R6	R273
  0x781E0002,  //  0065  JMPF	R7	#0069
  0x8C1C0D11,  //  0066  GETMET	R7	R6	R273
  0x7C1C0200,  //  0067  CALL	R7	1
  0x7002000C,  //  0068  JMP		#0076
  0x1C1C030E,  //  0069  EQ	R7	R1	R270
  0x781E0004,  //  006A  JMPF	R7	#0070
  0x881C0D12,  //  006B  GETMBR	R7	R6	R274
  0x781E0002,  //  006C  JMPF	R7	#0070
  0x8C1C0D12,  //  006D  GETMET	R7	R6	R274
  0x7C1C0200,  //  006E  CALL	R7	1
  0x70020005,  //  006F  JMP		#0076
  0x1C1C0312,  //  0070  EQ	R7	R1	R274
  0x781E0003,  //  0071  JMPF	R7	#0076
  0x881C0D12,  //  0072  GETMBR	R7	R6	R274
  0x781E0001,  //  0073  JMPF	R7	#0076
  0x8C1C0D12,  //  0074  GETMET	R7	R6	R274
  0x7C1C0200,  //  0075  CALL	R7	1
  0xA8040001,  //  0076  EXBLK	1	1
  0x7002000A,  //  0077  JMP		#0083
  0xAC1C0002,  //  0078  CATCH	R7	0	2
  0x70020007,  //  0079  JMP		#0082
  0xA4262600,  //  007A  IMPORT	R9	R275
  0x8C280114,  //  007B  GETMET	R10	R0	R276
  0x8C301315,  //  007C  GETMET	R12	R9	R277
  0x58380016,  //  007D  LDCONST	R14	K22
  0x583C0017,  //  007E  LDCONST	R15	K23
  0x7C300600,  //  007F  CALL	R12	3
  0x7C280400,  //  0080  CALL	R10	2
  0x70020000,  //  0081  JMP		#0083
  0xB0080000,  //  0082  RAISE	2	R0	R0
  0x7001FFA2,  //  0083  JMP		#0027
  0x58140018,  //  0084  LDCONST	R5	K24
  0xAC140200,  //  0085  CATCH	R5	1	0
  0xB0080000,  //  0086  RAISE	2	R0	R0
  0x80000000,  //  0087  RET	0	R0
};

be_define_local_proto(event, 16, 5, 1, 0, 0);
be_define_local_closure(event);

/*******************************************************************/

/********************************************************************
** Solidified function: add_driver
********************************************************************/

be_define_local_const_str(add_driver_str_name, "add_driver", 1654458371, 10);
be_define_local_const_str(add_driver_str_source, "string", 398550328, 6);
be_define_local_const_str(add_driver_str_0, "_drivers", -1034638311, 8);
be_define_local_const_str(add_driver_str_1, "push", -2022703139, 4);

static const bvalue add_driver_ktab[2] = {
  { { .s=be_local_const_str(add_driver_str_0) }, BE_STRING},
  { { .s=be_local_const_str(add_driver_str_1) }, BE_STRING},
};

static const uint32_t add_driver_code[12] = {
  0x88080100,  //  0000  GETMBR R2  R0  R256
  0x780A0004,  //  0001  JMPF R2  #0007
  0x88080100,  //  0002  GETMBR R2  R0  R256
  0x8C080501,  //  0003  GETMET R2  R2  R257
  0x5C100200,  //  0004  MOVE R4  R1
  0x7C080400,  //  0005  CALL R2  2
  0x70020003,  //  0006  JMP    #000B
  0x6008000A,  //  0007  GETGBL R2  G10
  0x7C080000,  //  0008  CALL R2  0
  0x400C0401,  //  0009  CONNECT  R3  R2  R1
  0x90020002,  //  000A  SETMBR R0  R256  R2
  0x80000000,  //  000B  RET  0 R0
};

static const bproto add_driver_proto = {
  NULL,     // bgcobject *next
  8,       // type
  0x08,        // marked
  5,       // nstack
  0,       // nupvals
  2,       // argc
  0,       // varg
  NULL,     // bgcobject *gray
  NULL,     // bupvaldesc *upvals
  (bvalue*) &add_driver_ktab,     // ktab
  NULL,     // bproto **ptab
  (binstruction*) &add_driver_code,     // code
  be_local_const_str(add_driver_str_name),       // name
  12,       // codesize
  2,       // nconst
  0,       // nproto
  be_local_const_str(add_driver_str_source),     // source
#if BE_DEBUG_RUNTIME_INFO /* debug information */
  NULL,     // lineinfo
  0,        // nlineinfo
#endif
#if BE_DEBUG_VAR_INFO
  NULL,     // varinfo
  0,        // nvarinfo
#endif
};

const bclosure add_driver_closure = {
  NULL,     // bgcobject *next
  36,       // type
  0x08,        // marked
  0,       // nupvals
  NULL,     // bgcobject *gray
  (bproto*) &add_driver_proto,     // proto
  { NULL }     // upvals
};

/*******************************************************************/

/********************************************************************
** Solidified function: load
********************************************************************/

/********** Solidified proto: load */
be_define_local_const_str(load_str_name, "load", -435725847, 4);
be_define_local_const_str(load_str_source, "input", -103256197, 5);
be_define_local_const_str(load_str_0, "string", 398550328, 6);
be_define_local_const_str(load_str_1, "split", -2017972765, 5);
be_define_local_const_str(load_str_2, ".", 722245873, 1);
be_define_local_const_str(load_str_4, "be", 942383232, 2);
be_define_local_const_str(load_str_5, "bec", 1336821081, 3);
be_define_local_const_str(load_str_6, "io_error", 1970281036, 8);
be_define_local_const_str(load_str_7, "file extension is not '.be' or '.bec'", -1199247657, 37);
be_define_local_const_str(load_str_8, "c", -435409838, 1);
be_define_local_const_str(load_str_9, "r", -150190315, 1);
be_define_local_const_str(load_str_10, "close", 667630371, 5);
be_define_local_const_str(load_str_11, "file", -1427482813, 4);
be_define_local_const_str(load_str_12, "save", -855671224, 4);
be_define_local_const_str(load_str_13, "log", 1062293841, 3);
be_define_local_const_str(load_str_14, "format", -1180859054, 6);
be_define_local_const_str(load_str_15, "BRY: could not save compiled file %s (%s)", 736659787, 41);

static const bvalue load_ktab[16] = {
  { { .s=be_local_const_str(load_str_0) }, BE_STRING},
  { { .s=be_local_const_str(load_str_1) }, BE_STRING},
  { { .s=be_local_const_str(load_str_2) }, BE_STRING},
  { { .i=1 }, BE_INT},
  { { .s=be_local_const_str(load_str_4) }, BE_STRING},
  { { .s=be_local_const_str(load_str_5) }, BE_STRING},
  { { .s=be_local_const_str(load_str_6) }, BE_STRING},
  { { .s=be_local_const_str(load_str_7) }, BE_STRING},
  { { .s=be_local_const_str(load_str_8) }, BE_STRING},
  { { .s=be_local_const_str(load_str_9) }, BE_STRING},
  { { .s=be_local_const_str(load_str_10) }, BE_STRING},
  { { .s=be_local_const_str(load_str_11) }, BE_STRING},
  { { .s=be_local_const_str(load_str_12) }, BE_STRING},
  { { .s=be_local_const_str(load_str_13) }, BE_STRING},
  { { .s=be_local_const_str(load_str_14) }, BE_STRING},
  { { .s=be_local_const_str(load_str_15) }, BE_STRING},
};

static const uint32_t load_code[68] = {
  0xA40A0000,  //  0000  IMPORT	R2	R256
  0x8C0C0501,  //  0001  GETMET	R3	R2	R257
  0x5C140200,  //  0002  MOVE	R5	R1
  0x58180002,  //  0003  LDCONST	R6	K2
  0x7C0C0600,  //  0004  CALL	R3	3
  0x60100012,  //  0005  GETGBL	R4	G18
  0x5C140600,  //  0006  MOVE	R5	R3
  0x7C100200,  //  0007  CALL	R4	1
  0x18100903,  //  0008  LE	R4	R4	R259
  0x74120007,  //  0009  JMPT	R4	#0012
  0x5411FFFE,  //  000A  LDINT	R4	-1
  0x94100604,  //  000B  GETIDX	R4	R3	R4
  0x20100904,  //  000C  NE	R4	R4	R260
  0x78120004,  //  000D  JMPF	R4	#0013
  0x5411FFFE,  //  000E  LDINT	R4	-1
  0x94100604,  //  000F  GETIDX	R4	R3	R4
  0x20100905,  //  0010  NE	R4	R4	R261
  0x78120000,  //  0011  JMPF	R4	#0013
  0xB0060D07,  //  0012  RAISE	1	R262	R263
  0x60100012,  //  0013  GETGBL	R4	G18
  0x5C140200,  //  0014  MOVE	R5	R1
  0x7C100200,  //  0015  CALL	R4	1
  0x4100903,  //  0016  SUB	R4	R4	R259
  0x94100204,  //  0017  GETIDX	R4	R1	R4
  0x1C100908,  //  0018  EQ	R4	R4	R264
  0xA8020007,  //  0019  EXBLK	0	#0022
  0x6014000E,  //  001A  GETGBL	R5	G14
  0x5C180200,  //  001B  MOVE	R6	R1
  0x581C0009,  //  001C  LDCONST	R7	K9
  0x7C140400,  //  001D  CALL	R5	2
  0x8C180B0A,  //  001E  GETMET	R6	R5	R266
  0x7C180200,  //  001F  CALL	R6	1
  0xA8040001,  //  0020  EXBLK	1	1
  0x70020006,  //  0021  JMP		#0029
  0x58140006,  //  0022  LDCONST	R5	K6
  0xAC140200,  //  0023  CATCH	R5	1	0
  0x70020002,  //  0024  JMP		#0028
  0x50140000,  //  0025  LDBOOL	R5	0	0
  0x80040A00,  //  0026  RET	1	R5
  0x70020000,  //  0027  JMP		#0029
  0xB0080000,  //  0028  RAISE	2	R0	R0
  0x60140005,  //  0029  GETGBL	R5	G5
  0x5C180200,  //  002A  MOVE	R6	R1
  0x581C000B,  //  002B  LDCONST	R7	K11
  0x7C140400,  //  002C  CALL	R5	2
  0x74120011,  //  002D  JMPT	R4	#0040
  0xA8020005,  //  002E  EXBLK	0	#0035
  0x8C18010C,  //  002F  GETMET	R6	R0	R268
  0x200308,  //  0030  ADD	R8	R1	R264
  0x5C240A00,  //  0031  MOVE	R9	R5
  0x7C180600,  //  0032  CALL	R6	3
  0xA8040001,  //  0033  EXBLK	1	1
  0x7002000A,  //  0034  JMP		#0040
  0xAC180001,  //  0035  CATCH	R6	0	1
  0x70020007,  //  0036  JMP		#003F
  0x8C1C010D,  //  0037  GETMET	R7	R0	R269
  0x8C24050E,  //  0038  GETMET	R9	R2	R270
  0x582C000F,  //  0039  LDCONST	R11	K15
  0x300308,  //  003A  ADD	R12	R1	R264
  0x5C340C00,  //  003B  MOVE	R13	R6
  0x7C240800,  //  003C  CALL	R9	4
  0x7C1C0400,  //  003D  CALL	R7	2
  0x70020000,  //  003E  JMP		#0040
  0xB0080000,  //  003F  RAISE	2	R0	R0
  0x5C180A00,  //  0040  MOVE	R6	R5
  0x7C180000,  //  0041  CALL	R6	0
  0x50180200,  //  0042  LDBOOL	R6	1	0
  0x80040C00,  //  0043  RET	1	R6
};

be_define_local_proto(load, 14, 2, 1, 0, 0);
be_define_local_closure(load);

/*******************************************************************/

/********************************************************************
    // tasmota.wire_scan(addr:int [, index:int]) -> wire1 or wire2 or nil
    // scan for the first occurrence of the addr, starting with bus1 then bus2
    // optional: skip if index is disabled via I2CEnable
    "def wire_scan(addr,idx) "
      // skip if the I2C index is disabled
      "if idx != nil && !self.i2c_enabled(idx) return nil end "
      "if self.wire1.detect(addr) return self.wire1 end "
      "if self.wire2.detect(addr) return self.wire2 end "
      "return nil "
    "end "
********************************************************************/
/********************************************************************
** Solidified function: wire_scan
********************************************************************/

be_define_local_const_str(wire_scan_str_name, "wire_scan", -1623691416, 9);
be_define_local_const_str(wire_scan_str_source, "string", 398550328, 6);
be_define_local_const_str(wire_scan_str_0, "i2c_enabled", 218388101, 11);
be_define_local_const_str(wire_scan_str_1, "wire1", -1082245877, 5);
be_define_local_const_str(wire_scan_str_2, "detect", 8884370, 6);
be_define_local_const_str(wire_scan_str_3, "wire2", -1065468258, 5);

static const bvalue wire_scan_ktab[4] = {
  { { .s=be_local_const_str(wire_scan_str_0) }, BE_STRING},
  { { .s=be_local_const_str(wire_scan_str_1) }, BE_STRING},
  { { .s=be_local_const_str(wire_scan_str_2) }, BE_STRING},
  { { .s=be_local_const_str(wire_scan_str_3) }, BE_STRING},
};

static const uint32_t wire_scan_code[25] = {
  0x4C0C0000,  //  0000  LDNIL	3
  0x200C0403,  //  0001  NE	R3	R2	R3
  0x780E0005,  //  0002  JMPF	R3	#0009
  0x8C0C0100,  //  0003  GETMET	R3	R0	R256
  0x5C140400,  //  0004  MOVE	R5	R2
  0x7C0C0400,  //  0005  CALL	R3	2
  0x740E0001,  //  0006  JMPT	R3	#0009
  0x4C0C0000,  //  0007  LDNIL	3
  0x80040600,  //  0008  RET	1	R3
  0x880C0101,  //  0009  GETMBR	R3	R0	R257
  0x8C0C0702,  //  000A  GETMET	R3	R3	R258
  0x5C140200,  //  000B  MOVE	R5	R1
  0x7C0C0400,  //  000C  CALL	R3	2
  0x780E0001,  //  000D  JMPF	R3	#0010
  0x880C0101,  //  000E  GETMBR	R3	R0	R257
  0x80040600,  //  000F  RET	1	R3
  0x880C0103,  //  0010  GETMBR	R3	R0	R259
  0x8C0C0702,  //  0011  GETMET	R3	R3	R258
  0x5C140200,  //  0012  MOVE	R5	R1
  0x7C0C0400,  //  0013  CALL	R3	2
  0x780E0001,  //  0014  JMPF	R3	#0017
  0x880C0103,  //  0015  GETMBR	R3	R0	R259
  0x80040600,  //  0016  RET	1	R3
  0x4C0C0000,  //  0017  LDNIL	3
  0x80040600,  //  0018  RET	1	R3
};

static const bproto wire_scan_proto = {
  NULL,     // bgcobject *next
  8,       // type
  0x08,        // marked
  6,       // nstack
  0,       // nupvals
  3,       // argc
  0,       // varg
  NULL,     // bgcobject *gray
  NULL,     // bupvaldesc *upvals
  (bvalue*) &wire_scan_ktab,     // ktab
  NULL,     // bproto **ptab
  (binstruction*) &wire_scan_code,     // code
  be_local_const_str(wire_scan_str_name),       // name
  25,       // codesize
  4,       // nconst
  0,       // nproto
  be_local_const_str(wire_scan_str_source),     // source
#if BE_DEBUG_RUNTIME_INFO /* debug information */
  NULL,     // lineinfo
  0,        // nlineinfo
#endif
#if BE_DEBUG_VAR_INFO
  NULL,     // varinfo
  0,        // nvarinfo
#endif
};

static const bclosure wire_scan_closure = {
  NULL,     // bgcobject *next
  36,       // type
  0x08,        // marked
  0,       // nupvals
  NULL,     // bgcobject *gray
  (bproto*) &wire_scan_proto,     // proto
  { NULL }     // upvals
};

/*******************************************************************/

/********************************************************************
  def time_str(time)
    import string
    var tm = self.time_dump(time)
    return string.format("%04d-%02d-%02dT%02d:%02d:%02d", tm['year'], tm['month'], tm['day'], tm['hour'], tm['min'], tm['sec'])
  end
********************************************************************/
/********************************************************************
** Solidified function: time_str
********************************************************************/

/********** Solidified proto: time_str */
be_define_local_const_str(time_str_str_name, "time_str", -1681139684, 8);
be_define_local_const_str(time_str_str_source, "input", -103256197, 5);
be_define_local_const_str(time_str_str_0, "string", 398550328, 6);
be_define_local_const_str(time_str_str_1, "time_dump", -964556549, 9);
be_define_local_const_str(time_str_str_2, "format", -1180859054, 6);
be_define_local_const_str(time_str_str_3, "%04d-%02d-%02dT%02d:%02d:%02d", -869438695, 29);
be_define_local_const_str(time_str_str_4, "year", -1367388900, 4);
be_define_local_const_str(time_str_str_5, "month", -696646139, 5);
be_define_local_const_str(time_str_str_6, "day", -464576003, 3);
be_define_local_const_str(time_str_str_7, "hour", -1241306097, 4);
be_define_local_const_str(time_str_str_8, "min", -913357481, 3);
be_define_local_const_str(time_str_str_9, "sec", -1155074638, 3);

static const bvalue time_str_ktab[10] = {
  { { .s=be_local_const_str(time_str_str_0) }, BE_STRING},
  { { .s=be_local_const_str(time_str_str_1) }, BE_STRING},
  { { .s=be_local_const_str(time_str_str_2) }, BE_STRING},
  { { .s=be_local_const_str(time_str_str_3) }, BE_STRING},
  { { .s=be_local_const_str(time_str_str_4) }, BE_STRING},
  { { .s=be_local_const_str(time_str_str_5) }, BE_STRING},
  { { .s=be_local_const_str(time_str_str_6) }, BE_STRING},
  { { .s=be_local_const_str(time_str_str_7) }, BE_STRING},
  { { .s=be_local_const_str(time_str_str_8) }, BE_STRING},
  { { .s=be_local_const_str(time_str_str_9) }, BE_STRING},
};

static const uint32_t time_str_code[14] = {
  0xA40A0000,  //  0000  IMPORT	R2	R256
  0x8C0C0101,  //  0001  GETMET	R3	R0	R257
  0x5C140200,  //  0002  MOVE	R5	R1
  0x7C0C0400,  //  0003  CALL	R3	2
  0x8C100502,  //  0004  GETMET	R4	R2	R258
  0x58180003,  //  0005  LDCONST	R6	K3
  0x941C0704,  //  0006  GETIDX	R7	R3	R260
  0x94200705,  //  0007  GETIDX	R8	R3	R261
  0x94240706,  //  0008  GETIDX	R9	R3	R262
  0x94280707,  //  0009  GETIDX	R10	R3	R263
  0x942C0708,  //  000A  GETIDX	R11	R3	R264
  0x94300709,  //  000B  GETIDX	R12	R3	R265
  0x7C101000,  //  000C  CALL	R4	8
  0x80040800,  //  000D  RET	1	R4
};

be_define_local_proto(time_str, 13, 2, 1, 0, 0);
be_define_local_closure(time_str);

/*******************************************************************/

/********************************************************************
    // cmd high-level function
    "def cmd(command) "
      "import json "
      "var ret = self._cmd(command) "
      "var j = json.load(ret) "
      "if type(j) == 'instance' "
        "return j "
      "else "
        "return {'response':j} "
      "end "
    "end "
********************************************************************/
/********************************************************************
** Solidified function: cmd
********************************************************************/

be_define_local_const_str(cmd_str_name, "cmd", -158181397, 3);
be_define_local_const_str(cmd_str_source, "string", 398550328, 6);
be_define_local_const_str(cmd_str_0, "json", 916562499, 4);
be_define_local_const_str(cmd_str_1, "_cmd", -875145154, 4);
be_define_local_const_str(cmd_str_2, "load", -435725847, 4);
be_define_local_const_str(cmd_str_3, "instance", 193386898, 8);
be_define_local_const_str(cmd_str_4, "response", 1499316702, 8);

static const bvalue cmd_ktab[5] = {
  { { .s=be_local_const_str(cmd_str_0) }, BE_STRING},
  { { .s=be_local_const_str(cmd_str_1) }, BE_STRING},
  { { .s=be_local_const_str(cmd_str_2) }, BE_STRING},
  { { .s=be_local_const_str(cmd_str_3) }, BE_STRING},
  { { .s=be_local_const_str(cmd_str_4) }, BE_STRING},
};

static const uint32_t cmd_code[19] = {
  0xA40A0000,  //  0000  IMPORT R2  R256
  0x8C0C0101,  //  0001  GETMET R3  R0  R257
  0x5C140200,  //  0002  MOVE R5  R1
  0x7C0C0400,  //  0003  CALL R3  2
  0x8C100502,  //  0004  GETMET R4  R2  R258
  0x5C180600,  //  0005  MOVE R6  R3
  0x7C100400,  //  0006  CALL R4  2
  0x60140015,  //  0007  GETGBL R5  G21
  0x5C180800,  //  0008  MOVE R6  R4
  0x7C140200,  //  0009  CALL R5  1
  0x1C140B03,  //  000A  EQ R5  R5  R259
  0x78160001,  //  000B  JMPF R5  #000E
  0x80040800,  //  000C  RET  1 R4
  0x70020003,  //  000D  JMP    #0012
  0x6014000B,  //  000E  GETGBL R5  G11
  0x7C140000,  //  000F  CALL R5  0
  0x98160804,  //  0010  SETIDX R5  R260  R4
  0x80040A00,  //  0011  RET  1 R5
  0x80000000,  //  0012  RET  0 R0
};

static const bproto cmd_proto = {
  NULL,     // bgcobject *next
  8,       // type
  0x08,        // marked
  7,       // nstack
  0,       // nupvals
  2,       // argc
  0,       // varg
  NULL,     // bgcobject *gray
  NULL,     // bupvaldesc *upvals
  (bvalue*) &cmd_ktab,     // ktab
  NULL,     // bproto **ptab
  (binstruction*) &cmd_code,     // code
  be_local_const_str(cmd_str_name),       // name
  19,       // codesize
  5,       // nconst
  0,       // nproto
  be_local_const_str(cmd_str_source),     // source
#if BE_DEBUG_RUNTIME_INFO /* debug information */
  NULL,     // lineinfo
  0,        // nlineinfo
#endif
#if BE_DEBUG_VAR_INFO
  NULL,     // varinfo
  0,        // nvarinfo
#endif
};

static const bclosure cmd_closure = {
  NULL,     // bgcobject *next
  36,       // type
  0x08,        // marked
  0,       // nupvals
  NULL,     // bgcobject *gray
  (bproto*) &cmd_proto,     // proto
  { NULL }     // upvals
};

/*******************************************************************/


/********************************************************************
    "def get_light(l) "
      "print('tasmota.get_light() is deprecated, use light.get()') "
      "import light "
      "if l != nil "
        "return light.get(l) "
      "else "
        "return light.get() "
      "end "
    "end "
********************************************************************/
/********************************************************************
** Solidified function: get_light
********************************************************************/

be_define_local_const_str(get_light_str_name, "get_light", 381930476, 9);
be_define_local_const_str(get_light_str_source, "string", 398550328, 6);
be_define_local_const_str(get_light_str_0, "tasmota.get_light() is deprecated, use light.get()", -769213649, 50);
be_define_local_const_str(get_light_str_1, "light", -493019601, 5);
be_define_local_const_str(get_light_str_2, "get", 1410115415, 3);

static const bvalue get_light_ktab[3] = {
  { { .s=be_local_const_str(get_light_str_0) }, BE_STRING},
  { { .s=be_local_const_str(get_light_str_1) }, BE_STRING},
  { { .s=be_local_const_str(get_light_str_2) }, BE_STRING},
};

static const uint32_t get_light_code[16] = {
  0x6008000F,  //  0000  GETGBL	R2	G15
  0x580C0000,  //  0001  LDCONST	R3	K0
  0x7C080200,  //  0002  CALL	R2	1
  0xA40A0200,  //  0003  IMPORT	R2	R257
  0x4C0C0000,  //  0004  LDNIL	3
  0x200C0203,  //  0005  NE	R3	R1	R3
  0x780E0004,  //  0006  JMPF	R3	#000C
  0x8C0C0502,  //  0007  GETMET	R3	R2	R258
  0x5C140200,  //  0008  MOVE	R5	R1
  0x7C0C0400,  //  0009  CALL	R3	2
  0x80040600,  //  000A  RET	1	R3
  0x70020002,  //  000B  JMP		#000F
  0x8C0C0502,  //  000C  GETMET	R3	R2	R258
  0x7C0C0200,  //  000D  CALL	R3	1
  0x80040600,  //  000E  RET	1	R3
  0x80000000,  //  000F  RET	0	R0
};

static const bproto get_light_proto = {
  NULL,     // bgcobject *next
  8,       // type
  0x08,        // marked
  6,       // nstack
  0,       // nupvals
  2,       // argc
  0,       // varg
  NULL,     // bgcobject *gray
  NULL,     // bupvaldesc *upvals
  (bvalue*) &get_light_ktab,     // ktab
  NULL,     // bproto **ptab
  (binstruction*) &get_light_code,     // code
  be_local_const_str(get_light_str_name),       // name
  16,       // codesize
  3,       // nconst
  0,       // nproto
  be_local_const_str(get_light_str_source),     // source
#if BE_DEBUG_RUNTIME_INFO /* debug information */
  NULL,     // lineinfo
  0,        // nlineinfo
#endif
#if BE_DEBUG_VAR_INFO
  NULL,     // varinfo
  0,        // nvarinfo
#endif
};

static const bclosure get_light_closure = {
  NULL,     // bgcobject *next
  36,       // type
  0x08,        // marked
  0,       // nupvals
  NULL,     // bgcobject *gray
  (bproto*) &get_light_proto,     // proto
  { NULL }     // upvals
};

/*******************************************************************/

/********************************************************************

    // set_light and get_light deprecetaion
    "def set_light(v,l) "
      "print('tasmota.set_light() is deprecated, use light.set()') "
      "import light "
      "if l != nil "
        "return light.set(v,l) "
      "else "
        "return light.set(v) "
      "end "
    "end "

********************************************************************/
/********************************************************************
** Solidified function: set_light
********************************************************************/

be_define_local_const_str(set_light_str_name, "set_light", -1118891144, 9);
be_define_local_const_str(set_light_str_source, "string", 398550328, 6);
be_define_local_const_str(set_light_str_0, "tasmota.set_light() is deprecated, use light.set()", 2124937871, 50);
be_define_local_const_str(set_light_str_1, "light", -493019601, 5);
be_define_local_const_str(set_light_str_2, "set", -970520829, 3);

static const bvalue set_light_ktab[3] = {
  { { .s=be_local_const_str(set_light_str_0) }, BE_STRING},
  { { .s=be_local_const_str(set_light_str_1) }, BE_STRING},
  { { .s=be_local_const_str(set_light_str_2) }, BE_STRING},
};

static const uint32_t set_light_code[18] = {
  0x600C000F,  //  0000  GETGBL R3  G15
  0x58100000,  //  0001  LDCONST  R4  K0
  0x7C0C0200,  //  0002  CALL R3  1
  0xA40E0200,  //  0003  IMPORT R3  R257
  0x4C100000,  //  0004  LDNIL  4
  0x20100404,  //  0005  NE R4  R2  R4
  0x78120005,  //  0006  JMPF R4  #000D
  0x8C100702,  //  0007  GETMET R4  R3  R258
  0x5C180200,  //  0008  MOVE R6  R1
  0x5C1C0400,  //  0009  MOVE R7  R2
  0x7C100600,  //  000A  CALL R4  3
  0x80040800,  //  000B  RET  1 R4
  0x70020003,  //  000C  JMP    #0011
  0x8C100702,  //  000D  GETMET R4  R3  R258
  0x5C180200,  //  000E  MOVE R6  R1
  0x7C100400,  //  000F  CALL R4  2
  0x80040800,  //  0010  RET  1 R4
  0x80000000,  //  0011  RET  0 R0
};

static const bproto set_light_proto = {
  NULL,     // bgcobject *next
  8,       // type
  0x08,        // marked
  8,       // nstack
  0,       // nupvals
  3,       // argc
  0,       // varg
  NULL,     // bgcobject *gray
  NULL,     // bupvaldesc *upvals
  (bvalue*) &set_light_ktab,     // ktab
  NULL,     // bproto **ptab
  (binstruction*) &set_light_code,     // code
  be_local_const_str(set_light_str_name),       // name
  18,       // codesize
  3,       // nconst
  0,       // nproto
  be_local_const_str(set_light_str_source),     // source
#if BE_DEBUG_RUNTIME_INFO /* debug information */
  NULL,     // lineinfo
  0,        // nlineinfo
#endif
#if BE_DEBUG_VAR_INFO
  NULL,     // varinfo
  0,        // nvarinfo
#endif
};

static const bclosure set_light_closure = {
  NULL,     // bgcobject *next
  36,       // type
  0x08,        // marked
  0,       // nupvals
  NULL,     // bgcobject *gray
  (bproto*) &set_light_proto,     // proto
  { NULL }     // upvals
};

/*******************************************************************/


/********************************************************************
** Solidified function: cb_dispatch
********************************************************************/

/********** Solidified proto: cb_dispatch */
be_define_local_const_str(cb_dispatch_str_name, "cb_dispatch", 1741510499, 11);
be_define_local_const_str(cb_dispatch_str_source, "input", -103256197, 5);
be_define_local_const_str(cb_dispatch_str_0, "_cb", -251666929, 3);
be_define_local_const_str(cb_dispatch_str_2, "find", -1108310694, 4);

static const bvalue cb_dispatch_ktab[3] = {
  { { .s=be_local_const_str(cb_dispatch_str_0) }, BE_STRING},
  { { .i=0 }, BE_INT},
  { { .s=be_local_const_str(cb_dispatch_str_2) }, BE_STRING},
};

static const uint32_t cb_dispatch_code[20] = {
  0x88180100,  //  0000  GETMBR	R6	R0	R256
  0x4C1C0000,  //  0001  LDNIL	7
  0x1C180C07,  //  0002  EQ	R6	R6	R7
  0x781A0000,  //  0003  JMPF	R6	#0005
  0x80060200,  //  0004  RET	1	R257
  0x88180100,  //  0005  GETMBR	R6	R0	R256
  0x8C180D02,  //  0006  GETMET	R6	R6	R258
  0x5C200200,  //  0007  MOVE	R8	R1
  0x7C180400,  //  0008  CALL	R6	2
  0x4C1C0000,  //  0009  LDNIL	7
  0x201C0C07,  //  000A  NE	R7	R6	R7
  0x781E0006,  //  000B  JMPF	R7	#0013
  0x5C1C0C00,  //  000C  MOVE	R7	R6
  0x5C200400,  //  000D  MOVE	R8	R2
  0x5C240600,  //  000E  MOVE	R9	R3
  0x5C280800,  //  000F  MOVE	R10	R4
  0x5C2C0A00,  //  0010  MOVE	R11	R5
  0x7C1C0800,  //  0011  CALL	R7	4
  0x80040E00,  //  0012  RET	1	R7
  0x80060200,  //  0013  RET	1	R257
};

be_define_local_proto(cb_dispatch, 12, 6, 1, 0, 0);
be_define_local_closure(cb_dispatch);

/*******************************************************************/

/********************************************************************
** Solidified function: gen_cb
********************************************************************/

/********** Solidified proto: gen_cb */
be_define_local_const_str(gen_cb_str_name, "gen_cb", -1049739745, 6);
be_define_local_const_str(gen_cb_str_source, "input", -103256197, 5);
be_define_local_const_str(gen_cb_str_0, "_cb", -251666929, 3);
be_define_local_const_str(gen_cb_str_2, "find", -1108310694, 4);
be_define_local_const_str(gen_cb_str_3, "_get_cb", 1448849122, 7);
be_define_local_const_str(gen_cb_str_4, "stop_iteration", -121173395, 14);
be_define_local_const_str(gen_cb_str_5, "internal_error", -1775809127, 14);
be_define_local_const_str(gen_cb_str_6, "No callback available", 633786138, 21);

static const bvalue gen_cb_ktab[7] = {
  { { .s=be_local_const_str(gen_cb_str_0) }, BE_STRING},
  { { .i=0 }, BE_INT},
  { { .s=be_local_const_str(gen_cb_str_2) }, BE_STRING},
  { { .s=be_local_const_str(gen_cb_str_3) }, BE_STRING},
  { { .s=be_local_const_str(gen_cb_str_4) }, BE_STRING},
  { { .s=be_local_const_str(gen_cb_str_5) }, BE_STRING},
  { { .s=be_local_const_str(gen_cb_str_6) }, BE_STRING},
};

static const uint32_t gen_cb_code[34] = {
  0x88080100,  //  0000  GETMBR	R2	R0	R256
  0x4C0C0000,  //  0001  LDNIL	3
  0x1C080403,  //  0002  EQ	R2	R2	R3
  0x780A0002,  //  0003  JMPF	R2	#0007
  0x6008000B,  //  0004  GETGBL	R2	G11
  0x7C080000,  //  0005  CALL	R2	0
  0x90020002,  //  0006  SETMBR	R0	R256	R2
  0x60080000,  //  0007  GETGBL	R2	G0
  0x540E0012,  //  0008  LDINT	R3	19
  0x400E0203,  //  0009  CONNECT	R3	R257	R3
  0x7C080200,  //  000A  CALL	R2	1
  0xA8020010,  //  000B  EXBLK	0	#001D
  0x5C0C0400,  //  000C  MOVE	R3	R2
  0x7C0C0000,  //  000D  CALL	R3	0
  0x88100100,  //  000E  GETMBR	R4	R0	R256
  0x8C100902,  //  000F  GETMET	R4	R4	R258
  0x5C180600,  //  0010  MOVE	R6	R3
  0x7C100400,  //  0011  CALL	R4	2
  0x4C140000,  //  0012  LDNIL	5
  0x1C100805,  //  0013  EQ	R4	R4	R5
  0x78120006,  //  0014  JMPF	R4	#001C
  0x88100100,  //  0015  GETMBR	R4	R0	R256
  0x98100601,  //  0016  SETIDX	R4	R3	R1
  0x8C100103,  //  0017  GETMET	R4	R0	R259
  0x5C180600,  //  0018  MOVE	R6	R3
  0x7C100400,  //  0019  CALL	R4	2
  0xA8040001,  //  001A  EXBLK	1	1
  0x80040800,  //  001B  RET	1	R4
  0x7001FFEE,  //  001C  JMP		#000C
  0x58080004,  //  001D  LDCONST	R2	K4
  0xAC080200,  //  001E  CATCH	R2	1	0
  0xB0080000,  //  001F  RAISE	2	R0	R0
  0xB0060B06,  //  0020  RAISE	1	R261	R262
  0x80000000,  //  0021  RET	0	R0
};

be_define_local_proto(gen_cb, 7, 2, 1, 0, 0);
be_define_local_closure(gen_cb);

/*******************************************************************/


#if BE_USE_PRECOMPILED_OBJECT
#include "../generate/be_fixed_be_class_tasmota.h"
#endif


// Class definition
void be_load_tasmota_ntvlib(bvm *vm)
{
#if !BE_USE_PRECOMPILED_OBJECT
    static const bnfuncinfo members[] = {
        { "_rules", NULL },
        { "_timers", NULL },
        { "_ccmd", NULL },
        { "_drivers", NULL },
        { "_cb", NULL},
        { "wire1", NULL },
        { "wire2", NULL },
        { "get_free_heap", l_getFreeHeap },
        { "publish", l_publish },
        { "_cmd", l_cmd },
        { "_get_cb", l_get_cb },
        { "get_option", l_getoption },
        { "millis", l_millis },
        { "time_reached", l_timereached },
        { "rtc", l_rtc },
        { "time_dump", l_time_dump },
        { "memory", l_memory },
        { "yield", l_yield },
        { "delay", l_delay },
        { "scale_uint", l_scaleuint },
        { "log", l_logInfo },
        { "save", l_save },

        { "resp_cmnd", l_respCmnd },
        { "resp_cmnd_str", l_respCmndStr },
        { "resp_cmnd_done", l_respCmndDone },
        { "resp_cmnd_error", l_respCmndError },
        { "resp_cmnd_failed", l_respCmndFailed },
        { "resolvecmnd", l_resolveCmnd },

        { "response_append", l_respAppend },
        { "web_send", l_webSend },
        { "web_send_decimal", l_webSendDecimal },

        { "get_power", l_getpower },
        { "set_power", l_setpower },

        { "i2c_enabled", l_i2cenabled },

        { NULL, (bntvfunc) BE_CLOSURE }, /* mark section for berry closures */
        { "cmd", (bntvfunc) &cmd_closure },
        { "chars_in_string", (bntvfunc) &chars_in_string_closure },
        { "find_key_i", (bntvfunc) &find_key_i_closure },
        { "find_op", (bntvfunc) &find_op_closure },
        { "add_rule", (bntvfunc) &add_rule_closure },
        { "remove_rule", (bntvfunc) &remove_rule_closure },
        { "try_rule", (bntvfunc) &try_rule_closure },
        { "exec_rules", (bntvfunc) &exec_rules_closure },
        { "set_timer", (bntvfunc) &set_timer_closure },
        { "run_deferred", (bntvfunc) &run_deferred_closure },
        { "add_cmd", (bntvfunc) &add_cmd_closure },
        { "remove_cmd", (bntvfunc) &remove_cmd_closure },
        { "exec_cmd", (bntvfunc) &exec_cmd_closure },
        { "gc", (bntvfunc) &gc_closure },
        { "event", (bntvfunc) &event_closure },
        { "add_driver", (bntvfunc) &add_driver_closure },
        { "load", (bntvfunc) &load_closure },
        { "wire_scan", (bntvfunc) &wire_scan_closure },
        { "time_str", (bntvfunc) &time_str_closure },

        // callbacks
        { "cb_dispatch", (bntvfunc) &cb_dispatch_closure },
        { "gen_cb", (bntvfunc) &gen_cb_closure },

        // deprecated
        { "get_light", (bntvfunc) &get_light_closure },
        { "set_light", (bntvfunc) &set_light_closure },

        { NULL, NULL }
    };
    be_regclass(vm, "Tasmota", members);
#else
    be_pushntvclass(vm, &be_class_tasmota);
    be_setglobal(vm, "Tasmota");
    be_pop(vm, 1);
#endif
}

/* @const_object_info_begin
class be_class_tasmota (scope: global, name: Tasmota) {
    _rules, var
    _timers, var
    _ccmd, var
    _drivers, var
    _cb, var
    wire1, var
    wire2, var

    get_free_heap, func(l_getFreeHeap)
    publish, func(l_publish)
    _cmd, func(l_cmd)
    _get_cb, func(l_get_cb)
    get_option, func(l_getoption)
    millis, func(l_millis)
    time_reached, func(l_timereached)
    rtc, func(l_rtc)
    time_dump, func(l_time_dump)
    memory, func(l_memory)
    yield, func(l_yield)
    delay, func(l_delay)
    scale_uint, func(l_scaleuint)
    log, func(l_logInfo)
    save, func(l_save)

    resp_cmnd, func(l_respCmnd)
    resp_cmnd_str, func(l_respCmndStr)
    resp_cmnd_done, func(l_respCmndDone)
    resp_cmnd_error, func(l_respCmndError)
    resp_cmnd_failed, func(l_respCmndFailed)
    resolvecmnd, func(l_resolveCmnd)

    response_append, func(l_respAppend)
    web_send, func(l_webSend)
    web_send_decimal, func(l_webSendDecimal)

    get_power, func(l_getpower)
    set_power, func(l_setpower)

    i2c_enabled, func(l_i2cenabled)

    cmd, closure(cmd_closure)
    chars_in_string, closure(chars_in_string_closure)
    find_key_i, closure(find_key_i_closure)
    find_op, closure(find_op_closure)
    add_rule, closure(add_rule_closure)
    remove_rule, closure(remove_rule_closure)
    try_rule, closure(try_rule_closure)
    exec_rules, closure(exec_rules_closure)
    set_timer, closure(set_timer_closure)
    run_deferred, closure(run_deferred_closure)
    add_cmd, closure(add_cmd_closure)
    remove_cmd, closure(remove_cmd_closure)
    exec_cmd, closure(exec_cmd_closure)
    gc, closure(gc_closure)
    event, closure(event_closure)
    add_driver, closure(add_driver_closure)
    load, closure(load_closure)
    wire_scan, closure(wire_scan_closure)
    time_str, closure(time_str_closure)

    cb_dispatch, closure(cb_dispatch_closure)
    gen_cb, closure(gen_cb_closure)

    get_light, closure(get_light_closure)
    set_light, closure(set_light_closure)
}
@const_object_info_end */
