Attention when updating library. Changes in lib needed!!
All OpenTherm constants shall be prepended with `OPTH_` to avoid conflicts with other libs.

See commit https://github.com/arendst/Tasmota/commit/960291729ccc7cb4da50108e5299d44a79cb06de

As of OpenTherm-0.9.0, hte list is:
        OPTH_NONE
        OPTH_SUCCESS
        OPTH_INVALID
        OPTH_TIMEOUT
        OPTH_READ_DATA
        OPTH_READ
        OPTH_WRITE_DATA
        OPTH_WRITE
        OPTH_INVALID_DATA
        OPTH_RESERVED
        OPTH_READ_ACK
        OPTH_WRITE_ACK
        OPTH_DATA_INVALID
        OPTH_UNKNOWN_DATA_ID
        OPTH_NOT_INITIALIZED
        OPTH_READY
        OPTH_DELAY
        OPTH_REQUEST_SENDING
        OPTH_RESPONSE_WAITING
        OPTH_RESPONSE_START_BIT
        OPTH_RESPONSE_RECEIVING
        OPTH_RESPONSE_READY
        OPTH_RESPONSE_INVALID
