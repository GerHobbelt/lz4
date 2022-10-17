// swift-tools-version: 5.6
// The swift-tools-version declares the minimum version of Swift required to build this package.
//
// lz4-cpp-SwiftPM
//
import PackageDescription

let package = Package(
    name: "lz4-cpp",
    products: [
        .library(
            name: "lz4",
            targets: ["lz4"]),
    ],
    targets: [
        .target(
            name: "lz4",
            path: ".",
            exclude: [
                "build/",
                "contrib/",
                "doc/",
                "examples/",
                "lib/dll",
                "lib/liblz4-dll.rc.in",
                "lib/liblz4.pc.in",
                "ossfuzz/",
                "programs/",
                "tests/",
                "appveyor.yml",
                "INSTALL",
                "LICENSE",
                "Makefile",
                "Makefile.inc",
                "NEWS",
                "README.md"
            ],
            sources: [
                "lib/lz4.c",
                "lib/lz4.h",
                "lib/lz4file.c",
                "lib/lz4file.h",
                "lib/lz4frame_static.h",
                "lib/lz4frame.c",
                "lib/lz4frame.h",
                "lib/lz4hc.c",
                "lib/lz4hc.h",
                "lib/xxhash.c",
                "lib/xxhash.h",
            ],
            publicHeadersPath: "lib")
    ]
)