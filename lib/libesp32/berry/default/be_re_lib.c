/********************************************************************
 * Tasmota lib
 * 
 * To use: `import re`
 * 
 * Regex using re1.5
 *******************************************************************/
#include "be_constobj.h"
#include "be_mem.h"
#include "be_object.h"
#include "be_exec.h"
#include "../../re1.5/re1.5.h"

/********************************************************************
# Berry skeleton for `re` module
#

class re_pattern
  var _p          # comobj containing the compiled bytecode for the pattern

  def search() end
  def match() end
  def split() end
end

re = module("re")

re.compile = def (regex_str) end    # native
re.match = def (regex_str, str) end # native
re.search = def (regex_str, str) end # native
re.split = def (regex_str, str) end # native


*******************************************************************/

extern const bclass be_class_re_pattern;

// Native functions be_const_func()
// Berry: `re.compile(pattern:string) -> instance(be_pattern)`
int be_re_compile(bvm *vm) {
  int32_t argc = be_top(vm); // Get the number of arguments
  if (argc >= 1 && be_isstring(vm, 1)) {
    const char * regex_str = be_tostring(vm, 1);
    int sz = re1_5_sizecode(regex_str);
    if (sz < 0) {
      be_raise(vm, "internal_error", "error in regex");
    }

    ByteProg *code = be_os_malloc(sizeof(ByteProg) + sz);
    if (code == NULL) {
      be_throw(vm, BE_MALLOC_FAIL);   /* lack of heap space */
    }
    int ret = re1_5_compilecode(code, regex_str);
    if (ret != 0) {
      be_raise(vm, "internal_error", "error in regex");
    }
    be_pushntvclass(vm, &be_class_re_pattern);
    be_call(vm, 0);
    be_newcomobj(vm, code, &be_commonobj_destroy_generic);
    be_setmember(vm, -2, "_p");
    be_pop(vm, 1);
    be_return(vm);
  }
  be_raise(vm, "type_error", NULL);
}

// Native functions be_const_func()
// Berry: `re.compilebytes(pattern:string) -> instance(bytes)`
int be_re_compilebytes(bvm *vm) {
  int32_t argc = be_top(vm); // Get the number of arguments
  if (argc >= 1 && be_isstring(vm, 1)) {
    const char * regex_str = be_tostring(vm, 1);
    int sz = re1_5_sizecode(regex_str);
    if (sz < 0) {
      be_raise(vm, "internal_error", "error in regex");
    }

    be_pushbytes(vm, NULL, sizeof(ByteProg) + sz);    
    ByteProg *code = (ByteProg*) be_tobytes(vm, -1, NULL);
    re1_5_compilecode(code, regex_str);
    be_return(vm);
  }
  be_raise(vm, "type_error", NULL);
}

// pushes either a list if matched, else `nil`
// return index of next offset, or -1 if not found
const char *be_re_match_search_run(bvm *vm, ByteProg *code, const char *hay, bbool is_anchored, bbool size_only) {
  Subject subj = {hay, hay + strlen(hay)};

  int sub_els = (code->sub + 1) * 2;
  const char *sub[sub_els];
  memset(sub, 0, sub_els * sizeof sub[0]);

  if (!re1_5_recursiveloopprog(code, &subj, sub, sub_els, is_anchored)) {
    be_pushnil(vm);
    return NULL;    // no match
  }

  be_newobject(vm, "list");
  int k = sub_els;
  for (int i = 0; i < k; i += 2) {
    if (sub[i] == nil || sub[i+1] == nil) {
      be_pushnil(vm);
    } else {
      if (size_only && i==0) {
        be_pushint(vm, sub[i+1] - sub[i]);
      } else {
        be_pushnstring(vm, sub[i], sub[i+1] - sub[i]);
      }
    }
    be_data_push(vm, -2);
    be_pop(vm, 1);
  }
  be_pop(vm, 1);    // remove list
  return sub[1];
}

