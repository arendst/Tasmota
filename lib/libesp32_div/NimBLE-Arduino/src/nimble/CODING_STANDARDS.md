# Coding Style for Apache NimBLE

Apache NimBLE project is part of Apache Mynewt projct and follows its coding
style.

# Coding Style for Apache Mynewt Core

This document is meant to define the coding style for Apache Mynewt, and
all subprojects of Apache Mynewt.  This covers C and Assembly coding
conventions, *only*.  Other languages (such as Go), have their own
coding conventions.

## Headers

* All files that are newly written, should have the Apache License clause
at the top of them.

* For files that are copied from another source, but contain an Apache
compatible license, the original license header shall be maintained.

* For more information on applying the Apache license, the definitive
source is here: http://www.apache.org/dev/apply-license.html

* The Apache License clause for the top of files is as follows:

```no-highlight
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
```

## Whitespace and Braces

* Code must be indented to 4 spaces, tabs should not be used.

* Do not add whitespace at the end of a line.

* Put space after keywords (for, if, return, switch, while).

* for, else, if, while statements must have braces around their
code blocks, i.e., do:

```
    if (x) {
        assert(0);
    } else {
        assert(0);
    }
```

Not:

```
    if (x)
        assert(0);
    else
        assert(0);
```

* Braces for statements must be on the same line as the statement.  Good:

```
    for (i = 0; i < 10; i++) {
        if (i == 5) {
            break;
        } else {
            continue;
        }
    }
```

Not:

```
    for (i = 0; i < 10; i++)
    { <-- brace must be on same line as for
        if (i == 5) {
            break;
        } <-- no new line between else
        else {
            continue;
        }
    }
```

* After a function declaration, the braces should be on a newline, i.e. do:

```
    static void *
    function(int var1, int var2)
    {
```

not:

```
    static void *
    function(int var1, int var2) {
```

## Line Length and Wrap

* Line length should never exceed 79 columns.

* When you have to wrap a long statement, put the operator at the end of the
  line.  i.e.:

```
    if (x &&
        y == 10 &&
        b)
```

Not:

```
    if (x
        && y == 10
        && b)
```

## Comments

* No C++ style comments allowed.

* When using a single line comment, put it above the line of code that you
intend to comment, i.e., do:

```
    /* check variable */
    if (a) {
```

Not:

```
    if (a) { /* check variable */
```


* All public APIs should be commented with Doxygen style comments describing
purpose, parameters and return values.  Private APIs need not be documented.


## Header files

* Header files must contain the following structure:
    * Apache License (see above)
    * ```#ifdef``` aliasing, to prevent multiple includes
    * ```#include``` directives for other required header files
    * ```#ifdef __cplusplus``` wrappers to maintain C++ friendly APIs
    * Contents of the header file

* ```#ifdef``` aliasing, shall be in the following format, where
the package name is "os" and the file name is "callout.h":

```no-highlight
#ifndef _OS_CALLOUT_H
#define _OS_CALLOUT_H
```

* ```#include``` directives must happen prior to the cplusplus
wrapper.

* The cplusplus wrapper must have the following format, and precedes
any contents of the header file:

```no-highlight
#ifdef __cplusplus
#extern "C" {
##endif
```

## Naming

* Names of functions, structures and variables must be in all lowercase.

* Names should be as short as possible, but no shorter.

* Globally visible names must be prefixed with the name of the module,
followed by the '_' character, i.e.:

```
    os_callout_init(&c)
```

Not:

```
    callout_init(c)
```

## Functions

* No spaces after function names when calling a function, i.e, do:

```
    rc = function(a)
```

Not:

```
    rc = function (a)
```


* Arguments to function calls should have spaces between the comma, i.e. do:

```
    rc = function(a, b)
```

Not:

```
    rc = function(a,b)
```

* The function type must be on a line by itself preceding the function, i.e. do:

```
    static void *
    function(int var1, int var2)
    {
```

Not:

```
    static void *function(int var1, int var2)
    {
```

* In general, for functions that return values that denote success or error, 0
shall be success, and non-zero shall be the failure code.

## Variables and Macros

* Do not use typedefs for structures.  This makes it impossible for
applications to use pointers to those structures opaquely.

* typedef may be used for non-structure types, where it is beneficial to
hide or alias the underlying type used (e.g. ```os_time_t```.)   Indicate
typedefs by applying the ```_t``` marker to them.

* Place all function-local variable definitions at the top of the function body, before any statements.

## Compiler Directives

* Code must compile cleanly with -Wall enabled.

