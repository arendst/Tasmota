/*
  xnrg_25_v9240.ino - v9240 energy sensor support for Tasmota

  SPDX-FileCopyrightText: 2025  Ivan Chopa (@aquaforum)

  SPDX-License-Identifier: GPL-3.0-only
*/

#ifdef USE_ENERGY_SENSOR
#ifdef USE_V9240
/*********************************************************************************************\
 * Support the Vango Technologies V924x ULTRALOW POWER, UART, SINGLE-PHASE, POWER MEASUREMENT IC
\*********************************************************************************************/

#define XNRG_25             25

#include <TasmotaSerial.h>

#define V9240_SERIAL_BAUDRATE 19200


namespace Address
{
    static const uint16_t SysCtrl   = 0x0180;
    static const uint16_t AnaCtrl0  = 0x0182;
    static const uint16_t AnaCtrl1  = 0x0183;


    static const uint16_t PAC       = 0x00F6;
    static const uint16_t PHC       = 0x00F7;
    static const uint16_t PADCC     = 0x00F8;
    static const uint16_t QAC       = 0x00F9;
    static const uint16_t QADCC     = 0x00FB;
    static const uint16_t IAC       = 0x00FD;
    static const uint16_t IADCC     = 0x00FE;
    static const uint16_t UC        = 0x00FF;
    static const uint16_t IAADCC    = 0x0104;
    static const uint16_t UADCC     = 0x0106;
    static const uint16_t BPFPARA   = 0x0107;
    static const uint16_t UDCC      = 0x0108;
    static const uint16_t CKSUM     = 0x0109;

    static const uint16_t RW_START  = PAC;

    static const uint16_t SysStsClr = 0x019D;
    static const uint16_t SFTRST    = 0x01BF;

    static const uint16_t SysSts    = 0x00CA;
    static const uint16_t FREQINST  = 0x00CB;
    static const uint16_t PAINST    = 0x00CC;
    static const uint16_t QINST     = 0x00CD;
    static const uint16_t IAINST    = 0x00CE;
    static const uint16_t UINST     = 0x00CF;
    static const uint16_t PAAVG     = 0x00D0;
    static const uint16_t QAVG      = 0x00D1;
    static const uint16_t FREQAVG   = 0x00D2;
    static const uint16_t IAAVG     = 0x00D3;
    static const uint16_t UAVG      = 0x00D4;
    static const uint16_t UDCINST   = 0x00D9;
    static const uint16_t IADCINST  = 0x00DA;
    static const uint16_t ZXDATREG  = 0x00DC;
    static const uint16_t ZXDAT     = 0x00DD;
    static const uint16_t PHDAT     = 0x00DE;

    static const uint16_t T8BAUD     = 0x00E0;

    static const uint16_t RO_START    = SysSts;
} ;


// register structures

union SysSts // 0x0CA
{
    int32_t reg;
    struct __attribute__ ((packed))
    {
        uint32_t ref:1;       // 0
        uint32_t phsdone:1;   // 1
        uint32_t chkerr:1;    // 2
        uint32_t rstsrc:3;    // 3:5
        uint32_t pdn_r:1;     // 6
        uint32_t pdn:1;       // 7
        uint32_t bisterr:1;   // 8
        uint32_t phsdone_r:1; // 9
        uint32_t Resered1:1;  // 10
        uint32_t usign:1;     // 11
    };
} ;

union SysCtrl // 0x0180
{
    int32_t reg;
    struct __attribute__ ((packed))
    {
        uint32_t Reserved0:1;    // 0
        uint32_t pgau:1;         // 1
        uint32_t bphpf:1;        // 2
        uint32_t iesul:1;        // 3
        uint32_t iepdn:1;        // 4
        uint32_t iehse:1;        // 5
        uint32_t rcx12:1;        // 6
        uint32_t rctrim:5;       // 7:11
        uint32_t shorti:1;       // 12
        uint32_t shortu:1;       // 13
        uint32_t restl:2;        // 14:15
        uint32_t rest:3;         // 16:18
        uint32_t meaclksel:1;    // 19
        uint32_t adcclksel:2;    // 20:21
        uint32_t gai:2;          // 22:23
        uint32_t Reserved1:2;    // 24:25
        uint32_t gu:1;           // 26
        uint32_t adciapdn:1;     // 27
        uint32_t Reserved2:1;    // 28
        uint32_t adcupdn:1;      // 29
        uint32_t Reserved3:2;    // 30:31
    };
};


