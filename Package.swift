// swift-tools-version: 6.1
// The swift-tools-version declares the minimum version of Swift required to build this package.
import PackageDescription

let package = Package(
    name: "wmm-swift",
    products: [
        .library(
            name: "WMMTinySwift",
            targets: ["WMMTinySwift"]
        ),
        .executable(
            name: "WMMConverter",
            targets: ["WMMConverter"]
        )
    ],
    targets: [
        .target(
            name: "WMMTinyWrapper",
            publicHeadersPath: "include",
            cSettings: [
                .headerSearchPath("include")
            ]
        ),
        .target(
            name: "WMMTinySwift",
            dependencies: ["WMMTinyWrapper"]
        ),
        .testTarget(
            name: "WMMTinySwiftTests",
            dependencies: ["WMMTinySwift"],
            resources: [
                .process("Resources")
            ]
        ),
        .executableTarget(
            name: "WMMConverter",
            path: "Sources/WMMConverter"
        )
    ]
)
