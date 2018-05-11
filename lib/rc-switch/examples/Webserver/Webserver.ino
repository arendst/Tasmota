/*
  A simple RCSwitch/Ethernet/Webserver demo
  
  https://github.com/sui77/rc-switch/
*/

#include <SPI.h>
#include <Ethernet.h>
#include <RCSwitch.h>

// Ethernet configuration
uint8_t mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED }; // MAC Address
uint8_t ip[] = { 192,168,0, 2 };                        // IP Address
EthernetServer server(80);                           // Server Port 80

// RCSwitch configuration
RCSwitch mySwitch = RCSwitch();
int RCTransmissionPin = 7;

// More to do...
// You should also modify the processCommand() and 
// httpResponseHome() functions to fit your needs.



/**
 * Setup
 */
void setup() {
  Ethernet.begin(mac, ip);
  server.begin();
  mySwitch.enableTransmit( RCTransmissionPin );
}

/**
 * Loop
 */
void loop() {
  char* command = httpServer();
}

/**
 * Command dispatcher
 */
void processCommand(char* command) {
  if        (strcmp(command, "1-on") == 0) {
    mySwitch.switchOn(1,1);
  } else if (strcmp(command, "1-off") == 0) {
    mySwitch.switchOff(1,1);
  } else if (strcmp(command, "2-on") == 0) {
    mySwitch.switchOn(1,2);
  } else if (strcmp(command, "2-off") == 0) {
    mySwitch.switchOff(1,2);
  }
}

/**
 * HTTP Response with homepage
 */
void httpResponseHome(EthernetClient c) {
  c.println("HTTP/1.1 200 OK");
  c.println("Content-Type: text/html");
  c.println();
  c.println("<html>");
  c.println("<head>");
  c.println(    "<title>RCSwitch Webserver Demo</title>");
  c.println(    "<style>");
  c.println(        "body { font-family: Arial, sans-serif; font-size:12px; }");
  c.println(    "</style>");
  c.println("</head>");
  c.println("<body>");
  c.println(    "<h1>RCSwitch Webserver Demo</h1>");
  c.println(    "<ul>");
  c.println(        "<li><a href=\"./?1-on\">Switch #1 on</a></li>");
  c.println(        "<li><a href=\"./?1-off\">Switch #1 off</a></li>");
  c.println(    "</ul>");
  c.println(    "<ul>");
  c.println(        "<li><a href=\"./?2-on\">Switch #2 on</a></li>");
  c.println(        "<li><a href=\"./?2-off\">Switch #2 off</a></li>");
  c.println(    "</ul>");
  c.println(    "<hr>");
  c.println(    "<a href=\"https://github.com/sui77/rc-switch/\">https://github.com/sui77/rc-switch/</a>");
  c.println("</body>");
  c.println("</html>");
}

/**
 * HTTP Redirect to homepage
 */
void httpResponseRedirect(EthernetClient c) {
  c.println("HTTP/1.1 301 Found");
  c.println("Location: /");
  c.println();
}

/**
 * HTTP Response 414 error
 * Command must not be longer than 30 characters
 **/
void httpResponse414(EthernetClient c) {
  c.println("HTTP/1.1 414 Request URI too long");
  c.println("Content-Type: text/plain");
  c.println();
  c.println("414 Request URI too long");
}

/**
 * Process HTTP requests, parse first request header line and 
 * call processCommand with GET query string (everything after
 * the ? question mark in the URL).
 */
char*  httpServer() {
  EthernetClient client = server.available();
  if (client) {
    char sReturnCommand[32];
    int nCommandPos=-1;
    sReturnCommand[0] = '\0';
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        if ((c == '\n') || (c == ' ' && nCommandPos>-1)) {
          sReturnCommand[nCommandPos] = '\0';
          if (strcmp(sReturnCommand, "\0") == 0) {
            httpResponseHome(client);
          } else {
            processCommand(sReturnCommand);
            httpResponseRedirect(client);
          }
          break;
        }
        if (nCommandPos>-1) {
          sReturnCommand[nCommandPos++] = c;
        }
        if (c == '?' && nCommandPos == -1) {
          nCommandPos = 0;
        }
      }
      if (nCommandPos > 30) {
        httpResponse414(client);
        sReturnCommand[0] = '\0';
        break;
      }
    }
    if (nCommandPos!=-1) {
      sReturnCommand[nCommandPos] = '\0';
    }
    // give the web browser time to receive the data
    delay(1);
    client.stop();
    
    return sReturnCommand;
  }
  return '\0';
}