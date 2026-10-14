# Matter 1.6.1 Semantic Tag Namespaces - Compact Spec

Version: 1.6.1 (2026-09-16)

Source: CSA Document 23-31936-009

## Overview

Semantic tags annotate endpoints via `TagList` in the Descriptor cluster. Tags combine namespace ID + tag ID.

**Namespace Ranges:**
- Common: 0x01-0x1F (usable across domains unless explicitly restricted)
- Device-specific: 0x41+ (restricted to specific device types or sets of device types)

**Rules:**
- `Namespace::Tag` is the preferred notation; `` `Namespace.Tag` `` is also acceptable
- `TagList` MAY combine tags from: device-specific namespace + common namespaces + manufacturer-specific namespaces
- Row/Column tags: Label field = Arabic numeral string (`"1"`, `"2"`...), NOT words
- Endpoints in a line/grid/matrix SHOULD be allocated top-to-bottom, left-to-right

---

## Common Namespaces

### 0x01 Common Closure (Deprecated)

Deprecated as of Matter 1.4.2. Clients SHOULD still support these tags for servers certified on earlier Matter revisions.

| ID | Name | Description |
|----|------|-------------|
| 0x00 | Opening | Move toward open position |
| 0x01 | Closing | Move toward closed position |
| 0x02 | Stop | Stop any movement |

### 0x02 Compass Direction

Movement into a compass direction (vs 0x03 which is location).

| ID | Name |
|----|------|
| 0x00 | Northward |
| 0x01 | NorthEastward |
| 0x02 | Eastward |
| 0x03 | SouthEastward |
| 0x04 | Southward |
| 0x05 | SouthWestward |
| 0x06 | Westward |
| 0x07 | NorthWestward |

### 0x03 Compass Location

Position in a compass direction (vs 0x02 which is movement).

| ID | Name |
|----|------|
| 0x00 | North |
| 0x01 | NorthEast |
| 0x02 | East |
| 0x03 | SouthEast |
| 0x04 | South |
| 0x05 | SouthWest |
| 0x06 | West |
| 0x07 | NorthWest |

### 0x04 Direction

Movement relative to device (vs 0x08 Position which is static location).

| ID | Name |
|----|------|
| 0x00 | Upward |
| 0x01 | Downward |
| 0x02 | Leftward |
| 0x03 | Rightward |
| 0x04 | Forward |
| 0x05 | Backward |

### 0x05 Level

| ID | Name |
|----|------|
| 0x00 | Low |
| 0x01 | Medium |
| 0x02 | High |

### 0x06 Location

| ID | Name | Description |
|----|------|-------------|
| 0x00 | Indoor | Indoors or related to indoor equipment/conditions |
| 0x01 | Outdoor | Outdoors or related to outdoor equipment/conditions |
| 0x02 | Inside | Located inside equipment |
| 0x03 | Outside | Located outside equipment |
| 0x04 | Zone | Part of a location divided into zones |

### 0x07 Number

| ID | Name |
|----|------|
| 0x00 | Zero |
| 0x01 | One |
| 0x02 | Two |
| 0x03 | Three |
| 0x04 | Four |
| 0x05 | Five |
| 0x06 | Six |
| 0x07 | Seven |
| 0x08 | Eight |
| 0x09 | Nine |
| 0x0A | Ten |
| 0x0B | Eleven |
| 0x0C | Twelve |
| 0x0D | Thirteen |
| 0x0E | Fourteen |
| 0x0F | Fifteen |
| 0x10 | Sixteen |
| 0x11 | Seventeen |
| 0x12 | Eighteen |
| 0x13 | Nineteen |
| 0x14 | Twenty |
| 0x15 | TwentyOne |
| 0x16 | TwentyTwo |
| 0x17 | TwentyThree |
| 0x18 | TwentyFour |
| 0x19 | TwentyFive |
| 0x1A | TwentySix |
| 0x1B | TwentySeven |
| 0x1C | TwentyEight |
| 0x1D | TwentyNine |
| 0x1E | Thirty |

### 0x08 Position

Position relative to device (vs 0x04 Direction which is movement).

