//
// Example sketch showing how to host a web server on your CYD
// and display JPEG images uploaded to it
// The code will scale incoming images by 1/2, 1/4 or 1/8 to
// make them fit on the LCD. Images larger than 64K (arbitrary)
// or larger than 8x either LCD dimension will not display.
//
//
#include <bb_spi_lcd.h>
#include <JPEGDEC.h>
#include <WiFi.h>
#include <ESPAsyncWebSrv.h> // Install ESPAsyncWebSrv by dvarrel

BB_SPI_LCD lcd;
JPEGDEC jpg;
#define MAX_FILE_SIZE 65536

const String default_ssid = "your_ssid";
const String default_wifipassword = "your_password";
uint8_t *pBuffer;
int iWidth, iHeight, iFileSize;
//
// The HTML is a static string with a simple form to upload the file
// This form contains a single submit button which initiates a multi-part
// file upload.
//
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML>
<html lang="en">
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <meta charset="UTF-8">
</head>
<body>
  <p><h1>CYD JPEG image viewer</h1></p>
  <p>Upload a baseline JPEG image up to 64K.<br>
  Images larger than the LCD will be scaled down.</p>
  <form method="POST" action="/upload" enctype="multipart/form-data"><input type="file" name="data"/><input type="submit" name="upload" value="Upload" title="Upload File"></form>
  <p>Image Dimensions: %IMAGESIZE%</p>
  <p>After clicking upload it will take some time for the file to upload,<br>
  there is no indicator that the upload began.  Please be patient.</p>
  <p>Once uploaded the page will refresh and image information will be displayed.</p>
  </body>
</html>
)rawliteral";

// configuration structure
struct Config {
  String ssid;               // wifi ssid
  String wifipassword;       // wifi password
  int webserverporthttp;     // http port number for web admin
};

// variables
Config config;                        // configuration
AsyncWebServer *server;               // initialise webserver

//
// This callback function draws the pixels as they're decoded
// Each block of pixels is 1 MCU tall (8 or 16 pixels) and a variable
// number of MCUs wide.
//
int JPEGDraw(JPEGDRAW *pDraw)
{
  lcd.setAddrWindow(pDraw->x, pDraw->y, pDraw->iWidth, pDraw->iHeight);
  lcd.pushPixels((uint16_t *)pDraw->pPixels, pDraw->iWidth * pDraw->iHeight);
  return 1;
} /* JPEGDraw() */

// Read the file info and decode it to the LCD
void DisplayFile(void)
{
  int options = 0, xoff, yoff, w, h;
  if (jpg.openRAM(pBuffer, iFileSize, JPEGDraw)) { // pass the data and its size
      jpg.setPixelType(RGB565_BIG_ENDIAN); // bb_spi_lcd uses big-endian RGB565 pixels
      // if the image is smaller than the LCD dimensions, center it
      w = iWidth = jpg.getWidth();
      h = iHeight = jpg.getHeight();
      if (w > lcd.width() || h > lcd.height()) { // try to scale it to fit the LCD
         if (w > lcd.width() * 4 || h > lcd.height() * 4) {
            options = JPEG_SCALE_EIGHTH;
            w = iWidth/8; h = iHeight/8;
         } else if (w > lcd.width() * 2 || h > lcd.height() * 2) {
            options = JPEG_SCALE_QUARTER;
            w = iWidth / 4; h = iHeight / 4;
         } else {
            options = JPEG_SCALE_HALF;
            w = iWidth / 2; h = iHeight / 2;
         }
      }
      xoff = (lcd.width() - w)/2;
      yoff = (lcd.height() - h)/2;
      if (xoff < 0) xoff = 0;
      if (yoff < 0) yoff = 0;
      lcd.fillScreen(TFT_BLACK);
      jpg.decode(xoff,yoff, options); // center the image and no extra options (e.g. scaling)
  } else {
    lcd.println("Error opening JPEG!");
  }
} /* DisplayFile() */

void configureWebServer() {
  server->on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
//    String logmessage = "Client:" + request->client()->remoteIP().toString() + + " " + request->url();
//    lcd.println(logmessage);
    request->send_P(200, "text/html", index_html, processor);
  });

  // run handleUpload function when any file is uploaded
  server->on("/upload", HTTP_POST, [](AsyncWebServerRequest *request) {
        request->send(200);
      }, handleUpload);
} /* configureWebServer() */

// handles jpeg file uploads
void handleUpload(AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final) {
  if (len) {
    if (index+len <= MAX_FILE_SIZE) {
      memcpy(&pBuffer[index], data, len);
    }
    iFileSize = index+len;
  }
  if (final) { // last chunk
   // lcd.print("Upload complete\n");
    if (iFileSize <= MAX_FILE_SIZE) { // we were able to hold the complete file in memory
      DisplayFile();
      request->redirect("/");
    } else { // we had to abandon the upload, after exceeding our buffer size
      lcd.setCursor(0,0);
      lcd.println("File too large!");
    }
  }
} /* handleUpload() */

// Process info requests from our one variable element
String processor(const String& var) {
  char szTemp[128];

  if (var == "IMAGESIZE") {
    sprintf(szTemp, "%d x %d, %d bytes", iWidth, iHeight, iFileSize);
    return String(szTemp);
  }
  return String();
} /* processor() */

void setup() {
  lcd.begin(DISPLAY_WS_AMOLED_18); //DISPLAY_CYD_2USB); // Set this to your display type
  lcd.fillScreen(TFT_BLACK);
  lcd.setTextColor(TFT_GREEN);
  lcd.setFont(FONT_12x16);
  iFileSize = 0; // no image loaded
  lcd.println("Booting ...");
  pBuffer = (uint8_t *)malloc(MAX_FILE_SIZE); // allow up to 64k JPEG
  if (!pBuffer) {
    lcd.println("malloc failed!");
    while (1) {};
  }
  config.ssid = default_ssid;
  config.wifipassword = default_wifipassword;
  config.webserverporthttp = 80;

  lcd.print("Connecting to Wifi: ");
  WiFi.begin(config.ssid.c_str(), config.wifipassword.c_str());
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    lcd.print(".");
  }
  lcd.print("\nServer IP: ");
  lcd.println(WiFi.localIP());

  // configure web server
  server = new AsyncWebServer(config.webserverporthttp);
  configureWebServer();

  // startup web server
  lcd.println("Starting Webserver");
  server->begin();
} /* setup() */

// once the server starts, there's nothing to do here
void loop() {
}

