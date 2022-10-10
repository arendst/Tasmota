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

// pushes either a list if matched, else `nil`
// return index of next offset, or -1 if not found
const char *be_re_match_search_run(bvm *vm, ByteProg *code, const char *hay, bbool is_anchored) {
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
    be_pushnstring(vm, sub[i], sub[i+1] - sub[i]);
    }
    be_data_push(vm, -2);
    be_pop(vm, 1);
  }
  be_pop(vm, 1);    // remove list
  return sub[1];
}

int be_re_match_search(bvm *vm, bbool is_anchored) {
  int32_t argc = be_top(vm); // Get the number of arguments
  if (argc >= 2 && be_isstring(vm, 1) && be_isstring(vm, 2)) {
    const char * regex_str = be_tostring(vm, 1);
    const char * hay = be_tostring(vm, 2);
	  int sz = re1_5_sizecode(regex_str);
  	if (sz < 0) {
      be_raise(vm, "internal_error", "error in regex");
	  }

    ByteProg *code = be_os_malloc(sizeof(ByteProg) + sz);
    int ret = re1_5_compilecode(code, regex_str);
    if (ret != 0) {
      be_raise(vm, "internal_error", "error in regex");
    }
    be_re_match_search_run(vm, code, hay, is_anchored);
    be_return(vm);
  }
  be_raise(vm, "type_error", NULL);
}

int be_re_match_search_all(bvm *vm, bbool is_anchored) {
  int32_t argc = be_top(vm); // Get the number of arguments
  if (argc >= 2 && be_isstring(vm, 1) && be_isstring(vm, 2)) {
    const char * regex_str = be_tostring(vm, 1);
    const char * hay = be_tostring(vm, 2);
    int limit = -1;
    if (argc >= 3) {
      limit = be_toint(vm, 3);
    }
	  int sz = re1_5_sizecode(regex_str);
  	if (sz < 0) {
      be_raise(vm, "internal_error", "error in regex");
	  }

    ByteProg *code = be_os_malloc(sizeof(ByteProg) + sz);
    int ret = re1_5_compilecode(code, regex_str);
    if (ret != 0) {
      be_raise(vm, "internal_error", "error in regex");
    }

    be_newobject(vm, "list");
    for (int i = limit; i != 0 && hay != NULL; i--) {
      hay = be_re_match_search_run(vm, code, hay, is_anchored);
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

// Berry: `re.match(value:int | s:string) -> nil`
int be_re_match(bvm *vm) {
  return be_re_match_search(vm, btrue);
}
// Berry: `re.search(value:int | s:string) -> nil`
int be_re_search(bvm *vm) {
  return be_re_match_search(vm, bfalse);
}

// Berry: `re.search_all`
int be_re_match_all(bvm *vm) {
  return be_re_match_search_all(vm, btrue);
}
// Berry: `re.search_all`
int be_re_search_all(bvm *vm) {
  return be_re_match_search_all(vm, bfalse);
}

// Berry: `re_pattern.search(s:string) -> list(string)`
int re_pattern_search(bvm *vm) {
  int32_t argc = be_top(vm); // Get the number of arguments
  if (argc >= 2 && be_isstring(vm, 2)) {
    const char * hay = be_tostring(vm, 2);
    be_getmember(vm, 1, "_p");
    ByteProg * code = (ByteProg*) be_tocomptr(vm, -1);
    be_re_match_search_run(vm, code, hay, bfalse);
    be_return(vm);
  }
  be_raise(vm, "type_error", NULL);
}

// Berry: `re_pattern.match(s:string) -> list(string)`
int re_pattern_match(bvm *vm) {
  int32_t argc = be_top(vm); // Get the number of arguments
  if (argc >= 2 && be_isstring(vm, 2)) {
    const char * hay = be_tostring(vm, 2);
    be_getmember(vm, 1, "_p");
    ByteProg * code = (ByteProg*) be_tocomptr(vm, -1);
    be_re_match_search_run(vm, code, hay, btrue);
    be_return(vm);
  }
  be_raise(vm, "type_error", NULL);
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
  if (argc >= 2 && be_isstring(vm, 1) && be_isstring(vm, 2)) {
    const char * regex_str = be_tostring(vm, 1);
    const char * hay = be_tostring(vm, 2);
    int split_limit = -1;
    if (argc >= 3) {
      split_limit = be_toint(vm, 3);
    }
	  int sz = re1_5_sizecode(regex_str);
  	if (sz < 0) {
      be_raise(vm, "internal_error", "error in regex");
	  }

    ByteProg *code = be_os_malloc(sizeof(ByteProg) + sz);
    int ret = re1_5_compilecode(code, regex_str);
    if (ret != 0) {
      be_raise(vm, "internal_error", "error in regex");
    }
    return re_pattern_split_run(vm, code, hay, split_limit);
  }
  be_raise(vm, "type_error", NULL);
}

#include "../generate/be_fixed_re.h"
#include "../generate/be_fixed_be_class_re_pattern.h"
/*
@const_object_info_begin
module re (scope: global) {
  compile, func(be_re_compile)
  search, func(be_re_search)
  searchall, func(be_re_search_all)
  match, func(be_re_match)
  matchall, func(be_re_match_all)
  split, func(be_re_split)
}
@const_object_info_end 

@const_object_info_begin
class be_class_re_pattern (scope: global, name: re_pattern) {
  _p, var
  search, func(re_pattern_search)
  match, func(re_pattern_match)
  split, func(re_pattern_split)
}
@const_object_info_end */
