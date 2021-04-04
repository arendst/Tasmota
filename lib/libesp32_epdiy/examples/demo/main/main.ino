/**
 * This is the Arduino wrapper for the "Demo" example.
 * Please go to the main.c for the main example file.
 *
 * This example was developed for the ESP IoT Development Framework (IDF).
 * You can still use this code in the Arduino IDE, but it may not look
 * and feel like a classic Arduino sketch.
 * If you are looking for an example with Arduino look-and-feel,
 * please check the other examples.
 */
extern "C"
{
    void idf_setup();
    void idf_loop();
}

void setup() {
    if(psramInit()){
        Serial.println("\nThe PSRAM is correctly initialized");
    } else{
        Serial.println("\nPSRAM does not work");
    }

    idf_setup();
}

void loop() {
    idf_loop();
}
