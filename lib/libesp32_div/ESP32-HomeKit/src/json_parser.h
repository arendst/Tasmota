/*
 *    Copyright 2020 Piyush Shah <shahpiyushv@gmail.com>
 *
 *   Licensed under the Apache License, Version 2.0 (the "License");
 *   you may not use this file except in compliance with the License.
 *   You may obtain a copy of the License at
 *
 *       http://www.apache.org/licenses/LICENSE-2.0
 *
 *   Unless required by applicable law or agreed to in writing, software
 *   distributed under the License is distributed on an "AS IS" BASIS,
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *   See the License for the specific language governing permissions and
 *   limitations under the License.
 */
#ifndef _JSON_PARSER_H_
#define _JSON_PARSER_H_

#define JSMN_HEADER
#include <jsmn/jsmn.h>
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C"
{
#endif

#define OS_SUCCESS  0
#define OS_FAIL     -1

typedef jsmn_parser json_parser_t;
typedef jsmntok_t json_tok_t;

typedef struct {
	json_parser_t parser;
	char *js;
	json_tok_t *tokens;
	json_tok_t *cur;
	int num_tokens;
} jparse_ctx_t;

int json_parse_start(jparse_ctx_t *jctx, char *js, int len);
int json_parse_end(jparse_ctx_t *jctx);

int json_obj_get_array(jparse_ctx_t *jctx, char *name, int *num_elem);
int json_obj_leave_array(jparse_ctx_t *jctx);
int json_obj_get_object(jparse_ctx_t *jctx, char *name);
int json_obj_leave_object(jparse_ctx_t *jctx);
int json_obj_get_bool(jparse_ctx_t *jctx, char *name, bool *val);
int json_obj_get_int(jparse_ctx_t *jctx, char *name, int *val);
int json_obj_get_int64(jparse_ctx_t *jctx, char *name, int64_t *val);
int json_obj_get_float(jparse_ctx_t *jctx, char *name, float *val);
int json_obj_get_string(jparse_ctx_t *jctx, char *name, char *val, int size);
int json_obj_get_strlen(jparse_ctx_t *jctx, char *name, int *strlen);
int json_obj_get_object_str(jparse_ctx_t *jctx, char *name, char *val, int size);
int json_obj_get_object_strlen(jparse_ctx_t *jctx, char *name, int *strlen);
int json_obj_get_array_str(jparse_ctx_t *jctx, char *name, char *val, int size);
int json_obj_get_array_strlen(jparse_ctx_t *jctx, char *name, int *strlen);

int json_arr_get_array(jparse_ctx_t *jctx, uint32_t index);
int json_arr_leave_array(jparse_ctx_t *jctx);
int json_arr_get_object(jparse_ctx_t *jctx, uint32_t index);
int json_arr_leave_object(jparse_ctx_t *jctx);
int json_arr_get_bool(jparse_ctx_t *jctx, uint32_t index, bool *val);
int json_arr_get_int(jparse_ctx_t *jctx, uint32_t index, int *val);
int json_arr_get_int64(jparse_ctx_t *jctx, uint32_t index, int64_t *val);
int json_arr_get_float(jparse_ctx_t *jctx, uint32_t index, float *val);
int json_arr_get_string(jparse_ctx_t *jctx, uint32_t index, char *val, int size);
int json_arr_get_strlen(jparse_ctx_t *jctx, uint32_t index, int *strlen);

#ifdef __cplusplus
}
#endif

#endif /* _JSON_PARSER_H_ */