union AnaCtrl0 // 0x0182
{
    int32_t reg;
    struct __attribute__ ((packed))
    {
        uint32_t Reserved1:8; // 0:7
        uint32_t IT:2;        // 8:9
        uint32_t Reserved2:22;  // 10:31
    };
};

union AnaCtrl1 // 0x0183
{
    int32_t reg;
    struct __attribute__ ((packed))
    {
        uint32_t Reserved1:28;  // 0:27
        uint32_t CSEL:2;        // 28:29
        uint32_t Reserved2:2;   // 30:31
    };
};

union SysStsClr // 0x019D
{
    int32_t reg;
    struct __attribute__ ((packed))
    {
        uint32_t Reserved1:6;  // 0:5
        uint32_t pdn_clr:1;    // 6
        uint32_t Reserved2:2;  // 7:8
        uint32_t phsdone_clr:1;// 9
        uint32_t Reserved3:22; // 10:31
    };
};

// communication structures
union packet
{
    struct __attribute__ ((packed)) {
        uint8_t header;
        uint16_t ctrl:4;
        uint16_t addr_h:4;
        uint16_t addr_l:8;
        uint32_t data;
        uint8_t check;
    } ;
    char buff[8];
} ;




class V9240
{
private:
    static constexpr size_t rw_len = 23;
    static constexpr size_t ro_len = 22;
    static constexpr size_t buffer_size = 128; // max response + max query

    static constexpr int32_t header = 0x7d;
    static constexpr int32_t ctrl_read = 0x1;
    static constexpr int32_t ctrl_write = 0x2;

    explicit V9240();
    ~V9240();

public:
    enum parameter{
        Voltage = 0,
        Amperage,
        Frequency,
        Power,
        Reactive
    };

    static V9240& instance();

    bool open(char const*);
    void close();
    void start();
    bool reset();
    inline void v9240_loop() {read_data_from_port();};


    bool process_command();

    float value(const parameter p) const;
    inline float operator [] (const parameter p) const {return  value(p);};
private:
    bool read(int32_t *ptr,uint16_t address, size_t n);
    bool write(int16_t address, int32_t data);
    void set_checksum();
    inline  char calc_check(char *buff,size_t len);

private :
    void read_data_from_port();
    void send_next();

private:
    TasmotaSerial *port;

    size_t step = 0;

    size_t next_read_len =  0;
    char serial_buff[buffer_size];
    int32_t *ptr_read = nullptr;
    uint32_t start_pause_timer = 0;
    uint32_t timeout = 0 ;

    // chip memory
    int32_t rw_mem[rw_len];
    int32_t ro_mem[ro_len];

    // RW Control & Calibration registers
    volatile SysCtrl &SYSCTRL; // rw_mem[0];
    volatile AnaCtrl0 &ANACTRL0;
    volatile AnaCtrl1 &ANACTRL1;

    volatile int32_t &UADCC;
    volatile int32_t &IAADCC ;
    volatile int32_t &PAC  ;   // = 1;
    volatile int32_t &PADCC;   // = 0;
    volatile int32_t &PHC;     // = 0;
    volatile int32_t &QAC;     // = 1;
    volatile int32_t &QADCC;   // = 0;
    volatile int32_t &IAC;     // = 0;
    volatile int32_t &IADCC;   // = 0;
    volatile int32_t &UC;      // = -1103500000;
    volatile int32_t &UDCC;    // = 1196289;
    volatile int32_t &BPFPARA; // = 0x806764B6;
    volatile int32_t &CKSUM;   // = 0;

