package(
    default_visibility = ["//visibility:public"],
)

licenses(["notice"])

load("@io_bazel_rules_go//go:def.bzl", "go_binary")

go_binary(
    name = "parse_patients",
    srcs = ["parse_patients.go"],
    deps = [
        "@com_google_fhir//go/google/fhir/jsonformat",
        "@com_google_fhir//proto/r4/core/resources:bundle_and_contained_resource_go_proto",
    ],
)
