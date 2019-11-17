workspace(name = "com_nickjpgeorge_fhirprotoexamples")

load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

http_archive(
    name = "com_google_fhir",
    sha256 = "48d1c22a2bc4aede3e7f327ee1f8f69dec838634552c56394d9d4e466c28bb5c",
    strip_prefix = "fhir-d47f9a1c1fb694384b708522582688312a96a2bc",
    urls = [
        "https://github.com/google/fhir/archive/d47f9a1c1fb694384b708522582688312a96a2bc.zip",
    ],
)

load("@com_google_fhir//bazel:dependencies.bzl", "fhirproto_dependencies")
fhirproto_dependencies()

load("@com_google_fhir//bazel:workspace.bzl", "fhirproto_workspace")
fhirproto_workspace()

load("@fhir_bazel_pip_dependencies//:requirements.bzl", "pip_install")
pip_install()

