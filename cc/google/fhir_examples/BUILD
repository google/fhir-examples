package(
    default_visibility = ["//visibility:public"],
)

licenses(["notice"])

cc_library(
    name = "example_utils",
    hdrs = ["example_utils.h"],
    strip_include_prefix = "//cc/",
    deps = [
        "@com_google_absl//absl/status:statusor",
        "@com_google_absl//absl/strings",
        "@com_google_absl//absl/time",
        "@com_google_fhir//cc/google/fhir/r4:json_format",
        "@com_google_fhir//cc/google/fhir/status:statusor",
        "@com_google_fhir//proto/google/fhir/proto/r4/core/resources:patient_cc_proto",
    ],
)

cc_binary(
    name = "ParsePatients",
    srcs = ["parse_patients.cc"],
    deps = [
        ":example_utils",
        "@com_google_absl//absl/strings",
        "@com_google_fhir//cc/google/fhir/r4:json_format",
        "@com_google_fhir//proto/google/fhir/proto/r4/core/resources:patient_cc_proto",
    ],
)

cc_binary(
    name = "ProfilePatientsToUsCore",
    srcs = ["profile_patients_to_uscore.cc"],
    deps = [
        ":example_utils",
        "@com_google_fhir//cc/google/fhir/r4:json_format",
        "@com_google_fhir//cc/google/fhir/r4:profiles",
        "@com_google_fhir//cc/google/fhir/r4:resource_validation",
        "@com_google_fhir//cc/google/fhir/status",
        "@com_google_fhir//proto/google/fhir/proto/r4:uscore_cc_proto",
        "@com_google_fhir//proto/google/fhir/proto/r4/core/resources:patient_cc_proto",
    ],
)

cc_binary(
    name = "ProfilePatientsToCustomProfile",
    srcs = ["profile_patients_to_custom_profile.cc"],
    deps = [
        ":example_utils",
        "//proto/google/fhir_examples/myprofile:myprofile_cc_proto",
        "@com_google_fhir//cc/google/fhir:json_format",
        "@com_google_fhir//cc/google/fhir/r4:profiles",
        "@com_google_fhir//proto/google/fhir/proto/r4:uscore_cc_proto",
        "@com_google_fhir//proto/google/fhir/proto/r4/core:datatypes_cc_proto",
    ],
)

cc_binary(
    name = "ValidatePatients",
    srcs = ["validate_patients.cc"],
    deps = [
        ":example_utils",
        "@com_google_fhir//cc/google/fhir/r4:json_format",
        "@com_google_fhir//cc/google/fhir/r4:resource_validation",
        "@com_google_fhir//cc/google/fhir/status",
        "@com_google_fhir//proto/google/fhir/proto/r4:uscore_cc_proto",
        "@com_google_fhir//proto/google/fhir/proto/r4/core/resources:patient_cc_proto",
    ],
)

cc_binary(
    name = "PrintToAnalyticNdJson",
    srcs = ["print_to_analytic_ndjson.cc"],
    deps = [
        ":example_utils",
        "//proto/google/fhir_examples/myprofile:myprofile_cc_proto",
        "@com_google_fhir//cc/google/fhir:json_format",
        "@com_google_fhir//cc/google/fhir/r4:profiles",
        "@com_google_fhir//proto/google/fhir/proto/r4/core/resources:patient_cc_proto",
    ],
)
