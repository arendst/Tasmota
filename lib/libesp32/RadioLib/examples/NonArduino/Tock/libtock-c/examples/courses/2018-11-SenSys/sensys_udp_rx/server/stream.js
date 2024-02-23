const fs = require('fs');
const stream = require('stream');
const SerialPort = require('serialport');

function createMyStream(){
  const magic = Buffer.from([0x80, 0x81]);
  const header_len = magic.length + 16 + 2;
  let address = null;
  let bytes_left = 0;
  let payload_len = 0;
  let buffer = Buffer.from('');

  return new stream.Writable({
    writableObjectMode: true,
    write: function(chunk, encoding, callback) {
      console.log(chunk.toString('hex'));
      buffer = Buffer.concat([buffer, chunk]);
      while (true) {
        if (bytes_left == 0) {
          let m = buffer.indexOf(magic);
          if (m >= 0 && buffer.length - m >= header_len) {
            buffer = buffer.slice(m + magic.length);
            payload_len = buffer[0];
            buffer = buffer.slice(1);
            address = buffer.slice(0, 16);
            let new_addr = [];
            for (const i of address.values()) {
              new_addr.push(i - 1);
	    }
            address = Buffer.from(new_addr);
            buffer = buffer.slice(16);
            if (payload_len == buffer.length) {
              bytes_left = -1;
	    } else {
              bytes_left = payload_len - buffer.length;
	    }
          } else {
            buffer = buffer.slice(-(magic.length - 1));
            break;
  	  }
        } else if (buffer.length >= payload_len) {
            let payload = buffer.slice(0, payload_len);
            buffer = buffer.slice(payload_len);
            bytes_left = 0;
            let result = {
              address: address,
              payload: JSON.parse(payload)
            };
            this.emit("packet", result);
        } else {
          console.log("want more", bytes_left, payload_len);
          bytes_left = payload_len - buffer.length;
          break;
        }
      }
      callback();
    }
  });
}

if (process.argv.length < 3) {
  console.log("Usage: node stream.js [serial-device], e.g. node stream.js /dev/ttyUSB0");
  process.exit(1);
}

const port = process.argv[2];
const serialport = new SerialPort(port, {baudRate: 115200});

serialport.on("open", () => {
  serialport.set({rts: false, dtr: false});
});

const mystream = createMyStream();
serialport.pipe(mystream);

let recent_data = {
  "ffed": {
    timestamp: new Date().valueOf(),
    payload: {
      humi: 5432,
      temp: 2376,
      lux: 5555,
    }
  }
};

mystream.on('packet', (packet) => {
  let address_last2 = packet.address.slice(-2).readUInt16BE().toString(16);
  if (!(address_last2 in recent_data)) {
    recent_data[address_last2] = {payload: {}};
  }
  let slot = recent_data[address_last2];
  slot.timestamp = new Date().valueOf();
  for (k in packet.payload) {
    slot.payload[k] = packet.payload[k];
  }
  recent_data[address_last2] = slot;
  console.log(recent_data);
});

const express = require('express');
      serveStatic = require('serve-static');
      app = express();
      web_port = 3000;

app.use(serveStatic('static'));
app.get('/data', (req, res) => res.json(recent_data));
app.get('/data/:addr', (req, res) => res.json(recent_data[req.params.addr]));

app.listen(web_port, () => console.log(`Web app listening on port ${web_port}!`));

