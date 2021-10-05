# Dummy Energy Driver - Shows dummy values to demonstrate the interface
# Requires a build with USE_BERRY_ENERGY_DRIVER (xnrg_29_berryenergy.ino)
# and GPIO configuration with Option_A6 to enable xnrg_9_berryenergy

import string

class NRG : Driver

  var temperature

  # -----------------------------------------------------------------------------------
  # init the driver
  def init()
    self.temperature = 39.16

    energy.voltage_common = 1
    energy.frequency_common = 1
    energy.frequency = 50.0
    energy.use_overtemp = 1
    energy.phase_count = 3
    tasmota.global.energy_driver = 29
    log("NRG: initialized", 2)
  end

  # -----------------------------------------------------------------------------------
  # update Tasmota Energy from raw data
  def update_energy()
    if true #energy.power_on # Powered on
      energy.voltage = 239.45
      # channel 0
      energy.current = 3.34579
      energy.active_power = energy.voltage * energy.current
      # channel 1
      if energy.phase_count > 1
        energy.current_2 = 7.3743
        energy.active_power_2 = energy.voltage * energy.current_2 * 0.8
      end
      # channel 2
      if energy.phase_count > 2
        energy.current_3 = 1.2222
        energy.active_power_3 = energy.voltage * energy.current_3 * 0.7
      end
    else # Powered off
      energy.voltage = 0.0
      # channel 0
      energy.current = 0.0
      energy.active_power = 0.0
      # channel 1
      if energy.phase_count > 1
        energy.current_2 = 0.0
        energy.active_power_2 = 0.0
      end
      # channel 2
      if energy.phase_count > 2
        energy.current_3 = 0.0
        energy.active_power_3 = 0.0
      end
    end
  end

  # -----------------------------------------------------------------------------------
  # Every second : send full frame reques
  def every_second()
    self.update_energy()
  end

  # -----------------------------------------------------------------------------------
  def web_sensor()
    tasmota.web_send_decimal(string.format("{s}Dummy Temperature{m}%.1f°C{e}", self.temperature))
  end

  # -----------------------------------------------------------------------------------
  def json_append()
    tasmota.response_append(string.format(",\"Dummy\":{\"Temperature\":%.1f}", self.temperature))
  end
end

# Create the intance
BerryEnergy = NRG()
# Setup as a tasmota driver
if BerryEnergy
  tasmota.add_driver(BerryEnergy)
end