    // RO Meterin control register
    volatile const SysSts  &SYSSTS;
    volatile const int32_t &FREQINST;
    volatile const int32_t &PAINST;
    volatile const int32_t &QINST;
    volatile const int32_t &IAINST;
    volatile const int32_t &UINST;
    volatile const int32_t &PAAVG;
    volatile const int32_t &QAVG;
    volatile const int32_t &FREQAVG;
    volatile const int32_t &IAAVG;
    volatile const int32_t &UAVG;
    volatile const int32_t &UDCINST;
    volatile const int32_t &IADCINST;
    volatile const int32_t &ZXDATREG;
    volatile const int32_t &ZXDAT;
    volatile const int32_t &PHDAT;
    volatile const int32_t &T8BAUD;

};

V9240& V9240::instance()
{
    static V9240 chip;
    return chip;
}


V9240::V9240() :   port(nullptr)
// mapping registers to arrays
  , SYSCTRL (*reinterpret_cast<SysCtrl*>(rw_mem + 0))
  , ANACTRL0(*reinterpret_cast<AnaCtrl0*>(rw_mem + 1))
  , ANACTRL1(*reinterpret_cast<AnaCtrl1*>(rw_mem + 2))
  , UADCC  (*(rw_mem+3+ (Address::UADCC    - Address::RW_START )))
  , IAADCC (*(rw_mem+3+ (Address::IAADCC   - Address::RW_START )))
  , PAC    (*(rw_mem+3+ (Address::PAC      - Address::RW_START )))
  , PADCC  (*(rw_mem+3+ (Address::PADCC    - Address::RW_START )))
  , PHC    (*(rw_mem+3+ (Address::PHC      - Address::RW_START )))
  , QAC    (*(rw_mem+3+ (Address::QAC      - Address::RW_START )))
  , QADCC  (*(rw_mem+3+ (Address::QADCC    - Address::RW_START )))
  , IAC    (*(rw_mem+3+ (Address::IAC      - Address::RW_START )))
  , IADCC  (*(rw_mem+3+ (Address::IADCC    - Address::RW_START )))
  , UC     (*(rw_mem+3+ (Address::UC       - Address::RW_START )))
  , UDCC   (*(rw_mem+3+ (Address::UDCC     - Address::RW_START )))
  , BPFPARA(*(rw_mem+3+ (Address::BPFPARA  - Address::RW_START )))
  , CKSUM  (*(rw_mem+3+ (Address::CKSUM    - Address::RW_START )))

  , SYSSTS  (*reinterpret_cast<SysSts*>(ro_mem + (Address::SysSts - Address::RO_START)))
  , FREQINST(*(ro_mem + (Address::FREQINST - Address::RO_START )))
  , PAINST  (*(ro_mem + (Address::PAINST   - Address::RO_START )))
  , QINST   (*(ro_mem + (Address::QINST    - Address::RO_START )))
  , IAINST  (*(ro_mem + (Address::IAINST   - Address::RO_START )))
  , UINST   (*(ro_mem + (Address::UINST    - Address::RO_START )))
  , PAAVG   (*(ro_mem + (Address::PAAVG    - Address::RO_START )))
  , QAVG    (*(ro_mem + (Address::QAVG     - Address::RO_START )))
  , FREQAVG (*(ro_mem + (Address::FREQAVG  - Address::RO_START )))
  , IAAVG   (*(ro_mem + (Address::IAAVG    - Address::RO_START )))
  , UAVG    (*(ro_mem + (Address::UAVG     - Address::RO_START )))
  , UDCINST (*(ro_mem + (Address::UDCINST  - Address::RO_START )))
  , IADCINST(*(ro_mem + (Address::IADCINST - Address::RO_START )))
  , ZXDATREG(*(ro_mem + (Address::ZXDATREG - Address::RO_START )))
  , ZXDAT   (*(ro_mem + (Address::ZXDAT    - Address::RO_START )))
  , PHDAT   (*(ro_mem + (Address::PHDAT    - Address::RO_START )))
  , T8BAUD  (*(ro_mem + (Address::T8BAUD   - Address::RO_START - 1 )))
{
    memset(rw_mem,0,sizeof (rw_mem));
    memset(ro_mem,0,sizeof (ro_mem));

    // its calibration coeficients for my chip

    PAC      = EnergyGetCalibration(ENERGY_POWER_CALIBRATION, 0);
    PADCC    = 0;
    PHC      = EnergyGetCalibration(ENERGY_FREQUENCY_CALIBRATION, 0);
    QAC      = -1;
    QADCC    = 0;
    IAC      = EnergyGetCalibration(ENERGY_CURRENT_CALIBRATION, 0);
    IADCC    = 75969; // 16758789;

    UC       = EnergyGetCalibration(ENERGY_VOLTAGE_CALIBRATION);
    UDCC     = 3596949; // 1196289;

    BPFPARA  = 0x806764B6;

}

