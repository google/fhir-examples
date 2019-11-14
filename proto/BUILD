package(
    default_visibility = ["//visibility:public"],
)

licenses(["notice"])

load("@com_google_fhir//bazel:protogen.bzl", "gen_fhir_definitions_and_protos")
load("@com_google_fhir//bazel:proto.bzl", "fhir_proto_library")

gen_fhir_definitions_and_protos(
    name = "demo",
    additional_proto_imports = [
        "proto/r4/uscore.proto",
        "proto/r4/uscore_codes.proto",
    ],
    extensions = [],
    package_deps = [
        "@com_google_fhir//spec:uscore_r4_package",
    ],
    package_info = "demo_package_info.prototxt",
    profiles = ["demo_profiles.prototxt"],
)

fhir_proto_library(
    srcs = [
        "demo.proto",
    ],
    proto_deps = [
        "@com_google_protobuf//:any_proto",
        "@com_google_fhir//proto:annotations_proto",
        "@com_google_fhir//proto/r4:uscore_codes_proto",
        "@com_google_fhir//proto/r4:uscore_proto",
        "@com_google_fhir//proto/r4/core:codes_proto",
        "@com_google_fhir//proto/r4/core:datatypes_proto",
        "@com_google_fhir//proto/r4/core:extensions_proto",
    ],
    proto_library_prefix = "demo",
)
