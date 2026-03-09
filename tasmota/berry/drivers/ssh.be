#- Simple SSH server in Berry by Christian Baars
#  includes rudimentary terminal and SFTP server
#  this is demo code and not intended for production use
#  written from scratch, no libraries used
#  DO NOT OPEN A GH ISSUE, but feel free to use GH discussions
#  needs some crypto primitives:
        #define USE_BERRY_CRYPTO_EC_C25519
        #define USE_BERRY_CRYPTO_CHACHA_POLY
        #define USE_BERRY_CRYPTO_ED25519
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

class SFTP_FILE
    var url, file, length, written, is_writing, is_reading, id
    var append_flag, chunk_limit

    #define SSH_FXF_READ            0x00000001
    #define SSH_FXF_WRITE           0x00000002
    #define SSH_FXF_APPEND          0x00000004
    #define SSH_FXF_CREAT           0x00000008
    #define SSH_FXF_TRUNC           0x00000010
    #define SSH_FXF_EXCL            0x00000020

    def init(url, pflags)
        import path
        if path.exists(url) != true
            if pflags&1 == false && pflags&4 == false
                return nil
            end
        end
        if pflags&1
           self.file = open(url,"r")
           log(f"SFTP: open file for read {url}",4)
        end
        if pflags&2
            self.file = open(url,"w")
            log(f"SFTP: open file for write {log}",4)
        end
        if pflags&4
            self.append_flag = true
            log(f"SFTP: open file for append {log}",4)
        else
            self.append_flag = false
        end
        self.url = url
        self.is_writing = false
        self.chunk_limit = 4096
    end

    def deinit()
        self.close()
    end

    def write(data, offset, id)
        log(f"SFTP: write file {data} at position {offset}",3)
        if self.append_flag == false
            self.file.seek(offset)
        end

        self.length = data.geti(0,-4)
        log(f"SFTP: file length {self.length}", 3)

        self.id = id
        self.written = size(data) - 4
        if self.written < self.length
            self.is_writing = true
        end
        return self.file.write(data[4..]) # skip length
    end

    def append(data)
        if self.file
            self.written += size(data)
            if self.written == self.length
                self.is_writing = false
            end
            return self.file.write(data)
        end
    end

    def read(len, offset, id)
        self.file.seek(offset)
        if len > self.chunk_limit # stay below 4096 max packet size in the end
            len = self.chunk_limit
        end
        if self.file
            var b = self.file.readbytes(len)
            return b
        end
        return nil
    end

    def close()
        log(f"SFTP: close file {self.url}",3)
        if self.file
            self.file.close()
        end
    end 
end

