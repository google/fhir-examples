workspace(name = "com_google_fhir_examples")

load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

http_archive(
    name = "com_google_fhir",
    sha256 = "c331326f9151954bed3be5a9537497046701fc079f744ca95f01990b6e63df20",
    strip_prefix = "fhir-c6eecb56f90e04d0565f99053d0ba85b8e91ac71",
    urls = [
        "https://github.com/google/fhir/archive/c6eecb56f90e04d0565f99053d0ba85b8e91ac71.zip",
    ],
)

load("@com_google_fhir//bazel:dependencies.bzl", "fhirproto_dependencies")
fhirproto_dependencies()

load("@com_google_fhir//bazel:workspace.bzl", "fhirproto_workspace")
fhirproto_workspace()

load("@fhir_bazel_pip_dependencies//:requirements.bzl", "pip_install")
pip_install()

