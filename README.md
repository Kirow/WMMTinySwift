# WMMTinySwift

A Swift wrapper for the original [WMM_Tiny](https://github.com/miniwinwm/WMM_Tiny) library.

---

## Features

- Calculate magnetic variation (declination) for any location, altitude, and date.
- Lightweight and easy to use.
- Uses the latest World Magnetic Model (WMM 2025).

---

## Usage

```swift
let wmm = WMMTiny.shared
let magVar = wmm.magneticVariation(
    coordinate: CLLocationCoordinate2D(
        latitude: 37.7749,
        longitude: -122.4194
    ),
    altitudeKm: 10,
    date: Date()
)
```

---

## Implementation Notes

This package uses `wmm.c` and `wmm.h` from the original WMM_Tiny, with a slight modification to include altitude as an input parameter:

```c
// include altitude above ellipsoid
r += height_km;
d += height_km;
```

---

## Regenerating WMM_COF.c

To re-generate the WMM_COF.c file from a WMM.COF model file, run:

```sh
swift run WWMConverter WMM.COF
```

---

## License

See [MIT License](LICENSE) for details.