| ID | Name | Notes |
|----|------|-------|
| 0x00 | Left | |
| 0x01 | Right | |
| 0x02 | Top | |
| 0x03 | Bottom | |
| 0x04 | Middle | |
| 0x05 | Row | Label = row number string |
| 0x06 | Column | Label = column number string |

**Row/Column Rules:**
- Use for grids or arrays larger than 3 elements in any direction
- Label field SHALL contain an Arabic numeral string
- Number words SHALL NOT be used
- First row/column SHALL use Label `"1"`

### 0x10 Area

Home areas (indoor/outdoor).

| ID | Name | Notes |
|----|------|-------|
| 0x00 | Aisle | |
| 0x01 | Attic | |
| 0x02 | BackDoor | |
| 0x03 | BackYard | |
| 0x04 | Balcony | |
| 0x05 | Ballroom | |
| 0x06 | Bathroom | Also known as Restroom |
| 0x07 | Bedroom | |
| 0x08 | Border | |
| 0x09 | Boxroom | Small storage room |
| 0x0A | BreakfastRoom | |
| 0x0B | Carport | |
| 0x0C | Cellar | |
| 0x0D | Cloakroom | |
| 0x0E | Closet | Small room for storing clothing, linens, and other items |
| 0x0F | Conservatory | |
| 0x10 | Corridor | |
| 0x11 | CraftRoom | |
| 0x12 | Cupboard | |
| 0x13 | Deck | |
| 0x14 | Den | Small room for work or hobbies |
| 0x15 | Dining | |
| 0x16 | DrawingRoom | |
| 0x17 | DressingRoom | |
| 0x18 | Driveway | |
| 0x19 | Elevator | |
| 0x1A | Ensuite | Bathroom directly accessible from a bedroom |
| 0x1B | Entrance | |
| 0x1C | Entryway | |
| 0x1D | FamilyRoom | |
| 0x1E | Foyer | |
| 0x1F | FrontDoor | |
| 0x20 | FrontYard | |
| 0x21 | GameRoom | |
| 0x22 | Garage | |
| 0x23 | GarageDoor | |
| 0x24 | Garden | |
| 0x25 | GardenDoor | |
| 0x26 | GuestBathroom | Also known as Guest Restroom |
| 0x27 | GuestBedroom | |
| 0x28 | Reserved28 | Deprecated: was Guest Restroom; use GuestBathroom |
| 0x29 | GuestRoom | Also known as Guest Bedroom |
| 0x2A | Gym | |
| 0x2B | Hallway | |
| 0x2C | HearthRoom | Cozy room containing a fireplace or other point heat source |
| 0x2D | KidsRoom | |
| 0x2E | KidsBedroom | |
| 0x2F | Kitchen | |
| 0x30 | Reserved30 | Deprecated: was Larder; use Pantry |
| 0x31 | LaundryRoom | |
| 0x32 | Lawn | |
| 0x33 | Library | |
| 0x34 | LivingRoom | |
| 0x35 | Lounge | |
| 0x36 | MediaTvRoom | |
| 0x37 | MudRoom | Space for removing soiled garments |
| 0x38 | MusicRoom | |
| 0x39 | Nursery | |
| 0x3A | Office | |
| 0x3B | OutdoorKitchen | |
| 0x3C | Outside | |
| 0x3D | Pantry | Also known as a larder; food storage |
| 0x3E | ParkingLot | |
| 0x3F | Parlor | |
| 0x40 | Patio | |
| 0x41 | PlayRoom | |
| 0x42 | PoolRoom | Room centered around a pool/billiards table |
| 0x43 | Porch | |
| 0x44 | PrimaryBathroom | |
| 0x45 | PrimaryBedroom | |
| 0x46 | Ramp | |
| 0x47 | ReceptionRoom | |
| 0x48 | RecreationRoom | |
| 0x49 | Reserved49 | Deprecated: was Restroom; use Bathroom |
| 0x4A | Roof | |
| 0x4B | Sauna | |
| 0x4C | Scullery | Utility space for cleaning dishes and laundry |
| 0x4D | SewingRoom | |
| 0x4E | Shed | |
| 0x4F | SideDoor | |
| 0x50 | SideYard | |
| 0x51 | SittingRoom | |
| 0x52 | Snug | Cozy informal space |
| 0x53 | Spa | |
| 0x54 | Staircase | |
| 0x55 | SteamRoom | |
| 0x56 | StorageRoom | |
| 0x57 | Studio | |
| 0x58 | Study | |
| 0x59 | SunRoom | |
| 0x5A | SwimmingPool | |
| 0x5B | Terrace | |
| 0x5C | UtilityRoom | |
| 0x5D | Ward | Innermost area of a large home |
| 0x5E | Workshop | |
| 0x5F | Toilet | Room dedicated to a toilet; water closet/WC |

