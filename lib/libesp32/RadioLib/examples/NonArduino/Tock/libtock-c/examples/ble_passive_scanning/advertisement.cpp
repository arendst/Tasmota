#include "advertisement.h"

Advertisement::Advertisement() {
  std::memset(&header_, 0, HEADER_SIZE);
  std::memset(&address_, 0, ADDRESS_SIZE);
  std::memset(&data_, 0, DATA_MAX_SIZE);
}

Advertisement::Advertisement(const unsigned char* buf, int len)
{
  std::memcpy(&header_, &buf[HEADER_START], HEADER_SIZE);
  std::memcpy(&address_, &buf[ADDRESS_START], ADDRESS_SIZE);
  unsigned char data_len = len - HEADER_SIZE - ADDRESS_SIZE;
  if (data_len > DATA_MAX_SIZE) {
    data_len = DATA_MAX_SIZE;
  }
  std::memcpy(&data_, &buf[DATA_START], data_len);
}

bool Advertisement::device_detected(const Advertisement& other) const
{
  return std::memcmp(&address_, &other.address_, ADDRESS_SIZE) == 0;
}

bool Advertisement::operator==(const Advertisement& other) const {
  return std::memcmp(this, &other, sizeof(Advertisement)) == 0;
}

bool Advertisement::operator!=(const Advertisement& other) const {
  return std::memcmp(this, &other, sizeof(Advertisement)) != 0;
}

void Advertisement::print() const
{
  printf("PDU Type: %d %s\r\n", pduType(), pduTypeStr());
  printf("PDU TxAdd: %d\r\n", pduTxAddSet() ? 1 : 0);
  printf("PDU RxAdd: %d\r\n", pduRxAddSet() ? 1 : 0);
  printf("PDU Length: %d\r\n", pduLength());
  printf("Address: %02x %02x %02x %02x %02x %02x\r\n", address_[5],
         address_[4], address_[3], address_[2], address_[1], address_[0]);
  printf("Data: ");
  for (int i = 0; i < pduLength() - ADDRESS_SIZE; i++) {
    printf("%02x ", data_[i]);
  }
  printf("\r\n\r\n");
}

unsigned char Advertisement::pduLength() const
{
  return header_[1] & PDU_LEN_HEADER_MASK;
}

unsigned char Advertisement::pduType() const
{
  return header_[0] & PDU_TYPE_HEADER_MASK;
}

bool Advertisement::pduTxAddSet() const
{
  return header_[0] & PDU_TXADD_HEADER_MASK;
}

bool Advertisement::pduRxAddSet() const
{
  return header_[0] & PDU_RXADD_HEADER_MASK;
}

const char* Advertisement::pduTypeStr() const
{
  switch (pduType()) {
    case 0:
      return "ADV_IND";
    case 1:
      return "ADV_DIRECT_IND";
    case 2:
      return "NON_CONNECT_IND";
    case 3:
      return "SCAN_REQ";
    case 4:
      return "SCAN_RSP";
    case 5:
      return "CONNECT_REQ";
    case 6:
      return "ADV_SCAN_IND";
    default:
      return "INVALID ADVERTISEMENT TYPE";
  }
}

bool Advertisement::checkScanResult(const unsigned char* buf, int len)
{
  if (buf == nullptr) {
    printf("Malformed scan result: Buffer was null!\n");
    return false;
  }
  if (len < ADV_MIN_SIZE) {
    printf("Malformed scan result: Result too small!\n");
    return false;
  }
  if (len > ADV_MAX_SIZE) {
    printf("Malformed scan result: Result too large!\n");
    return false;
  }
  return true;
}
