# Writing a uDisplay / uTouch descriptor (`display.ini`)

Use this as the authoring prompt for a new file in `tasmota/displaydesc/`. It describes the current parser, not the older public documentation. Start with the closest shipped descriptor; preserve its bus type and controller-specific initialization unless the hardware differs.

## Non-negotiable rules

- Store the file as `/display.ini`; enable uDisplay with any GPIO set to `Option A3`. Terminate it with `#`.
- `:H` is required and must be first. Put `:F` before `:A`. Fields are comma-separated.
- Do **not** use spaces in descriptor lines or inline comments: a space ends parsing for that line. A whole line beginning with `;` is a comment. uTouch instruction lines are the exception: they require single spaces between opcode and operands.
- Numbers are decimal except controller bytes, addresses, `:0`–`:3`, and `:b`, which are hexadecimal. Hex accepts `2A` or `0x2A`; write controller bytes as two uppercase hex digits.
- `*` substitutes the matching Tasmota GPIO assignment; `-1` means not connected. Missing values become `255`, so always provide flag fields.
- Source priority is `/display.ini`, then Scripter `>d`, disabled `Rule3` (all spaces become line breaks, so no uTouch scripts), then `DSP_ROM_DESC`. In-memory sources are limited to 999 bytes; keep `DSP_ROM_DESC` below 1000 bytes.

## 1. Required header: `:H`

`:H,name,width,height,bpp,interface,...`.

| Interface | Fields after interface | Notes |
|---|---|---|
| `I2C` / `I2C2` | `addr(H),scl,sda,rst` | `I2C2` selects bus 2. `*` maps SCL/SDA and `Display Rst`. |
| `SPI` | `bus,cs,clk,mosi,dc,bl,rst,miso,mhz` | Bus `1` HW SPI, `2` HW HSPI, `3` fast software SPI (GPIO < 32), `4` slow software SPI. `dc=-1` selects 3-wire 9-bit SPI; `miso` is the e-paper BUSY pin. |
| `PAR` | `8\|16,rst,cs,dc,wr,rd,bl,d0..d7[,d8..d15],mhz` | ESP32-S3 I80 only; use explicit pins. |
| `RGB` | `de,vsync,hsync,pclk,bl,B0..B4,G0..G5,R0..R4,mhz` | ESP32-S3; use explicit pins. Also write `:V`; use `:B,...,02` for this pin order. |
| `DSI` | `lanes,te,bl,rst,ldo_ch,ldo_mv,pclk_hz,lane_mbps,rgb_order,endian` | ESP32-P4 only. `pclk_hz` is Hz; `te`, `rgb_order`, and `endian` are presently unused. Also write `:V`. |

`name` is at most 15 characters. `width`/`height` are unrotated pixels. Use `bpp=1` for monochrome and `16` for color.

```ini
:H,SSD1306,128,64,1,I2C,3C,*,*,*
:H,ILI9341,240,320,16,SPI,1,*,*,*,*,*,*,*,40
:H,ILI9341,240,320,16,PAR,8,41,40,39,38,42,-1,8,9,10,11,12,13,14,15,20
:H,ST7262,800,480,16,RGB,40,41,39,42,2,15,16,4,45,48,47,21,14,8,3,46,9,1,5,6,7,12
:H,JD9165,1024,600,16,DSI,2,-1,23,27,3,2500,56000000,750,0,0
```

## 2. Controller initialization: `:I`, `:IC`, `:IS`, `:II`

`:I` begins normal initialization. `:IC` is the same, except SPI parameters are sent with DC low (for SSD13xx-style command-only controllers).

### SPI and PAR `:I` lines

Every line is `CMD,CNT[,ARG1,...,ARGn]`, all hexadecimal. `CMD` is the command byte; `ARG`s are its data bytes. `CNT` is **five-bit argument count plus a delay code**:

| `CNT` bits | Meaning |
|---|---|
| `CNT & 1F` | Number of argument bytes: `00`–`1F` (0–31). |
| `CNT & E0` | Delay: `00` none, `80` 150 ms, `A0` 10 ms, `E0` 500 ms. |

Add the values: `82` means 2 arguments then 150 ms; `A0` means no arguments then 10 ms. A wrong count shifts the remaining stream.

```ini
:I
01,A0
11,80
3A,01,55
C5,02,3E,28
E0,0F,0F,31,2B,0C,0E,08,4E,F1,37,07,10,03,0E,09,00
29,80
```

Keep each line below 255 characters and the combined `:I` + `:f` + `:p` data below 256 bytes (1024 on P4). To wait longer, repeat `00,E0` (DCS NOP + 500 ms). Do not trust an init-table `count` blindly: count the supplied arguments and convert delays to the three codes above.

