licenses(["notice"])

java_library(
    name = "example_utils",
    srcs = ["com/google/fhirexamples/ExampleUtils.java"],
    deps = [
        "@com_google_fhir//java/com/google/fhir/common:json_format",
        "@com_google_protobuf//:protobuf_java",
    ],
)

java_binary(
    name = "ParsePatients",
    srcs = ["com/google/fhirexamples/ParsePatients.java"],
    main_class = "com.google.fhirexamples.ParsePatients",
    deps = [
        "@com_google_fhir//java/com/google/fhir/common:json_format",
        "@com_google_fhir//proto/google/fhir/proto/r4/core/resources:patient_java_proto",
    ],
)

java_binary(
    name = "ValidatePatients",
    srcs = ["com/google/fhirexamples/ValidatePatients.java"],
    main_class = "com.google.fhirexamples.ValidatePatients",
    deps = [
        ":example_utils",
        "@com_google_fhir//java/com/google/fhir/common",
        "@com_google_fhir//java/com/google/fhir/common:resource_validator",
        "@com_google_fhir//proto/google/fhir/proto/r4/core/resources:patient_java_proto",
    ],
)

java_binary(
    name = "GenerateBigQuerySchema",
    srcs = ["com/google/fhirexamples/GenerateBigQuerySchema.java"],
    main_class = "com.google.fhirexamples.GenerateBigQuerySchema",
    deps = [
        "//proto/google/fhir_examples/myprofile:myprofile_java_proto",
        "@com_google_fhir//java/com/google/fhir/common:big_query_schema",
        "@com_google_protobuf//:protobuf_java",
        "@maven//:com_google_apis_google_api_services_bigquery",
        "@maven//:com_google_guava_guava",
        "@maven//:com_google_http_client_google_http_client_gson",
    ],
)
