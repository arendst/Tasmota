set(srcs 
    "src/JPEGDEC.cpp"
    "src/jpeg.inl"
)
idf_component_register(SRCS ${srcs}      
                    REQUIRES "jpegdec"
                    INCLUDE_DIRS "src"
)
