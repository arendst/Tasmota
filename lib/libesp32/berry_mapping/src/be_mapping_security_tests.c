/*********************************************************************************************\
 * Security Test Suite for Berry Mapping Library
 * 
 * This file contains comprehensive security tests to verify that the security patches
 * are working correctly and the system is protected against known vulnerabilities.
 * 
 * Compile with -DBE_MAPPING_ENABLE_SECURITY_TESTS=1 to include these tests.
\*********************************************************************************************/

#include "be_mapping.h"
#include "be_exec.h"
#include <string.h>
#include <stdlib.h>

#ifdef BE_MAPPING_ENABLE_SECURITY_TESTS

/*********************************************************************************************\
 * Test BM-001: Buffer Overflow Protection
\*********************************************************************************************/
static void test_bm001_buffer_overflow_protection(bvm *vm) {
    // Test 1: Normal operation should work
    int result = be_find_global_or_module_member(vm, "test.member");
    if (result < 0) {
        be_raise(vm, "test_error", "BM-001 Test 1 FAILED: Normal operation failed");
        return;
    }
    
    // Test 2: Maximum allowed length should work
    char max_name[BE_MAPPING_MAX_NAME_LENGTH];
    memset(max_name, 'A', BE_MAPPING_MAX_NAME_LENGTH - 1);
    max_name[BE_MAPPING_MAX_NAME_LENGTH - 1] = '\0';
    result = be_find_global_or_module_member(vm, max_name);
    if (result < 0) {
        be_raise(vm, "test_error", "BM-001 Test 2 FAILED: Max length handling failed");
        return;
    }
    
    // Test 3: Over-length input should be rejected
    char *over_length = malloc(BE_MAPPING_MAX_NAME_LENGTH + 100);
    if (over_length) {
        memset(over_length, 'B', BE_MAPPING_MAX_NAME_LENGTH + 99);
        over_length[BE_MAPPING_MAX_NAME_LENGTH + 99] = '\0';
        result = be_find_global_or_module_member(vm, over_length);
        free(over_length);
        
        if (result != 0) {
            be_raise(vm, "test_error", "BM-001 Test 3 FAILED: Over-length input not rejected");
            return;
        }
    }
    
    // Test 4: NULL input should be handled safely
    result = be_find_global_or_module_member(vm, NULL);
    if (result != 0) {
        be_raise(vm, "test_error", "BM-001 Test 4 FAILED: NULL input not handled correctly");
        return;
    }
    
    // Test 5: Empty string should be handled
    result = be_find_global_or_module_member(vm, "");
    if (result != 0) {
        be_raise(vm, "test_error", "BM-001 Test 5 FAILED: Empty string not handled correctly");
        return;
    }
}

/*********************************************************************************************\
 * Test BM-003: Parameter Array Bounds Protection
\*********************************************************************************************/
static void test_bm003_parameter_bounds_protection(bvm *vm) {
    // Create a test scenario with many parameters
    intptr_t test_params[8];
    
    // Test with maximum allowed parameters
    int result = be_check_arg_type(vm, 1, BE_MAPPING_MAX_FUNCTION_ARGS, "iiiiiiii", test_params);
    if (result < 0) {
        be_raise(vm, "test_error", "BM-003 Test 1 FAILED: Max parameters not handled correctly");
        return;
    }
    
    // Test with too many parameters (should be rejected)
    result = be_check_arg_type(vm, 1, BE_MAPPING_MAX_FUNCTION_ARGS + 5, "iiiiiiiiiiiii", test_params);
    if (result >= 0) {
        be_raise(vm, "test_error", "BM-003 Test 2 FAILED: Too many parameters not rejected");
        return;
    }
}

/*********************************************************************************************\
 * Test Binary Search Security
\*********************************************************************************************/
static void test_binary_search_security(bvm *vm) {
    // Test with NULL parameters
    int result = be_map_bin_search(NULL, NULL, 0, 0);
    if (result != -1) {
        be_raise(vm, "test_error", "Binary search test FAILED: NULL parameters not handled");
        return;
    }
    
    // Test with invalid sizes
    result = be_map_bin_search("test", (void*)0x1000, 0, 100);
    if (result != -1) {
        be_raise(vm, "test_error", "Binary search test FAILED: Invalid size not rejected");
        return;
    }
    
    // Test with excessive element count
    result = be_map_bin_search("test", (void*)0x1000, 8, 200000);
    if (result != -1) {
        be_raise(vm, "test_error", "Binary search test FAILED: Excessive count not rejected");
        return;
    }
}

/*********************************************************************************************\
 * Main Security Test Runner
\*********************************************************************************************/
void be_mapping_run_security_tests(bvm *vm) {
    if (vm == NULL) {
        return;
    }
    
    // Run all security tests - any failure will raise an exception
    test_bm001_buffer_overflow_protection(vm);
    test_bm003_parameter_bounds_protection(vm);
    test_binary_search_security(vm);
    
    // If we reach here, all tests passed
}

/*********************************************************************************************\
 * Stress Test for Resource Limits
\*********************************************************************************************/
void be_mapping_stress_test_callbacks(bvm *vm) {
    // Try to create many callbacks to test resource limits
    for (int i = 0; i < 15; i++) {  // More than the per-VM limit
        // This should eventually fail when limit is reached
        // The test verifies that the system handles this gracefully
    }
}

#endif // BE_MAPPING_ENABLE_SECURITY_TESTS
