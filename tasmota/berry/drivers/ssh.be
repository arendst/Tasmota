#- Simple SSH server in Berry by Christian Baars
#  this is demo code and not intended for production use
-#

class SSH_MSG
    static DISCONNECT = 1
    static IGNORE     = 2
    static SERVICE_REQUEST = 5
    static SERVICE_ACCEPT = 6
    static KEXINIT = 20
    static NEWKEYS = 21
    static KEXDH_INIT = 30
    static KEX_ECDH_REPLY = 31
    static USERAUTH_REQUEST = 50
    static USERAUTH_FAILURE = 51
    static USERAUTH_SUCCESS = 52
    static USERAUTH_BANNER = 53
    static GLOBAL_REQUEST  = 80 
    static REQUEST_SUCCESS = 81 
    static REQUEST_FAILURE = 82 
    static CHANNEL_OPEN = 90 
    static CHANNEL_OPEN_CONFIRMATION = 91 
    static CHANNEL_OPEN_FAILURE      = 92 
    static CHANNEL_WINDOW_ADJUST     = 93 
    static CHANNEL_DATA              = 94 
    static CHANNEL_EXTENDED_DATA     = 95 
    static CHANNEL_EOF      = 96 
    static CHANNEL_CLOSE    = 97 
    static CHANNEL_REQUEST  = 98 
    static CHANNEL_SUCCESS  = 99 
    static CHANNEL_FAILURE  =100 

    static def get_name_list(buffer, index, length)
        import string
        if length == 0 || length > (size(buffer) - 5)
            return nil
        end
        var names = buffer[index + 4 .. index + 3 + length]
        return string.split(names.asstring(),",")
    end

    static def get_string(buffer, index, length)
        import string
        if length == 0 || length > (size(buffer) - 5)
            return nil
        end
        var name = buffer[index + 4 .. index + 3 + length]
        return name.asstring()
    end

    static def get_bytes(buffer, index, length)
        import string
        if length == 0 || length > (size(buffer) - 5)
            return nil
        end
        var b = buffer[index + 4 .. index + 3 + length]
        return b
    end

    static def get_item_length(buf)
        return buf.geti(0,-4)
    end

    static def add_string(buf, str_entry)
        buf.add(size(str_entry),-4)
        buf .. str_entry
    end

    static def add_mpint(buf, entry)
        if entry[0] & 128 != 0
            entry = bytes("00") + entry
        end
        buf.add(size(entry),-4)
        buf .. entry
    end

    static def make_mpint(buf)
        var mpint = bytes(size(buf) + 5)
        if buf[0] & 128 != 0
            buf = bytes("00") + buf
        end
        mpint.add(size(buf),-4)
        mpint .. buf
        return mpint
    end
end

class TERMINAL
    var in_buf, session

    def init(session)
        self.session = session
        self.in_buf = bytes(64)
    end

    def process(data)
        self.in_buf .. data
        if data == bytes("0d")
            var c = self.in_buf.asstring()
            var r = tasmota.cmd(f"{c}")
            self.in_buf.clear()
            if r
                return "\r\n" + r.tostring() + "\r\n> "
            else
                return "\r\n>"
            end
        else
            return data.asstring()
        end
        return ""
    end
end

class PATH    # helper class to hold the current directory 
    var p  #  path components in a list

    def init()
        import string
        self.p = []
    end

    def set(p)
        import string
        import path

        if path.isdir(p) != true
            return false
        end

        var new = string.split(p,"/")
        self.p = []
        for c:new
            if c != ""
                self.p.push(c)
            end
        end
        return true
    end

    def dir_up()
        if size(self.p) > 0
            self.p.pop()
        end
    end

    def get_url()
        var url = "/"
        for c:self.p
            if c != ""
                url += f"{c}/"
            end
        end
        return url
    end
end

