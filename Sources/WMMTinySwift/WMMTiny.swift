import Foundation
import WMMTinyWrapper
import CoreLocation

public struct WMMTiny: Sendable {
    /// The shared instance of `WMMTiny`.
    public static let shared = WMMTiny()
    
    /// Private initializer to enforce singleton usage.
    private init() { wmm_init() }

    /// Returns the magnetic declination for a given latitude, longitude, altitude, and date.
    ///
    /// - Parameters:
    ///   - coordinate: The geographic coordinate (latitude and longitude).
    ///   - altitudeKm: The altitude in kilometers WGS-84.
    ///   - date: The date for which to calculate the magnetic declination.
    ///
    /// - Returns: The magnetic declination in degrees.
    public func magneticVariation(
        coordinate: CLLocationCoordinate2D,
        altitudeKm: Double = 0.0,
        date: Date = Date()
    ) -> Float {
        let latitude = Float(coordinate.latitude)
        let longitude = Float(coordinate.longitude)
        let altitude = Float(altitudeKm)
        let year = Calendar.current.component(.year, from: date)
        let month = Calendar.current.component(.month, from: date)
        let day = Calendar.current.component(.day, from: date)
        let date = wmm_get_date(UInt8(year - 2000), UInt8(month), UInt8(day))
        var dec: Float = 0.0
        E0000(latitude, longitude, altitude, date, &dec)
        return dec
    }
}

