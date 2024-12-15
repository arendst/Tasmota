const fs = require('fs');
const { RtcmTransport } = require('@gnss/rtcm');

function parseRTCMFile(filepath) {
    try {
        // Read the file
        const data = fs.readFileSync(filepath);
        let buffer = Buffer.from(data);
        let offset = 0;
        const messages = [];

        while (offset < buffer.length) {
            try {
                // Look for RTCM sync char (0xD3)
                while (offset < buffer.length && buffer[offset] !== RtcmTransport.SYNC_CHAR) {
                    offset++;
                }

                if (offset >= buffer.length) break;

                // Try to decode RTCM message
                const [message, bytesRead] = RtcmTransport.decode(buffer.slice(offset));
                
                // Store message info
                messages.push({
                    type: message.messageType,
                    length: bytesRead,
                    rawBytes: buffer.slice(offset, offset + bytesRead),
                    content: message
                });

                offset += bytesRead;
            } catch (error) {
                // If decode fails, move to next byte and continue searching
                offset++;
                console.error(`Error decoding at offset ${offset}: ${error.message}`);
            }
        }

        // Print results
        console.log(`Found ${messages.length} RTCM messages:\n`);
        messages.forEach((msg, index) => {
            console.log(`Message #${index + 1}:`);
            console.log(`Type: ${msg.type}`);
            console.log(`Length: ${msg.length} bytes`);
            console.log(`Raw bytes: ${msg.rawBytes.toString('hex')}`);
            console.log(`Content:`, msg.content);
            console.log('-------------------\n');
        });

        return messages;
    } catch (error) {
        console.error('Error processing file:', error);
        return [];
    }
}

// Check if file path is provided
const filepath = process.argv[2];
if (!filepath) {
    console.error('Please provide a file path as an argument');
    process.exit(1);
}

parseRTCMFile(filepath);
