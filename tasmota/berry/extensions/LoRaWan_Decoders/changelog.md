# Changelog

[v25.1.19.0]
### Changed
- LoRaWan End Device configuration Decoder File drop down box based on decoder files within `LoRaWan_Decoders.tapp` using function `path.listdir(<.tapp file>)` from Tasmota v15.2.0.3 before using the filelist (Theo Arends)

[v25.1.18.0]
### Changed
- LoRaWan End Device configuration Decoder File drop down box based on decoder files within `LoRaWan_Decoders.tapp` (Theo Arends)
- Build command `rm LoRaWan_Decoders.tapp; ls LoRaWan_Decoders > LoRaWan_Decoders/filelist; zip -j -0 LoRaWan_Decoders.tapp LoRaWan_Decoders/*`

[v25.1.15.0]
### Added
- First public release as `LoRaWan_Decoders.tapp` extension (Theo Arends)
- Build command `rm LoRaWan_Decoders.tapp; zip -j -0 LoRaWan_Decoders.tapp LoRaWan_Decoders/*`