// jpge.h - C++ class for JPEG compression.
// Public domain, Rich Geldreich <richgel99@gmail.com>
// Alex Evans: Added RGBA support, linear memory allocator.
#ifndef JPEG_ENCODER_H
#define JPEG_ENCODER_H

namespace jpge
{
    typedef unsigned char  uint8;
    typedef signed short   int16;
    typedef signed int     int32;
    typedef unsigned short uint16;
    typedef unsigned int   uint32;
    typedef unsigned int   uint;

    // JPEG chroma subsampling factors. Y_ONLY (grayscale images) and H2V2 (color images) are the most common.
    enum subsampling_t { Y_ONLY = 0, H1V1 = 1, H2V1 = 2, H2V2 = 3 };

    // JPEG compression parameters structure.
    struct params {
            inline params() : m_quality(85), m_subsampling(H2V2) { }

            inline bool check() const {
                if ((m_quality < 1) || (m_quality > 100)) {
                    return false;
                }
                if ((uint)m_subsampling > (uint)H2V2) {
                    return false;
                }
                return true;
            }

            // Quality: 1-100, higher is better. Typical values are around 50-95.
            int m_quality;

            // m_subsampling:
            // 0 = Y (grayscale) only
            // 1 = H1V1 subsampling (YCbCr 1x1x1, 3 blocks per MCU)
            // 2 = H2V1 subsampling (YCbCr 2x1x1, 4 blocks per MCU)
            // 3 = H2V2 subsampling (YCbCr 4x1x1, 6 blocks per MCU-- very common)
            subsampling_t m_subsampling;
    };
    
    // Output stream abstract class - used by the jpeg_encoder class to write to the output stream.
    // put_buf() is generally called with len==JPGE_OUT_BUF_SIZE bytes, but for headers it'll be called with smaller amounts.
    class output_stream {
        public:
            virtual ~output_stream() { };
            virtual bool put_buf(const void* Pbuf, int len) = 0;
            virtual uint get_size() const = 0;
    };
    
    // Lower level jpeg_encoder class - useful if more control is needed than the above helper functions.
    class jpeg_encoder {
        public:
            jpeg_encoder();
            ~jpeg_encoder();

            // Initializes the compressor.
            // pStream: The stream object to use for writing compressed data.
            // params - Compression parameters structure, defined above.
            // width, height  - Image dimensions.
            // channels - May be 1, or 3. 1 indicates grayscale, 3 indicates RGB source data.
            // Returns false on out of memory or if a stream write fails.
            bool init(output_stream *pStream, int width, int height, int src_channels, const params &comp_params = params());

            // Call this method with each source scanline.
            // width * src_channels bytes per scanline is expected (RGB or Y format).
            // You must call with NULL after all scanlines are processed to finish compression.
            // Returns false on out of memory or if a stream write fails.
            bool process_scanline(const void* pScanline);

            // Deinitializes the compressor, freeing any allocated memory. May be called at any time.
            void deinit();

        private:
            jpeg_encoder(const jpeg_encoder &);
            jpeg_encoder &operator =(const jpeg_encoder &);

            typedef int32 sample_array_t;
            enum { JPGE_OUT_BUF_SIZE = 512 };

            output_stream *m_pStream;
            params m_params;
            uint8 m_num_components;
            uint8 m_comp_h_samp[3], m_comp_v_samp[3];
            int m_image_x, m_image_y, m_image_bpp, m_image_bpl;
            int m_image_x_mcu, m_image_y_mcu;
            int m_image_bpl_xlt, m_image_bpl_mcu;
            int m_mcus_per_row;
            int m_mcu_x, m_mcu_y;
            uint8 *m_mcu_lines[16];
            uint8 m_mcu_y_ofs;
            sample_array_t m_sample_array[64];
            int16 m_coefficient_array[64];

            int m_last_dc_val[3];
            uint8 m_out_buf[JPGE_OUT_BUF_SIZE];
            uint8 *m_pOut_buf;
            uint m_out_buf_left;
            uint32 m_bit_buffer;
            uint m_bits_in;
            uint8 m_pass_num;
            bool m_all_stream_writes_succeeded;

            bool jpg_open(int p_x_res, int p_y_res, int src_channels);

            void flush_output_buffer();
            void put_bits(uint bits, uint len);

            void emit_byte(uint8 i);
            void emit_word(uint i);
            void emit_marker(int marker);

            void emit_jfif_app0();
            void emit_dqt();
            void emit_sof();
            void emit_dht(uint8 *bits, uint8 *val, int index, bool ac_flag);
            void emit_dhts();
            void emit_sos();

            void compute_quant_table(int32 *dst, const int16 *src);
            void load_quantized_coefficients(int component_num);

            void load_block_8_8_grey(int x);
            void load_block_8_8(int x, int y, int c);
            void load_block_16_8(int x, int c);
            void load_block_16_8_8(int x, int c);

            void code_coefficients_pass_two(int component_num);
            void code_block(int component_num);

            void process_mcu_row();
            bool process_end_of_image();
            void load_mcu(const void* src);
            void clear();
            void init();
    };
    
} // namespace jpge

#endif // JPEG_ENCODER
