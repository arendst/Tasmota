# Matter 1.4.1 Semantic Tag Namespaces - Compact Spec

Version: 1.4.1 (2025-03-17)

## Overview

Semantic tags annotate endpoints via TagList in Descriptor cluster. Tags combine namespace ID + tag ID.

**Namespace Ranges:**
- Common: 0x01-0x1F (usable across domains)
- Device-specific: 0x41+ (restricted to specific device types)

**Rules:**
- TagList MAY combine tags from: device-specific namespace + common namespaces + manufacturer-specific
- Row/Column tags: Label field = Arabic numeral string ("1", "2"...), NOT words
- Endpoints in grid/matrix: allocate top-to-bottom, left-to-right order

---

## Common Namespaces

### 0x01 Closure
| ID | Name | Description |
|----|------|-------------|
| 0x00 | Opening | Move toward open |
| 0x01 | Closing | Move toward closed |
| 0x02 | Stop | Stop movement |

### 0x02 Compass Direction
Movement into compass direction (vs 0x03 which is location).

| ID | Name |
|----|------|
| 0x00 | Northward |
| 0x01 | North-Eastward |
| 0x02 | Eastward |
| 0x03 | South-Eastward |
| 0x04 | Southward |
| 0x05 | South-Westward |
| 0x06 | Westward |
| 0x07 | North-Westward |

### 0x03 Compass Location
Position in compass direction (vs 0x02 which is movement).

| ID | Name |
|----|------|
| 0x00 | North |
| 0x01 | North-East |
| 0x02 | East |
| 0x03 | South-East |
| 0x04 | South |
| 0x05 | South-West |
| 0x06 | West |
| 0x07 | North-West |

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
| 0x00 | Indoor | Indoors or indoor conditions |
| 0x01 | Outdoor | Outdoors or outdoor conditions |
| 0x02 | Inside | Inside equipment |
| 0x03 | Outside | Outside equipment |

### 0x07 Number
| ID | Name |
|----|------|
| 0x00-0x0A | Zero-Ten |

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
- Use for grids >3 elements in any direction
- Label field SHALL contain Arabic numeral string
- First row/column = "1"

### 0x10 Area
Home areas (indoor/outdoor).

| ID | Name | Notes |
|----|------|-------|
| 0x00 | Aisle | |
| 0x01 | Attic | |
| 0x02 | Back Door | |
| 0x03 | Back Yard | |
| 0x04 | Balcony | |
| 0x05 | Ballroom | |
| 0x06 | Bathroom | Also: Restroom |
| 0x07 | Bedroom | |
| 0x08 | Border | |
| 0x09 | Boxroom | Small storage room |
| 0x0A | Breakfast Room | |
| 0x0B | Carport | |
| 0x0C | Cellar | |
| 0x0D | Cloakroom | |
| 0x0E | Closet | |
| 0x0F | Conservatory | |
| 0x10 | Corridor | |
| 0x11 | Craft Room | |
| 0x12 | Cupboard | |
| 0x13 | Deck | |
| 0x14 | Den | Small room for work/hobbies |
| 0x15 | Dining | |
| 0x16 | Drawing Room | |
| 0x17 | Dressing Room | |
| 0x18 | Driveway | |
| 0x19 | Elevator | |
| 0x1A | Ensuite | Bathroom from bedroom |
| 0x1B | Entrance | |
| 0x1C | Entryway | |
| 0x1D | Family Room | |
| 0x1E | Foyer | |
| 0x1F | Front Door | |
| 0x20 | Front Yard | |
| 0x21 | Game Room | |
| 0x22 | Garage | |
| 0x23 | Garage Door | |
| 0x24 | Garden | |
| 0x25 | Garden Door | |
| 0x26 | Guest Bathroom | Also: Guest Restroom |
| 0x27 | Guest Bedroom | |
| 0x28 | Guest Room | |
| 0x29 | Gym | |
| 0x2A | Hallway | |
| 0x2B | Hearth Room | Room with fireplace |
| 0x2C | Kids Room | |
| 0x2D | Kids Bedroom | |
| 0x2E | Kitchen | |
| 0x2F | Laundry Room | |
| 0x30 | Lawn | |
| 0x31 | Library | |
| 0x32 | Living Room | |
| 0x33 | Lounge | |
| 0x34 | Media/TV Room | |
| 0x35 | Mud Room | Remove soiled garments |
| 0x36 | Music Room | |
| 0x37 | Nursery | |
| 0x38 | Office | |
| 0x39 | Outdoor Kitchen | |
| 0x3A | Outside | |
| 0x3B | Pantry | Food storage (larder) |
| 0x3C | Parking Lot | |
| 0x3D | Parlor | |
| 0x3E | Patio | |
| 0x3F | Play Room | |
| 0x40 | Pool Room | |
| 0x41 | Porch | |
| 0x42 | Primary Bathroom | |
| 0x43 | Primary Bedroom | |
| 0x44 | Ramp | |
| 0x45 | Reception Room | |
| 0x46 | Recreation Room | |
| 0x47 | Roof | |
| 0x48 | Sauna | |
| 0x49 | Scullery | Cleaning dishes/laundry |
| 0x4A | Sewing Room | |
| 0x4B | Shed | |
| 0x4C | Side Door | |
| 0x4D | Side Yard | |
| 0x4E | Sitting Room | |
| 0x4F | Snug | Cozy informal space |
| 0x50 | Spa | |
| 0x51 | Staircase | |
| 0x52 | Steam Room | |
| 0x53 | Storage Room | |
| 0x54 | Studio | |
| 0x55 | Study | |
| 0x56 | Sun Room | |
| 0x57 | Swimming Pool | |
| 0x58 | Terrace | |
| 0x59 | Toilet | Water closet/WC |
| 0x5A | Utility Room | |
| 0x5B | Ward | |
| 0x5C | Workshop | |

