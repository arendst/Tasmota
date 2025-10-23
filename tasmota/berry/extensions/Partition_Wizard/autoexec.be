# rm Partition_Wizard.tapp; zip -j -0 Partition_Wizard.tapp Partition_Wizard/autoexec.be Partition_Wizard/partition_wizard.bec Partition_Wizard/manifest.json
do                          # embed in `do` so we don't add anything to global namespace
  import introspect
  var partition_wizard = introspect.module('partition_wizard', true)     # load module but don't cache
  tasmota.add_extension(partition_wizard)
end

# to remove:
#       tasmota.unload_extension('Partition Wizard')
