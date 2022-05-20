# generate all haspmota tapp files
rm haspmota.tapp; zip -j -0 haspmota.tapp haspmota_core/*
rm haspmota_demo.tapp ; zip -j -0 haspmota_demo.tapp haspmota_demo/* haspmota_core/haspmota.be
cp haspmota.tapp ../haspmota
cp haspmota_demo.tapp ../haspmota
