// Copyright 2017 David Conran

#ifndef TEST_IRRECV_TEST_H_
#define TEST_IRRECV_TEST_H_

#include <iostream>
#include <sstream>
#include <string>
#include "IRutils.h"

#define EXPECT_STATE_EQ(a, b, c)                \
  for (uint8_t i = 0; i < c / 8; ++i) {         \
    EXPECT_EQ(a[i], b[i]) << "Expected state "  \
                             "differs at i = "  \
                          << uint64ToString(i); \
  }
#endif  // TEST_IRRECV_TEST_H_