### 0x11 Landmark
Home landmarks (furniture, appliances, fixtures).

| ID | Name |
|----|------|
| 0x00 | Air Conditioner |
| 0x01 | Air Purifier |
| 0x02 | Back Door |
| 0x03 | Bar Stool |
| 0x04 | Bath Mat |
| 0x05 | Bathtub |
| 0x06 | Bed |
| 0x07 | Bookshelf |
| 0x08 | Chair |
| 0x09 | Christmas Tree |
| 0x0A | Coat Rack |
| 0x0B | Coffee Table |
| 0x0C | Cooking Range |
| 0x0D | Couch |
| 0x0E | Countertop |
| 0x0F | Cradle |
| 0x10 | Crib |
| 0x11 | Desk |
| 0x12 | Dining Table |
| 0x13 | Dishwasher |
| 0x14 | Door |
| 0x15 | Dresser |
| 0x16 | Laundry Dryer |
| 0x17 | Fan |
| 0x18 | Fireplace |
| 0x19 | Freezer |
| 0x1A | Front Door |
| 0x1B | High Chair |
| 0x1C | Kitchen Island |
| 0x1D | Lamp |
| 0x1E | Litter Box |
| 0x1F | Mirror |
| 0x20 | Nightstand |
| 0x21 | Oven |
| 0x22 | Pet Bed |
| 0x23 | Pet Bowl |
| 0x24 | Pet Crate |
| 0x25 | Refrigerator |
| 0x26 | Scratching Post |
| 0x27 | Shoe Rack |
| 0x28 | Shower |
| 0x29 | Side Door |
| 0x2A | Sink |
| 0x2B | Sofa |
| 0x2C | Stove |
| 0x2D | Table |
| 0x2E | Toilet |
| 0x2F | Trash Can |
| 0x30 | Laundry Washer |
| 0x31 | Window |
| 0x32 | Wine Cooler |

### 0x12 Relative Position
Position relative to external reference (user must specify reference).

| ID | Name | Description |
|----|------|-------------|
| 0x00 | Under | |
| 0x01 | Next To | Proximity to reference |
| 0x02 | Around | Surrounding reference |
| 0x03 | On | |
| 0x04 | Above | |
| 0x05 | Front Of | |
| 0x06 | Behind | |

---

## Domain-Specific Namespaces (Common Range)

### 0x0A Electrical Measurement
**Restricted to electrical measurement domain.**

| ID | Name | Description |
|----|------|-------------|
| 0x00 | DC | DC load |
| 0x01 | AC | Single-phase or collective polyphase |
| 0x02 | ACPhase1 | Phase 1 of polyphase |
| 0x03 | ACPhase2 | Phase 2 of polyphase |
| 0x04 | ACPhase3 | Phase 3 of polyphase |

### 0x0E Laundry
**Restricted to laundry domain.**

| ID | Name |
|----|------|
| 0x00 | Normal |
| 0x01 | Light Dry |
| 0x02 | Extra Dry |
| 0x03 | No Dry |

### 0x0F Power Source
**Restricted to power source domain.**

| ID | Name | Required Feature |
|----|------|------------------|
| 0x00 | Unknown | - |
| 0x01 | Grid | WIRED |
| 0x02 | Solar | WIRED |
| 0x03 | Battery | BAT |
| 0x04 | EV | BAT |

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
| 0x03 | Up | e.g., dim up |
| 0x04 | Down | e.g., dim down |
| 0x05 | Next | e.g., next scene |
| 0x06 | Previous | e.g., previous scene |
| 0x07 | Enter/OK/Select | |
| 0x08 | Custom | Label = button text/icon description |

**Custom Tag Rule:** Label field SHALL contain textual description of button function (e.g., "dining").

---

## Quick Reference: Namespace IDs

| ID | Namespace | Scope |
|----|-----------|-------|
| 0x01 | Closure | Common |
| 0x02 | Compass Direction | Common |
| 0x03 | Compass Location | Common |
| 0x04 | Direction | Common |
| 0x05 | Level | Common |
| 0x06 | Location | Common |
| 0x07 | Number | Common |
| 0x08 | Position | Common |
| 0x0A | Electrical Measurement | Domain (electrical) |
| 0x0E | Laundry | Domain (laundry) |
| 0x0F | Power Source | Domain (power) |
| 0x10 | Area | Common |
| 0x11 | Landmark | Common |
| 0x12 | Relative Position | Common |
| 0x41 | Refrigerator | Device-specific |
| 0x42 | Room Air Conditioner | Device-specific |
| 0x43 | Switches | Device-specific |
