//----------------------------------------------------------------------
// NeoPixelTileDump
// This will dump to the serial output a grid map of the defined tiles
// The output is displayed as row column labeled grid with the NeoPixelBus
// index of the pixel at the intersection of the row and column
//
// To help with physical layout, there maybe included a symbol following the index
//   <  means the index is the input index for the panel, the first on the panel
//   >  means the index is the output index for the panel, the last on the panel
//
// This is useful in visualising the tile layout of your panels to
// confirm you have them correctly wired together for the defined pattern
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

// change this to be one of the layouts which will define the layout
// of the panels themselves
typedef ColumnMajorLayout MyTilesLayout;

// make sure to set these panel and tile layout to match your sizes
const uint8_t PanelWidth = 8;  // 8 pixel x 8 pixel matrix of leds
const uint8_t PanelHeight = 8;
const uint8_t TileWidth = 4;  // laid out in 4 panels x 2 panels mosaic
const uint8_t TileHeight = 2;

NeoTiles <MyPanelLayout, MyTilesLayout> tiles(
    PanelWidth,
    PanelHeight,
    TileWidth,
    TileHeight);

void DumpTopo()
{
    Serial.println();

    Serial.print("\t\t");
    for (int x = 0; x < tiles.getWidth(); x++)
    {
        Serial.print(x);
        Serial.print("\t");
    }
    Serial.println();

    Serial.print("\t---");
    for (int x = 0; x < tiles.getWidth(); x++)
    {
        Serial.print("--------");
    }
    Serial.println();

    for (int y = 0; y < tiles.getHeight(); y++)
    {
        Serial.print("  ");
        Serial.print(y);
        Serial.print("\t|\t");

        for (int x = 0; x < tiles.getWidth(); x++)
        {
            NeoTopologyHint hint = tiles.TopologyHint(x, y);

            Serial.print(tiles.Map(x, y));
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

    DumpTopo();
}

void loop()
{

}

