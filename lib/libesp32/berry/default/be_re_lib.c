/********************************************************************
 * Tasmota lib
 * 
 * To use: `import re`
 * 
 * Regex using re1.5
 *******************************************************************/
#include "be_constobj.h"
#include "be_mem.h"
#include "re1.5.h"

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

int be_free_comobj(bvm* vm) {
  int argc = be_top(vm);
  if (argc > 0) {
    void * obj = be_tocomptr(vm, 1);
    if (obj != NULL) { be_os_free(obj); }
  }
  be_return_nil(vm);
}

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
    be_newcomobj(vm, code, &be_free_comobj);
    be_setmember(vm, -2, "_p");
    be_pop(vm, 1);
    be_return(vm);
  }
  be_raise(vm, "type_error", NULL);
}


int be_re_match_search_run(bvm *vm, ByteProg *code, const char *hay, bbool is_anchored) {
  Subject subj = {hay, hay + strlen(hay)};

  int sub_els = (code->sub + 1) * 2;
  const char *sub[sub_els];

  if (!re1_5_recursiveloopprog(code, &subj, sub, sub_els, is_anchored)) {
    be_return_nil(vm);    // no match
  }

  be_newobject(vm, "list");
  int k;
  for(k = sub_els; k > 0; k--)
    if(sub[k-1])
      break;
  for (int i = 0; i < k; i += 2) {
    be_pushnstring(vm, sub[i], sub[i+1] - sub[i]);
    be_data_push(vm, -2);
    be_pop(vm, 1);
  }
  be_pop(vm, 1);    // remove list 
  be_return(vm);    // return list object
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
    return be_re_match_search_run(vm, code, hay, is_anchored);
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

// Berry: `re_pattern.search(s:string) -> list(string)`
int re_pattern_search(bvm *vm) {
  int32_t argc = be_top(vm); // Get the number of arguments
  if (argc >= 2 && be_isstring(vm, 2)) {
    const char * hay = be_tostring(vm, 2);
    be_getmember(vm, 1, "_p");
    ByteProg * code = (ByteProg*) be_tocomptr(vm, -1);
    return be_re_match_search_run(vm, code, hay, bfalse);
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
    return be_re_match_search_run(vm, code, hay, btrue);
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

/********************************************************************
** Solidified module: re
********************************************************************/
be_local_module(re,
    "re",
    be_nested_map(4,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_nested_key("compile", 1000265118, 7, -1), be_const_func(be_re_compile) },
        { be_nested_key("search", -2144130903, 6, -1), be_const_func(be_re_search) },
        { be_nested_key("match", 2116038550, 5, 0), be_const_func(be_re_match) },
        { be_nested_key("split", -2017972765, 5, -1), be_const_func(be_re_split) },
    }))
);
BE_EXPORT_VARIABLE be_define_const_native_module(re);
/********************************************************************/

// ===================================================================

/********************************************************************
** Solidified class: re_pattern
********************************************************************/
be_local_class(re_pattern,
    1,
    NULL,
    be_nested_map(4,
    ( (struct bmapnode*) &(const bmapnode[]) {
        { be_nested_key("_p", 1594591802, 2, -1), be_const_var(0) },
        { be_nested_key("search", -2144130903, 6, -1), be_const_func(re_pattern_search) },
        { be_nested_key("match", 2116038550, 5, 0), be_const_func(re_pattern_match) },
        { be_nested_key("split", -2017972765, 5, -1), be_const_func(re_pattern_split) },
    })),
    (be_nested_const_str("re_pattern", 2041968961, 10))
);
/*******************************************************************/

