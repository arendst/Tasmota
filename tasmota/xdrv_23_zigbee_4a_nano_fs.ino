/*
  xdrv_23_zigbee_4a_eeprom.ino - zigbee support for Tasmota - nano filesystem for EEPROM, with anti-weavering

  Copyright (C) 2021  Theo Arends and Stephan Hadinger

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifdef USE_ZIGBEE
#ifdef USE_ZIGBEE_EZSP

#define Z_EEPROM_DEBUG

// The EEPROM is 64KB in size with individually writable bytes.
// They are conveniently organized in pages of 128 bytes to accelerate
// data transfer, but unlike flash memory, you don't need to erase an entire page.
// The chip spec says it supports over 2 million writes per byte.

// EEPROM LAYOUT:
// ==============
// 64KB EEPROM is divided in 256 block of 256 bytes.
// The internal page size is 128 bytes, so we're grouping 2 pages in one block
// The advantage is that any pointer to a block is a single byte
//
// Block 0, 1 and 255 are reserved.
//
// BLock 0 contains the directory of files
// Block 1 contains the linked list of blocks for each file
// Block 255 contains the bitmap of block and ageing information

// File structure
// Each file has :
//  - a name of 4 chars (no extension) that conveniently fit in uint32_t.
//  - a length in bytes, encoded with 16 bits (uint16_t)
//  - 1 byte indicating the first block of the file
//  - 1 byte reserved
//
// Then blocks are a linked-list of content. The next block is indicated in Block 1
//
// Note: the linked list could cause a circular reference loop and potentially an infinite loop.
// This is why the content lenght is used to check that the block count does not exceed
// the content length hence cannot cause an infinite loop.
// Any pointer to blocks 1 or 255 is considered invalid and means a corruption of the file system.

// Signature entry:
// - 4 bytes of signature, currently 'Tasm'. Any other entry indicates that the EEPROM was not formatted
// - 1 byte version number, currently 0x00
// - all other bytes (5..7) are reserved and filled with 0s

// DIRECTORY
// =========
// Block 0 is the directory. There is no support for folders.
// Each file entry is 8 bytes.
// First entry is a signature marker and version
// Entries 1..30 are for files
// Entry 31 (last entry) is reserved and filled with 0s

// BITMAP
// ==========
// Block 255:
// Each byte represents a block, remember there are 256 blocks in total
// Each byte is set as follows:
// bit 7 - block is used (1) or free (0) - note that blocks 0, 1 and 255 are always used
// bit 6 - block is damaged - not implemented yet but may be useful
// bit 0..5 - generation number for anti-weavering
//
// Caveat: this bitmap system may lead to wasted blocked marked as used but actually unused
// Periodical garbage collection and sanity checks can occur, for ex at boot.
//
// If the generation number overflows, all blocks start at generation `0`
// meaning that the entire bitmap block is overwritten.

// Version 0:
// Many features are not yet implemented.
// We start with hardcoded values:
//  - the two entries for files 'zig2' and 'dat2' are predefined
//  - the starting block for each file is fixed.
//    'Zig2' uses 32 blocks (8kb max) - starting at block 32
//    'Dat2' uses 32 blocks (8kb max) - starting at block 64
//  - the bitmap marks those blocks as used
//  - version number only uses first entry that doesn't get re-written
//  - only file size actually changes

/*********************************************************************************************\
 *
 * Constants
 *
\*********************************************************************************************/
const size_t ZFS_BLOCK_SIZE      = 256;
const size_t ZFS_ENTRY_SIZE      = 8;    // each entry is 32 bytes
const size_t ZFS_ENTRIES         = 30;
const uint32_t ZFS_SIGNATURE     = 0x6D736154;    // 'Tasm'


/*********************************************************************************************\
 * Specific to v2 (limited support)
\*********************************************************************************************/
const size_t ZFS_FILE_BLOCKS     = 31;    // 31 blocks

/*********************************************************************************************\
 * ZFS_File_Entry
\*********************************************************************************************/
class ZFS_File_Entry {
public:
  uint32_t        name;       // file name representing 4 chars, 0x00000000 means empty entry
  uint16_t        length;     // length of file in bytes
  uint8_t         blk_start;
  uint8_t         reserved;  // reserved for future use

