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

/*
 * JSON String Generator
 *
 * This module can be used to create JSON strings with a facility
 * to flush out data if the destination buffer is full. All commas
 * and colons as required are automatically added by the APIs
 *
 */
#ifndef _JSON_GENERATOR_H
#define _JSON_GENERATOR_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C"
{
#endif

/** Float precision i.e. number of digits after decimal point */
#ifndef JSON_FLOAT_PRECISION
#define JSON_FLOAT_PRECISION 5
#endif

/** JSON string flush callback prototype
 *
 * This is a prototype of the function that needs to be passed to
 * json_gen_str_start() and which will be invoked by the JSON generator
 * module either when the buffer is full or json_gen_str_end() ins invoked.
 *
 * \param[in] buf Pointer to a NULL terminated JSON string
 * \param[in] priv Private data to be passed to the flush callback. Will
 * be the same as the one passed to json_gen_str_start()
 */
typedef void (*json_gen_flush_cb_t) (char *buf, void *priv);

/** JSON String structure
 *
 * Please do not set/modify any elements.
 * Just define this structure and pass a pointer to it in the APIs below
 */
typedef struct {
    /** Pointer to the JSON buffer provided by the calling function */
	char *buf;
    /** Size of the above buffer */
	int buf_size;
    /** (Optional) callback function to invoke when the buffer gets full */
	json_gen_flush_cb_t flush_cb;
    /** (Optional) Private data to pass to the callback function */
	void *priv;
    /** (For Internal use only) */
	bool comma_req;
    /** (For Internal use only) */
	char *free_ptr;
} json_gen_str_t;

/** Start a JSON String
 *
 * This is the first function to be called for creating a JSON string.
 * It initializes the internal data structures. After the JSON string
 * generation is over, the json_gen_str_end() function should be called.
 *
 * \param[out] jstr Pointer to an allocated \ref json_gen_str_t structure.
 * This will be initialised internally and needs to be passed to all
 * subsequent function calls
 * \param[out] buf Pointer to an allocated buffer into which the JSON
 * string will be written
 * \param[in] buf_size Size of the buffer
 * \param[in] flush_cb Pointer to the flushing function of type \ref json_gen_flush_cb_t
 * which will be invoked either when the buffer is full or when json_gen_str_end()
 * is invoked. Can be left NULL.
 * \param[in] priv Private data to be passed to the flushing function callback.
 * Can be something like a session identifier (Eg. socket). Can be left NULL.
 */
void json_gen_str_start(json_gen_str_t *jstr, char *buf, int buf_size,
		json_gen_flush_cb_t flush_cb, void *priv);

/** End JSON string
 *
 * This should be the last function to be called after the entire JSON string
 * has been generated.
 *
 * \param[in] jstr Pointer to the \ref json_gen_str_t structure initialised by
 * json_gen_str_start()
 */
void json_gen_str_end(json_gen_str_t *jstr);

/** Start a JSON object
 *
 * This starts a JSON object by adding a '{'
 *
 * \param[in] jstr Pointer to the \ref json_gen_str_t structure initialised by
 * json_gen_str_start()
 *
 * \return 0 on Success
 * \return -1 if buffer is out of space (possible only if no callback function
 * is passed to json_gen_str_start(). Else, buffer will be flushed out and new data
 * added after that
 */
int json_gen_start_object(json_gen_str_t *jstr);

/** End a JSON object
 *
 * This ends a JSON object by adding a '}'
 *
 * \param[in] jstr Pointer to the \ref json_gen_str_t structure initialised by
 * json_gen_str_start()
 *
 * \return 0 on Success
 * \return -1 if buffer is out of space (possible only if no callback function
 * is passed to json_gen_str_start(). Else, buffer will be flushed out and new data
 * added after that
 */
int json_gen_end_object(json_gen_str_t *jstr);

/** Start a JSON array
 *
 * This starts a JSON object by adding a '['
 *
 * \param[in] jstr Pointer to the \ref json_gen_str_t structure initialised by
 * json_gen_str_start()
 *
 * \return 0 on Success
 * \return -1 if buffer is out of space (possible only if no callback function
 * is passed to json_gen_str_start(). Else, buffer will be flushed out and new data
 * added after that
 */
int json_gen_start_array(json_gen_str_t *jstr);

/** End a JSON object
 *
 * This ends a JSON object by adding a ']'
 *
 * \param[in] jstr Pointer to the \ref json_gen_str_t structure initialised by
 * json_gen_str_start()
 *
 * \return 0 on Success
 * \return -1 if buffer is out of space (possible only if no callback function
 * is passed to json_gen_str_start(). Else, buffer will be flushed out and new data
 * added after that
 */
int json_gen_end_array(json_gen_str_t *jstr);

/** Push a named JSON object
 *
 * This adds a JSON object like "name":{
 *
 * \param[in] jstr Pointer to the \ref json_gen_str_t structure initialised by
 * json_gen_str_start()
 * \param[in] name Name of the object
 *
 * \return 0 on Success
 * \return -1 if buffer is out of space (possible only if no callback function
 * is passed to json_gen_str_start(). Else, buffer will be flushed out and new data
 * added after that
 */
int json_gen_push_object(json_gen_str_t *jstr, char *name);

/** Pop a named JSON object
 *
 * This ends a JSON object by adding a '}'. This is basically same as
 * json_gen_end_object() but included so as to complement json_gen_push_object()
 *
 * \param[in] jstr Pointer to the \ref json_gen_str_t structure initialised by
 * json_gen_str_start()
 *
 * \return 0 on Success
 * \return -1 if buffer is out of space (possible only if no callback function
 * is passed to json_gen_str_start(). Else, buffer will be flushed out and new data
 * added after that
 */
int json_gen_pop_object(json_gen_str_t *jstr);

/** Push a JSON object string
 *
 * This adds a complete pre-formatted JSON object string to the JSON object.
 *
 * Eg. json_gen_push_object_str(jstr, "pre-formatted", "{\"a\":1,\"b\":2}");
 * This will add "pre-formatted":{"a":1,"b":2}
 *
 * \param[in] jstr Pointer to the \ref json_gen_str_t structure initialised by
 * json_gen_str_start()
 * \param[in] name Name of the JSON object string
 * \param[in] object_str The pre-formatted JSON object string
 *
 * \return 0 on Success
 * \return -1 if buffer is out of space (possible only if no callback function
 * is passed to json_gen_str_start(). Else, buffer will be flushed out and new data
 * added after that.
 */
int json_gen_push_object_str(json_gen_str_t *jstr, char *name, char *object_str);

/** Push a named JSON array
 *
 * This adds a JSON array like "name":[
 *
 * \param[in] jstr Pointer to the \ref json_gen_str_t structure initialised by
 * json_gen_str_start()
 * \param[in] name Name of the array
 *
 * \return 0 on Success
 * \return -1 if buffer is out of space (possible only if no callback function
 * is passed to json_gen_str_start(). Else, buffer will be flushed out and new data
 * added after that
 */
int json_gen_push_array(json_gen_str_t *jstr, char *name);

/** Pop a named JSON array
 *
 * This ends a JSON array by adding a ']'. This is basically same as
 * json_gen_end_array() but included so as to complement json_gen_push_array()
 *
 * \param[in] jstr Pointer to the \ref json_gen_str_t structure initialised by
 * json_gen_str_start()
 *
 * \return 0 on Success
 * \return -1 if buffer is out of space (possible only if no callback function
 * is passed to json_gen_str_start(). Else, buffer will be flushed out and new data
 * added after that
 */
int json_gen_pop_array(json_gen_str_t *jstr);

/** Push a JSON array string
 *
 * This adds a complete pre-formatted JSON array string to the JSON object.
 *
 * Eg. json_gen_push_object_str(jstr, "pre-formatted", "[1,2,3]");
 * This will add "pre-formatted":[1,2,3]
 *
 * \param[in] jstr Pointer to the \ref json_gen_str_t structure initialised by
 * json_gen_str_start()
 * \param[in] name Name of the JSON array string
 * \param[in] array_str The pre-formatted JSON array string
 *
 * \return 0 on Success
 * \return -1 if buffer is out of space (possible only if no callback function
 * is passed to json_gen_str_start(). Else, buffer will be flushed out and new data
 * added after that.
 */
int json_gen_push_array_str(json_gen_str_t *jstr, char *name, char *array_str);

/** Add a boolean element to an object
 *
 * This adds a boolean element to an object. Eg. "bool_val":true
 *
 * \note This must be called between json_gen_start_object()/json_gen_push_object()
 * and json_gen_end_object()/json_gen_pop_object()
 *
 * \param[in] jstr Pointer to the \ref json_gen_str_t structure initialised by
 * json_gen_str_start()
 * \param[in] name Name of the element
 * \param[in] val Boolean value of the element
 *
 * \return 0 on Success
 * \return -1 if buffer is out of space (possible only if no callback function
 * is passed to json_gen_str_start(). Else, buffer will be flushed out and new data
 * added after that
 */
int json_gen_obj_set_bool(json_gen_str_t *jstr, char *name, bool val);

/** Add an integer element to an object
 *
 * This adds an integer element to an object. Eg. "int_val":28
 *
 * \note This must be called between json_gen_start_object()/json_gen_push_object()
 * and json_gen_end_object()/json_gen_pop_object()
 *
 * \param[in] jstr Pointer to the \ref json_gen_str_t structure initialised by
 * json_gen_str_start()
 * \param[in] name Name of the element
 * \param[in] val Integer value of the element
 *
 * \return 0 on Success
 * \return -1 if buffer is out of space (possible only if no callback function
 * is passed to json_gen_str_start(). Else, buffer will be flushed out and new data
 * added after that
 */
int json_gen_obj_set_int(json_gen_str_t *jstr, char *name, int val);

/** Add a float element to an object
 *
 * This adds a float element to an object. Eg. "float_val":23.8
 *
 * \note This must be called between json_gen_start_object()/json_gen_push_object()
 * and json_gen_end_object()/json_gen_pop_object()
 *
 * \param[in] jstr Pointer to the \ref json_gen_str_t structure initialised by
 * json_gen_str_start()
 * \param[in] name Name of the element
 * \param[in] val Float value of the element
 *
 * \return 0 on Success
 * \return -1 if buffer is out of space (possible only if no callback function
 * is passed to json_gen_str_start(). Else, buffer will be flushed out and new data
 * added after that
 */
int json_gen_obj_set_float(json_gen_str_t *jstr, char *name, float val);

/** Add a string element to an object
 *
 * This adds a string element to an object. Eg. "string_val":"my_string"
 *
 * \note This must be called between json_gen_start_object()/json_gen_push_object()
 * and json_gen_end_object()/json_gen_pop_object()
 *
 * \param[in] jstr Pointer to the \ref json_gen_str_t structure initialised by
 * json_gen_str_start()
 * \param[in] name Name of the element
 * \param[in] val Null terminated string value of the element
 *
 * \return 0 on Success
 * \return -1 if buffer is out of space (possible only if no callback function
 * is passed to json_gen_str_start(). Else, buffer will be flushed out and new data
 * added after that
 */
int json_gen_obj_set_string(json_gen_str_t *jstr, char *name, char *val);

/** Add a NULL element to an object
 *
 * This adds a NULL element to an object. Eg. "null_val":null
 *
 * \note This must be called between json_gen_start_object()/json_gen_push_object()
 * and json_gen_end_object()/json_gen_pop_object()
 *
 * \param[in] jstr Pointer to the \ref json_gen_str_t structure initialised by
 * json_gen_str_start()
 * \param[in] name Name of the element
 *
 * \return 0 on Success
 * \return -1 if buffer is out of space (possible only if no callback function
 * is passed to json_gen_str_start(). Else, buffer will be flushed out and new data
 * added after that
 */
int json_gen_obj_set_null(json_gen_str_t *jstr, char *name);

/** Add a boolean element to an array
 *
 * \note This must be called between json_gen_start_array()/json_gen_push_array()
 * and json_gen_end_array()/json_gen_pop_array()
 *
 * \param[in] jstr Pointer to the \ref json_gen_str_t structure initialised by
 * json_gen_str_start()
 * \param[in] val Boolean value of the element
 *
 * \return 0 on Success
 * \return -1 if buffer is out of space (possible only if no callback function
 * is passed to json_gen_str_start(). Else, buffer will be flushed out and new data
 * added after that
 */
int json_gen_arr_set_bool(json_gen_str_t *jstr, bool val);

/** Add an integer element to an array
 *
 * \note This must be called between json_gen_start_array()/json_gen_push_array()
 * and json_gen_end_array()/json_gen_pop_array()
 *
 * \param[in] jstr Pointer to the \ref json_gen_str_t structure initialised by
 * json_gen_str_start()
 * \param[in] val Integer value of the element
 *
 * \return 0 on Success
 * \return -1 if buffer is out of space (possible only if no callback function
 * is passed to json_gen_str_start(). Else, buffer will be flushed out and new data
 * added after that
 */
int json_gen_arr_set_int(json_gen_str_t *jstr, int val);

/** Add a float element to an array
 *
 * \note This must be called between json_gen_start_array()/json_gen_push_array()
 * and json_gen_end_array()/json_gen_pop_array()
 *
 * \param[in] jstr Pointer to the \ref json_gen_str_t structure initialised by
 * json_gen_str_start()
 * \param[in] val Float value of the element
 *
 * \return 0 on Success
 * \return -1 if buffer is out of space (possible only if no callback function
 * is passed to json_gen_str_start(). Else, buffer will be flushed out and new data
 * added after that
 */
int json_gen_arr_set_float(json_gen_str_t *jstr, float val);

/** Add a string element to an array
 *
 * \note This must be called between json_gen_start_array()/json_gen_push_array()
 * and json_gen_end_array()/json_gen_pop_array()
 *
 * \param[in] jstr Pointer to the \ref json_gen_str_t structure initialised by
 * json_gen_str_start()
 * \param[in] val Null terminated string value of the element
 *
 * \return 0 on Success
 * \return -1 if buffer is out of space (possible only if no callback function
 * is passed to json_gen_str_start(). Else, buffer will be flushed out and new data
 * added after that
 */
int json_gen_arr_set_string(json_gen_str_t *jstr, char *val);

/** Add a NULL element to an array
 *
 * \note This must be called between json_gen_start_array()/json_gen_push_array()
 * and json_gen_end_array()/json_gen_pop_array()
 *
 * \param[in] jstr Pointer to the \ref json_gen_str_t structure initialised by
 * json_gen_str_start()
 *
 * \return 0 on Success
 * \return -1 if buffer is out of space (possible only if no callback function
 * is passed to json_gen_str_start(). Else, buffer will be flushed out and new data
 * added after that
 */
int json_gen_arr_set_null(json_gen_str_t *jstr);

/** Start a Long string in an object
 *
 * This starts a string in an object, but does not end it (i.e., does not add the
 * terminating quotes. This is useful for long strings. Eg. "string_val":"my_string.
 * The API json_gen_add_to_long_string() must be used to add to this string and the API
 * json_gen_end_long_string() must be used to terminate it (i.e. add the ending quotes).
 *
 * \note This must be called between json_gen_start_object()/json_gen_push_object()
 * and json_gen_end_object()/json_gen_pop_object()
 *
 * \param[in] jstr Pointer to the \ref json_gen_str_t structure initialised by
 * json_gen_str_start()
 * \param[in] name Name of the element
 * \param[in] val Null terminated initial part of the string value. It can also be NULL
 *
 * \return 0 on Success
 * \return -1 if buffer is out of space (possible only if no callback function
 * is passed to json_gen_str_start(). Else, buffer will be flushed out and new data
 * added after that
 */
int json_gen_obj_start_long_string(json_gen_str_t *jstr, char *name, char *val);

/** Start a Long string in an array
 *
 * This starts a string in an arrayt, but does not end it (i.e., does not add the
 * terminating quotes. This is useful for long strings.
 * The API json_gen_add_to_long_string() must be used to add to this string and the API
 * json_gen_end_long_string() must be used to terminate it (i.e. add the ending quotes).
 *
 * \note This must be called between json_gen_start_array()/json_gen_push_array()
 * and json_gen_end_array()/json_gen_pop_array()
 *
 * \param[in] jstr Pointer to the \ref json_gen_str_t structure initialised by
 * json_gen_str_start()
 * \param[in] val Null terminated initial part of the string value. It can also be NULL
 *
 * \return 0 on Success
 * \return -1 if buffer is out of space (possible only if no callback function
 * is passed to json_gen_str_start(). Else, buffer will be flushed out and new data
 * added after that
 */
int json_gen_arr_start_long_string(json_gen_str_t *jstr, char *val);

/** Add to a JSON Long string
 *
 * This extends the string initialised by json_gen_obj_start_long_string() or
 * json_gen_arr_start_long_string(). After the entire string is created, it should be terminated
 * with json_gen_end_long_string().
 *
 * \param[in] jstr Pointer to the \ref json_gen_str_t structure initialised by json_gen_str_start()
 * \param[in] val Null terminated extending part of the string value.
 *
 * \return 0 on Success
 * \return -1 if buffer is out of space (possible only if no callback function
 * is passed to json_gen_str_start(). Else, buffer will be flushed out and new data
 * added after that
 */
int json_gen_add_to_long_string(json_gen_str_t *jstr, char *val);

/** End a JSON Long string
 *
 * This ends the string initialised by json_gen_obj_start_long_string() or
 * json_gen_arr_start_long_string() by adding the ending quotes.
 *
 * \param[in] jstr Pointer to the \ref json_gen_str_t structure initialised by json_gen_str_start()
 *
 *
 * \return 0 on Success
 * \return -1 if buffer is out of space (possible only if no callback function
 * is passed to json_gen_str_start(). Else, buffer will be flushed out and new data
 * added after that
 */
int json_gen_end_long_string(json_gen_str_t *jstr);
#ifdef __cplusplus
}
#endif
#endif
