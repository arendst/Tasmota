#include "boost/python.hpp"
#include "RF24/RF24.h"
#include "RF24Network/RF24Network.h"
#include "RF24Mesh/RF24Mesh.h"

namespace bp = boost::python;


// ******************** explicit wrappers **************************
// where needed, especially where buffer is involved

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

bool write_wrap1(RF24Mesh& ref, bp::object buf, uint8_t msg_type)
{
	return ref.write(get_bytes_or_bytearray_str(buf), msg_type, get_bytes_or_bytearray_ln(buf));
}

bool write_wrap2(RF24Mesh& ref, bp::object buf, uint8_t msg_type, uint8_t nodeID)
{
	return ref.write(get_bytes_or_bytearray_str(buf), msg_type, get_bytes_or_bytearray_ln(buf), nodeID);
}

bool write_to_node_wrap(RF24Mesh& ref, uint16_t to_node,  bp::object buf, uint8_t msg_type)
{
	return ref.write(to_node, get_bytes_or_bytearray_str(buf), msg_type, get_bytes_or_bytearray_ln(buf));
}

// ******************** overload wrappers **************************
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(begin_overload, RF24Mesh::begin, 0, 3)
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(getNodeID_overload, RF24Mesh::getNodeID, 0, 1)
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(renewAddress_overload, RF24Mesh::renewAddress, 0, 1)

// ******************** RF24Mesh exposed  **************************
BOOST_PYTHON_MODULE(RF24Mesh)
{
    { //::RF24Mesh
        bp::class_<RF24Mesh>("RF24Mesh", bp::init<RF24&, RF24Network&>((bp::arg("_radio"), bp::arg("_network"))))
            //bool begin(uint8_t channel = MESH_DEFAULT_CHANNEL, rf24_datarate_e data_rate = RF24_1MBPS, uint32_t timeout=MESH_RENEWAL_TIMEOUT );
            .def("begin", &RF24Mesh::begin, begin_overload(bp::args("channel", "data_rate", "timeout")))
            //uint8_t update();
            .def("update", &RF24Mesh::update)
            //bool write(const void* data, uint8_t msg_type, size_t size, uint8_t nodeID=0);
            .def("write", &write_wrap1, (bp::arg("data"), bp::arg("msg_type")))
            .def("write", &write_wrap2, (bp::arg("data"), bp::arg("msg_type"), bp::arg("nodeID")))
            //bool write(uint16_t to_node, const void* data, uint8_t msg_type, size_t size );
            .def("write", &write_to_node_wrap, (bp::arg("to_node"), bp::arg("data"), bp::arg("msg_type"), bp::arg("size")))
            //void setNodeID(uint8_t nodeID);
            .def("setNodeID", &RF24Mesh::setNodeID, (bp::arg("nodeID")))
            //void DHCP();
            .def("DHCP", &RF24Mesh::DHCP)
            //int16_t getNodeID(uint16_t address=MESH_BLANK_ID);
            .def("getNodeID", &RF24Mesh::getNodeID, getNodeID_overload(bp::args("address")))
            //bool checkConnection();
            .def("checkConnection", &RF24Mesh::checkConnection)
            //uint16_t renewAddress(uint32_t timeout=MESH_RENEWAL_TIMEOUT);
            .def("renewAddress", &RF24Mesh::renewAddress, getNodeID_overload(bp::args("timeout")))
            //bool releaseAddress();
            .def("releaseAddress", &RF24Mesh::releaseAddress)
            //int16_t getAddress(uint8_t nodeID);
            .def("getAddress", &RF24Mesh::getAddress, (bp::arg("nodeID")))
            //void setChannel(uint8_t _channel);
            .def("setChannel", &RF24Mesh::setChannel, (bp::arg("_channel")))
            //void setChild(bool allow);
            .def("setChild", &RF24Mesh::setChild, (bp::arg("allow")))
            //void setAddress(uint8_t nodeID, uint16_t address);
            .def("setAddress", &RF24Mesh::setAddress, (bp::arg("nodeID"), bp::arg("address")))
            //void saveDHCP();
            .def("saveDHCP", &RF24Mesh::saveDHCP)
            //void loadDHCP();
            .def("loadDHCP", &RF24Mesh::loadDHCP)
            //void setStaticAddress(uint8_t nodeID, uint16_t address);
            .def("setStaticAddress", &RF24Mesh::setStaticAddress, (bp::arg("nodeID"), bp::arg("address")));
    }
}