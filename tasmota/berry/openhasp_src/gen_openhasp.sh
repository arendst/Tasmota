# generate all openhasp tapp files
rm openhasp.tapp; zip -j -0 openhasp.tapp openhasp_core/*
rm openhasp_demo.tapp ; zip -j -0 openhasp_demo.tapp openhasp_demo/* openhasp_core/openhasp.be
cp openhasp.tapp ../openhasp
cp openhasp_demo.tapp ../openhasp