### 0x11 Landmark

Home landmarks (furniture, appliances, fixtures).

| ID | Name |
|----|------|
| 0x00 | AirConditioner |
| 0x01 | AirPurifier |
| 0x02 | BackDoor |
| 0x03 | BarStool |
| 0x04 | BathMat |
| 0x05 | Bathtub |
| 0x06 | Bed |
| 0x07 | Bookshelf |
| 0x08 | Chair |
| 0x09 | ChristmasTree |
| 0x0A | CoatRack |
| 0x0B | CoffeeTable |
| 0x0C | CookingRange |
| 0x0D | Couch |
| 0x0E | Countertop |
| 0x0F | Cradle |
| 0x10 | Crib |
| 0x11 | Desk |
| 0x12 | DiningTable |
| 0x13 | Dishwasher |
| 0x14 | Door |
| 0x15 | Dresser |
| 0x16 | LaundryDryer |
| 0x17 | Fan |
| 0x18 | Fireplace |
| 0x19 | Freezer |
| 0x1A | FrontDoor |
| 0x1B | HighChair |
| 0x1C | KitchenIsland |
| 0x1D | Lamp |
| 0x1E | LitterBox |
| 0x1F | Mirror |
| 0x20 | Nightstand |
| 0x21 | Oven |
| 0x22 | PetBed |
| 0x23 | PetBowl |
| 0x24 | PetCrate |
| 0x25 | Refrigerator |
| 0x26 | ScratchingPost |
| 0x27 | ShoeRack |
| 0x28 | Shower |
| 0x29 | SideDoor |
| 0x2A | Sink |
| 0x2B | Sofa |
| 0x2C | Stove |
| 0x2D | Table |
| 0x2E | Toilet |
| 0x2F | TrashCan |
| 0x30 | LaundryWasher |
| 0x31 | Window |
| 0x32 | WineCooler |

### 0x12 Relative Position

Position relative to an external reference, which must be specified by the user.

| ID | Name | Description |
|----|------|-------------|
| 0x00 | Under | |
| 0x01 | NextTo | Proximity to reference |
| 0x02 | Around | Surrounding reference |
| 0x03 | On | |
| 0x04 | Above | |
| 0x05 | FrontOf | |
| 0x06 | Behind | |

---

## Domain-Specific Namespaces (Common Range)

### 0x0A Electrical Measurement

**Restricted to electrical measurement domain.**

| ID | Name | Description |
|----|------|-------------|
| 0x00 | DC | DC load |
| 0x01 | AC | Single-phase or collective polyphase AC load |
| 0x02 | ACPhase1 | Phase 1 of polyphase AC supply |
| 0x03 | ACPhase2 | Phase 2 of polyphase AC supply |
| 0x04 | ACPhase3 | Phase 3 of polyphase AC supply |

### 0x0B Commodity Tariff Chronology

**Restricted to energy calendar domain.**

| ID | Name | Description |
|----|------|-------------|
| 0x00 | Current | Current Commodity Tariff |
| 0x01 | Previous | Previous Commodity Tariff |
| 0x02 | Upcoming | Upcoming Commodity Tariff |

### 0x0D Commodity Tariff Commodity

**Restricted to Commodity Tariff commodity domain.**

| ID | Name |
|----|------|
| 0x00 | ElectricalEnergy |

### 0x0E Laundry

**Restricted to laundry domain.**

