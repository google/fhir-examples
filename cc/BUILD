package(
    default_visibility = ["//visibility:public"],
)

licenses(["notice"])

cc_library(
    name = "example_utils",
    hdrs = ["example_utils.h"],
    deps = [
        "@com_google_fhir//cc/google/fhir:json_format",
    ],
)

cc_binary(
    name = "ParsePatients",
    srcs = ["parse_patients.cc"],
    deps = [
        ":example_utils",
        "@com_google_fhir//proto/r4/core/resources:patient_cc_proto",
        "@com_google_absl//absl/strings",
        "@com_google_absl//absl/time",
    ],
)

cc_binary(
    name = "ProfilePatientsToUsCore",
    srcs = ["profile_patients_to_uscore.cc"],
    deps = [
        ":example_utils",
        "@com_google_fhir//cc/google/fhir/r4:profiles",
        "@com_google_fhir//proto/r4/core/resources:patient_cc_proto",
        "@com_google_fhir//proto/r4:uscore_cc_proto",
        "@com_google_absl//absl/strings",
        "@com_google_absl//absl/time",
    ],
)

cc_binary(
    name = "ProfilePatientsToCustomProfile",
    srcs = ["profile_patients_to_custom_profile.cc"],
    deps = [
        ":example_utils",
        "//proto:demo_cc_proto",
        "@com_google_fhir//cc/google/fhir/r4:profiles",
        "@com_google_fhir//proto/r4/core/resources:patient_cc_proto",
        "@com_google_absl//absl/strings",
        "@com_google_absl//absl/time",
    ],
)
