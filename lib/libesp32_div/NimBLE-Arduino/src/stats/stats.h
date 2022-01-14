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

#ifndef __STATS_H__
#define __STATS_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define STATS_SECT_DECL(__name)         struct stats_ ## __name
#define STATS_SECT_END                  };

#define STATS_SECT_START(__name)        STATS_SECT_DECL(__name) {
#define STATS_SECT_VAR(__var)

#define STATS_HDR(__sectname)           NULL

#define STATS_SECT_ENTRY(__var)
#define STATS_SECT_ENTRY16(__var)
#define STATS_SECT_ENTRY32(__var)
#define STATS_SECT_ENTRY64(__var)
#define STATS_RESET(__var)

#define STATS_SIZE_INIT_PARMS(__sectvarname, __size) \
                                        0, 0

#define STATS_INC(__sectvarname, __var)
#define STATS_INCN(__sectvarname, __var, __n)
#define STATS_CLEAR(__sectvarname, __var)

#define STATS_NAME_START(__name)
#define STATS_NAME(__name, __entry)
#define STATS_NAME_END(__name)
#define STATS_NAME_INIT_PARMS(__name)   NULL, 0

static inline int
stats_init(void *a, uint8_t b, uint8_t c, void *d, uint8_t e)
{
    /* dummy */
    return 0;
}

static inline int
stats_register(void *a, void *b)
{
    /* dummy */
    return 0;
}

static inline int
stats_init_and_reg(void *a, uint8_t b, uint8_t c, void *d, uint8_t e, const char *f)
{
    /* dummy */
    return 0;
}

#ifdef __cplusplus
}
#endif

#endif /* __STATS_H__ */
