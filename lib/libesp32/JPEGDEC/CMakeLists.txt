set(srcs 
    "src/JPEGDEC.cpp"
    "src/jpeg.inl"
    "src/s3_simd_420.S"
    "src/s3_simd_444.S"
    "src/s3_simd_dequant.S"
)
idf_component_register(SRCS ${srcs}      
                    REQUIRES "esp-dsp arduino-esp32"
                    INCLUDE_DIRS "src/."
)

project(jpegdec)