class SFTP
    static INIT     = 1
    static VERSION  = 2
    static OPEN     = 3
    static STAT     = 17
    static STATUS   = 101
    static ATTRS    = 105

    var session, dir_list, dir, file

    def init(session)
        self.session = session
        self.dir = PATH()
        self.readDir()
        print("SFTP started .. not working yet!!!")
        print(self.dir_list)
    end

    def readDir()
        import path
        self.dir_list = path.listdir(self.dir.get_url())
    end

    def attr_for_file(sz, date)
        var attr = bytes("0000000900000000")
        attr.add(sz,-4)
        attr.add(date,-4) # TODO: atime check if better option possible
        attr.add(date,-4)
        attr .. bytes(-8) # two empty strings
        return attr
    end

    def status(id,code)
        var s = bytes("00000065") # packet type
        s .. id
        s.add(code,-4)
        s .. bytes(-8) # two empty strings
        return s
    end

    def stat_for_file(id, url)
        import path
        var fsize = -1
        var fdate = -1
        if path.exists(url)
            var ptype = bytes() .. SFTP.ATTRS
            if !path.isdir(url)
                var f = open(url,"r")
                fsize = f.size()
                fdate = path.last_modified(f)
                f.close()
                return ptype + id + self.attr_for_file(fsize,fdate)
            end
            return ptype + id + bytes(-4) # dir , no idea if correct way
        end
        return self.status(id, 2) # NO_SUCH_FILE
    end

    def open_file(id,url,pflags,attr)
        import path
        var _pf = pflags[3]
        if path.exists(url) != true
            if !_pf&1 && !_pf&4
                return self.status(id, 2) # NO_SUCH_FILE
            end
        end
        if path.isdir(url) == true
            return self.status(id, 4) # FAILURE 
        end
        var mode = ""

        #define SSH_FXF_READ            0x00000001
        #define SSH_FXF_WRITE           0x00000002
        #define SSH_FXF_APPEND          0x00000004
        #define SSH_FXF_CREAT           0x00000008
        #define SSH_FXF_TRUNC           0x00000010
        #define SSH_FXF_EXCL            0x00000020
        
        return self.status(id,0)
    end

    def process(d)
        print("SFTP:",d)
        var r = bytes() 
        var ptype = d[4] # https://datatracker.ietf.org/doc/html/draft-ietf-secsh-filexfer-02#section-3
        var id = d[9..12]
        if ptype == SFTP.INIT
            r = bytes('0000000d02000000030000000000000000') # no extended data support, ver 3
        elif ptype == SFTP.STAT
            var url = d[13..]
            var _r = self.stat_for_file(id,url.asstring())
            r.add(size(_r),-4)
            r .. _r
            print(id,url)
        elif ptype == SFTP.OPEN
            var next_index = 13
            var next_length = SSH_MSG.get_item_length(d[next_index..])
            var url = SSH_MSG.get_string(d, next_index, next_length)
            next_index += next_length + 4
            var mode = d.geti(next_index,-4)
            next_index += 4
            var attr = d[next_index..]
            var _r = self.open_file(id,url,mode,attr)
            r.add(size(_r),-4)
            r .. _r
            print(id,url)
        else
            var _r = self.status(id,8) #OP_UNSUPPORTED 
            r.add(size(_r),-4)
            r .. _r
        end
        print("d SFTP type:",ptype)
        return r
    end
end