V9240::~V9240()
{
    close();
}

void V9240::start()
{
    if(port != nullptr)
    {
        step = 0;
        port->flush();
        read(ro_mem,Address::SysSts,1);
        timeout = millis();
    }
}

float V9240::value(const V9240::parameter p) const
{
    float v = 0;
    switch (p) {
    case Voltage:
        v = float(UAVG)*1e-6;
        break;
    case Amperage:
        v = float(IAAVG)*1e-6;
        break;
    case Frequency:
        v = 0.00390625*V9240_SERIAL_BAUDRATE*T8BAUD/float(FREQAVG);
        break;
    case Power:
        v = float(PAAVG)*1e-3;
        break;
    case Reactive:
        v = float(QAVG)*1e-3;
        break;
    default:
        break;
    }
    return v;
}


void V9240::set_checksum()
{
//    CKSUM = UINT32_MAX - std::accumulate(rw_mem,rw_mem+rw_len-1 ,0);
    int init = 0;
    for (uint32_t i = 0; i < rw_len-1; i++) {
      init += rw_mem[i];
    }
    CKSUM = UINT32_MAX - init;
    write(Address::CKSUM,CKSUM);
}

char V9240::calc_check(char *buff, size_t len)
{
//    return ~std::accumulate(buff,buff+len,0)+0x33;
    char init = 0;
    for (uint32_t i = 0; i < len; i++) {
      init += buff[i];
    }
    return ~init + 0x33;
}


void V9240::send_next()
{
    port->flush();

    switch (step++) {
    case 0:
        read(rw_mem,Address::SysCtrl,1);
        break;
    case 1:
        write(Address::AnaCtrl0,ANACTRL0.reg);
        break;
    case 2:
        // Configure CSEL (
        ANACTRL1.CSEL  = 1;
        write(Address::AnaCtrl1,ANACTRL1.reg);
        break;
    case 3:
        // Disable internal PGA
        SYSCTRL.gu = 1;
        SYSCTRL.gai = 3;
        // Enable ADC
        SYSCTRL.adcupdn  = 1;
        SYSCTRL.adciapdn = 1;
        write(Address::SysCtrl,SYSCTRL.reg);
        break;
    case 4:
        // DC calibratio. Paragraph 7.4 page 33
        SYSCTRL.shortu = 1 ;
        SYSCTRL.shorti = 1 ;
        write(Address::SysCtrl,SYSCTRL.reg);
        break;
    case 5:
        read(ro_mem, Address::RO_START,ro_len);
        break;
    case 6:
        UADCC  = UDCINST;
        IAADCC = IADCINST;

        SYSCTRL.shortu = 0 ;
        SYSCTRL.shorti = 0 ;
        write(Address::SysCtrl,SYSCTRL.reg);
        break;
    case 7:
        // Write calibration parameter
        write(Address::UADCC   ,UADCC);
        break;
    case 8:
        write(Address::IAADCC  ,IAADCC);
        break;
    case 9:
        write(Address::PAC     ,PAC);
        break;
    case 10:
        write(Address::PADCC   ,PADCC);
        break;
    case 11:
        write(Address::QAC     ,QAC);
        break;
    case 12:
        write(Address::QADCC   ,QADCC);
        break;
    case 13:
        write(Address::PHC     ,PHC);
        break;
    case 14:
        write(Address::IAC     ,IAC);
        break;
    case 15:
        write(Address::IADCC   ,IADCC);
        break;
    case 16:
        write(Address::UC      ,UC);
        break;
    case 17:
        write(Address::UDCC    ,UDCC);
        break;
    case 18:
        write(Address::BPFPARA ,BPFPARA);
        break;
    case 19:
        set_checksum();
        AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_DEBUG "*** V9240 started"));
        break;
    case 20:
        read(&ro_mem[ro_len-1],Address::T8BAUD,1);
        break;
    default:
        read(ro_mem,Address::RO_START,ro_len-1);
        step = 20;
    }

    timeout = millis();
}