| ID | Name |
|----|------|
| 0x00 | Normal |
| 0x01 | LightDry |
| 0x02 | ExtraDry |
| 0x03 | NoDry |

### 0x0F Power Source

**Restricted to power source domain.**

| ID | Name | Required Feature |
|----|------|------------------|
| 0x00 | Unknown | - |
| 0x01 | Grid | WIRED |
| 0x02 | Solar | WIRED |
| 0x03 | Battery | BAT |
| 0x04 | EV | BAT |

### 0x13 Commodity Tariff Flow

**Restricted to Commodity Tariff flow domain.**

| ID | Name |
|----|------|
| 0x00 | Import |
| 0x01 | Export |

---

## Device-Specific Namespaces

### 0x41 Refrigerator

**Restricted to refrigerator domain.**

| ID | Name |
|----|------|
| 0x00 | Refrigerator |
| 0x01 | Freezer |

### 0x42 Room Air Conditioner

**Restricted to room AC domain.**

| ID | Name |
|----|------|
| 0x00 | Evaporator |
| 0x01 | Condenser |

### 0x43 Switches

**Restricted to switches domain.** Indicates button function for UI optimization.

| ID | Name | Notes |
|----|------|-------|
| 0x00 | On | |
| 0x01 | Off | |
| 0x02 | Toggle | |
| 0x03 | Up | e.g. dim up |
| 0x04 | Down | e.g. dim down |
| 0x05 | Next | e.g. next scene |
| 0x06 | Previous | e.g. previous scene |
| 0x07 | EnterOkSelect | Enter/OK/Select |
| 0x08 | Custom | Label = button text/icon description |
| 0x09 | Open | e.g. open window covering |
| 0x0A | Close | e.g. close window covering |
| 0x0B | Stop | e.g. stop moving window covering |

**Custom Tag Rule:** Label field SHALL contain a textual description of the button function (e.g. `"dining"`).

### 0x44 Closure

**Restricted to closure domain.** Identifies the kind of closure.

| ID | Name | Description |
|----|------|-------------|
| 0x00 | Covering | Material or treatment covering a window |
| 0x01 | Window | Opening fitted with transparent material |
| 0x02 | Barrier | Obstruction preventing movement or access |
| 0x03 | Cabinet | Storage unit with shelves, drawers, or doors |
| 0x04 | Gate | Movable barrier controlling entry or exit |
| 0x05 | GarageDoor | Movable barrier providing access to a garage |
| 0x06 | Door | Movable barrier between spaces |

### 0x45 Closure Panel

**Restricted to closure panel domain.** Identifies panel motion.

| ID | Name | Description |
|----|------|-------------|
| 0x00 | Lift | Upward/downward motion, typically on a vertical axis |
| 0x01 | Tilt | Rotation or tilting along a horizontal or vertical axis |
| 0x02 | Sliding | Horizontal back-and-forth or side-to-side motion |
| 0x03 | Rotate | Circular motion around a fixed point or axis |

### 0x46 Closure Covering

**Restricted to closure domain.** Identifies the kind of covering.

| ID | Name |
|----|------|
| 0x00 | Blind |
| 0x01 | Awning |
| 0x02 | Shutter |
| 0x03 | Venetian |
| 0x04 | Curtain |

### 0x47 Closure Window

**Restricted to closure domain.** Identifies the kind of window.

| ID | Name | Description |
|----|------|-------------|
| 0x00 | Roof | Window installed in a roof |
| 0x01 | Facade | Window installed in a vertical exterior wall |

### 0x48 Closure Cabinet

**Restricted to closure domain.** Identifies the kind of cabinet closure.

| ID | Name | Description |
|----|------|-------------|
| 0x00 | CabinetDoor | Hinged or sliding panel covering a cabinet opening |
| 0x01 | Drawer | Sliding storage compartment |
| 0x02 | Flap | Hinged front or flexible cover |

### 0x4A Identified Sound

Used by detection/sensing implementations to identify sounds or audio context.