### Other buses

| Bus | Initialization format |
|---|---|
| I2C | Under `:I`, one or two hex bytes per line; each line is sent as controller command bytes. Extra bytes are ignored; there is no delay syntax. |
| RGB + SPI init | `:IS,clk,mosi,cs,rst`, then normal `CMD,CNT,ARG...` lines. This is software 3-wire, 9-bit SPI; its count uses `CNT & 7F` and bit 7 is a delay flag. |
| RGB + I2C init | `:II,bus,addr`, then `reg,val` lines; a single hex value is a delay in milliseconds. |
| DSI | Under `:I`, write `cmd,len,data...,delay_ms`, all hex. Provide exactly `len` data bytes, then the delay byte: `11,01,00,78`. |

## 3. Common display sections

Write only options that apply to the chosen bus/controller. For a color SPI/PAR display, always write `:A`, `:R`, and all four `:0`–`:3` entries.

| Tag | Fields | Purpose / constraints |
|---|---|---|
| `:S` | `font,size,fg,bg,x,y` (D) | Splash and default colors. `font=-1` suppresses splash. |
| `:o` / `:O` | `opcode` (H) | Display off/on, e.g. `28` / `29`; not sent on PAR. |
| `:i` | `off,on` (H) | Inversion commands, often `20,21`; unsupported on DSI. |
| `:A` color SPI/PAR | `col_cmd,row_cmd,ramwr[,mode]` (H) | `mode=8` uses byte-wide start/end coordinates (SSD1331/SSD1351); otherwise 16-bit. `FF` RAMWR suppresses RAMWR. |
| `:A` I2C mono | `col_cmd,page_start,page_end,page_cmd,col_start[,col_end,ramwr]` (H) | Common form: `:A,00,10,40,00,02`. |
| `:A` SPI mono page | `col_off,00,00,page_off` (H) | The parser uses the **first** value for column offset. |
| `:F` + `:A` packed mono SPI | `:F,bytes_w,bytes_h,flags` (D), then `:A,col,row,ramwr,mode,xs,xe,ys,ye` | `:F` must precede `:A`; flags: `1` invert, `2` reverse Y. |
| `:R` | `madctl[,startline]` (H) | Rotation register, normally `36`; optional start-line command. |
| `:0`–`:3` | `madctl,xoff,yoff,touch` (H) | Per rotation. Offsets are hex. On RGB, use only `:n,touch`. |
| `:P` | `bits` (D) | Pixel transfer size; default 16, use `18` for RGB666 panels such as SPI ILI9488. |
| `:D` | `opcode` (H) | Dimmer command plus brightness byte; SPI/I2C only. |
| `:b` | `flags` (H) | Backlight: bit `01` active low; bit `02` digital on/off only. |
| `:B` | `lines,flags` (D) | LVGL buffer lines (default 40). Flags: `02` swap color bytes, `08` e-paper BUSY active low, `16` invert monochrome B/W, `32` use PSRAM buffers. Bit `01` DMA has no current effect. |
| `:V` RGB | `hpol,hfp,hpw,hbp,vpol,vfp,vpw,vbp,pclk_neg` (D) | `0` polarity means idle low. |
| `:V` DSI | `hfp,vfp,hbp,hsw,vsw,vbp` (D) | DSI order differs from RGB. |
| `:r` | `0..3` (D) | Initial rotation, overriding `DisplayRotate`. |
| `:M` | `xmin,xmax,ymin,ymax` (D) | Map raw touch to display coordinates; required for simple resistive touch. |

Touch transform is the fourth field of `:0`–`:3`, applied after `:M`. Values: `0` normal, `1` y/H−x, `2` W−x/H−y, `3` W−y/x, `4` W−x/y, `5` x/H−y; add `80` to swap x/y first.

## 4. E-paper only (`SPI`, `bpp=1`)

- Use SPI `miso` for BUSY and add `:B,lines,08` when BUSY is active low.
- `:T,full,partial,after` provides fallback waits in units of 10 ms. Defaults are `350,35,10`.
- `:f` and `:p` begin full/partial update command streams using the same `CMD,CNT,ARG...` format as `:I`.
- Select either 2-LUT (`:L,size,cmd` and `:l,size,cmd` followed by hex data), or command-only (`:f`/`:p` only). The 5-LUT syntax is `:L1`–`:L5,size,cmd` plus data and `:a,cmd1,cmd2,cmd3`, but the current parser routes `:a` into SPI address fields; treat 5-LUT as unverified and avoid it unless hardware-tested.