// next methods is platfrm-specific (serial port & Qt signals)
void V9240::read_data_from_port()
{
    if(next_read_len != 0 )
    {
        auto bytes = port->available();

        if(bytes >= next_read_len)
        {
            port->read(serial_buff,next_read_len);

            char checksum =  calc_check(serial_buff+sizeof(packet),next_read_len-sizeof(packet)-1);
            if(checksum == serial_buff[next_read_len-1])
            {
                packet &recv = *reinterpret_cast<packet*>( serial_buff + sizeof (packet) );
                if(recv.ctrl == ctrl_read)
                {
                    int32_t *data = (int32_t*)(serial_buff + sizeof (packet) + 3);
                    memcpy(ptr_read,data,sizeof (int32_t) * (recv.addr_l==0 ? 1 : recv.addr_l));
                }
            }
            else {
                AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_DEBUG "*** V9240 checksum error in: %d calc: %d")
                       ,uint32_t(serial_buff[next_read_len-1]),uint32_t(checksum));
            }
            next_read_len = 0;
            ptr_read = nullptr;

            start_pause_timer = millis();
            timeout = 0;
        }
        else if(timeout > 0 && millis() - timeout > 1000)
        {
            AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_DEBUG "*** V9240 timeout"));
            start_pause_timer = 0;
            start();
        }
    }
    else {
        if(start_pause_timer != 0 && millis()-start_pause_timer > 10)
        {
            send_next();
            start_pause_timer = 0;
        }
    }
}


bool V9240::open(char const *portName)
{
    if(port != nullptr)
        return false;

    port = new TasmotaSerial(Pin(GPIO_V9240_RX),Pin(GPIO_V9240_TX),1,0,256,false);
    if (port->hardwareSerial()) {
        ClaimSerial();
        port->begin(V9240_SERIAL_BAUDRATE,SERIAL_8O1);
    }
    return true;
}

void V9240::close()
{
    if(port != nullptr)
    {
        port->end();
        delete port;
        port = nullptr;
    }
}

bool V9240::reset()
{
    if(port != nullptr)
    {
        port->begin(100,SERIAL_8O1);
        port->write(uint8_t(0));
        char zero;
        port->read(&zero,1);
        port->begin(V9240_SERIAL_BAUDRATE,SERIAL_8O1);
    }
    return true;
}


bool V9240::read(int32_t *ptr, uint16_t address, size_t n)
{
    packet p;
    p.header = header;
    p.ctrl = ctrl_read;
    p.addr_h = (address&0x0F00)>>8;
    p.addr_l = address&0x00FF;
    p.data = n;
    p.check = calc_check(p.buff,sizeof(p)-1);

    next_read_len = sizeof (p) + 4 + sizeof (int32_t)*n;
    ptr_read = ptr;

    port->write(p.buff,sizeof (p));

    return true;
}

bool V9240::write(int16_t address, int32_t data)
{
    packet p;
    p.header = header;
    p.ctrl = ctrl_write;
    p.addr_h = (address&0x0F00)>>8;
    p.addr_l = address&0x00FF;
    p.data = data;
    p.check = calc_check(p.buff,sizeof(p)-1);

    next_read_len = sizeof (p) + 4;

    port->write(p.buff,sizeof (p));

    return true;
}

