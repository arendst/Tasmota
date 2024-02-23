#pragma once

#include <stdint.h>

#include "tock.h"

#ifdef __cplusplus
extern "C" {
#endif

#define DRIVER_NUM_SDCARD 0x50002

// set a callback function for SD card commands
// Callback is called upon command completion or when an error occurs. See
// `sdcard_cb` in sdcard.c for callback documentation
//
// returns 0 if successful, < 0 if an error occurrs
int sdcard_set_callback (subscribe_upcall callback, void* callback_args);

// set a buffer that data read from the SD card will be placed into
// this buffer can be reused across multiple read calls
//
// returns 0 if successful, < 0 if an error occurrs
int sdcard_set_read_buffer (uint8_t* buffer, uint32_t len);

// set a buffer containing data to write to the SD card
// this buffer can be reused across multiple write calls by changing its
// contents. No need to call this function a second time
//
// returns 0 if successful, < 0 if an error occurrs
int sdcard_set_write_buffer (uint8_t* buffer, uint32_t len);

// check if an SD card is installed
// Completes synchronously
//
// returns 1 if installed, 0 if not installed, < 0 if an error occurrs
int sdcard_is_installed (void);

// initialize an SD card asynchronously
// Note that for a newly powered-on SD card, initialization can take over
// 100 ms to complete. Expects a callback to have already been set. Callback
// will be called when either initialization is complete or an error occurs
//
// returns 0 if started successfully, < 0 if an error occurrs
int sdcard_initialize (void);

// initialize an SD card synchronously
// Note that for a newly powered-on SD card, initialization can take over
// 100 ms to complete
//
// block_size - set to the block size of the SD card, if not NULL
// size_in_kB - set to the size of the SD card in kilobytes, if not NULL
//
// returns 0 if SD card is now initialized, < 0 if an error occurrs
int sdcard_initialize_sync (uint32_t* block_size, uint32_t* size_in_kB);

// read a single block from an SD card asynchronously
// Expects a read_buffer and a callback to already have been set up. Callback
// will be called when either the block has been read or an error occurs. When
// the callback is successful, data will be in the read_buffer
//
// sector - sector address of the block to be read
//
// returns 0 if started successfully, < 0 if an error occurrs
int sdcard_read_block (uint32_t sector);

// read a single block from an SD card synchronously
// Expects a read_buffer to already have been set up. When the command
// completes successfully, data will be in the read_buffer
//
// sector - sector address of the block to be read
//
// returns 0 if the block has been read, < 0 if an error occurrs
int sdcard_read_block_sync (uint32_t sector);

// write a single block to an SD card asynchronously
// Expects a write_buffer and a callback to already have been set up. Data in
// the write_buffer will be written to the SD card. Callback will be called
// when either the block has been written or an error occurs
//
// sector - sector address of the block to be written
//
// returns 0 if started successfully, < 0 if an error occurrs
int sdcard_write_block (uint32_t sector);

// write a single block to an SD card synchronously
// Expects a write_buffer to already have been set up. Data in the write_buffer
// will be written to the SD card
//
// sector - sector address of the block to be written
//
// returns 0 if the block has been written, < 0 if an error occurrs
int sdcard_write_block_sync (uint32_t sector);

#ifdef __cplusplus
}
#endif
