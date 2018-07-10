/*
Copyright: Joachim Banzhaf, 2018

This file is part of the Joba_Tsl2561 Library.

    Joba_Tsl2561 is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Joba_Tsl2561 is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Joba_Tsl2561.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "Tsl2561.h"

Tsl2561::Tsl2561( TwoWire &wire ) : _addr(ADDR_NONE), _wire(wire), _status(ERR_OK) {
}

bool Tsl2561::available() {
  _wire.beginTransmission(_addr);
  return (_status = static_cast<status_t>(_wire.endTransmission())) == ERR_OK;
}

bool Tsl2561::begin( address_t addr ) {
  _addr = addr;
  return available();
}

bool Tsl2561::begin() {
  static address_t addr[] = { ADDR_GND, ADDR_FLOAT, ADDR_VDD };

  for( uint8_t i=0; i<sizeof(addr)/sizeof(addr[0]); i++ ) {
    if( begin(addr[i]) ) {
      return true;
    }
  }

  _addr = ADDR_NONE;
  return false;
}

bool Tsl2561::readByte( register_t reg, uint8_t &val ) {
  _wire.beginTransmission(_addr);
  _wire.write(reg | CONTROL_CMD);
  if( (_status = static_cast<status_t>(_wire.endTransmission(false))) == ERR_OK ) {
    if( _wire.requestFrom(_addr, 1) == 1 ) {
      val = static_cast<uint8_t>(_wire.read());
    }
    else {
      _status = ERR_RW;
    }
  }
  return _status == ERR_OK;
}

bool Tsl2561::readWord( register_t reg, uint16_t &val ) {
  _wire.beginTransmission(_addr);
  _wire.write(reg | CONTROL_CMD);
  if( (_status = static_cast<status_t>(_wire.endTransmission(false))) == ERR_OK ) {
    if( _wire.requestFrom(_addr, 2) == 2 ) {
      val = static_cast<uint16_t>(_wire.read()) & 0xff;
      val |= (static_cast<uint16_t>(_wire.read()) & 0xff) << 8;
    }
    else {
      _status = ERR_RW;
    }
  }
  return _status == ERR_OK;
}

bool Tsl2561::writeByte( register_t reg, uint8_t val ) {
  _wire.beginTransmission(_addr);
  _wire.write(reg | CONTROL_CMD);
  _wire.write(val);
  return (_status = static_cast<status_t>(_wire.endTransmission())) == ERR_OK;
}

Tsl2561::status_t Tsl2561::status() const {
  return _status;
}

Tsl2561::address_t Tsl2561::address() const {
  return _addr;
}

bool Tsl2561::id( uint8_t &id ) {
  return readByte(REG_ID, id);
}

uint8_t Tsl2561::type( uint8_t id ) {
  return id & PKG_ID;
}

uint8_t Tsl2561::revision( uint8_t id ) {
  return id & PKG_REV;
}

bool Tsl2561::packageCS( uint8_t id ) {
  return type(id) == PKG_CS;
}

bool Tsl2561::packageT_FN_CL( uint8_t id ) {
  return type(id) == PKG_T_FN_CL;
}

bool Tsl2561::on() {
  return writeByte(REG_CONTROL, POWER_ON);
}

bool Tsl2561::off() {
  return writeByte(REG_CONTROL, POWER_OFF);
}

bool Tsl2561::setSensitivity( bool gain, exposure_t exposure ) {
  return writeByte(REG_TIMING, (gain ? GAIN_ON : GAIN_OFF) | exposure);
}

bool Tsl2561::getSensitivity( bool &gain, exposure_t &exposure )
{
  uint8_t val;
  if( readByte(REG_TIMING, val) ) {
    gain = val & GAIN_ON;
    exposure = static_cast<exposure_t>(val & EXP_ON);
  }
  return _status == ERR_OK;
}

bool Tsl2561::fullLuminosity( uint16_t &luminosity ) {
  return readWord(REG_DATA0LOW, luminosity);
}

bool Tsl2561::irLuminosity( uint16_t &luminosity ) {
  return readWord(REG_DATA1LOW, luminosity);
}