| Pseudo-opcode | Meaning |
|---|---|
| `60,01,T` | Toggle reset for `T` ms. |
| `61,00` / `62,00` | Load full / partial LUT. |
| `63,01,T` | Wait BUSY, or `T × 10` ms without BUSY. |
| `64,00` / `65,00` | Set full memory area / reset memory pointer. |
| `66,00` / `67,00` | Send framebuffer / clear RAM to `FF`. |
| `68,00` | Complete send-frame sequence. |
| `69,01,X` / `6A,01,X` | Stop when reset reason equals / differs from `X`. |

In 2-LUT and command-only modes, opcodes `60`–`6A` are reserved pseudo-opcodes: do not send real controller commands in that range.

## 5. uTouch (`:UTI`, `:UTT`, `:UTX`, `:UTY`)

Use uTouch rather than legacy `:TI`, `:TS`, and `:TR`; legacy tags require optional driver build flags. The touch name is at most 7 characters.

```ini
:UTI,name,I1,addr,rst,irq
<init script; must return non-zero>
:UTT
<touch script; bit 0 is touched, bits 8–15 are gesture>
:UTX
<x script>
:UTY
<y script>
```

- I2C form: `I1` or `I2`; `addr` is hexadecimal and cannot be `*`. `rst`/`irq` can be `*`.
- SPI form: `:UTI,name,S1,cs,rst,irq`; bus numbering matches `:H`; `*` maps to `TS SPI CS`, `TS RST`, and `TS IRQ`.
- Resistive PAR form: `:UTI,name,R`.
- An IRQ means `:UTT` runs only after an interrupt. Each script has a 64-byte compiled limit; keep it short.

| Opcode | Parameters | Effect |
|---|---|---|
| `RD` / `RDM` | `reg(H)` / `reg(H),count(D)` | Read 1 / many bytes from an 8-bit I2C register into `A[]`. |
| `RDW` / `RDWM` | `reg(H)` / `reg(H),count(D)` | Same with a 16-bit register. |
| `WR` / `WRW` | `reg,value` (H) | Write to 8-/16-bit I2C register. |
| `CP`, `CPM`, `CPR` | hex values | Compare `A[0]`, any listed value (`CPM` count is hex), or `R`. |
| `MV` | `index(D),mode(D)` | Copy `A[index]` to `R`: mode 1 byte, 2 big-endian word, 3 little-endian word. |
| `MVB` | `high(D),index(D)` | Copy `A[index]` into low (`0`) or high (`1`) byte of `R`. |
| `AND`, `SCL`, `LIM` | value | Mask `R`; scale `(R-offset)*factor`; cap `R`. |
| `RTF`, `RTT`, `RT` | — | Return 0 if `R==0`; return 0 if `R>0`; return `R`. |
| `XPT`, `GSRT` | threshold(D) | Read XPT2046 / simple resistive touch; coordinates enter `A[0..3]`. |
| `DBG` | id(D) | Log `R` and `A[0..3]`. |

Minimal known-good I2C capacitive pattern:

```ini
:UTI,FT5206,I1,38,*,*
RD A8
CP 11
RTF
RD A3
CP 64
RTF
RT
:UTT
RDM 00 16
MV 2 1
RT
:UTX
MV 3 2
RT
:UTY
MV 5 2
RT
```

## 6. Build and debug workflow

1. Copy the closest descriptor, change `:H` pinout/resolution, then preserve its `:I` sequence initially.
2. Port controller initialization in `CMD,CNT,ARG...` format. Verify every count in hex.
3. For color displays add `:o`, `:O`, `:A`, `:R`, and `:0`–`:3`; correct MADCTL, offsets, inversion, and `:B` byte swap one at a time.
4. Add touch only after graphics work. Map its orientation using the fourth rotation field, then calibrate with `:M` or `SCL`/`LIM`.
5. Confirm `Option A3`, the final `#`, controller sleep-out/display-on commands, bus/address, and the Tasmota log (`UTI: <name> initialized`). Add `DBG 1` to inspect touch state.

## Code-verified corrections to the public document

- SPI init counts use five bits (0–31), not four; RGB `:IS` uses seven bits.
- `SPI,3` and `SPI,4` are fast and slow software SPI; `dc=-1` is 9-bit SPI; MISO is e-paper BUSY.
- `:0`–`:3` offsets are hex; `:B`, `:S`, `:M`, `:T`, and `:V` are decimal. Do not omit a `:B` flag field (`:B,64` becomes `255` flags).
- `:T` and pseudo-opcode `63` time values are ×10 ms. I2C `:I` uses only the first two bytes of each line.
- DSI, packed monochrome (`:F`), 5-LUT `:a`, backlight flag `02`, address mode `8`, and PSRAM flag `32` are implemented but absent or incomplete in the public document.
