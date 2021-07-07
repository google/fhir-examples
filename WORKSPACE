workspace(name = "com_google_fhir_examples")

load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")
http_archive(
    name = "com_google_fhir",
    sha256 = "29a0f0957642f9167cdd0763c794665d44a9e19adc3c0c8ea6d1ad8f9a2a147f",
    strip_prefix = "fhir-a54aa66a3967470096775850602b4b06f00c3d79",
    urls = [
        "https://github.com/google/fhir/archive/a54aa66a3967470096775850602b4b06f00c3d79.zip",
    ],
)

load("@com_google_fhir//bazel:dependencies.bzl", "fhirproto_dependencies")
fhirproto_dependencies()

load("@com_google_protobuf//:protobuf_deps.bzl", "protobuf_deps")
protobuf_deps()

load("@com_google_fhir//bazel:workspace.bzl", "fhirproto_workspace")
fhirproto_workspace()

load("@rules_python//python:pip.bzl", "pip_install")
pip_install(
   name = "fhir_bazel_pip_dependencies",
    requirements = "@com_google_fhir//bazel:requirements.txt",
)

load("@com_google_fhir//bazel:go_dependencies.bzl", "fhir_go_dependencies")
fhir_go_dependencies()