int be_re_match_search(bvm *vm, bbool is_anchored, bbool size_only) {
  int32_t argc = be_top(vm); // Get the number of arguments
  if (argc >= 2 && (be_isstring(vm, 1) || be_isbytes(vm, 1)) && be_isstring(vm, 2)) {
    const char * hay = be_tostring(vm, 2);
    ByteProg *code = NULL;

    int32_t offset = 0;
    if (argc >= 3 && be_isint(vm, 3)) {
      offset = be_toint(vm, 3);
    }
    int32_t hay_len = strlen(hay);
    if (offset < 0) { offset = 0; }
    if (offset >= hay_len) { be_return_nil(vm); }      // any match of empty string returns nil, this catches implicitly when hay_len == 0
    hay += offset;                  // shift to offset

    if (be_isstring(vm, 1)) {
      const char * regex_str = be_tostring(vm, 1);
      int sz = re1_5_sizecode(regex_str);
      if (sz < 0) {
        be_raise(vm, "internal_error", "error in regex");
      }

      code = be_os_malloc(sizeof(ByteProg) + sz);
      if (code == NULL) {
        be_throw(vm, BE_MALLOC_FAIL);   /* lack of heap space */
      }
      int ret = re1_5_compilecode(code, regex_str);
      if (ret != 0) {
        be_os_free(code);
        be_raise(vm, "internal_error", "error in regex");
      }
    } else {
      code = (ByteProg *) be_tobytes(vm, 1, NULL);
    }
    // do the match
    be_re_match_search_run(vm, code, hay, is_anchored, size_only);
    // cleanup
    if (be_isstring(vm, 1)) {
      be_os_free(code);
    }
    be_return(vm);
  }
  be_raise(vm, "type_error", NULL);
}

int be_re_match_search_all(bvm *vm, bbool is_anchored) {
  int32_t argc = be_top(vm); // Get the number of arguments
  if (argc >= 2 && (be_isstring(vm, 1) || be_isbytes(vm, 1)) && be_isstring(vm, 2)) {
    const char * hay = be_tostring(vm, 2);
    ByteProg *code = NULL;
    int limit = -1;
    if (argc >= 3) {
      limit = be_toint(vm, 3);
    }

    if (be_isstring(vm, 1)) {
      const char * regex_str = be_tostring(vm, 1);
      int sz = re1_5_sizecode(regex_str);
      if (sz < 0) {
        be_raise(vm, "internal_error", "error in regex");
      }

      code = be_os_malloc(sizeof(ByteProg) + sz);
      if (code == NULL) {
        be_throw(vm, BE_MALLOC_FAIL);   /* lack of heap space */
      }
      int ret = re1_5_compilecode(code, regex_str);
      if (ret != 0) {
        be_os_free(code);
        be_raise(vm, "internal_error", "error in regex");
      }
    } else {
      code = (ByteProg *) be_tobytes(vm, 1, NULL);
    }

    be_newobject(vm, "list");
    for (int i = limit; i != 0 && hay != NULL; i--) {
      hay = be_re_match_search_run(vm, code, hay, is_anchored, bfalse);
      if (hay != NULL) {
        be_data_push(vm, -2);   // add sub list to list
      }
      be_pop(vm, 1);
    }
    be_pop(vm, 1);
    // cleanup
    if (be_isstring(vm, 1)) {
      be_os_free(code);
    }
    be_return(vm);
  }
  be_raise(vm, "type_error", NULL);
}

// Berry: `re.match(s:string [, offset:int]) -> nil`
int be_re_match(bvm *vm) {
  return be_re_match_search(vm, btrue, bfalse);
}
// Berry: `re.match2(s:string [, offset:int]) -> nil`
int be_re_match2(bvm *vm) {
  return be_re_match_search(vm, btrue, btrue);
}
// Berry: `re.search(s:string [, offset:int]) -> nil`
int be_re_search(bvm *vm) {
  return be_re_match_search(vm, bfalse, bfalse);
}

// Berry: `re.search_all`
int be_re_match_all(bvm *vm) {
  return be_re_match_search_all(vm, btrue);
}
// Berry: `re.search_all`
int be_re_search_all(bvm *vm) {
  return be_re_match_search_all(vm, bfalse);
}

