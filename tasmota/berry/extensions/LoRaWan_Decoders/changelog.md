# Changelog

[v26.4.6.1]
### Added
- HAss discovery for DDS75L and LHT52 if `SetOption19 0` by UBWH (#24458)

[v26.4.6.0]
### Fixed
- GUI cache timer overflow
- Lock/Unlock icons

[v26.1.19.0]
### Changed
- LoRaWan End Device configuration Decoder File drop down box based on decoder files within `LoRaWan_Decoders.tapp` using function `path.listdir(<.tapp file>)` from Tasmota v15.2.0.3 before using the filelist (Theo Arends)

[v26.1.18.0]
### Changed
- LoRaWan End Device configuration Decoder File drop down box based on decoder files within `LoRaWan_Decoders.tapp` (Theo Arends)
- Build command `rm LoRaWan_Decoders.tapp; ls LoRaWan_Decoders > LoRaWan_Decoders/filelist; zip -j -0 LoRaWan_Decoders.tapp LoRaWan_Decoders/*`

[v26.1.15.0]
### Added
- First public release as `LoRaWan_Decoders.tapp` extension (Theo Arends)
- Build command `rm LoRaWan_Decoders.tapp; zip -j -0 LoRaWan_Decoders.tapp LoRaWan_Decoders/*`