/*
 * ESPRESSIF MIT License
 *
 * Copyright (c) 2020 <ESPRESSIF SYSTEMS (SHANGHAI) PTE LTD>
 *
 * Permission is hereby granted for use on ESPRESSIF SYSTEMS products only, in which case,
 * it is free of charge, to any person obtaining a copy of this software and associated
 * documentation files (the "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the Software is furnished
 * to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 */
#ifndef _HAP_BCT_H_
#define _HAP_BCT_H_

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Change the name of the Bonjour Service for BCT
 *
 * This is required as per the "Manual Name Change" step of "Tester Interaction"
 * section of Bonjour Conformance Test.
 *
 * @param[in] name The desired new name for the service. For BCT 1.4 or earlier: "New - Bonjour Service Name".
 * For BCT 1.5 or later: "New-BCT"
 */
void hap_bct_change_name(const char *name);

/**
 * @brief Trigger a Hot plug of the network interface for BCT
 *
 * This is required as per the "Cable Change Handling" and "Hot Plugging" steps
 * of "Tester Interaction" section of Bonjout Conformance Test
 */
void hap_bct_hot_plug();

#ifdef __cplusplus
}
#endif

#endif /* _HAP_BCT_H_ */
