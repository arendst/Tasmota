#
# Matter_BTP.be - implements the Matter Bluetooth Transport Protocol (BTP)
#
# Copyright (C) 2026 Stephan Hadinger, Christian Baars & Theo Arends
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#

import matter

#@ solidify:Matter_BTP,weak
#if USE_MI_EXT_GUI

#################################################################################
# Matter_BTP
#
# Pure protocol implementation of the Matter Bluetooth Transport Protocol (BTP).
# Transport-agnostic: only requires the owning `device` to expose `ble_send(payload)`.
#################################################################################
class Matter_BTP

    static MTU = 244
    static VERSION = 4
    static WINDOW_SIZE = 6
    # control flag masks
    static F_HANDSHAKE = 0x40
    static F_MGMT      = 0x20
    static F_ACK        = 0x08
    static F_END        = 0x04
    static F_CONT       = 0x02
    static F_BEGIN      = 0x01

    var device
    var flags                   # control flags byte
    var management_opcode
    var ack_number
    var seq_tx
    var seq_rx
    var message_length
    var segment_payload
    var combined_payload
    var send_queue
    var can_send

    def init(device)
        self.flags = 0
        self.send_queue = []
        self.device = device
        self.seq_tx = 0
        self.can_send = true
    end

    # Serialize the frame to bytes
    def serialize()
        var frame = bytes()
        frame.add(self.flags)
        if (self.flags & self.F_MGMT) && self.management_opcode != nil
            frame.add(self.management_opcode)
        end
        if (self.flags & self.F_HANDSHAKE)
            if self.segment_payload != nil  frame += self.segment_payload end
            return frame
        end
        if (self.flags & self.F_ACK) && self.ack_number != nil
            frame.add(self.ack_number & 0xff)
        end
        frame.add(self.seq_tx & 0xff)
        if (self.flags & self.F_BEGIN) && self.message_length != nil
            frame.add(self.message_length, 2)
        end
        if self.segment_payload != nil
            frame += self.segment_payload
        end
        return frame
    end

    # Parse frame from bytes
    def parse(data)
        if size(data) < 1  return false end

        var offset = 0
        self.flags = data[offset]
        offset += 1

        # management message (handshake)
        if (self.flags & self.F_MGMT)
            self.management_opcode = data[offset]
            self.segment_payload = data[2..]
            return true
        end

        if (self.flags & self.F_ACK)
            self.ack_number = data[offset]
            offset += 1
            self.can_send = true
        end

        self.seq_rx = data[offset]
        if self.seq_rx > 1 && self.can_send == false
            self.seq_rx = nil
        end
        offset += 1

        if size(data) < 4
            # pure ACK
            self.message_length = 0
            self.combined_payload = nil
            self.continue_queue()
            return false
        end

        if (self.flags & self.F_BEGIN)
            self.message_length = data.get(offset, 2)
            offset += 2
            self.combined_payload = bytes(1024)
        end

        if offset < size(data)
            self.combined_payload .. data[offset..]
        end

        if !(self.flags & self.F_END)
            return false
        end
        return true
    end

    def continue_queue()
        self.can_send = true
        var p = self.get_packet()
        if !p return end
        self.can_send = false
        tasmota.defer(/-> self.device.ble_send(p))
        self.segment_payload = nil
    end

    def create_send_queue(payload)
        var bytes_left = size(payload)
        var pl_sz = self.MTU - 5
        var offset = 0
        var segment_type = 0
        if bytes_left > pl_sz  segment_type = 1 end
        while bytes_left > 0
            if bytes_left - pl_sz <= 0 && offset != 0  segment_type = 3 end
            self.send_queue.push([payload[offset .. (offset + pl_sz - 1)], segment_type, bytes_left])
            segment_type = 2
            bytes_left -= pl_sz
            offset += pl_sz
        end
    end

    def get_packet()
        self.seq_tx += 1
        if size(self.send_queue) > 0
            var el = self.send_queue.pop(0)
            self.segment_payload = el[0]
            if el[1] == 0       # single segment
                self.flags = self.F_BEGIN | self.F_END
                self.message_length = size(el[0])
            elif el[1] == 1     # first segment
                self.flags = self.F_BEGIN
                self.message_length = el[2]
            elif el[1] == 3     # last segment
                self.flags = self.F_CONT | self.F_END
            else                # middle segment
                self.flags = self.F_CONT
            end
        else
            self.flags = 0
            self.segment_payload = nil
        end
        # add ack
        if self.seq_rx != nil
            self.ack_number = self.seq_rx
            self.flags |= self.F_ACK
            self.seq_rx = nil
        end
        return self.serialize()
    end

    # Create handshake response frame
    def create_handshake_response()
        self.flags = 0x65
        self.management_opcode = 0x6C
        var payload = bytes()
        payload.add(self.VERSION & 0x0F)
        payload.add(self.MTU, 2)
        payload.add(self.WINDOW_SIZE)
        self.segment_payload = payload
    end

    def delete()
        self.combined_payload = nil
        self.message_length = 0
    end
end

matter.BTP = Matter_BTP
#else
class Matter_BTP end # will be discarded
#endif // USE_MI_EXT_GUI