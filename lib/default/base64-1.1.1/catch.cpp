#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "base64.hpp"

TEST_CASE("encode_base64_length()", "[]") {
  SECTION("Zero length") {
    REQUIRE(encode_base64_length(0) == 0);
  }
  
  SECTION("Divisible by 3 (no padding)") {
    REQUIRE(encode_base64_length(3) == 4);
    REQUIRE(encode_base64_length(6) == 8);
    REQUIRE(encode_base64_length(60) == 80);
  }
  
  SECTION("Not divisible by 3 (padded)") {
    REQUIRE(encode_base64_length(1) == 4);
    REQUIRE(encode_base64_length(2) == 4);
    REQUIRE(encode_base64_length(4) == 8);
    REQUIRE(encode_base64_length(5) == 8);
    REQUIRE(encode_base64_length(7) == 12);
    REQUIRE(encode_base64_length(256) == 344);
  }
  
  SECTION("Large") {
    REQUIRE(encode_base64_length(65536) == 87384);
  }
}

TEST_CASE("encode_base64()", "[]") {
  unsigned char actual_base64[100];
  
  SECTION("Zero length") {
    unsigned char binary_0[] = {};
    encode_base64(binary_0, 0, actual_base64); // Should give 'AQIDBUNDQQgEIIY4'
    REQUIRE(memcmp(actual_base64, "", 1) == 0);
  }
  
  SECTION("Length 1 (single section padded)") {
    unsigned char binary_0[] = {0};
    encode_base64(binary_0, 1, actual_base64);
    REQUIRE(memcmp(actual_base64, "AA==", 5) == 0);
    
    unsigned char binary_1[] = {3};
    encode_base64(binary_1, 1, actual_base64);
    REQUIRE(memcmp(actual_base64, "Aw==", 5) == 0);
    
    unsigned char binary_2[] = {8};
    encode_base64(binary_2, 1, actual_base64);
    REQUIRE(memcmp(actual_base64, "CA==", 5) == 0);
    
    unsigned char binary_3[] = {145};
    encode_base64(binary_3, 1, actual_base64);
    REQUIRE(memcmp(actual_base64, "kQ==", 5) == 0);
    
    unsigned char binary_4[] = {56};
    encode_base64(binary_4, 1, actual_base64);
    REQUIRE(memcmp(actual_base64, "OA==", 5) == 0);
    
    unsigned char binary_5[] = {54};
    encode_base64(binary_5, 1, actual_base64);
    REQUIRE(memcmp(actual_base64, "Ng==", 5) == 0);
    
    unsigned char binary_6[] = {181};
    encode_base64(binary_6, 1, actual_base64);
    REQUIRE(memcmp(actual_base64, "tQ==", 5) == 0);
    
    unsigned char binary_7[] = {79};
    encode_base64(binary_7, 1, actual_base64);
    REQUIRE(memcmp(actual_base64, "Tw==", 5) == 0);
    
    unsigned char binary_8[] = {115};
    encode_base64(binary_8, 1, actual_base64);
    REQUIRE(memcmp(actual_base64, "cw==", 5) == 0);
    
    unsigned char binary_9[] = {255};
    encode_base64(binary_9, 1, actual_base64);
    REQUIRE(memcmp(actual_base64, "/w==", 5) == 0);
  }
  
  SECTION("Length 2 (single section padded)") {
    unsigned char binary_0[] = {0, 0};
    encode_base64(binary_0, 2, actual_base64);
    REQUIRE(memcmp(actual_base64, "AAA=", 5) == 0);
    
    unsigned char binary_1[] = {49, 42};
    encode_base64(binary_1, 2, actual_base64);
    REQUIRE(memcmp(actual_base64, "MSo=", 5) == 0);
    
    unsigned char binary_2[] = {133, 38};
    encode_base64(binary_2, 2, actual_base64);
    REQUIRE(memcmp(actual_base64, "hSY=", 5) == 0);
    
    unsigned char binary_3[] = {61, 127};
    encode_base64(binary_3, 2, actual_base64);
    REQUIRE(memcmp(actual_base64, "PX8=", 5) == 0);
    
    unsigned char binary_4[] = {109, 80};
    encode_base64(binary_4, 2, actual_base64);
    REQUIRE(memcmp(actual_base64, "bVA=", 5) == 0);
    
    unsigned char binary_5[] = {47, 213};
    encode_base64(binary_5, 2, actual_base64);
    REQUIRE(memcmp(actual_base64, "L9U=", 5) == 0);
    
    unsigned char binary_6[] = {172, 205};
    encode_base64(binary_6, 2, actual_base64);
    REQUIRE(memcmp(actual_base64, "rM0=", 5) == 0);
    
    unsigned char binary_7[] = {191, 240};
    encode_base64(binary_7, 2, actual_base64);
    REQUIRE(memcmp(actual_base64, "v/A=", 5) == 0);
    
    unsigned char binary_8[] = {107, 248};
    encode_base64(binary_8, 2, actual_base64);
    REQUIRE(memcmp(actual_base64, "a/g=", 5) == 0);
    
    unsigned char binary_9[] = {255, 255};
    encode_base64(binary_9, 2, actual_base64);
    REQUIRE(memcmp(actual_base64, "//8=", 5) == 0);
  }
  
  SECTION("Length 3 (single section no padding)") {
    unsigned char binary_0[] = {0, 0, 0};
    encode_base64(binary_0, 3, actual_base64);
    REQUIRE(memcmp(actual_base64, "AAAA", 5) == 0);
    
    unsigned char binary_1[] = {151, 167, 18};
    encode_base64(binary_1, 3, actual_base64);
    REQUIRE(memcmp(actual_base64, "l6cS", 5) == 0);
    
    unsigned char binary_2[] = {23, 174, 50};
    encode_base64(binary_2, 3, actual_base64);
    REQUIRE(memcmp(actual_base64, "F64y", 5) == 0);
    
    unsigned char binary_3[] = {143, 205, 227};
    encode_base64(binary_3, 3, actual_base64);
    REQUIRE(memcmp(actual_base64, "j83j", 5) == 0);
    
    unsigned char binary_4[] = {60, 18, 186};
    encode_base64(binary_4, 3, actual_base64);
    REQUIRE(memcmp(actual_base64, "PBK6", 5) == 0);
    
    unsigned char binary_5[] = {100, 34, 201};
    encode_base64(binary_5, 3, actual_base64);
    REQUIRE(memcmp(actual_base64, "ZCLJ", 5) == 0);
    
    unsigned char binary_6[] = {52, 83, 129};
    encode_base64(binary_6, 3, actual_base64);
    REQUIRE(memcmp(actual_base64, "NFOB", 5) == 0);
    
    unsigned char binary_7[] = {241, 202, 185};
    encode_base64(binary_7, 3, actual_base64);
    REQUIRE(memcmp(actual_base64, "8cq5", 5) == 0);
    
    unsigned char binary_8[] = {149, 190, 208};
    encode_base64(binary_8, 3, actual_base64);
    REQUIRE(memcmp(actual_base64, "lb7Q", 5) == 0);
    
    unsigned char binary_9[] = {255, 255, 255};
    encode_base64(binary_9, 3, actual_base64);
    REQUIRE(memcmp(actual_base64, "////", 5) == 0);
  }
  
  SECTION("Length divisible by 3 (no padding)") {
    unsigned char binary_0[] = {117, 213, 35, 151, 133, 255};
    encode_base64(binary_0, 6, actual_base64);
    REQUIRE(memcmp(actual_base64, "ddUjl4X/", 9) == 0);
    
    unsigned char binary_1[] = {90, 95, 209, 235, 58, 255};
    encode_base64(binary_1, 6, actual_base64);
    REQUIRE(memcmp(actual_base64, "Wl/R6zr/", 9) == 0);
    
    unsigned char binary_2[] = {133, 244, 117, 206, 178, 195, 249, 84, 248};
    encode_base64(binary_2, 9, actual_base64);
    REQUIRE(memcmp(actual_base64, "hfR1zrLD+VT4", 13) == 0);
    
    unsigned char binary_3[] = {7, 27, 226, 144, 59, 237, 79, 62, 191};
    encode_base64(binary_3, 9, actual_base64);
    REQUIRE(memcmp(actual_base64, "BxvikDvtTz6/", 13) == 0);
    
    unsigned char binary_4[] = {99, 225, 39, 195, 8, 43, 209, 151, 8, 43, 195, 183};
    encode_base64(binary_4, 12, actual_base64);
    REQUIRE(memcmp(actual_base64, "Y+Enwwgr0ZcIK8O3", 17) == 0);
    
    unsigned char binary_5[] = {171, 65, 164, 64, 60, 221, 46, 226, 252, 167, 250, 252};
    encode_base64(binary_5, 12, actual_base64);
    REQUIRE(memcmp(actual_base64, "q0GkQDzdLuL8p/r8", 17) == 0);
    
    unsigned char binary_6[] = {248, 127, 14, 241, 93, 177, 152, 46, 255, 127, 92, 84, 56, 59, 152, 132, 113, 115, 252, 70, 190, 224, 130, 155, 86, 172, 159, 162, 30, 127};
    encode_base64(binary_6, 30, actual_base64);
    REQUIRE(memcmp(actual_base64, "+H8O8V2xmC7/f1xUODuYhHFz/Ea+4IKbVqyfoh5/", 41) == 0);
    
    unsigned char binary_7[] = {157, 12, 248, 83, 148, 156, 196, 30, 186, 28, 52, 192, 171, 142, 6, 105, 128, 131, 89, 5, 3, 131, 215, 192, 87, 215, 244, 141, 127, 17};
    encode_base64(binary_7, 30, actual_base64);
    REQUIRE(memcmp(actual_base64, "nQz4U5ScxB66HDTAq44GaYCDWQUDg9fAV9f0jX8R", 41) == 0);
    
    unsigned char binary_8[] = {180, 179, 175, 132, 162, 219, 3, 18, 96, 162, 214, 232, 49, 120, 59, 133, 102, 93, 67, 34, 186, 28, 6, 28, 195, 69, 249, 44, 140, 115, 55, 215, 68, 99, 130, 160, 32, 181, 172, 125, 144, 8, 21, 119, 60, 213, 156, 230, 243, 87, 101, 167, 136, 94, 242, 174, 239, 81, 67, 101};
    encode_base64(binary_8, 60, actual_base64);
    REQUIRE(memcmp(actual_base64, "tLOvhKLbAxJgotboMXg7hWZdQyK6HAYcw0X5LIxzN9dEY4KgILWsfZAIFXc81Zzm81dlp4he8q7vUUNl", 81) == 0);
    
    unsigned char binary_9[] = {165, 186, 12, 82, 241, 34, 63, 218, 215, 28, 105, 126, 37, 69, 255, 36, 235, 103, 194, 236, 81, 115, 192, 61, 247, 128, 43, 38, 58, 140, 208, 9, 34, 145, 252, 209, 150, 227, 35, 241, 46, 25, 170, 198, 191, 87, 43, 206, 250, 199, 158, 193, 96, 249, 79, 142, 39, 216, 36, 236};
    encode_base64(binary_9, 60, actual_base64);
    REQUIRE(memcmp(actual_base64, "pboMUvEiP9rXHGl+JUX/JOtnwuxRc8A994ArJjqM0AkikfzRluMj8S4Zqsa/VyvO+seewWD5T44n2CTs", 81) == 0);
  }
  
  SECTION("Length not divisible by 3 (padded)") {
    unsigned char binary_0[] = {216, 183, 235, 10};
    encode_base64(binary_0, 4, actual_base64);
    REQUIRE(memcmp(actual_base64, "2LfrCg==", 9) == 0);
    
    unsigned char binary_1[] = {7, 254, 182, 49, 158};
    encode_base64(binary_1, 5, actual_base64);
    REQUIRE(memcmp(actual_base64, "B/62MZ4=", 9) == 0);
    
    unsigned char binary_2[] = {71, 58, 223, 154, 93, 69, 18};
    encode_base64(binary_2, 7, actual_base64);
    REQUIRE(memcmp(actual_base64, "Rzrfml1FEg==", 13) == 0);
    
    unsigned char binary_3[] = {226, 127, 31, 206, 19, 75, 35, 80};
    encode_base64(binary_3, 8, actual_base64);
    REQUIRE(memcmp(actual_base64, "4n8fzhNLI1A=", 13) == 0);
    
    unsigned char binary_4[] = {5, 36, 50, 78, 218, 198, 242, 85, 235, 72, 78, 139, 103};
    encode_base64(binary_4, 13, actual_base64);
    REQUIRE(memcmp(actual_base64, "BSQyTtrG8lXrSE6LZw==", 21) == 0);
    
    unsigned char binary_5[] = {161, 176, 49, 33, 148, 150, 94, 252, 21, 249, 106, 49, 216, 124, 219, 233, 133, 102, 32, 182, 193};
    encode_base64(binary_5, 21, actual_base64);
    REQUIRE(memcmp(actual_base64, "obAxIZSWXvwV+Wox2Hzb6YVmILbB", 29) == 0);
    
    unsigned char binary_6[] = {136, 116, 151, 174, 215, 54, 64, 218, 197, 148, 149, 17, 183, 59, 177, 54, 172, 135, 192, 202, 183, 3, 254, 51, 83, 217};
    encode_base64(binary_6, 26, actual_base64);
    REQUIRE(memcmp(actual_base64, "iHSXrtc2QNrFlJURtzuxNqyHwMq3A/4zU9k=", 37) == 0);
    
    unsigned char binary_7[] = {181, 16, 71, 30, 145, 101, 21, 170, 45, 24, 201, 78, 83, 31, 175, 132, 127, 108, 88, 7, 37, 154, 196, 139, 87, 68, 243, 6, 180, 36, 89, 10, 67, 73};
    encode_base64(binary_7, 34, actual_base64);
    REQUIRE(memcmp(actual_base64, "tRBHHpFlFaotGMlOUx+vhH9sWAclmsSLV0TzBrQkWQpDSQ==", 49) == 0);
    
    unsigned char binary_8[] = {24, 6, 234, 175, 34, 198, 47, 173, 234, 158, 106, 203, 80, 171, 218, 163, 60, 105, 183, 152, 73, 142, 190, 107, 189, 223, 215, 169, 63, 169, 163, 29, 9, 134, 235, 107, 35, 5, 16, 50, 7};
    encode_base64(binary_8, 41, actual_base64);
    REQUIRE(memcmp(actual_base64, "GAbqryLGL63qnmrLUKvaozxpt5hJjr5rvd/XqT+pox0JhutrIwUQMgc=", 57) == 0);
    
    unsigned char binary_9[] = {220, 92, 67, 95, 157, 76, 162, 210, 224, 202, 136, 157, 104, 178, 103, 81, 35, 103, 244, 71, 92, 25, 69, 64, 61, 232, 198, 108, 217, 106, 63, 103, 234, 39, 156, 108, 4, 101, 212, 198, 57, 223, 75, 132, 160, 26, 193, 139, 16, 89, 12, 45, 183, 133, 33};
    encode_base64(binary_9, 55, actual_base64);
    REQUIRE(memcmp(actual_base64, "3FxDX51MotLgyoidaLJnUSNn9EdcGUVAPejGbNlqP2fqJ5xsBGXUxjnfS4SgGsGLEFkMLbeFIQ==", 77) == 0);
  }
}