| ID | Name | Detected sound/context |
|----|------|------------------------|
| 0x00 | Unknown | Unidentifiable audio context |
| 0x01 | ObjectFall | Object falling to floor |
| 0x02 | Snoring | Human snoring |
| 0x03 | Coughing | Human coughing |
| 0x04 | Barking | Dog barking |
| 0x05 | Shattering | Object shattering |
| 0x06 | BabyCrying | Baby crying |
| 0x07 | UtilityAlarm | Utility device alarm/warning |
| 0x08 | UrgentShouting | Urgent shouting |
| 0x09 | Doorbell | Doorbell ringing |
| 0x0A | Knocking | Door knocking |
| 0x0B | UrgentSiren | Urgent situational siren |
| 0x0C | FaucetRunning | Faucet water running |
| 0x0D | KettleBoiling | Kettle water boiling |
| 0x0E | FanDryer | Hair/hand fan dryer |
| 0x0F | Clapping | Hand clapping |
| 0x10 | FingerSnapping | Finger snapping |
| 0x11 | Meowing | Cat meowing |
| 0x12 | Laughing | Human laughing |
| 0x13 | GlassBreaking | Glass breaking |
| 0x14 | DoorKnocking | Door knocking |
| 0x15 | PersonTalking | Person talking |

### 0x49 Identified Object

Used by detection/sensing implementations to identify objects. Use a specific pet or human tag instead of `Pet` or `Person` when the implementation can make that distinction.

| ID | Name | Detected object |
|----|------|-----------------|
| 0x00 | Unknown | Unknown object |
| 0x01 | Adult | Human adult |
| 0x02 | Child | Human child |
| 0x03 | Person | Person |
| 0x04 | RVC | Robot Vacuum Cleaner |
| 0x05 | Pet | Pet animal |
| 0x06 | Dog | Dog |
| 0x07 | Cat | Cat |
| 0x08 | Animal | Animal |
| 0x09 | Car | Car |
| 0x0A | Vehicle | Vehicle |
| 0x0B | Package | Package |
| 0x0C | Clothes | Clothes |

### 0x4B Identified Human Activity

Used by detection/sensing implementations to identify human activity.

| ID | Name | Detected activity |
|----|------|-------------------|
| 0x00 | Unknown | Unknown human activity |
| 0x01 | Fall | Human fall |
| 0x02 | Sleeping | Human sleeping |
| 0x03 | Walking | Human walking |
| 0x04 | Workout | Human workout |
| 0x05 | Sitting | Human sitting |
| 0x06 | Standing | Human standing |
| 0x07 | Dancing | Human dancing |
| 0x08 | PackageDelivery | Human delivery of package |
| 0x09 | PackageRetrieval | Human retrieval of package |

---

## Quick Reference: Namespace IDs

| ID | Namespace | Scope |
|----|-----------|-------|
| 0x01 | Common Closure | Common (deprecated) |
| 0x02 | Compass Direction | Common |
| 0x03 | Compass Location | Common |
| 0x04 | Direction | Common |
| 0x05 | Level | Common |
| 0x06 | Location | Common |
| 0x07 | Number | Common |
| 0x08 | Position | Common |
| 0x0A | Electrical Measurement | Domain (electrical) |
| 0x0B | Commodity Tariff Chronology | Domain (energy calendar) |
| 0x0D | Commodity Tariff Commodity | Domain (tariff commodity) |
| 0x0E | Laundry | Domain (laundry) |
| 0x0F | Power Source | Domain (power) |
| 0x10 | Area | Common |
| 0x11 | Landmark | Common |
| 0x12 | Relative Position | Common |
| 0x13 | Commodity Tariff Flow | Domain (tariff flow) |
| 0x41 | Refrigerator | Device-specific |
| 0x42 | Room Air Conditioner | Device-specific |
| 0x43 | Switches | Device-specific |
| 0x44 | Closure | Device-specific |
| 0x45 | Closure Panel | Device-specific |
| 0x46 | Closure Covering | Device-specific |
| 0x47 | Closure Window | Device-specific |
| 0x48 | Closure Cabinet | Device-specific |
| 0x49 | Identified Object | Device-specific |
| 0x4A | Identified Sound | Device-specific |
| 0x4B | Identified Human Activity | Device-specific |