class SFTP
    static INIT     = 1
    static VERSION  = 2
    static OPEN     = 3
    static CLOSE    = 4
    static READ     = 5
    static WRITE    = 6
    static LSTAT    = 7
    static FSETSTAT = 10
    static OPENDIR  = 11
    static READDIR  = 12
    static REALPATH = 16
    static STAT     = 17
    static STATUS   = 101
    static DATA     = 103
    static NAME     = 104
    static ATTRS    = 105

    var session, dir_list, dir, file

    def init(session)
        self.session = session
        self.dir = PATH()
        log("SFTP started .. very incomplete!",1)
    end

    def fsize(url)
        import path
        if path.exists(url) == true && path.isdir(url) == false
            var f = open(url,"r")
            var sz = f.size()
            f.close()
            return sz
        end
        return 0
    end

    def fdate(url)
        import path
        if path.exists(url) == true
            return path.last_modified(url)
        end
        return 0
    end

    def long_name(url)
        var date = self.fdate(url)
        var sz = self.fsize(url)
        var m = tasmota.strftime("%B", date)[0..2]
        var dt = tasmota.strftime("%d %H:%M", date)
        var pre = "-"
        if sz ==0 pre = "d" end # TODO: really check if dir
        return f"{pre}rwxrwxr-x   1 admin    all      {sz:8i} {m} {dt} {url}"
    end

    def read_dir(url, id)
        if size(self.dir_list) == 0
            return self.status(id, 1) # EOF
        end
        var r = bytes("00000000") # size
        r .. SFTP.NAME
        r .. id
        r.add(size(self.dir_list),-4) # count
        for i:self.dir_list
            SSH_MSG.add_string(r,i)
            SSH_MSG.add_string(r,self.long_name(i))
            r .. self.attribs(i) # file attributes
        end
        r.seti(0,size(r)-4,-4)
        self.dir_list = []
        return r
    end

    def attribs(url)
        import path
        var date = self.fdate(url)
        var sz = self.fsize(url)
        var perms = 777
        var a = bytes("0800000f") # flags for extended size|uid|perm|time
        a.add(0, -4)      # high bytes of size
        a.add(sz,-4)      # is uint64
        a.add(0,-4)      # uid - superuser
        a.add(0,-4)      # gid - superuser
        if path.isdir(url)
            a.add(perms|40000, -4) # permissions for dir
        else
            a.add(perms|100000, -4) # permissions for file
        end
        a.add(date,-4)
        a.add(date,-4)
        return a
    end

    def status(id,code)
        var s = bytes("0000000065") # packet type SSH_FXP_STATUS 101
        s .. id
        s.add(code,-4)
        s .. bytes(-8) # two empty strings
        s.seti(0,size(s)-4,-4)
        log(f"SFTP: status {code} for {id}",4)  
        return s
    end

    def handle(id,url)
        var h = bytes("0000000066") # packet type SSH_FXP_HANDLE 102
        h .. id
        SSH_MSG.add_string(h,url)
        h.seti(0,size(h)-4,-4)
        return h
    end

    def stat_for_url(id, url)
        import path
        if path.exists(url)
            var r = bytes("00000000") # size
            r .. SFTP.ATTRS
            r..id
            r .. self.attribs(url) # file attributes
            r.seti(0,size(r)-4,-4)
            return r
        end
        return self.status(id, 2) # NO_SUCH_FILE
    end

    def open_file(id,url,pflags,attr)
        self.file = SFTP_FILE(url,pflags)
        if self.file
            return self.handle(id,url)
        end
        return self.status(id, 2) # NO_SUCH_FILE
    end

    def path_name(url,id)
        var r = bytes("00000000") # size
        r .. SFTP.NAME
        r .. id
        r.add(1,-4) # count
        SSH_MSG.add_string(r,url)
        SSH_MSG.add_string(r,"")
        r .. self.attribs(url) # file attributes
        r.seti(0,size(r)-4,-4)
        return r
    end

    def process(d)
        log(f"SFTP: process SFTP __________________________",3)
        var r = bytes()
        var unfinished = true
        var ptype, id
        if self.file
            log(f"SFTP: file is open {self.file.url} {self.file.written} {self.file.length} {self.file.is_writing}",4)
            if self.file.is_writing == true
                log(f"SFTP: append {d}",3)
                self.file.append(d)
                if self.file.is_writing == false
                    return self.status(self.file.id, 0) # SSH_FX_OK
                end
                return "" # will resolve later to MSG_IGNORE
            end
        end
        if self.file
            var cmds = size(d)/32
            if cmds == 0
                cmds = 1
            end
            self.file.chunk_limit = 4096/cmds # read command 32 bytes
            log(f"SSH: multiple commands: {cmds}",3)
        end
        while unfinished == true
            ptype = d[4] # https://datatracker.ietf.org/doc/html/draft-ietf-secsh-filexfer-02#section-3
            id = d[5..8]
            log(f"SFTP: type {ptype}, id {id}, data {d}", 3)
            if ptype == SFTP.INIT
                r = bytes('000000050200000003') # no extended data support, ver 3
            elif ptype == SFTP.LSTAT
                var url = d[13..].asstring()
                log(f"SFTP LSTAT for: {url}",3) 
                r = self.stat_for_url(id,url)
            elif ptype == SFTP.STAT
                var url = d[13..].asstring()
                log(f"SFTP STAT for: {url}",3) 
                r = self.stat_for_url(id,url)
            elif ptype == SFTP.OPEN
                var next_index = 9
                var next_length = SSH_MSG.get_item_length(d[next_index..])
                var url = SSH_MSG.get_string(d, next_index, next_length)
                next_index += next_length + 4
                var pflags = d.geti(next_index,-4)
                next_index += 4
                var attr = d[next_index..]
                log(f"SFTP OPEN: {url} with {pflags} and {attr}",3)
                r = self.open_file(id,url,pflags,attr)
            elif ptype == SFTP.READ
                var next_index = 9
                var next_length = SSH_MSG.get_item_length(d[next_index..])
                var url = SSH_MSG.get_string(d, next_index, next_length)
                next_index += next_length + 8
                var offset = d.geti(next_index,-4) # uint64
                next_index += 4
                var len = d.geti(next_index,-4) # uint32
                next_index += 4
                log(f"SFTP READ: {url} - {len} bytes from {offset}",3)
                var fbytes = self.file.read(len,offset,id)
                if size(fbytes) == 0
                    r = self.status(id, 1) # FX_EOF  1
                else
                    var _r = bytes("00000000") # size
                    _r .. SFTP.DATA
                    _r .. id
                    SSH_MSG.add_string(_r, fbytes)
                    _r.seti(0,size(_r)-4,-4)
                    r .. _r
                end
                if next_index < size(d) - 9
                    unfinished = true
                    d = d[next_index..]
                    continue
                end
            elif ptype == SFTP.WRITE
                var next_index = 9
                var next_length = SSH_MSG.get_item_length(d[next_index..])
                var url = SSH_MSG.get_string(d, next_index, next_length)
                next_index += next_length + 8
                var offset = d.geti(next_index,-4) # uint64
                next_index += 4
                var data = d[next_index..]
                log(f"SFTP WRITE: {url}",3)
                self.file.write(data,offset, id) # Todo: check success
                if self.file.is_writing == false
                    r = self.status(self.file.id, 0) # SSH_FX_OK
                else
                    r = "" # -> MSG_IGNORE
                end
            elif ptype == SFTP.OPENDIR
                var url = d[13..].asstring()
                if url == ""
                    url = "/"
                end
                log(f"SFTP OPENDIR: {url}",3)
                if self.dir.set(url)
                    import path
                    self.dir_list = path.listdir(self.dir.get_url())
                    r = self.handle(id,url)
                else
                    r = self.status(id, 2) # NO_SUCH_FILE
                end
            elif ptype == SFTP.READDIR
                var url = d[13..].asstring()
                log(f"SFTP READDIR: {url}",3)
                r = self.read_dir(url,id)
            elif ptype == SFTP.CLOSE
                log("SFTP CLOSE",3)
                r = self.status(id, 0) # SSH_FX_OK
                self.file = nil
            elif ptype == SFTP.REALPATH
                log("SFTP REALPATH",3)
                var url = d[13..].asstring()
                if url == "."
                    url = "/"
                end 
                r = self.path_name(url,id)
            elif ptype == SFTP.FSETSTAT
                log("SFTP FSETSTAT",3)
                #ignore for now
                self.file.close()
                r = self.status(id, 0) # SSH_FX_OK
            else
                log(f"SFTP: unknown packet type {ptype}", 2)
                r = self.status(id,8) #OP_UNSUPPORTED
            end
        unfinished = false
        end
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
            log(f"SSH: new bin_packet with {self.packet_length} bytes",3)
            self.expected_length = self.packet_length + 4 + 16 # mac
        end
        if self.expected_length > 32768
            log(f"SSH: Unusual high packet length {self.expected_length} - assume decoding error!!",1)
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
            #TODO: disconect
            log(f"SSH: MAC MISMATCH!! {mac} - {given_mac} ", 1)
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
            log(f"must split TCP packet:{self.expected_length} _ {size(self.buf) - self.expected_length} ",4)
            self.session.overrun_buf = self.buf[self.expected_length ..]
        end
        if size(self.buf) >= self.expected_length
            log(f"SSH: got complete packet: {self.expected_length} _ {size(self.buf)}",4)
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
        # print(SIG)

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
        log("SSH: create ephemeral keys",3)
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
            if string.find(i, "kex-strict-c") >= 0 self.session.strict_mode = true end
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
        log("SSH: send new keys",2)
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
                    log("SSH: unknown packet type: {self.bin_packet.payload[0]}", 1)
                end
                self.bin_packet = nil
            end
            return response
        elif self.state == 2

        end
        log("SSH: unknown packet",1)
        return ""
    end