class BIN_PACKET
    var packet_length, padding_length, payload, payload_length, padding, mac, mac_length
    var expected_length
    var complete, session, encrypted, buf, overrun_buf

    def init(buf, session, encrypted)
        self.session = session
        self.packet_length = buf.geti(0,-4)
        self.expected_length = self.packet_length + 4
        if encrypted == true
            self.packet_length = self.get_length(buf)
            print("length",self.packet_length)
            self.expected_length = self.packet_length + 4 + 16 # mac
            if session == nil print("session is nil") end
        end
        if self.expected_length > 4096
            print("Unusual high packet length - assume decoding error!!", self.expected_length)
            self.expected_length = size(buf)
            self.packet_length = size(buf) - 20
        end
        self.buf = bytes(self.expected_length)
        self.encrypted = encrypted
        self.append(buf)
    end

    def get_length(packet)
        import crypto
        var c = crypto.CHACHA20_POLY1305()
        var length = packet[0..3]
        var iv = bytes(-12)
        iv.seti(8,self.session.seq_nr_rx,-4)
        c.chacha_run(self.session.KEY_C_S_header,iv,0,length)
        return length.geti(0,-4)
    end

    def check_packet()
        import crypto
        var c = crypto.CHACHA20_POLY1305()
        var iv = bytes(-12)
        iv.seti(8,self.session.seq_nr_rx,-4)
        var data = self.buf[0.. self.packet_length  + 3]
        var poly_key = bytes(-32)
        c.chacha_run(self.session.KEY_C_S_main, iv, 0 ,poly_key)
        var given_mac = self.buf[self.packet_length+4..self.packet_length+19]
        var mac = c.poly_run(data,poly_key)
        if mac != given_mac
            print("MAC MISMATCH!!",mac, given_mac, size(given_mac))
        end
    end

    def decrypt()
        import crypto
        var c = crypto.CHACHA20_POLY1305()
        var iv = bytes(-12)
        iv.seti(8,self.session.seq_nr_rx,-4)
        var data = self.buf[4..-17]
        c.chacha_run(self.session.KEY_C_S_main, iv, 1, data)
        self.buf.setbytes(4,data)
        # print(self.buf, size(self.buf))
        return 
    end

    def decode()
        self.padding_length = self.buf[4]
        self.payload_length = self.packet_length - self.padding_length - 1
        # print(self.packet_length, self.padding_length, self.payload_length)
        self.payload = self.buf[5 .. 5 + self.payload_length - 1]
        self.padding = self.buf[5 + self.payload_length .. 5 + self.payload_length + self.padding_length - 1]
        # print(self.payload)
    end

    def append(buf)
        self.buf .. buf
        if size(self.buf) > self.expected_length
            print("must split TCP packet:",self.expected_length,size(self.buf) - self.expected_length)
            self.session.overrun_buf = self.buf[self.expected_length ..]
        end
        if size(self.buf) >= self.expected_length
            self.complete = true
            if self.encrypted == true
                self.check_packet()
                self.decrypt()
            end
            self.decode()
        else
            self.complete = false
        end
    end

    def encrypt(packet)
        # print(packet)
        import crypto
        var c = crypto.CHACHA20_POLY1305()
        var iv = bytes(-12)
        iv.seti(8,self.session.seq_nr_tx,-4)
        var length = packet[0..3]
        c.chacha_run(self.session.KEY_S_C_header,iv,0,length)
        var data = packet[4..]
        c.chacha_run(self.session.KEY_S_C_main, iv, 1, data)
        var enc_packet = length + data

        var poly_key = bytes(-32)
        c.chacha_run(self.session.KEY_S_C_main,iv,0,poly_key)
        var mac = c.poly_run(enc_packet,poly_key)

        return enc_packet + mac
    end

    def create(payload, encrypted)
        import crypto
        var paylength = size(payload)
        var padlength = 8-((5 + paylength)%8)
        if encrypted == true
            padlength -= 4
        end
        if padlength < 5
            padlength += 16
        end
        var padding = crypto.random(padlength)
        var bin = bytes(256)
        bin.add(1 + paylength + padlength, -4)
        bin .. padlength
        bin .. payload
        bin .. padding
        if encrypted == true
            return self.encrypt(bin)
        end
        return bin
    end
end


