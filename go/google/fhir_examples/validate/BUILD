package(
    default_visibility = ["//visibility:public"],
)

licenses(["notice"])

load("@io_bazel_rules_go//go:def.bzl", "go_binary")

go_binary(
    name = "validate_patients",
    srcs = ["validate_patients.go"],
    deps = [
        "@com_google_fhir//go/jsonformat",
        "@com_google_fhir//go/jsonformat/fhirvalidate",
        "@com_google_fhir//proto/google/fhir/proto/r4/core:datatypes_go_proto",
        "@com_google_fhir//proto/google/fhir/proto/r4/core/resources:bundle_and_contained_resource_go_proto",
        "@com_google_fhir//proto/google/fhir/proto/r4/core/resources:patient_go_proto",
    ],
)
