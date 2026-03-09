set(srcs 
    "src/JPEGDEC.cpp"
    "src/jpeg.inl"
    "src/s3_simd_420.S"
    "src/s3_simd_444.S"
    "src/s3_simd_dequant.S"
)
idf_component_register(SRCS ${srcs}      
                    REQUIRES "jpegdec" "esp-dsp"
                    INCLUDE_DIRS "src"
)