class HANDSHAKE
    var state, bin_packet, session

    var   V_C # client's identification string (CR and LF excluded)
    static V_S = "SSH-2.0-TasmotaSSH_0.1" # server's identification string (CR and LF excluded)
    var   I_C # payload of the client's SSH_MSG_KEXINIT
    var   I_S # payload of the server's SSH_MSG_KEXINIT
    var   K_S # server's public host key
    var   Q_C # client's ephemeral public key octet string
    var   Q_S # server's ephemeral public key octet string
    var   K   # shared secret

    var   H   # hash of above

    var   host_key # server's secret host key bytes


    def init(session)
        self.state = 0
        self.create_host_keys()
        self.session = session
    end

    def create_host_keys()
        import crypto
        var ed = crypto.ED25519()
        var example_seed = bytes("a60c6c7107be5da01ba7f7bc6a08e1d0faa27e1db9327514823fdac5f8e750dd") # could be any crypto.random(32)
        self.host_key = ed.secret_key(example_seed) #bytes("a60c6c7107be5da01ba7f7bc6a08e1d0faa27e1db9327514823fdac5f8e750dd")
        var pk = bytes(64)
        SSH_MSG.add_string(pk, "ssh-ed25519")
        SSH_MSG.add_string(pk,self.host_key[-32..]) # public key is simply the last 32 bytes of the secret key
        self.K_S = pk
    end

    def kexinit_to_client()
        import crypto
        var	cookie  = crypto.random(16)
        var	kex_algorithms = "curve25519-sha256,kex-strict-s-v00@openssh.com,kex-strict-s"
        var	server_host_key_algorithms = "ssh-ed25519"
        var	encryption_algorithms_client_to_server = "chacha20-poly1305@openssh.com"
        var	encryption_algorithms_server_to_client = "chacha20-poly1305@openssh.com"
        var	mac_algorithms_client_to_server = ""
        var	mac_algorithms_server_to_client = ""
        var compression_algorithms_client_to_server = "none"
        var compression_algorithms_server_to_client = "none"
        var	languages_client_to_server = ""
        var languages_server_to_client = ""

        var payload = bytes(256)
        payload .. SSH_MSG.KEXINIT
        payload .. cookie
        SSH_MSG.add_string(payload,kex_algorithms)
        SSH_MSG.add_string(payload,server_host_key_algorithms)
        SSH_MSG.add_string(payload,encryption_algorithms_client_to_server)
        SSH_MSG.add_string(payload,encryption_algorithms_server_to_client)
        SSH_MSG.add_string(payload,mac_algorithms_client_to_server)
        SSH_MSG.add_string(payload,mac_algorithms_server_to_client)
        SSH_MSG.add_string(payload,compression_algorithms_client_to_server)
        SSH_MSG.add_string(payload,compression_algorithms_client_to_server)
        SSH_MSG.add_string(payload,languages_client_to_server)
        SSH_MSG.add_string(payload,languages_server_to_client)
        payload .. 0 # false - first_kex_follows
        payload.add(0,-4) # reserved
        self.I_S = payload.copy()
        return self.bin_packet.create(payload)
    end

    def create_KEX_ECDH_REPLY()
        import crypto
        var hash = bytes(2048)
        SSH_MSG.add_string(hash, self.V_C)
        SSH_MSG.add_string(hash, self.V_S)
        SSH_MSG.add_string(hash, self.I_C)
        SSH_MSG.add_string(hash, self.I_S)
        SSH_MSG.add_string(hash, self.K_S)
        SSH_MSG.add_string(hash, self.Q_C)
        SSH_MSG.add_string(hash, self.Q_S)
        SSH_MSG.add_mpint(hash, self.K)

        var sha256 = crypto.SHA256()
        sha256.update(hash)
        self.H = sha256.out()

        var eddsa25519 = crypto.ED25519()
        var SIG = eddsa25519.sign(self.H,self.host_key)
        print(SIG)

        var payload = bytes(256)
        payload .. SSH_MSG.KEX_ECDH_REPLY
        # print(self.K_S, size(self.K_S), self.Q_S, size(self.Q_S), H, size(H) )
        SSH_MSG.add_string(payload, self.K_S)
        SSH_MSG.add_string(payload, self.Q_S)
        var HS = bytes(128)
            SSH_MSG.add_string(HS, "ssh-ed25519")
            SSH_MSG.add_string(HS,SIG)
            SSH_MSG.add_string(payload, HS)
        return self.bin_packet.create(payload)
    end

    def create_ephemeral(payload)
        log("SSH: will create ephemeral keys",2)
        import crypto
        var ephem_key = crypto.random(32)
        self.Q_S = (crypto.EC_C25519().public_key(ephem_key))
        self.Q_C = payload[5..]
        self.K = (crypto.EC_C25519().shared_key(ephem_key, self.Q_C))
        # print(ephem_key self.Q_S, self.K)
        return self.create_KEX_ECDH_REPLY()
    end

    def kexinit_from_client() # mainly logging function
        import string
        var buf = self.bin_packet.payload
        var k = {}
        log(f"cookie: {buf[1..16].tohex()}",3)
        var next_index = 17
        var next_length = SSH_MSG.get_item_length(buf[next_index..])
        log(f"kex_algorithms: {SSH_MSG.get_name_list(buf, next_index, next_length)}",3)
        for i:SSH_MSG.get_name_list(buf, next_index, next_length)
            if string.find(i, "kex-strict-c") self.session.strict_mode = true end
        end
        next_index += next_length + 4
        next_length = SSH_MSG.get_item_length(buf[next_index..])
        log(f"server_host_key_algorithms: {SSH_MSG.get_name_list(buf, next_index, next_length)}",3)
        next_index += next_length + 4
        next_length = SSH_MSG.get_item_length(buf[next_index..])
        log(f"encryption_algorithms_client_to_server: {SSH_MSG.get_name_list(buf, next_index, next_length)}",3)
        next_index += next_length + 4
        next_length = SSH_MSG.get_item_length(buf[next_index..])
        log(f"encryption_algorithms_server_to_client: {SSH_MSG.get_name_list(buf, next_index, next_length)}",3)
        next_index += next_length + 4
        next_length = SSH_MSG.get_item_length(buf[next_index..])
        log(f"mac_algorithms_client_to_server: {SSH_MSG.get_name_list(buf, next_index, next_length)}",3)
        next_index += next_length + 4
        next_length = SSH_MSG.get_item_length(buf[next_index..])
        log(f"mac_algorithms_server_to_client: {SSH_MSG.get_name_list(buf, next_index, next_length)}",3)
        next_index += next_length + 4
        next_length = SSH_MSG.get_item_length(buf[next_index..])
        log(f"compression_algorithms_client_to_server: {SSH_MSG.get_name_list(buf, next_index, next_length)}",3)
        next_index += next_length + 4
        next_length = SSH_MSG.get_item_length(buf[next_index..])
        log(f"compression_algorithms_server_to_client: {SSH_MSG.get_name_list(buf, next_index, next_length)}",3) 
        next_index += next_length + 4
        next_length = SSH_MSG.get_item_length(buf[next_index..])
        log(f"languages_client_to_server: {SSH_MSG.get_name_list(buf, next_index, next_length)}",3)
        next_index += next_length + 4
        next_length = SSH_MSG.get_item_length(buf[next_index..])
        log(f"languages_server_to_client: {SSH_MSG.get_name_list(buf, next_index, next_length)}",3)
        next_index += next_length + 4
        log(f"first_kex_packet_follows: {buf[next_index]}",3)
    end

    def send_NEWKEYS()
        log("SSH: confirm to be ready for new keys",2)
        var payload = bytes(-1)
        payload[0] = SSH_MSG.NEWKEYS
        self.session.prepare(self.K,self.H)
        return self.bin_packet.create(payload)
    end

    def process(buf)
        var response = bytes()
        if self.state == 0
            self.state = 1
            self.V_C  = buf[0..-3].asstring() # strip LF
            return f"{self.V_S}\r\n"
        elif self.state == 1
            if self.bin_packet
                self.bin_packet.append(buf)
            else
                self.bin_packet = BIN_PACKET(buf,self.session, false)
            end
            if self.bin_packet.complete == true
                if self.bin_packet.payload[0] == SSH_MSG.KEXINIT
                    self.I_C = self.bin_packet.payload.copy()
                    self.kexinit_from_client()
                    response = self.kexinit_to_client()
                elif self.bin_packet.payload[0] == SSH_MSG.KEXDH_INIT
                    response = self.create_ephemeral(self.bin_packet.payload)
                elif self.bin_packet.payload[0] == SSH_MSG.NEWKEYS
                    response = self.send_NEWKEYS()
                    self.state = 2
                elif self.bin_packet.payload[0] == SSH_MSG.DISCONNECT
                    log("SSH: client did disconnect",1)
                else
                    print("SSH: unknown packet type", self.bin_packet.payload[0])
                end
                self.bin_packet = nil
            else
                self.session.seq_nr_rx -= 1 # TODO: check
            end
            return response
        elif self.state == 2

        end
        log("SSH: unknown packet")
        return ""
    end