// Berry: `re.dump(b:bytes) -> nil``
int be_re_dump(bvm *vm) {
#ifdef USE_BERRY_DEBUG
  int32_t argc = be_top(vm); // Get the number of arguments
  if (argc >= 1 && be_isbytes(vm, 1)) {
    ByteProg *code = (ByteProg*) be_tobytes(vm, 1, NULL);
    re1_5_dumpcode(code);
    be_return_nil(vm);
  }
  be_raise(vm, "type_error", NULL);
#else // USE_BERRY_DEBUG
  be_return_nil(vm);
#endif
}

// Berry: `re_pattern.search(s:string [, offset:int]) -> list(string)`
int re_pattern_search(bvm *vm) {
  int32_t argc = be_top(vm); // Get the number of arguments
  if (argc >= 2 && be_isstring(vm, 2)) {
    const char * hay = be_tostring(vm, 2);
    int32_t offset = 0;
    if (argc >= 3 && be_isint(vm, 3)) {
      offset = be_toint(vm, 3);
    }
    int32_t hay_len = strlen(hay);
    if (offset < 0) { offset = 0; }
    if (offset >= hay_len) { be_return_nil(vm); }      // any match of empty string returns nil, this catches implicitly when hay_len == 0
    hay += offset;                  // shift to offset
    be_getmember(vm, 1, "_p");
    ByteProg * code = (ByteProg*) be_tocomptr(vm, -1);
    be_re_match_search_run(vm, code, hay, bfalse, bfalse);
    be_return(vm);
  }
  be_raise(vm, "type_error", NULL);
}

// Berry: `re_pattern.searchall(s:string) -> list(list(string))`
int re_pattern_match_search_all(bvm *vm, bbool is_anchored) {
  int32_t argc = be_top(vm); // Get the number of arguments
  if (argc >= 2 && be_isstring(vm, 2)) {
    const char * hay = be_tostring(vm, 2);
    be_getmember(vm, 1, "_p");
    ByteProg * code = (ByteProg*) be_tocomptr(vm, -1);
    int limit = -1;
    if (argc >= 3) {
      limit = be_toint(vm, 3);
    }

    be_newobject(vm, "list");
    for (int i = limit; i != 0 && hay != NULL; i--) {
      hay = be_re_match_search_run(vm, code, hay, is_anchored, bfalse);
      if (hay != NULL) {
        be_data_push(vm, -2);   // add sub list to list
      }
      be_pop(vm, 1);
    }
    be_pop(vm, 1);
    be_return(vm);
  }
  be_raise(vm, "type_error", NULL);
}

// Berry: `re_pattern.searchall(s:string) -> list(list(string))`
int re_pattern_search_all(bvm *vm) {
  return re_pattern_match_search_all(vm, bfalse);
}

// Berry: `re_pattern.matchall(s:string) -> list(list(string))`
int re_pattern_match_all(bvm *vm) {
  return re_pattern_match_search_all(vm, btrue);
}

// Berry: `re_pattern.match(s:string [, offset:int]) -> list(string)`
int re_pattern_match_size(bvm *vm, bbool size_only) {
  int32_t argc = be_top(vm); // Get the number of arguments
  if (argc >= 2 && be_isstring(vm, 2)) {
    const char * hay = be_tostring(vm, 2);
    int32_t offset = 0;
    if (argc >= 3 && be_isint(vm, 3)) {
      offset = be_toint(vm, 3);
    }
    int32_t hay_len = strlen(hay);
    if (offset < 0) { offset = 0; }
    if (offset >= hay_len) { be_return_nil(vm); }      // any match of empty string returns nil, this catches implicitly when hay_len == 0
    hay += offset;                  // shift to offset
    be_getmember(vm, 1, "_p");
    ByteProg * code = (ByteProg*) be_tocomptr(vm, -1);
    be_re_match_search_run(vm, code, hay, btrue, size_only);
    be_return(vm);
  }
  be_raise(vm, "type_error", NULL);
}

// Berry: `re_pattern.match(s:string [, offset:int]) -> list(string)`
int re_pattern_match(bvm *vm) {
  return re_pattern_match_size(vm, bfalse);
}

// Berry: `re_pattern.match(s:string [, offset:int]) -> list(string)`
int re_pattern_match2(bvm *vm) {
  return re_pattern_match_size(vm, btrue);
}