  ZFS_File_Entry() :
    name(0),
    length(0),
    blk_start(0),
    reserved(0)
    {}

  inline static bool validIdx(uint8_t blk_start) { return ((blk_start != 0x00) && (blk_start != 0x01) && (blk_start != 0xFF)); };
  static uint16_t getAddress(uint8_t entry_idx);
  void read(uint8_t entry_idx);
  void write(uint8_t entry_idx) const ;
};

/*********************************************************************************************\
 * ZFS_File_Entry
\*********************************************************************************************/
class ZFS_Root_Entry {
public:
  uint32_t      signature;    // Signature that the block is correctly formatted
  uint8_t       version;      // version of file system structure
  uint8_t       reserved[3];

  ZFS_Root_Entry() :
    signature(ZFS_SIGNATURE),    // 'Tasm'
    version(0),
    reserved{}
    {};
};

/*********************************************************************************************\
 * ZFS_File_Entry
\*********************************************************************************************/
class ZFS_Dir_Block {
public:
  ZFS_Root_Entry    b0;               // signature entry
  ZFS_File_Entry    e[ZFS_ENTRIES];   // 7 entries for files
  ZFS_File_Entry    reserved;         // reserved for future use

  void format(void);                  // prepare default values for formatting
};

/*********************************************************************************************\
 * ZFS_Bitmap at block 0xFF
\*********************************************************************************************/
// Individual block
union ZFS_Bitmap_Entry {
  uint8_t   raw;
  struct {
    uint8_t   gen : 6;
    bool      damaged : 1;
    bool      used : 1;
  };
};

class ZFS_Bitmap {
public:
  ZFS_Bitmap_Entry         block[ZFS_BLOCK_SIZE];

  void format(void);
};

/*********************************************************************************************\
 * ZFS_Map, linked list of blocks, at block 1
\*********************************************************************************************/
class ZFS_Map {
public:
  uint8_t                  next_blk[ZFS_BLOCK_SIZE];

  void format(void);
};

/*********************************************************************************************\
 *
 * Formatting implementations
 *
\*********************************************************************************************/

void ZFS_Dir_Block::format(void) {
  // entry 0 - 'zig2'
  e[0].name = ZIGB_NAME2;
  e[0].length = 0;
  e[0].blk_start = 2;             // start at block 2 to 32
  // entry 1 - 'dat2'
  e[1].name = ZIGB_DATA2;
  e[1].length = 0;
  e[1].blk_start = 2 + 31;        // start at block 33 to 63
}

void ZFS_Bitmap::format(void) {
  ZFS_Bitmap_Entry val_used;
  val_used.gen = 0;
  val_used.damaged = false;
  val_used.used = true;
  // block 0, 1, 255
  // block[0x00] = val_used;    // already in loop
  // block[0x01] = val_used;
  block[0xFF] = val_used;
  // reserve block 32->63 for file 0 and 64->95 for file 1
  for (uint32_t i = 0; i < 64; i++) {
    block[i] = val_used;
  }
}

void ZFS_Map::format(void) {
  // map a linear linked list for v1
  for (uint32_t i = 2; i < ZFS_BLOCK_SIZE - 2; i++) {
    next_blk[i] = i+1;
  }
}

/*********************************************************************************************\
 *
 * Writing a file
 *
\*********************************************************************************************/

class ZFS_Write_File {
public:
  // file info
  uint32_t  name;
  uint16_t  cursor;
  uint16_t  length;
  uint8_t   blk_start;      // if 0x00 then file does not exist
  uint8_t   entry_idx;      // entry number in the directory

  ZFS_Write_File(uint32_t _name) : name(_name), cursor(0), length(0), blk_start(0) { findOrCreate(); }

  inline bool valid(void) const { return blk_start != 0; }       // does the file exist?

  int32_t addBytes(void* buffer, size_t buffer_len);
  int32_t close(void);

protected:
  void findOrCreate(void);
};


/*********************************************************************************************\
 *
 * Check that the EEPROM is formatted
 *
\*********************************************************************************************/

// Main class for the Zigbee filesystem
class ZFS {
public:

  static void initOrFormat(void);     // <0 means error
  static void format(void);           // format EEPROM

  static int32_t getLength(uint32_t name);
  static bool findFileEntry(uint32_t name, ZFS_File_Entry & entry, uint8_t * entry_idx);
  static void erase(void);              // erase EEPROM

  // read file
  static int32_t readBytes(uint32_t name, void* buffer, size_t buffer_len, uint16_t start, uint16_t len);
};

/*********************************************************************************************\
 *
 * Check that the EEPROM is formatted
 *
\*********************************************************************************************/

bool ZFS::findFileEntry(uint32_t name, ZFS_File_Entry & entry, uint8_t * _entry_idx) {
  if (!zigbee.eeprom_ready) { return false; }
  for (uint32_t entry_idx = 0; entry_idx < ZFS_ENTRIES; entry_idx++) {
    // read entry from EEPROM
    uint16_t  entry_addr = 0x0000 + sizeof(ZFS_Root_Entry) + sizeof(ZFS_File_Entry) * entry_idx;
    zigbee.eeprom.readBytes(entry_addr, sizeof(ZFS_File_Entry), (byte*)&entry);
#ifdef Z_EEPROM_DEBUG
    // {
    //   char hex_char[(sizeof(ZFS_File_Entry) * 2) + 2];
    //   AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_ZIGBEE "Read entry %d at address 0x%04X contains %s"), entry_idx, entry_addr, ToHex_P((uint8_t*)&entry, sizeof(entry), hex_char, sizeof(hex_char)));
    // }
#endif
    if (entry.name == name) {
      if (_entry_idx) { *_entry_idx = entry_idx; }
      return true;
    }
  }
  return false;
}

int32_t ZFS::getLength(uint32_t name) {
  ZFS_File_Entry entry;
  if (ZFS::findFileEntry(name, entry, nullptr)) {
    return entry.length;
  }
  return -1;
}

void ZFS::erase(void) {
  if (!zigbee.eeprom_present) { return; }
  uint32_t zero = 0;
  zigbee.eeprom.writeBytes(0x0000, sizeof(zero), (byte*)&zero);
}

/*********************************************************************************************\
 *
 * Reading a file
 *
\*********************************************************************************************/
int32_t ZFS::readBytes(uint32_t name, void* buffer, size_t buffer_len, uint16_t read_start, uint16_t read_len) {
  if (!zigbee.eeprom_ready) { return -1; }
#ifdef Z_EEPROM_DEBUG
    // AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_ZIGBEE "readBytes name=%08X, buffer_len=%d, read_start=0x%04X, read_len=%d"), name, buffer_len, read_start, read_len);
#endif
  if (name == 0x00000000) { return -1; }
  if (buffer_len == 0) { return 0; }

  // look for file
  ZFS_File_Entry entry;
  uint8_t entry_idx;
  if (!findFileEntry(name, entry, &entry_idx)) { return -1; }   // file not found

  if (read_start >= entry.length) { return 0; }    // start of read is beyond end of file, return nothing
  uint16_t max_read_len = entry.length - read_start;   // we know it's > 0
  if (read_len > max_read_len) { read_len = max_read_len; }
  if (read_len > buffer_len) { read_len = buffer_len; }
  // we know read_len is the correct max value now

  // compute the start block for the file
  // V1 it's the first one
  uint8_t  blk = entry.blk_start;

  zigbee.eeprom.readBytes((blk << 8) + read_start, read_len, (byte*) buffer);
  return read_len;
}

/*********************************************************************************************\
 *
 * Check that the EEPROM is formatted
 *
\*********************************************************************************************/