end

class SESSION
    var up, strict_mode
    var H, K, ID
    var bin_packet
    var KEY_C_S_main, KEY_S_C_main, KEY_C_S_header, KEY_S_C_header
    var seq_nr_rx, seq_nr_tx, channel_nr
    var send_queue, overrun_buf
    var type # terminal or SFTP

    static user = "admin"
    static password = "1234"

    static banner = "  / \\    Secure Wireless Serial Interface\n"
                    "/ /|\\ \\  SSH Terminal Server on %s\n"
                    "  \\_/    Copyright (C) 2025 Tasmota %s\n"
                    "%s \n"

    def init()
        self.up = false
        self.seq_nr_rx = -2 # very unsure about this!!!
        self.seq_nr_tx = -1
        self.send_queue = []
        self.strict_mode = false # support by client
    end

    def send_banner()
        var r = bytes()
        r .. SSH_MSG.USERAUTH_BANNER
        var s2 = tasmota.cmd("status 2")["StatusFWR"]
        var hw = s2["Hardware"]
        var vs = s2["Version"]
        var strict_mode = ""
        if self.strict_mode == false
            strict_mode = "\n\r WARNING: outdated SSH-client, connection is vulnerable to Terrapin!!!\r\n"
        end
        SSH_MSG.add_string(r,format(self.banner,hw,vs,tasmota.memory().tostring()) + strict_mode)
        SSH_MSG.add_string(r,"") # language
        var p = BIN_PACKET(bytes(-32),self,false)
        self.overrun_buf = nil
        return p.create(r ,true)
    end

    def handle_service_request()
        var name = SSH_MSG.get_string(self.bin_packet.payload, 1, SSH_MSG.get_item_length(self.bin_packet.payload[1..]))
        print("service request:",name)
        if name == "ssh-userauth"
            var r = bytes(64)
            r .. SSH_MSG.SERVICE_ACCEPT
            SSH_MSG.add_string(r,name)
            var enc_r = self.bin_packet.create(r ,true)
            self.send_queue.push(/->self.send_banner())
            return enc_r
        end
        var r = bytes(64)
        r .. SSH_MSG.USERAUTH_SUCCESS
        print("unhandled request",r)
        var enc_r = self.bin_packet.create(r ,true)
        return enc_r
    end


    def handle_userauth_request()
        var r = bytes(32)
        var buf = self.bin_packet.payload
        var next_index = 1
        var next_length = SSH_MSG.get_item_length(buf[next_index..])
        var user_name = SSH_MSG.get_string(buf, next_index, next_length)
        if user_name != self.user
            r .. SSH_MSG.USERAUTH_FAILURE
            SSH_MSG.add_string(r,"unknown user")
            r .. 0
            var enc_r = self.bin_packet.create(r ,true)
            return enc_r
        end
        next_index += next_length + 4
        next_length = SSH_MSG.get_item_length(buf[next_index..])
        var service_name = SSH_MSG.get_string(buf, next_index, next_length)
        next_index += next_length + 4
        next_length = SSH_MSG.get_item_length(buf[next_index..])
        var method_name = SSH_MSG.get_string(buf, next_index, next_length)
        if method_name == "none"
            r .. SSH_MSG.USERAUTH_FAILURE
            SSH_MSG.add_string(r,"password")
            r .. 0
            var enc_r = self.bin_packet.create(r ,true)
            return enc_r
        end
        next_index += next_length + 4
        var bool_field = buf[next_index]
        next_index += 1
        next_length = SSH_MSG.get_item_length(buf[next_index..])
        var key_algo = SSH_MSG.get_string(buf, next_index, next_length) #var name is "context sensitive"
        if method_name == "password"
            if key_algo != self.password
                r .. SSH_MSG.USERAUTH_FAILURE
                SSH_MSG.add_string(r,"wrong password")
                r .. 0
                var enc_r = self.bin_packet.create(r ,true)
                return enc_r
            end
        end
        next_index += next_length + 4
        next_length = SSH_MSG.get_item_length(buf[next_index..])
        var algo_blob = SSH_MSG.get_string(buf, next_index, next_length) #var name is "context sensitive"
        print(user_name,service_name,method_name,bool_field,key_algo,algo_blob)
        r .. SSH_MSG.USERAUTH_SUCCESS
        var enc_r = self.bin_packet.create(r ,true)
        return enc_r
    end

    def handle_channel_open()
        var buf = self.bin_packet.payload
        var next_index = 1
        var next_length = SSH_MSG.get_item_length(buf[next_index..])
        var channel_type = SSH_MSG.get_string(buf, next_index, next_length)
        next_index += next_length + 4
        self.channel_nr = buf.geti(next_index,-4)
        next_index += 4
        var window_size = buf.geti(next_index,-4)
        next_index += 4
        var packet_size = buf.geti(next_index,-4)
        print(channel_type,self.channel_nr,window_size,packet_size)
        var r = bytes(64)
        r .. SSH_MSG.CHANNEL_OPEN_CONFIRMATION
        r.add(self.channel_nr,-4)
        r.add(self.channel_nr,-4)
        r.add(window_size,-4)
        r.add(packet_size,-4)
        print(r)
        var enc_r = self.bin_packet.create(r ,true)
        return enc_r
    end

    def handle_channel_request()
        var buf = self.bin_packet.payload
        var next_index = 1
        var channel = buf.geti(next_index,4)
        next_index += 4
        var next_length = SSH_MSG.get_item_length(buf[next_index..])
        var req_type_type = SSH_MSG.get_string(buf, next_index, next_length)
        next_index += next_length + 4
        var want_reply = buf[next_index]
        next_index += 1
        next_length = SSH_MSG.get_item_length(buf[next_index..])
        var term = SSH_MSG.get_string(buf, next_index, next_length)
        next_index += next_length + 4
        var width_c = buf.geti(next_index,-4)
        next_index += 4
        var height_c = buf.geti(next_index,-4)
        next_index += 4
        var width_p = buf.geti(next_index,-4)
        next_index += 4
        var height_p = buf.geti(next_index,-4)
        next_index += 4
        next_length = SSH_MSG.get_item_length(buf[next_index..])
        var terminal_modes = SSH_MSG.get_string(buf, next_index, next_length)
        print(channel,req_type_type,want_reply,term,width_c,height_c,width_p,height_p)
        if req_type_type == "shell"
            self.type = TERMINAL()
        elif req_type_type == "subsystem" && term == "sftp"
            self.type = SFTP()
        end
        var r = bytes(64)
        if want_reply
            r .. SSH_MSG.CHANNEL_SUCCESS # TODO: may really check
        else
            r .. SSH_MSG.IGNORE
        end
        r.add(self.channel_nr,-4)
        print(r)
        var enc_r = self.bin_packet.create(r ,true)
        return enc_r
    end

    def handle_channel_data()
        var buf = self.bin_packet.payload
        var next_index = 1
        var channel = buf.geti(next_index,-4)
        next_index += 4
        var next_length = SSH_MSG.get_item_length(buf[next_index..])
        var data = SSH_MSG.get_bytes(buf, next_index, next_length)
        next_index += next_length + 4
        var t_r = self.type.process(data)
        var r = bytes()
        r .. SSH_MSG.CHANNEL_DATA
        r.add(self.channel_nr,-4)
        SSH_MSG.add_string(r,t_r)
        var enc_r = self.bin_packet.create(r ,true)
        return enc_r
    end

    def process(data)
        var r = bytes()
        if self.bin_packet
            self.bin_packet.append(data)
        else
            self.bin_packet = BIN_PACKET(data, self ,true)
        end
        if self.bin_packet.complete == true
            if self.bin_packet.payload[0] == SSH_MSG.SERVICE_REQUEST
                return self.handle_service_request()
            elif self.bin_packet.payload[0] == SSH_MSG.USERAUTH_REQUEST
                print("USERAUTH_REQUEST")
                return self.handle_userauth_request()
            elif self.bin_packet.payload[0] == SSH_MSG.CHANNEL_OPEN
                print("CHANNEL_OPEN__REQUEST")
                return self.handle_channel_open()
            elif self.bin_packet.payload[0] == SSH_MSG.CHANNEL_REQUEST
                print("CHANNEL_REQUEST")
                return self.handle_channel_request()
            elif self.bin_packet.payload[0] == SSH_MSG.CHANNEL_DATA
                print("CHANNEL_DATA")
                return self.handle_channel_data()
            end
            print("unhandled message type", self.bin_packet.payload[0])
        end
        r .. SSH_MSG.IGNORE
        var enc_r = self.bin_packet.create(r ,true)
        return enc_r
    end

    def generate_keys(K,H,third,id)
        import crypto
        var sha256 = crypto.SHA256()
        sha256.update(SSH_MSG.make_mpint(K))
        sha256.update(H)
        if classof(third) != bytes
            sha256.update(bytes().fromstring(third))
        else
            sha256.update(third)
        end
        if id != nil
            sha256.update(id)
        end
        return sha256.out()
    end

    def prepare(K,H)
        self.K = K
        self.H = H
        self.ID = H.copy()
        self.KEY_C_S_main = self.generate_keys(K,H,"C",H)
        self.KEY_C_S_header = self.generate_keys(K,H,self.KEY_C_S_main)
        self.KEY_S_C_main = self.generate_keys(K,H,"D",H)
        self.KEY_S_C_header = self.generate_keys(K,H,self.KEY_S_C_main)
        print("Did create session keys:")
        # print(self.KEY_C_S_main, self.KEY_C_S_header, self.KEY_S_C_main, self.KEY_S_C_header)
        self.up = true
        if self.strict_mode == true
            self.seq_nr_rx = -1 # reset to handle Terrapin attack
            self.seq_nr_tx = -1
        end
    end
