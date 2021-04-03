Weather display example
=======================================

This is **port of project https://github.com/G6EJD/ESP32-e-Paper-Weather-Display by David Bird 2018** to work with ED097OC4 Kindle display and this driver.
**Please note that David Bird (https://github.com/G6EJD) owns the copyright to this software!** Only minor modifications where made for it to run with the epdiy driver.
THe  license details are outlined in **License.txt**.

Building It
-----------

 - Girst you need to install Arduino esp-idf as a component https://github.com/espressif/arduino-esp32/blob/master/docs/esp-idf_component.md (the easiest way is to put it into components folder of ESP-IDF)
 - Put Arduino JSON https://github.com/bblanchon/ArduinoJson into components/arduino/
 - Dont forget to insert your Wi-Fi settings and openweathermap API key into owm_credentials.h
 - If you want to add more fonts, firmware may not fit into 1M and easiest way to fix it is to edit components/partition_table/partitions_singleapp.csv and change 1M to 2M

![weather image](weather.jpg)

License
-------

This software, the ideas and concepts is Copyright (c) David Bird 2014 and beyond.

All rights to this software are reserved.
 
It is prohibited to redistribute or reproduce of any part or all of the software contents in any form other than the following:

 1. You may print or download to a local hard disk extracts for your personal and non-commercial use only.

 2. You may copy the content to individual third parties for their personal use, but only if you acknowledge the author David Bird as the source of the material.

 3. You may not, except with my express written permission, distribute or commercially exploit the content.

 4. You may not transmit it or store it in any other website or other form of electronic retrieval system for commercial purposes.

 5. You MUST include all of this copyright and permission notice ('as annotated') and this shall be included in all copies or substantial portions of the software and where the software use is visible to an end-user.
 
THE SOFTWARE IS PROVIDED "AS IS" FOR PRIVATE USE ONLY, IT IS NOT FOR COMMERCIAL USE IN WHOLE OR PART OR CONCEPT.

FOR PERSONAL USE IT IS SUPPLIED WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.

IN NO EVENT SHALL THE AUTHOR OR COPYRIGHT HOLDER BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
