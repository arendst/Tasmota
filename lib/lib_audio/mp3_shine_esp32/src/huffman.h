#define HUFFBITS uint16_t
#define HTN     34
#define MXOFF   250

struct huffcodetab {
  unsigned int xlen;         /*max. x-index+                         */
  unsigned int ylen;         /*max. y-index+                         */
  unsigned int linbits;      /*number of linbits                     */
  unsigned int linmax;       /*max number to be stored in linbits    */
  const HUFFBITS *table;     /*pointer to array[xlen][ylen]          */
  const unsigned char *hlen; /*pointer to array[xlen][ylen]          */
};

extern const struct huffcodetab shine_huffman_table[HTN];/* global memory block                */
                                                         /* array of all huffcodtable headers    */
                                                         /* 0..31 Huffman code table 0..31       */
                                                         /* 32,33 count1-tables                  */