bool V9240::process_command()
{
    float value = atof(XdrvMailbox.data);
    XdrvMailbox.payload = 0;

    auto calc_cor = [] (float new_value,float old_value,int32_t k) -> int32_t
    { return int32_t( (float(INT_MAX) *( new_value - old_value ) + float(k) * new_value ) / old_value ); };

    switch (Energy->command_code) {
    case CMND_POWERCAL:
//        PADCC = calc_cor (value,(*this)[Power],P);
        break;
    case CMND_VOLTAGECAL:
//        UDCC = float(INT_MAX)*value;
        break;
    case CMND_CURRENTCAL:
//        IADCC = float(INT_MAX)*value;
        break;
    case CMND_FREQUENCYCAL:
        PHC = value ;
        EnergySetCalibration(ENERGY_FREQUENCY_CALIBRATION, PHC, 0);
        break;
    case CMND_POWERSET:
        PAC = calc_cor (value,(*this)[Power],PAC);
        EnergySetCalibration(ENERGY_POWER_CALIBRATION, PAC, 0);
        break;
    case CMND_VOLTAGESET:
        UC = calc_cor (value,(*this)[Voltage],UC);
        EnergySetCalibration(ENERGY_VOLTAGE_CALIBRATION, UC, 0);
        break;
    case CMND_CURRENTSET:
        IAC = calc_cor (value,(*this)[Amperage],IAC);
        EnergySetCalibration(ENERGY_CURRENT_CALIBRATION, IAC, 0);
        break;
    case CMND_FREQUENCYSET:        
        break;
    case CMND_MODULEADDRESS:
        break;
    case CMND_ENERGYCONFIG:
        break;
    default:
        break;
    }

    step = 0;

    return true;
}

void Xnrg34Init()
{
    Energy->phase_count = 1;
    Energy->voltage_common = true;    // Phase voltage = false, Common voltage = true
    Energy->frequency_common = true;  // Phase frequency = false, Common frequency = true
    Energy->type_dc = false;          // AC = false, DC = true;
    Energy->use_overtemp = true;      // Use global temperature for overtemp detection
    V9240::instance().start();
}

void Xnrg34Second()
{
    static int32_t last_power = 0;

    if (Energy->power_on) {
        Energy->data_valid[0] = 0;
        Energy->frequency[0] = V9240::instance()[V9240::Frequency];
        Energy->voltage[0] = V9240::instance()[V9240::Voltage];
        Energy->current[0] = V9240::instance()[V9240::Amperage];
        Energy->active_power[0] = V9240::instance()[V9240::Power];
//        Energy->reactive_power[0] = V9240::instance()[V9240::Reactive];

        Energy->kWhtoday_delta[0] += (Energy->active_power[0]+last_power) * 500.0 / 36.0; // trapezoid integration
        last_power = Energy->active_power[0];
        EnergyUpdateTotal();
    }
}

void Xnrg34PreInit()
{
    if (PinUsed(GPIO_V9240_RX) && PinUsed(GPIO_V9240_TX))
    {
      TasmotaGlobal.energy_driver = XNRG_25;
      V9240::instance().open("");
      V9240::instance().reset();
    }

}



bool Xnrg25(uint32_t function) {
    bool result = false;

    switch (function) {
    case FUNC_LOOP:
        V9240::instance().v9240_loop();
        break;
    case FUNC_ENERGY_EVERY_SECOND:
        Xnrg34Second();
        break;
    case FUNC_EVERY_250_MSECOND:
        break;
    case FUNC_COMMAND:
        result = V9240::instance().process_command();
        break;
    case FUNC_INIT:
        Xnrg34Init();
        break;
    case FUNC_PRE_INIT:
        Xnrg34PreInit();
        break;
    }
    return result;
}

#endif  // USE_ENERGY_V9240
#endif  // USE_ENERGY_SENSOR
