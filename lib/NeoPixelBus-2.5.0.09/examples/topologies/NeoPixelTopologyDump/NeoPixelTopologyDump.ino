//----------------------------------------------------------------------
// NeoPixelTopologyDump
// This will dump to the serial output a grid map of the defined topology
// The output is displayed as row column labeled grid with the NeoPixelBus
// index of the pixel at the intersection of the row and column
//
// This is useful in visualising the layout of your panel so you can
// confirm you have the correct pattern
//
// It does not require that you have the actual panel connected
//----------------------------------------------------------------------

#include <NeoPixelAnimator.h>
#include <NeoPixelBus.h>

// uncomment one of these that matches your panel pixel layouts and
// how you want them rotated.  Not all the rotations are listed here
// but you can modifiy the name to include the rotation of 90,180, or 270.

typedef ColumnMajorAlternatingLayout MyPanelLayout;
// typedef ColumnMajorLayout MyPanelLayout;
// typedef RowMajorAlternatingLayout MyPanelLayout;
// typedef RowMajorLayout MyPanelLayout;
// typedef RowMajor90Layout MyPanelLayout; // note rotation 90 was used

// make sure to set these panel values to the sizes of yours
const uint8_t PanelWidth = 8;  // 8 pixel x 8 pixel matrix of leds
const uint8_t PanelHeight = 8;

NeoTopology<MyPanelLayout> topo(PanelWidth, PanelHeight);

void DumpTopo()
{
    Serial.println();

    Serial.print("\t\t");
    for (int x = 0; x < topo.getWidth(); x++)
    {
        Serial.print(x);
        Serial.print("\t");
    }
    Serial.println();

    Serial.print("\t--");
    for (int x = 0; x < topo.getWidth(); x++)
    {
        Serial.print("--------");
    }
    Serial.println();

    for (int y = 0; y < topo.getHeight(); y++)
    {
        Serial.print("  ");
        Serial.print(y);
        Serial.print("\t|\t");

        for (int x = 0; x < topo.getWidth(); x++)
        {
            Serial.print(topo.Map(x, y));
            Serial.print("\t");
        }
        Serial.println();
    }
}

void setup()
{
    Serial.begin(115200);
    while (!Serial); // wait for serial attach

    DumpTopo();
}

void loop()
{

}

