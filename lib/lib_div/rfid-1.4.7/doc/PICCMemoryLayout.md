# Memory Layout of common PICCs

To read and write from MIFARE PICCs, the MIFARE protocol is used after the PICC has been selected.

## Datasheet References

The **MIFARE Classic** chips and protocol is described in the datasheets:
  *  1K:   https://www.mouser.com/ds/2/302/MF1S503x-89574.pdf
  *  4K:   https://datasheet.octopart.com/MF1S7035DA4,118-NXP-Semiconductors-datasheet-11046188.pdf
  *  Mini: http://www.idcardmarket.com/download/mifare_S20_datasheet.pdf

The **MIFARE Ultralight** chip and protocol is described in the datasheets:
  *  Ultralight:   https://www.nxp.com/documents/data_sheet/MF0ICU1.pdf
  *  Ultralight C: https://www.nxp.com/documents/short_data_sheet/MF0ICU2_SDS.pdf

## MIFARE Classic 1K (MF1S503x)

Has 16 sectors4 blocks/sector16 bytes/block = 1024 bytes.

    The blocks are numbered 0-63.
    Block 3 in each sector is the Sector Trailer. See https://www.mouser.com/ds/2/302/MF1S503x-89574.pdf sections 8.6 and 8.7:
        Bytes 0-5:   Key A
        Bytes 6-8:   Access Bits
        Bytes 9:     User data
        Bytes 10-15: Key B (or user data)
    Block 0 is read-only manufacturer data.
    To access a block, an authentication using a key from the block's sector must be performed first.
    Example: To read from block 10, first authenticate using a key from sector 3 (blocks 8-11).
    All keys are set to FFFFFFFFFFFFh at chip delivery.
    Warning: Please read section 8.7 "Memory Access". It includes this text: if the PICC detects a format violation the whole sector is irreversibly blocked.
     To use a block in "value block" mode (for Increment/Decrement operations) you need to change the sector trailer. Use PICC_SetAccessBits() to calculate the bit patterns.

## MIFARE Classic 4K (MF1S703x):

Has (32 sectors4 blocks/sector + 8 sectors16 blocks/sector)16 bytes/block = 4096 bytes.

    The blocks are numbered 0-255.
    The last block in each sector is the Sector Trailer like above.

## MIFARE Classic Mini (MF1 IC S20):

Has 5 sectors4 blocks/sector16 bytes/block = 320 bytes.

    The blocks are numbered 0-19.
    The last block in each sector is the Sector Trailer like above.

## MIFARE Ultralight (MF0ICU1):

Has 16 pages of 4 bytes = 64 bytes.

    Pages 0 + 1 is used for the 7-byte UID.
    Page 2 contains the last check digit for the UID, one byte manufacturer internal data, and the lock bytes (see https://www.nxp.com/documents/data_sheet/MF0ICU1.pdf section 8.5.2)
    Page 3 is OTP, One Time Programmable bits. Once set to 1 they cannot revert to 0.
    Pages 4-15 are read/write unless blocked by the lock bytes in page 2. 

## MIFARE Ultralight C (MF0ICU2):

Has 48 pages of 4 bytes = 192 bytes.

    Pages 0 + 1 is used for the 7-byte UID.
    Page 2 contains the last check digit for the UID, one byte manufacturer internal data, and the lock bytes (see https://www.nxp.com/documents/data_sheet/MF0ICU1.pdf section 8.5.2)
    Page 3 is OTP, One Time Programmable bits. Once set to 1 they cannot revert to 0.
    Pages 4-39 are read/write unless blocked by the lock bytes in page 2. 
    Page 40 Lock bytes
    Page 41 16 bit one way counter
    Pages 42-43 Authentication configuration
    Pages 44-47 Authentication key 