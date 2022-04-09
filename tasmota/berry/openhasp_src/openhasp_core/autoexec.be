# start openhasp
# package with
#   rm openhasp.tapp; zip -j -0 openhasp.tapp openhasp_core/*
import openhasp

# defer start to make sure all modules and tapp applications were loaded first
tasmota.set_timer(1000, /-> openhasp.start())
