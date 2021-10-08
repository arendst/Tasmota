/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

#ifndef H_OS_UTIL_
#define H_OS_UTIL_

/* Helpers to pass integers as pointers and vice-versa */
#define POINTER_TO_UINT(p) ((unsigned int) ((uintptr_t) (p)))
#define UINT_TO_POINTER(u) ((void *) ((uintptr_t) (u)))
#define POINTER_TO_INT(p) ((int) ((intptr_t) (p)))
#define INT_TO_POINTER(u) ((void *) ((intptr_t) (u)))

/* Helper to retrieve pointer to "parent" object in structure */
#define CONTAINER_OF(ptr, type, field) \
        ((type *)(((char *)(ptr)) - offsetof(type, field)))

/* Helper to calculate number of elements in array */
#ifndef ARRAY_SIZE
#define ARRAY_SIZE(array) \
        (sizeof(array) / sizeof((array)[0]))
#endif
#endif