TEST_CASE("decode_base64_length()", "[]") {
  SECTION("Zero length") {
    REQUIRE(decode_base64_length((unsigned char*) "") == 0);
  }
  
  SECTION("Divisible by 4 (no padding)") {
    REQUIRE(decode_base64_length((unsigned char*) "AAAA") == 3);
    REQUIRE(decode_base64_length((unsigned char*) "////") == 3);
    REQUIRE(decode_base64_length((unsigned char*) "Y+Enwwgr0ZcIK8O3") == 12);
    REQUIRE(decode_base64_length((unsigned char*) "tLOvhKLbAxJgotboMXg7hWZdQyK6HAYcw0X5LIxzN9dEY4KgILWsfZAIFXc81Zzm81dlp4he8q7vUUNl") == 60);
  }
  
  SECTION("Not divisible by 4 (padded)") {
    REQUIRE(decode_base64_length((unsigned char*) "AA==") == 1);
    REQUIRE(decode_base64_length((unsigned char*) "Aw==") == 1);
    REQUIRE(decode_base64_length((unsigned char*) "a/g=") == 2);
    REQUIRE(decode_base64_length((unsigned char*) "//8=") == 2);
    REQUIRE(decode_base64_length((unsigned char*) "4n8fzhNLI1A=") == 8);
    REQUIRE(decode_base64_length((unsigned char*) "3FxDX51MotLgyoidaLJnUSNn9EdcGUVAPejGbNlqP2fqJ5xsBGXUxjnfS4SgGsGLEFkMLbeFIQ==") == 55);
  }
  
  SECTION("Not divisible by 4 (padding missing)") {
    REQUIRE(decode_base64_length((unsigned char*) "AA") == 1);
    REQUIRE(decode_base64_length((unsigned char*) "Aw") == 1);
    REQUIRE(decode_base64_length((unsigned char*) "a/g") == 2);
    REQUIRE(decode_base64_length((unsigned char*) "//8") == 2);
    REQUIRE(decode_base64_length((unsigned char*) "4n8fzhNLI1A") == 8);
    REQUIRE(decode_base64_length((unsigned char*) "3FxDX51MotLgyoidaLJnUSNn9EdcGUVAPejGbNlqP2fqJ5xsBGXUxjnfS4SgGsGLEFkMLbeFIQ") == 55);
  }
  
  SECTION("Padding in middle cuts off string") {
    REQUIRE(decode_base64_length((unsigned char*) "AA==4n8fzhNL") == 1);
    REQUIRE(decode_base64_length((unsigned char*) "Aw=4n8fzhNL") == 1);
    REQUIRE(decode_base64_length((unsigned char*) "a/g=4n8fzhNL==") == 2);
    REQUIRE(decode_base64_length((unsigned char*) "//8=4n8fzhNL") == 2);
    REQUIRE(decode_base64_length((unsigned char*) "4n8fzhNLI1A=4n8fzhNL====") == 8);
    REQUIRE(decode_base64_length((unsigned char*) "3FxDX51MotLgyoidaLJnUSNn9EdcGUVAPejGbNlqP2fqJ5xsBGXUxjnfS4SgGsGLEFkMLbeFIQ==4n8fzhNL") == 55);
  }
  
  SECTION("Extra padding is ignored") {
    REQUIRE(decode_base64_length((unsigned char*) "Aw========") == 1);
    REQUIRE(decode_base64_length((unsigned char*) "a/g=======") == 2);
    REQUIRE(decode_base64_length((unsigned char*) "Aw========") == 1);
    REQUIRE(decode_base64_length((unsigned char*) "a/g==========") == 2);
    REQUIRE(decode_base64_length((unsigned char*) "4n8fzhNLI1A===========") == 8);
    REQUIRE(decode_base64_length((unsigned char*) "3FxDX51MotLgyoidaLJnUSNn9EdcGUVAPejGbNlqP2fqJ5xsBGXUxjnfS4SgGsGLEFkMLbeFIQ=========") == 55);
  }
  
  SECTION("Non-base64 characcters are interpreted as padding") {
    REQUIRE(decode_base64_length((unsigned char*) "Aw:;") == 1);
    REQUIRE(decode_base64_length((unsigned char*) "Aw`'@") == 1);
    REQUIRE(decode_base64_length((unsigned char*) "a/g~") == 2);
    REQUIRE(decode_base64_length((unsigned char*) "a/g[|") == 2);
    REQUIRE(decode_base64_length((unsigned char*) "4n8fzhNLI1A]") == 8);
    REQUIRE(decode_base64_length((unsigned char*) "Y+Enwwgr0ZcIK8O3{}") == 12);
    REQUIRE(decode_base64_length((unsigned char*) "AA-^4n8fzhNL") == 1);
  }
}

