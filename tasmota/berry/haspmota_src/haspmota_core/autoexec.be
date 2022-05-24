# start haspmota
# package with
#   rm haspmota.tapp; zip -j -0 haspmota.tapp haspmota_core/*
import haspmota

# defer start to make sure all modules and tapp applications were loaded first
tasmota.set_timer(1000, /-> haspmota.start())
