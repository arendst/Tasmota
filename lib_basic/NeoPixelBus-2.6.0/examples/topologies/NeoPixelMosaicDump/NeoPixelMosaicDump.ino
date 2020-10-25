//----------------------------------------------------------------------
// NeoPixelMosaicDump
// This will dump to the serial output a grid map of the defined mosaic
// The output is displayed as row column labeled grid with the NeoPixelBus
// index of the pixel at the intersection of the row and column.
//
// To help with physical layout, there maybe included a symbol following the index
//   <  means the index is the input index for the panel, the first on the panel
//   >  means the index is the output index for the panel, the last on the panel
//
// This is useful in visualising the mosaic layout of your panels to
// confirm you have them correctly wired together for this mosaic pattern
//
// It does not require that you have the actual panel connected
//----------------------------------------------------------------------

#include <NeoPixelAnimator.h>
#include <NeoPixelBus.h>

// uncomment one of these that matches your panel pixel layouts
// rotation is ignored for mosaic as it applies a rotation for you
// that is specific to the location of the panel within the mosaic
// to reduce connection lengths

typedef ColumnMajorAlternatingLayout MyPanelLayout;
// typedef ColumnMajorLayout MyPanelLayout;
// typedef RowMajorAlternatingLayout MyPanelLayout;
// typedef RowMajorLayout MyPanelLayout;

// make sure to set these panel and tile layout to match your sizes
const uint8_t PanelWidth = 8;  // a 8 pixel x 8 pixel matrix of leds on the panel
const uint8_t PanelHeight = 8;
const uint8_t TileWidth = 4;  // laid out in 4 panels x 2 panels mosaic
const uint8_t TileHeight = 2;

NeoMosaic <MyPanelLayout> mosaic(
    PanelWidth,
    PanelHeight,
    TileWidth,
    TileHeight);

void DumpMosaic()
{
    Serial.println();

    Serial.print("\t\t");
    for (int x = 0; x < mosaic.getWidth(); x++)
    {
        Serial.print(x);
        Serial.print("\t");
    }
    Serial.println();

    Serial.print("\t---");
    for (int x = 0; x < mosaic.getWidth(); x++)
    {
        Serial.print("--------");
    }
    Serial.println();

    for (int y = 0; y < mosaic.getHeight(); y++)
    {
        Serial.print("  ");
        Serial.print(y);
        Serial.print("\t|\t");

        for (int x = 0; x < mosaic.getWidth(); x++)
        {
            NeoTopologyHint hint = mosaic.TopologyHint(x, y);

            Serial.print(mosaic.Map(x, y));
            if (hint == NeoTopologyHint_FirstOnPanel)
            {
                Serial.print("<");
            }
            else if (hint == NeoTopologyHint_LastOnPanel)
            {
                Serial.print(">");
            }
            Serial.print("\t");
        }
        Serial.println();
    }
}

void setup()
{
    Serial.begin(115200);
    while (!Serial); // wait for serial attach

    DumpMosaic();
}

void loop()
{

}