TEST_CASE("decode_base64()", "[]") {
  unsigned char actual_binary[100];
  
  // Zero length case ignored, because it is verified to return no data in decode_base64_length()
  
  SECTION("Divisible by 4 (no padding)") {
    unsigned char expected_binary_0[] = {0, 0, 0};
    decode_base64((unsigned char*) "AAAA", actual_binary);
    REQUIRE(memcmp(actual_binary, expected_binary_0, 3) == 0);
    
    unsigned char expected_binary_1[] = {255, 255, 255};
    decode_base64((unsigned char*) "////", actual_binary);
    REQUIRE(memcmp(actual_binary, expected_binary_1, 3) == 0);
    
    unsigned char expected_binary_2[] = {99, 225, 39, 195, 8, 43, 209, 151, 8, 43, 195, 183};
    decode_base64((unsigned char*) "Y+Enwwgr0ZcIK8O3", actual_binary);
    REQUIRE(memcmp(actual_binary, expected_binary_2, 12) == 0);
    
    unsigned char expected_binary_3[] = {180, 179, 175, 132, 162, 219, 3, 18, 96, 162, 214, 232, 49, 120, 59, 133, 102, 93, 67, 34, 186, 28, 6, 28, 195, 69, 249, 44, 140, 115, 55, 215, 68, 99, 130, 160, 32, 181, 172, 125, 144, 8, 21, 119, 60, 213, 156, 230, 243, 87, 101, 167, 136, 94, 242, 174, 239, 81, 67, 101};
    decode_base64((unsigned char*) "tLOvhKLbAxJgotboMXg7hWZdQyK6HAYcw0X5LIxzN9dEY4KgILWsfZAIFXc81Zzm81dlp4he8q7vUUNl", actual_binary);
    REQUIRE(memcmp(actual_binary, expected_binary_3, 60) == 0);
  }
  
  SECTION("Not divisible by 4 (padded)") {
    unsigned char expected_binary_0[] = {0};
    decode_base64((unsigned char*) "AA==", actual_binary);
    REQUIRE(memcmp(actual_binary, expected_binary_0, 1) == 0);
    
    unsigned char expected_binary_1[] = {3};
    decode_base64((unsigned char*) "Aw==", actual_binary);
    REQUIRE(memcmp(actual_binary, expected_binary_1, 1) == 0);
    
    unsigned char expected_binary_2[] = {107, 248};
    decode_base64((unsigned char*) "a/g=", actual_binary);
    REQUIRE(memcmp(actual_binary, expected_binary_2, 2) == 0);
    
    unsigned char expected_binary_3[] = {255, 255};
    decode_base64((unsigned char*) "//8=", actual_binary);
    REQUIRE(memcmp(actual_binary, expected_binary_3, 2) == 0);
    
    unsigned char expected_binary_4[] = {226, 127, 31, 206, 19, 75, 35, 80};
    decode_base64((unsigned char*) "4n8fzhNLI1A=", actual_binary);
    REQUIRE(memcmp(actual_binary, expected_binary_4, 8) == 0);
    
    unsigned char expected_binary_5[] = {220, 92, 67, 95, 157, 76, 162, 210, 224, 202, 136, 157, 104, 178, 103, 81, 35, 103, 244, 71, 92, 25, 69, 64, 61, 232, 198, 108, 217, 106, 63, 103, 234, 39, 156, 108, 4, 101, 212, 198, 57, 223, 75, 132, 160, 26, 193, 139, 16, 89, 12, 45, 183, 133, 33};
    decode_base64((unsigned char*) "3FxDX51MotLgyoidaLJnUSNn9EdcGUVAPejGbNlqP2fqJ5xsBGXUxjnfS4SgGsGLEFkMLbeFIQ==", actual_binary);
    REQUIRE(memcmp(actual_binary, expected_binary_5, 55) == 0);
  }
  
  SECTION("Not divisible by 4 (padding missing)") {
    unsigned char expected_binary_0[] = {0};
    decode_base64((unsigned char*) "AA", actual_binary);
    REQUIRE(memcmp(actual_binary, expected_binary_0, 1) == 0);
    
    unsigned char expected_binary_1[] = {3};
    decode_base64((unsigned char*) "Aw", actual_binary);
    REQUIRE(memcmp(actual_binary, expected_binary_1, 1) == 0);
    
    unsigned char expected_binary_2[] = {107, 248};
    decode_base64((unsigned char*) "a/g", actual_binary);
    REQUIRE(memcmp(actual_binary, expected_binary_2, 2) == 0);
    
    unsigned char expected_binary_3[] = {255, 255};
    decode_base64((unsigned char*) "//8", actual_binary);
    REQUIRE(memcmp(actual_binary, expected_binary_3, 2) == 0);
    
    unsigned char expected_binary_4[] = {226, 127, 31, 206, 19, 75, 35, 80};
    decode_base64((unsigned char*) "4n8fzhNLI1A", actual_binary);
    REQUIRE(memcmp(actual_binary, expected_binary_4, 8) == 0);
    
    unsigned char expected_binary_5[] = {220, 92, 67, 95, 157, 76, 162, 210, 224, 202, 136, 157, 104, 178, 103, 81, 35, 103, 244, 71, 92, 25, 69, 64, 61, 232, 198, 108, 217, 106, 63, 103, 234, 39, 156, 108, 4, 101, 212, 198, 57, 223, 75, 132, 160, 26, 193, 139, 16, 89, 12, 45, 183, 133, 33};
    decode_base64((unsigned char*) "3FxDX51MotLgyoidaLJnUSNn9EdcGUVAPejGbNlqP2fqJ5xsBGXUxjnfS4SgGsGLEFkMLbeFIQ", actual_binary);
    REQUIRE(memcmp(actual_binary, expected_binary_5, 55) == 0);
  }
  
  SECTION("Padding in middle cuts off string") {
    unsigned char expected_binary_0[] = {0};
    decode_base64((unsigned char*) "AA==4n8fzhNL", actual_binary);
    REQUIRE(memcmp(actual_binary, expected_binary_0, 1) == 0);
    
    unsigned char expected_binary_1[] = {3};
    decode_base64((unsigned char*) "Aw=4n8fzhNL", actual_binary);
    REQUIRE(memcmp(actual_binary, expected_binary_1, 1) == 0);
    
    unsigned char expected_binary_2[] = {107, 248};
    decode_base64((unsigned char*) "a/g=4n8fzhNL==", actual_binary);
    REQUIRE(memcmp(actual_binary, expected_binary_2, 2) == 0);
    
    unsigned char expected_binary_3[] = {255, 255};
    decode_base64((unsigned char*) "//8=4n8fzhNL", actual_binary);
    REQUIRE(memcmp(actual_binary, expected_binary_3, 2) == 0);
    
    unsigned char expected_binary_4[] = {226, 127, 31, 206, 19, 75, 35, 80};
    decode_base64((unsigned char*) "4n8fzhNLI1A=4n8fzhNL====", actual_binary);
    REQUIRE(memcmp(actual_binary, expected_binary_4, 8) == 0);
    
    unsigned char expected_binary_5[] = {220, 92, 67, 95, 157, 76, 162, 210, 224, 202, 136, 157, 104, 178, 103, 81, 35, 103, 244, 71, 92, 25, 69, 64, 61, 232, 198, 108, 217, 106, 63, 103, 234, 39, 156, 108, 4, 101, 212, 198, 57, 223, 75, 132, 160, 26, 193, 139, 16, 89, 12, 45, 183, 133, 33};
    decode_base64((unsigned char*) "3FxDX51MotLgyoidaLJnUSNn9EdcGUVAPejGbNlqP2fqJ5xsBGXUxjnfS4SgGsGLEFkMLbeFIQ==4n8fzhNL", actual_binary);
    REQUIRE(memcmp(actual_binary, expected_binary_5, 55) == 0);
  }
  
  SECTION("Extra padding is ignored") {
    unsigned char expected_binary_0[] = {3};
    decode_base64((unsigned char*) "Aw========", actual_binary);
    REQUIRE(memcmp(actual_binary, expected_binary_0, 1) == 0);
    
    unsigned char expected_binary_1[] = {107, 248};
    decode_base64((unsigned char*) "a/g=======", actual_binary);
    REQUIRE(memcmp(actual_binary, expected_binary_1, 2) == 0);
    
    unsigned char expected_binary_2[] = {3};
    decode_base64((unsigned char*) "Aw========", actual_binary);
    REQUIRE(memcmp(actual_binary, expected_binary_2, 1) == 0);
    
    unsigned char expected_binary_3[] = {107, 248};
    decode_base64((unsigned char*) "a/g==========", actual_binary);
    REQUIRE(memcmp(actual_binary, expected_binary_3, 2) == 0);
    
    unsigned char expected_binary_4[] = {226, 127, 31, 206, 19, 75, 35, 80};
    decode_base64((unsigned char*) "4n8fzhNLI1A===========", actual_binary);
    REQUIRE(memcmp(actual_binary, expected_binary_4, 8) == 0);
    
    unsigned char expected_binary_5[] = {220, 92, 67, 95, 157, 76, 162, 210, 224, 202, 136, 157, 104, 178, 103, 81, 35, 103, 244, 71, 92, 25, 69, 64, 61, 232, 198, 108, 217, 106, 63, 103, 234, 39, 156, 108, 4, 101, 212, 198, 57, 223, 75, 132, 160, 26, 193, 139, 16, 89, 12, 45, 183, 133, 33};
    decode_base64((unsigned char*) "3FxDX51MotLgyoidaLJnUSNn9EdcGUVAPejGbNlqP2fqJ5xsBGXUxjnfS4SgGsGLEFkMLbeFIQ=========", actual_binary);
    REQUIRE(memcmp(actual_binary, expected_binary_5, 55) == 0);
  }
  
  SECTION("Non-base64 characcters are interpreted as padding") {
    unsigned char expected_binary_0[] = {3};
    decode_base64((unsigned char*) "Aw==", actual_binary);
    REQUIRE(memcmp(actual_binary, expected_binary_0, 1) == 0);
    
    unsigned char expected_binary_1[] = {3};
    decode_base64((unsigned char*) "Aw===", actual_binary);
    REQUIRE(memcmp(actual_binary, expected_binary_1, 1) == 0);
    
    unsigned char expected_binary_2[] = {107, 248};
    decode_base64((unsigned char*) "a/g=", actual_binary);
    REQUIRE(memcmp(actual_binary, expected_binary_2, 2) == 0);
    
    unsigned char expected_binary_3[] = {107, 248};
    decode_base64((unsigned char*) "a/g==", actual_binary);
    REQUIRE(memcmp(actual_binary, expected_binary_3, 2) == 0);
    
    unsigned char expected_binary_4[] = {226, 127, 31, 206, 19, 75, 35, 80};
    decode_base64((unsigned char*) "4n8fzhNLI1A=", actual_binary);
    REQUIRE(memcmp(actual_binary, expected_binary_4, 8) == 0);
    
    unsigned char expected_binary_5[] = {99, 225, 39, 195, 8, 43, 209, 151, 8, 43, 195, 183};
    decode_base64((unsigned char*) "Y+Enwwgr0ZcIK8O3==", actual_binary);
    REQUIRE(memcmp(actual_binary, expected_binary_5, 12) == 0);
    
    unsigned char expected_binary_6[] = {0};
    decode_base64((unsigned char*) "AA==4n8fzhNL", actual_binary);
    REQUIRE(memcmp(actual_binary, expected_binary_6, 1) == 0);
  }
}