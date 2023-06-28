#ifdef __cplusplus
extern "C"
{
#endif

#define serialize_u32(data)           + sizeof(uint32_t)
#define serialize_u64(data)           + sizeof(uint64_t)
#define serialize_write(data, length) + (length)

/*
if (!nem_can_write(ctx, NEM_SERIALIZE)) {
	return false;
}
*/
    
#undef serialize_u32
#undef serialize_u64
#undef serialize_write

#define serialize_u32(data)           nem_write_u32(ctx, (data));
#define serialize_u64(data)           nem_write_u64(ctx, (data));
#define serialize_write(data, length) nem_write(ctx, (data), (length));

#define NEM_SERIALIZE

#undef serialize_u32
#undef serialize_u64
#undef serialize_write

#undef NEM_SERIALIZE
    
#ifdef __cplusplus
} /* end of extern "C" */
#endif