void ZFS::initOrFormat(void) {
  if (!zigbee.eeprom_present) { return; }

#ifdef Z_EEPROM_DEBUG
  // AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_ZIGBEE "sizeof(ZFS_Bitmap)=%d sizeof(ZFS_File_Entry)=%d sizeof(ZFS_Root_Entry)=%d sizeof(ZFS_Dir_Block)=%d"), sizeof(ZFS_Bitmap), sizeof(ZFS_File_Entry), sizeof(ZFS_Root_Entry), sizeof(ZFS_Dir_Block));
  {
    byte map[256];
    char hex_char[(256 * 2) + 2];
    zigbee.eeprom.readBytes(0x0000, 256, map);
    AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_ZIGBEE "BLK 00 %s"), ToHex_P(map, sizeof(map), hex_char, sizeof(hex_char)));
    // zigbee.eeprom.readBytes(0x0100, 256, map);
    // AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_ZIGBEE "BLK 01 %s"), ToHex_P(map, sizeof(map), hex_char, sizeof(hex_char)));
    zigbee.eeprom.readBytes(0x0200, 256, map);
    AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_ZIGBEE "BLK 02 %s"), ToHex_P(map, sizeof(map), hex_char, sizeof(hex_char)));
    zigbee.eeprom.readBytes(0x2100, 256, map);
    AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_ZIGBEE "BLK 21 %s"), ToHex_P(map, sizeof(map), hex_char, sizeof(hex_char)));
    // zigbee.eeprom.readBytes(0xFF00, 256, map);
    // AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_ZIGBEE "BLK FF %s"), ToHex_P(map, sizeof(map), hex_char, sizeof(hex_char)));
  }
#endif

  ZFS_Dir_Block * dir = new ZFS_Dir_Block();
  zigbee.eeprom.readBytes(0, sizeof(ZFS_Dir_Block), (byte*) dir);

  if (dir->b0.signature == ZFS_SIGNATURE) {
    // Good
    AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_ZIGBEE "EEPROM signature 0x%08X is correct"), dir->b0.signature);
  } else {
    AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_ZIGBEE "EEPROM signature 0x%08X is incorrect, formatting"), dir->b0.signature);
    format();
  }
  delete dir;

  zigbee.eeprom_ready = true;
}

//
// Format EEPROM
//
void ZFS::format(void) {
  AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_ZIGBEE "Formatting EEPROM"));

  // First write the bitmap
  ZFS_Bitmap * bitmap = new ZFS_Bitmap();
  bitmap->format();
  zigbee.eeprom.writeBytes(0xFF00, 256, (byte*) bitmap);
  delete bitmap;

  // Map
  ZFS_Map * map = new ZFS_Map();
  map->format();
  zigbee.eeprom.writeBytes(0x0100, 256, (byte*) map);
  delete map;

  // Dir
  ZFS_Dir_Block * dir = new ZFS_Dir_Block();
  dir->format();
  zigbee.eeprom.writeBytes(0x0000, 256, (byte*) dir);
  delete dir;
}

uint16_t ZFS_File_Entry::getAddress(uint8_t entry_idx) {
  return sizeof(ZFS_Root_Entry) + sizeof(ZFS_File_Entry) * entry_idx;
}

void ZFS_File_Entry::read(uint8_t entry_idx) {
  if (!zigbee.eeprom_ready) { return; }
  zigbee.eeprom.readBytes(getAddress(entry_idx), sizeof(ZFS_File_Entry), (byte*)this);
}

void ZFS_Write_File::findOrCreate(void) {
  ZFS_File_Entry entry;

  if (ZFS::findFileEntry(name, entry, &entry_idx)) {
    blk_start = entry.blk_start;
  }
};

int32_t ZFS_Write_File::addBytes(void* buffer, size_t buffer_len) {
  if (!zigbee.eeprom_ready) { return -1; }
  if ((buffer == nullptr) || (buffer_len == 0)) { return 0; }
  if (length + buffer_len > ZFS_FILE_BLOCKS * 256) { return -1; }   // exceeded max size

// #ifdef Z_EEPROM_DEBUG
//   AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_ZIGBEE "eeprom.writeBytes address=0x%04X, len=%d"), (blk_start << 8) + length, buffer_len);
// #endif
  zigbee.eeprom.writeBytes((blk_start << 8) + length, buffer_len, (byte*)buffer);
  length += buffer_len;
  return length;
}

int32_t ZFS_Write_File::close(void) {
  if (!zigbee.eeprom_ready) { return -1; }
  // write the final length
  uint16_t address = ZFS_File_Entry::getAddress(entry_idx);
  zigbee.eeprom.writeBytes(address + sizeof(name), 2, (byte*)&length);
  return length;
}

#endif // USE_ZIGBEE_EZSP
#endif // USE_ZIGBEE
