load(
    "@rules_python//python:python.bzl",
    "py_binary",
)

licenses(["notice"])

py_binary(
    name = "json_format_example",
    srcs = ["json_format_example.py"],
    legacy_create_init = 0,
    python_version = "PY3",
    deps = [
        "@com_google_fhir//proto/google/fhir/proto/r4/core/resources:patient_py_pb2",
        "@com_google_fhir//py/r4:json_format",
    ],
)

py_binary(
    name = "validation_example",
    srcs = ["validation_example.py"],
    legacy_create_init = 0,
    python_version = "PY3",
    deps = [
        "@com_google_fhir//proto/google/fhir/proto/r4/core/resources:patient_py_pb2",
        "@com_google_fhir//py:fhir_errors",
        "@com_google_fhir//py/r4:json_format",
        "@com_google_fhir//py/r4:resource_validation",
    ],
)
