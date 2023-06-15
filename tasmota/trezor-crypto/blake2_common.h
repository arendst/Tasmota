static inline uint32_t load32( const void *src )
{
    uint32_t w;
    memcpy(&w, src, sizeof w);
    return w;
}

static inline uint64_t load64( const void *src )
{
  uint64_t w;
  memcpy(&w, src, sizeof w);
  return w;
}

static inline void store16( void *dst, uint16_t w )
{
    memcpy(dst, &w, sizeof w);
}

static inline void store32( void *dst, uint32_t w )
{
    memcpy(dst, &w, sizeof w);
}

static inline void store64( void *dst, uint64_t w )
{
  memcpy(dst, &w, sizeof w);
}

static inline uint32_t rotr32( const uint32_t w, const unsigned c )
{
    return ( w >> c ) | ( w << ( 32 - c ) );
}

static inline uint64_t rotr64( const uint64_t w, const unsigned c )
{
  return ( w >> c ) | ( w << ( 64 - c ) );
}

