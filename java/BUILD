java_library(
    name = "example_utils",
    srcs = ["com/google/fhirexamples/ExampleUtils.java"],
    deps = [
        "@maven//:com_google_guava_guava",
        "@com_google_fhir//java:json_format",
        "@com_google_fhir//java:common",
        "@com_google_protobuf//:protobuf_java",
    ],
)

java_binary(
    name = "ParsePatients",
    main_class = "com.google.fhirexamples.ParsePatients",
    srcs = ["com/google/fhirexamples/ParsePatients.java"],
    deps = [
        "@maven//:com_google_guava_guava",
        "@com_google_fhir//java:json_format",
        "@com_google_fhir//java:common",
        "@com_google_fhir//proto/r4/core/resources:patient_java_proto",
        "@com_google_protobuf//:protobuf_java",
    ],
)

java_binary(
    name = "ValidatePatients",
    main_class = "com.google.fhirexamples.ValidatePatients",
    srcs = ["com/google/fhirexamples/ValidatePatients.java"],
    deps = [
        ":example_utils",
        "@com_google_fhir//java:common",
        "@com_google_fhir//proto/r4/core/resources:patient_java_proto",
        "@com_google_fhir//proto/r4/core:datatypes_java_proto",
    ],
)

java_binary(
    name = "GenerateBigQuerySchema",
    main_class = "com.google.fhirexamples.GenerateBigQuerySchema",
    srcs = ["com/google/fhirexamples/GenerateBigQuerySchema.java"],
    deps = [
        "//proto/myprofile:myprofile_java_proto",
        "@com_google_fhir//java:big_query_schema",
        "@com_google_protobuf//:protobuf_java",
        "@maven//:com_google_apis_google_api_services_bigquery",
        "@maven//:com_google_guava_guava",
        "@maven//:com_google_http_client_google_http_client_gson",
    ],
)