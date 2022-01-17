/*
  xdrv_52_3_berry_MI32.ino - Berry scripting language, native functions

  Copyright (C) 2021 Christian Baars & Stephan Hadinger, Berry language by Guan Wenliang https://github.com/Skiars/berry

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#ifdef USE_BERRY

#include <berry.h>

#if defined(USE_MI_ESP32) && !defined(USE_BLE_ESP32)

/*********************************************************************************************\
 * Native functions mapped to Berry functions
 * 
 * 
\*********************************************************************************************/
extern "C" {

/********************************************************************
**  MI32 - sensor specific functions
********************************************************************/ 

  extern uint32_t MI32numberOfDevices();
  extern const char * MI32getDeviceName(uint32_t slot);
  extern void MI32setBatteryForSlot(uint32_t slot, uint8_t value);
  extern void MI32setHumidityForSlot(uint32_t slot, float value);
  extern void MI32setTemperatureForSlot(uint32_t slot, float value);
  extern uint8_t * MI32getDeviceMAC(uint32_t slot);

  int be_MI32_devices(bvm *vm);
  int be_MI32_devices(bvm *vm) {
    uint32_t devices  = MI32numberOfDevices();
    be_pushint(vm, devices);
    be_return(vm);
  }

  int be_MI32_set_bat(bvm *vm);
  int be_MI32_set_bat(bvm *vm){    
    int32_t argc = be_top(vm); // Get the number of arguments
    if (argc == 3 && be_isint(vm, 2) && be_isint(vm, 3)) {
      uint32_t slot = be_toint(vm, 2);
      int32_t bat_val = be_toint(vm, 3);
      MI32setBatteryForSlot(slot,bat_val);
      be_return(vm); // Return
    }
    be_raise(vm, kTypeError, nullptr);
  }

  int be_MI32_get_name(bvm *vm);
  int be_MI32_get_name(bvm *vm){    
    int32_t argc = be_top(vm); // Get the number of arguments
    if (argc == 2 && be_isint(vm, 2)) {
      uint32_t slot = be_toint(vm, 2);
      const char * name = MI32getDeviceName(slot);
      be_pushstring(vm,name);
      be_return(vm); // Return
    }
    be_raise(vm, kTypeError, nullptr);
  }

  int be_MI32_get_MAC(bvm *vm);
  int be_MI32_get_MAC(bvm *vm){    
    int32_t argc = be_top(vm); // Get the number of arguments
    if (argc == 2 && be_isint(vm, 2)) {
      uint32_t slot = be_toint(vm, 2);
      uint8_t *buffer = MI32getDeviceMAC(slot);
      size_t len = 6;
      if(buffer != NULL) {
        be_pushbytes(vm,buffer,len);
        be_return(vm); // Return
      }
    }
    be_raise(vm, kTypeError, nullptr);
  }

  int be_MI32_set_hum(bvm *vm);
  int be_MI32_set_hum(bvm *vm){    
    int32_t argc = be_top(vm); // Get the number of arguments
    if (argc == 3 && be_isint(vm, 2) && be_isreal(vm, 3)) {
      uint32_t slot = be_toint(vm, 2);
      float hum_val = be_toreal(vm, 3);
      MI32setHumidityForSlot(slot,hum_val);
      be_return(vm); // Return
    }
    be_raise(vm, kTypeError, nullptr);
  }

  int be_MI32_set_temp(bvm *vm);
  int be_MI32_set_temp(bvm *vm){    
    int32_t argc = be_top(vm); // Get the number of arguments
    if (argc == 3 && be_isint(vm, 2) && be_isreal(vm, 3)) {
      uint32_t slot = be_toint(vm, 2);
      float temp_val = be_toreal(vm, 3);
      MI32setTemperatureForSlot(slot,temp_val);
      be_return(vm); // Return
    }
    be_raise(vm, kTypeError, nullptr);
  }


/********************************************************************
**  BLE - generic BLE functions
********************************************************************/ 
  extern void MI32setBerryAdvCB(void* function, uint8_t *buffer);
  extern void MI32setBerryConnCB(void* function, uint8_t *buffer);
  extern bool MI32runBerryConnection(uint8_t operation);
  extern bool MI32setBerryCtxSvc(const char *Svc);
  extern bool MI32setBerryCtxChr(const char *Chr);
  extern bool MI32setBerryCtxMAC(uint8_t *MAC, uint8_t type);


  int be_BLE_reg_conn_cb(bvm *vm);
  int be_BLE_reg_conn_cb(bvm *vm){    
    int32_t argc = be_top(vm); // Get the number of arguments
    if (argc == 3 && be_iscomptr(vm, 2)) {
      void* cb = be_tocomptr(vm, 2);
      size_t len;
      uint8_t * buf = (uint8_t*)be_tobytes(vm, 3, &len);
      MI32setBerryConnCB(cb,buf);
      be_return(vm);
    }
    be_raise(vm, kTypeError, nullptr);
  }

  int be_BLE_reg_adv_cb(bvm *vm);
  int be_BLE_reg_adv_cb(bvm *vm){    
    int32_t argc = be_top(vm); // Get the number of arguments
    if (argc == 3 && be_iscomptr(vm, 2)) {
      void* cb = be_tocomptr(vm, 2);
      size_t len;
      uint8_t * buf = (uint8_t*)be_tobytes(vm, 3, &len);
      MI32setBerryAdvCB(cb,buf);
      be_return(vm); // Return
    }
    else if(argc == 2 && be_isint(vm, 2)){
      if(be_toint(vm, 2) == 0){
        MI32setBerryAdvCB(NULL,NULL);
        be_return(vm); // Return
      }
    }
    be_raise(vm, kTypeError, nullptr);
  }

  int be_BLE_set_MAC(bvm *vm);
  int be_BLE_set_MAC(bvm *vm){    
    int32_t argc = be_top(vm); // Get the number of arguments
    if (argc > 1 && be_isbytes(vm, 2)) {
      size_t len = 6;
      uint8_t type = 0;
      if(argc == 3 && be_isint(vm, 3)){
        type = be_toint(vm,3);
      }
      if (MI32setBerryCtxMAC((uint8_t*)be_tobytes(vm, 2, &len),type)) be_return(vm);
    }
    be_raise(vm, kTypeError, nullptr);
  }

  int be_BLE_set_service(bvm *vm);
  int be_BLE_set_service(bvm *vm){    
    int32_t argc = be_top(vm); // Get the number of arguments
    if (argc == 2 && be_isstring(vm, 2)) {
      if (MI32setBerryCtxSvc(be_tostring(vm, 2))) be_return(vm);
    }
    be_raise(vm, kTypeError, nullptr);
  }

  int be_BLE_set_characteristic(bvm *vm);
  int be_BLE_set_characteristic(bvm *vm){    
    int32_t argc = be_top(vm); // Get the number of arguments
    if (argc == 2 && be_isstring(vm, 2)) {
      if (MI32setBerryCtxChr(be_tostring(vm, 2))) be_return(vm);
    }
    be_raise(vm, kTypeError, nullptr);
  }

  int be_BLE_run(bvm *vm);
  int be_BLE_run(bvm *vm){    
    int32_t argc = be_top(vm); // Get the number of arguments
    if (argc == 2 && be_isint(vm, 2)) {
      if (MI32runBerryConnection(be_toint(vm, 2))) be_return(vm);
    }
    be_raise(vm, kTypeError, nullptr);
  }
} //extern "C"


#endif // USE_MI_ESP32

#endif  // USE_BERRY

/*
BLE.set_svc
BLE.set_chr

BLE.set_MAC
BLE.run(op)
be_BLE_op:
1 read
2 write
3 subscribe
4 unsubscribe
5 disconnect

11 read once, then disconnect
12 write once, then disconnect
13 subscribe once, then disconnect
14 unsubscribe once, then disconnect

BLE.conn_cb(cb,buffer)
BLE.adv_cb(cb,buffer)

MI32.devices()
MI32.get_name(slot)
MI32.get_MAC(slot)
MI32.set_bat(slot,int)
MI32.set_hum(slot,float)
MI32.set_temp(slot,float)

*/