end

class SESSION
    var up, strict_mode, client_pub_key
    var H, K, ID
    var bin_packet
    var KEY_C_S_main, KEY_S_C_main, KEY_C_S_header, KEY_S_C_header
    var seq_nr_rx, seq_nr_tx, channel_nr
    var send_queue, overrun_buf
    var type # terminal or SFTP

    static MAX_PACKET_SIZE = 4096 # we must process the whole packet (crypt, auth, etc)

    static user = "admin"
    static password = "1234"

    static banner = "  / \\    Secure Wireless Serial Interface\n"
                    "/ /|\\ \\  SSH Terminal Server on %s\n"
                    "  \\_/    Copyright (C) 2025 Tasmota %s\n"

    def init()
        self.up = false
        self.seq_nr_rx = -2 # very unsure about this!!!
        self.seq_nr_tx = -1
        self.send_queue = []
        self.strict_mode = false # support by client
    end

    def deinit()
        self.type = nil
        self.bin_packet = nil
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
        SSH_MSG.add_string(r,format(self.banner,hw,vs) + strict_mode)
        SSH_MSG.add_string(r,"") # language
        var p = BIN_PACKET(bytes(-32),self,false)
        self.overrun_buf = nil
        return p.create(r ,true)
    end

    def check_pub_key()
        import persist
        var r = bytes(32)
        if persist.known_hosts == nil
            persist.known_hosts = []
        end
        for key:persist.known_hosts
            if key == self.client_pub_key
                log("SSH: known client",2)
                r .. SSH_MSG.USERAUTH_SUCCESS
                var enc_r = self.bin_packet.create(r ,true)
                return enc_r
            end
        end
        r .. SSH_MSG.USERAUTH_FAILURE
        SSH_MSG.add_string(r,"password")
        r .. 0
        var enc_r = self.bin_packet.create(r ,true)
        return enc_r
    end

    def handle_service_request()
        var name = SSH_MSG.get_string(self.bin_packet.payload, 1, SSH_MSG.get_item_length(self.bin_packet.payload[1..]))
        log(f"SSH: service request: {name}",2)
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
        log(f"SSH: unhandled request {r}",1)
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
            SSH_MSG.add_string(r,"publickey,password")
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
        var algo_blob = SSH_MSG.get_bytes(buf, next_index, next_length) #var name is "context sensitive"
        if method_name == "publickey"
            log(f"SSH: public key auth: {key_algo}",2)
            self.client_pub_key = algo_blob[-32..].tohex()
            return self.check_pub_key()
        end
        # print(user_name,service_name,method_name,bool_field,key_algo,size(algo_blob),algo_blob)
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
        log(f"SSH: type {channel_type}, nr{self.channel_nr}, window size {window_size}, packet size {packet_size}",2)
        var r = bytes(64)
        r .. SSH_MSG.CHANNEL_OPEN_CONFIRMATION
        r.add(self.channel_nr,-4)
        r.add(self.channel_nr,-4)
        r.add(window_size,-4)
        r.add(SESSION.MAX_PACKET_SIZE,-4)
        # print(r)
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
        log(f"{channel},{req_type_type},{want_reply},{term,width_c},{height_c},{width_p},{height_p}",3)
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
        # print(r)
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
        log(f"SSH: ch {channel} data {next_length} {data}",3)
        var t_r = self.type.process(data)
        if t_r == ""
            # self.seq_nr_rx -= 1 # pending write job or something else
            var r = bytes()
            r .. SSH_MSG.IGNORE
            var enc_r = self.bin_packet.create(r ,true)
            return enc_r
        end
        var r = bytes()
        r .. SSH_MSG.CHANNEL_DATA
        r.add(self.channel_nr,-4)
        SSH_MSG.add_string(r,t_r)
        var enc_r = self.bin_packet.create(r ,true)
        return enc_r
    end

    def close_channel()
        var r = bytes(16)
        r .. SSH_MSG.CHANNEL_CLOSE
        r.add(self.channel_nr,-4)
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
                log("USERAUTH_REQUEST",3)
                return self.handle_userauth_request()
            elif self.bin_packet.payload[0] == SSH_MSG.CHANNEL_OPEN
                log("CHANNEL_OPEN__REQUEST",3)
                return self.handle_channel_open()
            elif self.bin_packet.payload[0] == SSH_MSG.CHANNEL_REQUEST
                log("CHANNEL_REQUEST",3)
                return self.handle_channel_request()
            elif self.bin_packet.payload[0] == SSH_MSG.CHANNEL_DATA
                log("CHANNEL_DATA",3)
                return self.handle_channel_data()
            elif self.bin_packet.payload[0] == SSH_MSG.CHANNEL_EOF
                log("CHANNEL_EOF",3)
                return self.close_channel()
            elif self.bin_packet.payload[0] == SSH_MSG.CHANNEL_CLOSE
                log("CHANNEL_CLOSE",3)
                return self.close_channel()
            elif self.bin_packet.payload[0] == SSH_MSG.DISCONNECT
                log("SSH: client did disconnect",1)
                return ""
            else
                log(f"SSH: unhandled session message type: {self.bin_packet.payload[0]}", 2)
            end
        else
            self.seq_nr_rx -= 1 # TODO: check
            return ""
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
        log("SSH: session keys created",3)  
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
    var handshake, session, loop
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
            self.loop = /->self.run_loop()
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
                self.session = nil
                self.client = nil
            end
        end
    end

    def pubClientInfo()
        import mqtt
        var payload = self.client.info().tostring()
        mqtt.publish("SSH",format("{'server':%s}", payload))
    end

    def run_loop()
        if self.connection == true
            self.handleConnection()
        end
    end

    def send(packet)
        if self.client.listening() == false
            log("SSH: client not listening",3)
            self.loop = /->self.send(packet)
            return # back to Tasmota
        end
        var written = self.client.write(packet)
        while written < size(packet)
            log(f"SSH: written only {written} of {size(packet)}",1)
            self.loop = /->self.send(packet[written..])
            return # back to Tasmota
        end
        self.session.seq_nr_tx += 1
        self.loop = /->self.run_loop()
    end

    def sendResponse(resp)
        var session = self.session
        var bin = session.bin_packet
        session.bin_packet = nil
        self.send(resp)
        if size(session.send_queue) != 0
            self.send(session.send_queue.pop()())
        end
        log(f"SSH: {self.session.seq_nr_tx} >>> {resp} _ {size(resp)} bytes",3)
    end

    def handleConnection() # main loop for incoming commands
        var response
        var d
        if self.session.overrun_buf
            d = self.session.overrun_buf.copy()
            self.session.overrun_buf = nil
            log(f"SSH: got overrun packet: {size(d)}",3)
        else
            d = self.client.readbytes()
        end
        if size(d) == 0
            return 
        end
        self.session.seq_nr_rx += 1
        log(f"SSH: {self.session.seq_nr_rx} <<< {d} _ {size(d)} bytes",3)
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

    def key_save()
        if self.session
            if self.session.client_pub_key
                import persist
                if persist.known_hosts == nil
                    persist.known_hosts = []
                end
                for key:persist.known_hosts
                    if key == self.session.client_pub_key
                        tasmota.resp_cmnd_str("SSH: key already known")
                        return
                    end
                end
                persist.known_hosts.push(self.session.client_pub_key)
                persist.save(true)
                tasmota.resp_cmnd_str("SSH: key saved")
            end
        end
    end
end

var ssh =  SSH()

tasmota.add_cmd("ssh_key_save", /->ssh.key_save())
