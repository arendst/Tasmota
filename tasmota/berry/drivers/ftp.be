#- Simple FTP server in Berry by Christian Baars
#  supports active and passive mode - but passive is preferred!
#  only light error handling
-#

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

class FTP : Driver

    var connection, server, client, data_server, data_client, data_ip
    var dir, dir_list, dir_pos
    var file, file_size, file_rename, retries, chunk_size
    var binary_mode, active_ip, active_port, user_input
    var data_buf, data_ptr, fast_loop, data_op
    static port = 21
    static data_port = 20         # data connection in passive mode
    static allow_anonymous = true # allow everything ..
    static user = "user"
    static password = "pass"

    def init()
        self.server = tcpserver(self.port) # connection for control data
        self.connection = false
        self.data_ip = tasmota.wifi()['ip']
        self.dir = PATH()
        self.readDir()
        self.data_ptr = 0
        self.active_port = nil
        tasmota.add_driver(self)
        log(f"FTP: init server on port {self.port}",1)
    end

    def deinit()
        self.server.deinit()
        self.data_server.deinit()
        tasmota.remove_driver(self)
    end

    def every_50ms()
        if self.connection == true
            self.loop()
        elif self.server.hasclient()
            self.client = self.server.acceptasync()
            self.sendResponse("220 Welcome")
            self.connection = true
            self.pubClientInfo()
        else
            self.connection = false
        end
    end

    def every_second()
        if self.client && self.connection != false
            if self.client.connected() == false
                self.pubClientInfo()
                self.connection = false
                self.abortDataOp()
            end
        end
    end

    def pubClientInfo()
        import mqtt
        var payload = self.client.info().tostring()
        mqtt.publish("FTP",format("{'server':%s}", payload))
    end

    def loop()
        if self.connection == true
            self.handleConnection()
        end
    end

    def abortDataOp()
        if self.data_op == "d"
            self.finishDownload(true)
        elif self.data_op == "u"
            self.finishUpload(true)
        elif self.data_op == "dir"
            self.finishUpload(false)
        end
    end

    def download() # ESP -> client
        self.data_buf..self.file.readbytes(self.chunk_size)
        if size(self.data_buf) == 0
            self.retries -= 1
            if self.retries > 0
                return
            end
        else
            var written = self.data_client.write(self.data_buf)
            self.data_buf.clear()
            self.data_ptr += written
            if self.data_ptr < self.file_size
                self.file.seek(self.data_ptr)
                if self.retries > 0
                    return
                end
            end
        end
        self.finishDownload()
    end

    def finishDownload(error)
        self.data_client.close()
        tasmota.remove_fast_loop(self.fast_loop)
        self.file.close()
        if error
            self.sendResponse(f"426 Connection closed; transfer aborted after {self.data_ptr} bytes.") 
        else
            self.sendResponse(f"250 download done with {self.data_ptr} bytes.")
        end
        self.data_op = nil
        tasmota.gc()
    end

    def upload() # client -> ESP
        self.data_buf..self.data_client.readbytes()

        if size(self.data_buf) > 0
            self.file.write(self.data_buf)
            self.data_ptr += size(self.data_buf)
            self.data_buf.clear()
        else
            log(f"FTP: {self.retries} retries",4)
            self.retries -= 1
            if self.retries > 0
                return
            end
            self.finishUpload()
        end
    end

    def finishUpload(error)
        self.data_client.close()
        tasmota.remove_fast_loop(self.fast_loop)
        self.file.close()
        if error
            self.sendResponse(f"426 Connection closed; transfer after {self.data_ptr} bytes")
        else
            self.sendResponse(f"250 upload done with {self.data_ptr} bytes")
        end
        self.data_op = nil
        tasmota.gc()
    end

    def transferDir(mode)
        import path
        var sz, date, isdir
        var i = self.dir_list[self.dir_pos]
        var url = f"{self.dir.get_url()}{i}"
        isdir = path.isdir(url)
        if isdir == false
            var f = open(url,"r")
            sz = f.size()
            f.close()
            date = path.last_modified(url)
        end
        if self.data_client.connected()
            var dir = ""
            if mode == "MLSD"
                if  isdir
                    dir = "Type=dir;Perm=edlmp; "
                else
                    date = tasmota.time_dump(date)
                    var y = str(date['year'])
                    var m = f"{date['month']:02s}"
                    var d = f"{date['day']:02s}"
                    var h = f"{date['hour']:02s}"
                    var min = f"{date['min']:02s}"
                    var sec = f"{date['sec']:02s}"
                    var modif =f"{y}{m}{d}{h}{min}{sec}"
                    dir = f"Type=file;Perm=rwd;Modify={modif};Size={sz}; "
                end
            elif mode == "LIST"
                var d = "-"
                if isdir
                    d = "d"
                    date = ""
                    sz = ""
                else
                    date = tasmota.strftime("%b %d %H:%M", date)
                end
                dir = f"{d}rw-------  1 all all{sz:14s} {date} "

            elif mode == "NLST"
                dir=self.dir.get_url()
            end
            var entry = f"{dir}{i}"
            log(entry,4)
            self.data_client.write(entry + "\r\n")
            self.dir_pos += 1
        else
            self.finishTransferDir(false)
        end
        if self.dir_pos < size(self.dir_list)
            return
        end
        self.finishTransferDir(true)
    end

    def finishTransferDir(success)
        self.data_client.close()
        if success
            var n = size(self.dir_list)
            self.sendResponse(f"226 {n} files in {self.dir.get_url()}")
        else
            self.sendResponse("426 Transfer aborted")
        end
        self.data_op = nil
        tasmota.remove_fast_loop(self.fast_loop)
        tasmota.gc()
    end

    def readDir()
        import path
        self.dir_list = path.listdir(self.dir.get_url())
    end

    def openFile(name,mode)
        import path
        var url = f"{self.dir.get_url()}{name}"
        if path.isdir(url) == true
            log(f"FTP: {url} is a folder",2)
            return false
        end
        if mode == "r"
            if path.exists(url) != true
                log(f"FTP: {url} not found",2)
                return false
            end
        end
        log(f"FTP: Open file {url} in {mode} mode",3)
        self.file = open(f"{url}",mode)
        if mode == "a"
            if self.data_ptr != 0
                log(f"FTP: Appending file {url} at position {self.data_ptr}",3)
                if self.data_ptr != self.file.size()
                    log(f"FTP: !!! resume position of {self.data_ptr} != file size of {self.file.size()} !!!",2)
                end
            end
        end
        return true
    end

    def close()
        self.sendResponse("221 Closing connection")
        self.connection = false
    end


    def deinitConnectServer()
        if self.data_server != nil
            self.data_server.close()
            self.data_server.deinit()
            self.data_server = nil
            log("FTP: Delete server for passive data connection",2)
        end
    end

    def initConnectServer()
        if self.data_server == nil
            self.data_server = tcpserver(self.data_port)
            log("FTP: Start server for passive data connection",2)
        end
    end

    def connectActive()
        self.data_client = tcpclientasync()
        if self.data_client.connect(self.active_ip,self.active_port) != false
            log(f"FTP: Try to connect to {self.active_ip}:{self.active_port}",3)
        end
    end

    def connectPassive()
        if self.data_server.hasclient()
            self.data_client = self.data_server.acceptasync()
        end
    end

    def dataconnect()
        if self.data_client != nil
            self.data_client.close()
            self.data_client.deinit()
        end
        if self.active_port != nil
            self.connectActive()
        else
            self.connectPassive()
        end
        if self.data_client == nil
            self.sendResponse("425 Data connection failed")
            return false
        end
        self.data_buf = bytes()
        self.retries = 10
        self.chunk_size = 5760
        self.sendResponse("150 Ready for data transfer")
        return true
    end

    def sendResponse(resp)
        self.client.write(f"{resp}\r\n")
        log(f"FTP: Response: {resp}",3)
    end

    def handleConnection() # main loop for incoming commands
        import string
        import mqtt
        import path
        var d = self.client.read()
        if size(d) == 0 return end
        var items = string.split(d," ")
        var cmd = items[0]
        var arg = ""
        var response = "" 
        if size(items) > 1
            arg = string.split(items[1],'\r\n')[0]
        else
            cmd = string.split(cmd,'\r\n')[0]
        end

        log(f"FTP: Received: {cmd} {arg}",3)
        
        # connect
        if cmd == "USER"
            if self.allow_anonymous
                response = "230 accept any/anonymous user"
            else
                self.user_input = arg
                response = "331 Password required"
            end
        elif cmd == "PASS"
            if self.user_input == self.user && arg == self.password
                response = "230 User accepted"
            else
                response = "530 Wrong login credentials"
                mqtt.publish("FTP","{'login':'wrong credentials'}")
            end
        elif cmd == "AUTH"
            response = f"500 Server does not support {arg}"
        elif cmd == "ABOR"
            self.abortDataOp()
            response = f"200 Aborting"
        elif cmd == "QUIT"
            self.close()
        #options
        elif cmd == "FEAT"
            self.sendResponse("211-Extensions supported:")
            self.sendResponse(" MLSD")
            self.sendResponse(" EPSV")
            self.sendResponse(" SIZE")
            # self.sendResponse(" MDTM")
            self.sendResponse(" REST STREAM")
            response = "211 End"
        elif cmd == "OPTS"
            if arg == "UTF8"
                response = "200 UTF Ok"
            else
                response = f"500 Server does not support {arg}"
            end
        elif cmd == "STRU"
            if arg == "F"
                response = "200 F Ok"
            else
                response = "504 Only F (ile) is supported"
            end
        elif cmd == "SYST"
            response = "215 UNIX"
        elif cmd == "LPRT"
                  response = f"501 active connection with long address not supported"
        elif cmd == "PORT"
            var el = string.split(arg,",")
            self.active_ip = f"{el[0]}.{el[1]}.{el[2]}.{el[3]}"
            self.active_port = int(el[4])*256 + int(el[5])
            response = f"200 port received {self.active_ip}:{self.active_port}"
            self.deinitConnectServer()
            #   response = f"501 active connection not supported"
        elif cmd == "EPRT"
            var el = string.split(arg,"|") # |1|192.168.1.54|65519| -> 1 IPV4, 2 IPV6
            self.active_ip = el[2]
            self.active_port = int(el[3])
            self.deinitConnectServer()
            response = f"200 extended port received {self.active_ip}:{self.active_port}"
        elif cmd == "TYPE"
            if arg == "I"
                response = "200 binary mode"
                self.binary_mode = true
            elif arg == "A"
                response = "200 ascii mode"
                self.binary_mode = false
            end
        elif cmd == "EPSV"
            self.active_port = nil
            self.initConnectServer()
            response = f"229 Entering Extended Passive Mode (|||{self.data_port}|)"
        elif cmd == "PASV"
            self.active_port = nil
            var el = string.split(self.data_ip,".")
            var hi = self.data_port >> 8
            var lo = self.data_port & 0xff
            self.initConnectServer()
            response = f"227 Entering passive mode ({el[0]},{el[1]},{el[2]},{el[3]},{hi},{lo})"
        elif cmd == "DELE"
            if path.remove(f"{self.dir.get_url()}{arg}")
                response = f"250 {self.dir.get_url()}{arg} deleted"
            else
                response = f"550 Could not delete file {self.dir.get_url()}{arg}"
            end
        elif  cmd == "RMD"
            var url = arg
            if arg[0] != "/"
                url = f"{self.dir.get_url()}{arg}"
            end
            if path.rmdir(url)
                response = f"250 {url} deleted"
            else
                response = f"550 Could not delete folder {url}"
            end
        elif cmd == "STOR"
            self.dataconnect()
            if self.data_client != nil
                response = ""
                var mode = "w"
                if self.data_ptr > 0
                    mode = "a"
                end
                if self.openFile(arg,mode)
                    self.data_op = "u"
                    self.fast_loop = /->self.upload()
                    tasmota.add_fast_loop(self.fast_loop)
                else
                    response = f"550 Could not open file"
                end
            else
                response = f"501 Could not init data connection"
            end
        elif cmd == "REST"
            self.data_ptr = int(arg)
            response = f"350 {self.data_ptr}"
        elif cmd == "RNFR"
            if self.openFile(arg,"r")
                self.file_rename = f"{self.dir.get_url()}{arg}"
                response = f"350 {arg}"
                self.file.close()
            else
                self.file_rename = nil
                response = f"550 Could not open file"
            end
        elif cmd == "RNTO"
            if self.file_rename != nil
                tasmota.cmd(f"UfsRename {self.file_rename},{self.dir.get_url()}{arg}")
                response = f"250 Renamed {self.file_rename} -> {arg}"
            else
                response = f"550 Could not rename file"
            end
            self.file_rename = nil
        elif cmd == "SIZE"
            if self.openFile(arg,"r")
                response = f"213 {self.file.size()}"
                self.file.close()
            else
                response = f"550 Could not open file"
            end
        elif cmd == "RETR"
            self.dataconnect()
            if self.data_client != nil
                if self.openFile(arg,"r")
                    self.file_size = self.file.size()
                    self.data_op = "d"
                    self.fast_loop = /->self.download()
                    tasmota.add_fast_loop(self.fast_loop)
                else
                    response = f"550 Could not open file"
                end
            else
                response = f"501 Could not init data connection"
            end
        # folder 
        elif cmd == "CDUP"
            self.dir.dir_up()
            response = "250 okay"
        elif cmd == "CWD"
            if self.dir.set(arg)
                response = "250 okay"
            else
                response = "550 Failed to change directory."
            end
        elif cmd == "PWD"
            self.readDir()
            response = f"250 {self.dir.get_url()}"
        elif cmd == "MKD"
            path.mkdir(f"{self.dir.get_url()}{arg}")
            response = f"250 {self.dir.get_url()}{arg} created"
        elif cmd == "LIST" || cmd == "MLSD" || cmd == "NLST"
            if arg != ""
                self.dir.set(arg)
            end
            self.readDir()
            if self.dataconnect()
                if size(self.dir_list) > 0
                    self.data_op = "dir"
                    self.dir_pos = 0
                    self.fast_loop = /->self.transferDir(cmd)
                    tasmota.add_fast_loop(self.fast_loop)
                else
                    self.finishTransferDir(true)
                end
            else
                response = f"501 Could not init data connection"
            end
        else # any unknown command
            response = "202 Command not implemented in Berry FTP"
        end

        if response != ""
            self.sendResponse(response)
        end
    end
end

var ftp =  FTP()
