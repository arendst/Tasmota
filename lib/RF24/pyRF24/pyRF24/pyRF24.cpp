#include <RF24/RF24.h>
#include <boost/python.hpp>

namespace bp = boost::python;

// ******************** explicit wrappers **************************
// for methods which need it - mostly for buffer operations
// 

void throw_ba_exception(void)
{
    PyErr_SetString(PyExc_TypeError, "buf parameter must be bytes or bytearray");
    bp::throw_error_already_set();
}

char *get_bytes_or_bytearray_str(bp::object buf)
{
    PyObject *py_ba;
    py_ba = buf.ptr();
    if (PyByteArray_Check(py_ba))
        return PyByteArray_AsString(py_ba);
    else if (PyBytes_Check(py_ba))
        return PyBytes_AsString(py_ba);
    else
        throw_ba_exception();
    return NULL;
}

int get_bytes_or_bytearray_ln(bp::object buf)
{
    PyObject *py_ba;
    py_ba = buf.ptr();
    if (PyByteArray_Check(py_ba))
        return PyByteArray_Size(py_ba);
    else if (PyBytes_Check(py_ba))
        return PyBytes_Size(py_ba);
    else
        throw_ba_exception();
    return 0;
}

bp::object read_wrap(RF24& ref, int maxlen)
{
    char *buf = new char[maxlen+1];
    ref.read(buf, maxlen);
    bp::object py_ba(bp::handle<>(PyByteArray_FromStringAndSize(buf, maxlen<ref.getPayloadSize()?maxlen:ref.getPayloadSize())));
    delete[] buf;
    return py_ba;
}

bool write_wrap1(RF24& ref, bp::object buf)
{
    return ref.write(get_bytes_or_bytearray_str(buf), get_bytes_or_bytearray_ln(buf));
}

bool write_wrap2(RF24& ref, bp::object buf, const bool multicast)
{
    return ref.write(get_bytes_or_bytearray_str(buf), get_bytes_or_bytearray_ln(buf), multicast);
}

void writeAckPayload_wrap(RF24& ref, uint8_t pipe, bp::object buf)
{
    ref.writeAckPayload(pipe, get_bytes_or_bytearray_str(buf), get_bytes_or_bytearray_ln(buf));

}

bool writeFast_wrap1(RF24& ref, bp::object buf)
{
    return ref.writeFast(get_bytes_or_bytearray_str(buf), get_bytes_or_bytearray_ln(buf));
}

bool writeFast_wrap2(RF24& ref, bp::object buf, const bool multicast)
{
    return ref.writeFast(get_bytes_or_bytearray_str(buf), get_bytes_or_bytearray_ln(buf), multicast);
}

bool writeBlocking_wrap(RF24& ref, bp::object buf, uint32_t timeout)
{
    return ref.writeBlocking(get_bytes_or_bytearray_str(buf), get_bytes_or_bytearray_ln(buf), timeout);
}

void startFastWrite_wrap1(RF24& ref, bp::object buf, const bool multicast)
{
    ref.startFastWrite(get_bytes_or_bytearray_str(buf), get_bytes_or_bytearray_ln(buf), multicast);
}

void startFastWrite_wrap2(RF24& ref, bp::object buf, const bool multicast, bool startTx)
{
    ref.startFastWrite(get_bytes_or_bytearray_str(buf), get_bytes_or_bytearray_ln(buf), multicast, startTx);
}

void startWrite_wrap(RF24& ref, bp::object buf, const bool multicast)
{
    ref.startWrite(get_bytes_or_bytearray_str(buf), get_bytes_or_bytearray_ln(buf), multicast);
}

void openWritingPipe_wrap(RF24& ref, const bp::object address)
{
    ref.openWritingPipe((const uint8_t *)(get_bytes_or_bytearray_str(address)));
}

void openReadingPipe_wrap(RF24& ref, uint8_t number, const bp::object address)
{
    ref.openReadingPipe(number, (const uint8_t *)(get_bytes_or_bytearray_str(address)));
}

bp::tuple whatHappened_wrap(RF24& ref)
{
    bool tx_ok;
    bool tx_fail;
    bool tx_ready;

    ref.whatHappened(tx_ok, tx_fail, tx_ready);
    return bp::make_tuple(tx_ok, tx_fail, tx_ready);
}

bp::tuple available_wrap(RF24& ref)
{
    bool result;
    uint8_t pipe;

    result = ref.available(&pipe);
    return bp::make_tuple(result, pipe);
}

BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(txStandBy_wrap1, RF24::txStandBy, 0, 2)
//BOOST_PYTHON_FUNCTION_OVERLOADS(txStandBy_wrap2, RF24::txStandBy, 1, 2)

// ******************** enums **************************
// from both RF24 and bcm2835
// 

BOOST_PYTHON_MODULE(RF24){

#ifdef BCM2835_H
    bp::enum_< RPiGPIOPin>("RPiGPIOPin")
        .value("RPI_GPIO_P1_03", RPI_GPIO_P1_03)
        .value("RPI_GPIO_P1_05", RPI_GPIO_P1_05)
        .value("RPI_GPIO_P1_07", RPI_GPIO_P1_07)
        .value("RPI_GPIO_P1_08", RPI_GPIO_P1_08)
        .value("RPI_GPIO_P1_10", RPI_GPIO_P1_10)
        .value("RPI_GPIO_P1_11", RPI_GPIO_P1_11)
        .value("RPI_GPIO_P1_12", RPI_GPIO_P1_12)
        .value("RPI_GPIO_P1_13", RPI_GPIO_P1_13)
        .value("RPI_GPIO_P1_15", RPI_GPIO_P1_15)
        .value("RPI_GPIO_P1_16", RPI_GPIO_P1_16)
        .value("RPI_GPIO_P1_18", RPI_GPIO_P1_18)
        .value("RPI_GPIO_P1_19", RPI_GPIO_P1_19)
        .value("RPI_GPIO_P1_21", RPI_GPIO_P1_21)
        .value("RPI_GPIO_P1_22", RPI_GPIO_P1_22)
        .value("RPI_GPIO_P1_23", RPI_GPIO_P1_23)
        .value("RPI_GPIO_P1_24", RPI_GPIO_P1_24)
        .value("RPI_GPIO_P1_26", RPI_GPIO_P1_26)
        .value("RPI_V2_GPIO_P1_03", RPI_V2_GPIO_P1_03)
        .value("RPI_V2_GPIO_P1_05", RPI_V2_GPIO_P1_05)
        .value("RPI_V2_GPIO_P1_07", RPI_V2_GPIO_P1_07)
        .value("RPI_V2_GPIO_P1_08", RPI_V2_GPIO_P1_08)
        .value("RPI_V2_GPIO_P1_10", RPI_V2_GPIO_P1_10)
        .value("RPI_V2_GPIO_P1_11", RPI_V2_GPIO_P1_11)
        .value("RPI_V2_GPIO_P1_12", RPI_V2_GPIO_P1_12)
        .value("RPI_V2_GPIO_P1_13", RPI_V2_GPIO_P1_13)
        .value("RPI_V2_GPIO_P1_15", RPI_V2_GPIO_P1_15)
        .value("RPI_V2_GPIO_P1_16", RPI_V2_GPIO_P1_16)
        .value("RPI_V2_GPIO_P1_18", RPI_V2_GPIO_P1_18)
        .value("RPI_V2_GPIO_P1_19", RPI_V2_GPIO_P1_19)
        .value("RPI_V2_GPIO_P1_21", RPI_V2_GPIO_P1_21)
        .value("RPI_V2_GPIO_P1_22", RPI_V2_GPIO_P1_22)
        .value("RPI_V2_GPIO_P1_23", RPI_V2_GPIO_P1_23)
        .value("RPI_V2_GPIO_P1_24", RPI_V2_GPIO_P1_24)
        .value("RPI_V2_GPIO_P1_26", RPI_V2_GPIO_P1_26)
        .value("RPI_V2_GPIO_P5_03", RPI_V2_GPIO_P5_03)
        .value("RPI_V2_GPIO_P5_04", RPI_V2_GPIO_P5_04)
        .value("RPI_V2_GPIO_P5_05", RPI_V2_GPIO_P5_05)
        .value("RPI_V2_GPIO_P5_06", RPI_V2_GPIO_P5_06)
        .value("RPI_BPLUS_GPIO_J8_03", RPI_BPLUS_GPIO_J8_03)
        .value("RPI_BPLUS_GPIO_J8_05", RPI_BPLUS_GPIO_J8_05)
        .value("RPI_BPLUS_GPIO_J8_07", RPI_BPLUS_GPIO_J8_07)
        .value("RPI_BPLUS_GPIO_J8_08", RPI_BPLUS_GPIO_J8_08)
        .value("RPI_BPLUS_GPIO_J8_10", RPI_BPLUS_GPIO_J8_10)
        .value("RPI_BPLUS_GPIO_J8_11", RPI_BPLUS_GPIO_J8_11)
        .value("RPI_BPLUS_GPIO_J8_12", RPI_BPLUS_GPIO_J8_12)
        .value("RPI_BPLUS_GPIO_J8_13", RPI_BPLUS_GPIO_J8_13)
        .value("RPI_BPLUS_GPIO_J8_15", RPI_BPLUS_GPIO_J8_15)
        .value("RPI_BPLUS_GPIO_J8_16", RPI_BPLUS_GPIO_J8_16)
        .value("RPI_BPLUS_GPIO_J8_18", RPI_BPLUS_GPIO_J8_18)
        .value("RPI_BPLUS_GPIO_J8_19", RPI_BPLUS_GPIO_J8_19)
        .value("RPI_BPLUS_GPIO_J8_21", RPI_BPLUS_GPIO_J8_21)
        .value("RPI_BPLUS_GPIO_J8_22", RPI_BPLUS_GPIO_J8_22)
        .value("RPI_BPLUS_GPIO_J8_23", RPI_BPLUS_GPIO_J8_23)
        .value("RPI_BPLUS_GPIO_J8_24", RPI_BPLUS_GPIO_J8_24)
        .value("RPI_BPLUS_GPIO_J8_26", RPI_BPLUS_GPIO_J8_26)
        .value("RPI_BPLUS_GPIO_J8_29", RPI_BPLUS_GPIO_J8_29)
        .value("RPI_BPLUS_GPIO_J8_31", RPI_BPLUS_GPIO_J8_31)
        .value("RPI_BPLUS_GPIO_J8_32", RPI_BPLUS_GPIO_J8_32)
        .value("RPI_BPLUS_GPIO_J8_33", RPI_BPLUS_GPIO_J8_33)
        .value("RPI_BPLUS_GPIO_J8_35", RPI_BPLUS_GPIO_J8_35)
        .value("RPI_BPLUS_GPIO_J8_36", RPI_BPLUS_GPIO_J8_36)
        .value("RPI_BPLUS_GPIO_J8_37", RPI_BPLUS_GPIO_J8_37)
        .value("RPI_BPLUS_GPIO_J8_38", RPI_BPLUS_GPIO_J8_38)
        .value("RPI_BPLUS_GPIO_J8_40", RPI_BPLUS_GPIO_J8_40)
        .export_values()
        ;

    bp::enum_< bcm2835SPIClockDivider>("bcm2835SPIClockDivider")
        .value("BCM2835_SPI_CLOCK_DIVIDER_65536", BCM2835_SPI_CLOCK_DIVIDER_65536)
        .value("BCM2835_SPI_CLOCK_DIVIDER_32768", BCM2835_SPI_CLOCK_DIVIDER_32768)
        .value("BCM2835_SPI_CLOCK_DIVIDER_16384", BCM2835_SPI_CLOCK_DIVIDER_16384)
        .value("BCM2835_SPI_CLOCK_DIVIDER_8192", BCM2835_SPI_CLOCK_DIVIDER_8192)
        .value("BCM2835_SPI_CLOCK_DIVIDER_4096", BCM2835_SPI_CLOCK_DIVIDER_4096)
        .value("BCM2835_SPI_CLOCK_DIVIDER_2048", BCM2835_SPI_CLOCK_DIVIDER_2048)
        .value("BCM2835_SPI_CLOCK_DIVIDER_1024", BCM2835_SPI_CLOCK_DIVIDER_1024)
        .value("BCM2835_SPI_CLOCK_DIVIDER_512", BCM2835_SPI_CLOCK_DIVIDER_512)
        .value("BCM2835_SPI_CLOCK_DIVIDER_256", BCM2835_SPI_CLOCK_DIVIDER_256)
        .value("BCM2835_SPI_CLOCK_DIVIDER_128", BCM2835_SPI_CLOCK_DIVIDER_128)
        .value("BCM2835_SPI_CLOCK_DIVIDER_64", BCM2835_SPI_CLOCK_DIVIDER_64)
        .value("BCM2835_SPI_CLOCK_DIVIDER_32", BCM2835_SPI_CLOCK_DIVIDER_32)
        .value("BCM2835_SPI_CLOCK_DIVIDER_16", BCM2835_SPI_CLOCK_DIVIDER_16)
        .value("BCM2835_SPI_CLOCK_DIVIDER_8", BCM2835_SPI_CLOCK_DIVIDER_8)
        .value("BCM2835_SPI_CLOCK_DIVIDER_4", BCM2835_SPI_CLOCK_DIVIDER_4)
        .value("BCM2835_SPI_CLOCK_DIVIDER_2", BCM2835_SPI_CLOCK_DIVIDER_2)
        .value("BCM2835_SPI_CLOCK_DIVIDER_1", BCM2835_SPI_CLOCK_DIVIDER_1)
        .export_values();


    bp::enum_< bcm2835SPIChipSelect>("bcm2835SPIChipSelect")
        .value("BCM2835_SPI_CS0", BCM2835_SPI_CS0)
        .value("BCM2835_SPI_CS1", BCM2835_SPI_CS1)
        .value("BCM2835_SPI_CS2", BCM2835_SPI_CS2)
        .value("BCM2835_SPI_CS_NONE", BCM2835_SPI_CS_NONE)
        .export_values();

// exposing '#define's for SPI speed as this is needed for RF24 constructor
    bp::scope().attr("BCM2835_SPI_SPEED_64MHZ") = BCM2835_SPI_SPEED_64MHZ;
    bp::scope().attr("BCM2835_SPI_SPEED_32MHZ") = BCM2835_SPI_SPEED_32MHZ;
    bp::scope().attr("BCM2835_SPI_SPEED_16MHZ") = BCM2835_SPI_SPEED_16MHZ;
    bp::scope().attr("BCM2835_SPI_SPEED_8MHZ") = BCM2835_SPI_SPEED_8MHZ;
    bp::scope().attr("BCM2835_SPI_SPEED_4MHZ") = BCM2835_SPI_SPEED_4MHZ;
    bp::scope().attr("BCM2835_SPI_SPEED_2MHZ") = BCM2835_SPI_SPEED_2MHZ;
    bp::scope().attr("BCM2835_SPI_SPEED_1MHZ") = BCM2835_SPI_SPEED_1MHZ;
    bp::scope().attr("BCM2835_SPI_SPEED_512KHZ") = BCM2835_SPI_SPEED_512KHZ;
    bp::scope().attr("BCM2835_SPI_SPEED_256KHZ") = BCM2835_SPI_SPEED_256KHZ;
    bp::scope().attr("BCM2835_SPI_SPEED_128KHZ") = BCM2835_SPI_SPEED_128KHZ;
    bp::scope().attr("BCM2835_SPI_SPEED_64KHZ") = BCM2835_SPI_SPEED_64KHZ;
    bp::scope().attr("BCM2835_SPI_SPEED_32KHZ") = BCM2835_SPI_SPEED_32KHZ;
    bp::scope().attr("BCM2835_SPI_SPEED_16KHZ") = BCM2835_SPI_SPEED_16KHZ;
    bp::scope().attr("BCM2835_SPI_SPEED_8KHZ") = BCM2835_SPI_SPEED_8KHZ;
#endif  // BCM2835_H

    bp::enum_< rf24_crclength_e>("rf24_crclength_e")
        .value("RF24_CRC_DISABLED", RF24_CRC_DISABLED)
        .value("RF24_CRC_8", RF24_CRC_8)
        .value("RF24_CRC_16", RF24_CRC_16)
        .export_values()
        ;

    bp::enum_< rf24_datarate_e>("rf24_datarate_e")
        .value("RF24_1MBPS", RF24_1MBPS)
        .value("RF24_2MBPS", RF24_2MBPS)
        .value("RF24_250KBPS", RF24_250KBPS)
        .export_values()
        ;

    bp::enum_< rf24_pa_dbm_e>("rf24_pa_dbm_e")
        .value("RF24_PA_MIN", RF24_PA_MIN)
        .value("RF24_PA_LOW", RF24_PA_LOW)
        .value("RF24_PA_HIGH", RF24_PA_HIGH)
        .value("RF24_PA_MAX", RF24_PA_MAX)
        .value("RF24_PA_ERROR", RF24_PA_ERROR)
        .export_values()
        ;

// ******************** RF24 class  **************************
// 
    bp::class_< RF24 >( "RF24", bp::init< uint8_t, uint8_t >(( bp::arg("_cepin"), bp::arg("_cspin") )) )    
        .def( bp::init< uint8_t, uint8_t, uint32_t >(( bp::arg("_cepin"), bp::arg("_cspin"), bp::arg("spispeed") )) )    
        .def("available", (bool ( ::RF24::* )(  ) )( &::RF24::available ) )
        .def("available_pipe", &available_wrap )    // needed to rename this method as python does not allow such overloading
        .def("begin", &RF24::begin)    
        .def("closeReadingPipe", &RF24::closeReadingPipe)
        .def("disableCRC", &RF24::disableCRC)    
        .def("enableAckPayload", &RF24::enableAckPayload)
        .def("enableDynamicAck", &RF24::enableDynamicAck)    
        .def("enableDynamicPayloads", &RF24::enableDynamicPayloads)
        .def("flush_tx", &RF24::flush_tx)
        .def("getCRCLength", &RF24::getCRCLength)
        .def("getDataRate", &RF24::getDataRate)
        .def("getDynamicPayloadSize", &RF24::getDynamicPayloadSize)
        .def("getPALevel", &RF24::getPALevel)
        .def("isAckPayloadAvailable", &RF24::isAckPayloadAvailable)    
        .def("isPVariant", &RF24::isPVariant)
        .def("isValid", &RF24::isValid)
        .def("maskIRQ", &RF24::maskIRQ, ( bp::arg("tx_ok"), bp::arg("tx_fail"), bp::arg("rx_ready")))    
        .def("openReadingPipe", &openReadingPipe_wrap, (bp::arg("number"), bp::arg("address")))    
        .def("openReadingPipe", (void ( ::RF24::* )( ::uint8_t,::uint64_t ) )( &::RF24::openReadingPipe), (bp::arg("number"), bp::arg("address")))    
        .def("openWritingPipe", &openWritingPipe_wrap, (bp::arg("address")))    
        .def("openWritingPipe", (void ( ::RF24::* )( ::uint64_t ) )( &::RF24::openWritingPipe), ( bp::arg("address") ) )    
        .def("powerDown", &RF24::powerDown)    
        .def("powerUp", &RF24::powerUp)    
        .def("printDetails", &RF24::printDetails)
        .def("reUseTX", &RF24::reUseTX)
        .def("read", &read_wrap, (bp::arg("maxlen")))    
        .def("rxFifoFull", &RF24::rxFifoFull)
        .def("setAddressWidth", &RF24::setAddressWidth)
        .def("setAutoAck", (void ( ::RF24::* )( bool ) )( &::RF24::setAutoAck ), ( bp::arg("enable") ) )    
        .def("setAutoAck", (void ( ::RF24::* )( ::uint8_t,bool ) )( &::RF24::setAutoAck ), ( bp::arg("pipe"), bp::arg("enable") ) )    
        .def("setCRCLength", &RF24::setCRCLength, ( bp::arg("length") ) )    
        .def("setChannel", &RF24::setChannel, ( bp::arg("channel") ) )    
        .def("setDataRate", &RF24::setDataRate, ( bp::arg("speed") ) )    
        .def("setPALevel", &RF24::setPALevel, ( bp::arg("level") ) )    
        .def("setRetries", &RF24::setRetries , (bp::arg("delay"), bp::arg("count")))    
        .def("startFastWrite", &startFastWrite_wrap1, ( bp::arg("buf"), bp::arg("len"), bp::arg("multicast") ) )    
        .def("startFastWrite", &startFastWrite_wrap2, ( bp::arg("buf"), bp::arg("len"), bp::arg("multicast"), bp::arg("startTx") ) )    
        .def("startListening", &RF24::startListening)    
        .def("startWrite", &startWrite_wrap, ( bp::arg("buf"), bp::arg("len"), bp::arg("multicast") ) )    
        .def("stopListening", &RF24::stopListening)
        .def("testCarrier", &RF24::testCarrier)
        .def("testRPD", &RF24::testRPD)
        .def("txStandBy", (bool ( ::RF24::* )( ::uint32_t,bool))(&RF24::txStandBy), txStandBy_wrap1( bp::args("timeout", "startTx") ) )
        .def("whatHappened", &whatHappened_wrap)    
        .def("write", &write_wrap1, ( bp::arg("buf") ) )    
        .def("write", &write_wrap2, ( bp::arg("buf"), bp::arg("multicast") ) )    
        .def("writeAckPayload", writeAckPayload_wrap, ( bp::arg("pipe"), bp::arg("buf") ) )    
        .def("writeBlocking", &writeBlocking_wrap, ( bp::arg("buf"), bp::arg("timeout") ) )    
        .def("writeFast", &writeFast_wrap1, ( bp::arg("buf") ) )    
        .def("writeFast", &writeFast_wrap2, ( bp::arg("buf"), bp::arg("multicast") ) )    
        .add_property("payloadSize", &RF24::getPayloadSize, &RF24::setPayloadSize)
        .def_readwrite( "failureDetected", &RF24::failureDetected );
}