int re_pattern_split_run(bvm *vm, ByteProg *code, const char *hay, int split_limit) {
  Subject subj = {hay, hay + strlen(hay)};

  int sub_els = (code->sub + 1) * 2;
  const char *sub[sub_els];

  be_newobject(vm, "list");
  while (1) {
    if (split_limit == 0 || !re1_5_recursiveloopprog(code, &subj, sub, sub_els, bfalse)) {
      be_pushnstring(vm, subj.begin, subj.end - subj.begin);
      be_data_push(vm, -2);
      be_pop(vm, 1);
      break;
    }

    if (sub[0] == NULL || sub[1] == NULL || sub[0] == sub[1]) {
      be_raise(vm, "internal_error", "can't match");
    }
    be_pushnstring(vm, subj.begin, sub[0] - subj.begin);
    be_data_push(vm, -2);
    be_pop(vm, 1);
    subj.begin = sub[1];
    split_limit--;
  }
  be_pop(vm, 1);    // remove list 
  be_return(vm);    // return list object
}

// Berry: `re_pattern.split(s:string [, split_limit:int]) -> list(string)`
int re_pattern_split(bvm *vm) {
  int32_t argc = be_top(vm); // Get the number of arguments
  if (argc >= 2 && be_isstring(vm, 2)) {
    int split_limit = -1;
    if (argc >= 3) {
      split_limit = be_toint(vm, 3);
    }
    const char * hay = be_tostring(vm, 2);
    be_getmember(vm, 1, "_p");
    ByteProg * code = (ByteProg*) be_tocomptr(vm, -1);

    return re_pattern_split_run(vm, code, hay, split_limit);
  }
  be_raise(vm, "type_error", NULL);
}

// Berry: `re.split(pattern:string, s:string [, split_limit:int]) -> list(string)`
int be_re_split(bvm *vm) {
  int32_t argc = be_top(vm); // Get the number of arguments
  if (argc >= 2 && (be_isstring(vm, 1) || be_isbytes(vm, 1)) && be_isstring(vm, 2)) {
    const char * hay = be_tostring(vm, 2);
    ByteProg *code = NULL;
    int split_limit = -1;
    if (argc >= 3) {
      split_limit = be_toint(vm, 3);
    }
    if (be_isstring(vm, 1)) {
      const char * regex_str = be_tostring(vm, 1);
      int sz = re1_5_sizecode(regex_str);
      if (sz < 0) {
        be_raise(vm, "internal_error", "error in regex");
      }

      code = be_os_malloc(sizeof(ByteProg) + sz);
      if (code == NULL) {
        be_throw(vm, BE_MALLOC_FAIL);   /* lack of heap space */
      }
      int ret = re1_5_compilecode(code, regex_str);
      if (ret != 0) {
        be_os_free(code);
        be_raise(vm, "internal_error", "error in regex");
      }
    } else {
      code = (ByteProg *) be_tobytes(vm, 1, NULL);
    }
    int ret = re_pattern_split_run(vm, code, hay, split_limit);
    if (be_isstring(vm, 1)) {
      be_os_free(code);
    }
    return ret;
  }
  be_raise(vm, "type_error", NULL);
}

#include "../generate/be_fixed_re.h"
#include "../generate/be_fixed_be_class_re_pattern.h"
/*
@const_object_info_begin
module re (scope: global) {
  compile, func(be_re_compile)
  compilebytes, func(be_re_compilebytes)
  search, func(be_re_search)
  searchall, func(be_re_search_all)
  match, func(be_re_match)
  match2, func(be_re_match2)
  matchall, func(be_re_match_all)
  split, func(be_re_split)
  dump, func(be_re_dump)
}
@const_object_info_end 

@const_object_info_begin
class be_class_re_pattern (scope: global, name: re_pattern) {
  _p, var
  search, func(re_pattern_search)
  searchall, func(re_pattern_search_all)
  match, func(re_pattern_match)
  match2, func(re_pattern_match2)
  matchall, func(re_pattern_match_all)
  split, func(re_pattern_split)
}
@const_object_info_end */