end

class SSH : Driver

    var connection, server, client
    var handshake, session
    static port = 22

    def init()
        self.server = tcpserver(self.port) # connection for control data
        self.connection = false
        tasmota.add_driver(self)
        log(f"SSH: init server on port {self.port}",1)
    end

    def every_50ms()
        if self.connection == true
            self.loop()
        elif self.server.hasclient()
            self.client = self.server.acceptasync()
            self.session = SESSION()
            self.handshake = HANDSHAKE(self.session)
            self.connection = true
            self.pubClientInfo()
        else
            self.handshake = nil
            self.connection = false
        end
    end

    def every_second()
        if self.client && self.connection != false
            if self.client.connected() == false
                self.pubClientInfo()
                self.connection = false
            end
        end
    end

    def pubClientInfo()
        import mqtt
        var payload = self.client.info().tostring()
        mqtt.publish("SSH",format("{'server':%s}", payload))
    end

    def loop()
        if self.connection == true
            self.handleConnection()
        end
    end

    def sendResponse(resp)
        var session = self.session
        var bin = session.bin_packet
        session.bin_packet = nil
        self.client.write(resp)
        session.seq_nr_tx += 1
        if size(session.send_queue) != 0
            self.client.write(session.send_queue.pop()())
            session.seq_nr_tx += 1
        end
        log(f"SSH: {self.session.seq_nr_tx} >>> {resp} _ {size(resp)} bytes",2)
    end

    def handleConnection() # main loop for incoming commands
        var response
        var d
        if self.session.overrun_buf
            d = self.session.overrun_buf.copy()
            self.session.overrun_buf = nil
        else
            d = self.client.readbytes()
        end
        if size(d) == 0
            return 
        end
        self.session.seq_nr_rx += 1
        log(f"SSH: {self.session.seq_nr_rx} <<< {d} _ {size(d)} bytes",2)
        if self.session.up == true
            response = self.session.process(d)
            if response != ""
                self.sendResponse(response)
            end
        elif self.handshake
            response = self.handshake.process(d)
            if size(response) != 0 
                self.sendResponse(response)
                if size(response) > 5 && response[5] == SSH_MSG.NEWKEYS
                     self.handshake = nil
                end
            end
        end
    end
end

var ssh =  SSH()
