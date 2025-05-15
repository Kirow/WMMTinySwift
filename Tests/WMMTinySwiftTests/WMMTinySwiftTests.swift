//
//  WMMTinySwiftTests.swift
//  wmm-swift
//
//  Created by Kirow on 15.05.2025.
//


import XCTest
@testable import WMMTinySwift
import CoreLocation

struct TestData {
    /*
    {
        "latitude": 37.7749,
        "longitude": -122.4194,
        "altitude": 5,
        "declination": 12.95
    }
    */
    struct Sample: Codable {
        let latitude: Double
        let longitude: Double
        let altitude: Double
        let declination: Float
    }
    let date: Date
    let samples: [Sample]

    init() {
        let predefinedDate = "May 15, 2025"
        let dateFormatter = DateFormatter()
        dateFormatter.dateFormat = "MMMM dd, yyyy"
        guard let date = dateFormatter.date(from: predefinedDate) else {
            fatalError("Invalid date format")
        }
        self.date = date

        // read JSON data from file test_data_05152025.json
        guard let url = Bundle.module.url(forResource: "test_data_05152025", withExtension: "json") else {
            fatalError("File not found")
        }
        guard let data = try? Data(contentsOf: url) else {
            fatalError("Unable to read file")
        }
        let decoder = JSONDecoder()
        do {
            let decodedData = try decoder.decode([Sample].self, from: data)
            self.samples = decodedData
        } catch {
            fatalError("Failed to decode JSON: \(error)")
        }
    }
}


final class WMMTinySwiftTests: XCTestCase {
    
    func testCalculations() throws {
        let testData = TestData()
        let wmm = WMMTiny.shared
        for input in testData.samples {
            let result = wmm.magneticVariation(
                coordinate: CLLocationCoordinate2D(
                    latitude: input.latitude,
                    longitude: input.longitude
                ),
                altitudeKm: input.altitude,
                date: testData.date
            )
            let expectedDeclination = input.declination
            let tolerance: Float = 0.01 // Adjust tolerance as needed
            print("lon: \(input.longitude), lat: \(input.latitude), alt: \(input.altitude), decl: \(expectedDeclination), result: \(result)")
            XCTAssertEqual(result, expectedDeclination, accuracy: tolerance, "Failed for input: \(input)")
        }
    }

    func testPerformanceWMMTiny() throws {
        let testData = TestData()
        let wmm = WMMTiny.shared
        measure {
            for _ in 0..<1000 {
                for input in testData.samples {
                    _ = wmm.magneticVariation(
                        coordinate: CLLocationCoordinate2D(
                            latitude: input.latitude,
                            longitude: input.longitude
                        ),
                        altitudeKm: input.altitude,
                        date: testData.date
                    )
                }
            }
        }
        print("Operation count: \(testData.samples.count * 10000)")
    }
}
