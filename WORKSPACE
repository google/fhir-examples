workspace(name = "com_nickjpgeorge_fhirprotoexamples")

load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

http_archive(
    name = "com_google_fhir",
    sha256 = "039a4079f66d3f9393f86290a32b139518ffb0d9f7dc8b63b80df696a67f4427",
    strip_prefix = "fhir-7528173fff5e29e3f3b4fa6532c4fdb813bda2f8",
    urls = [
        "https://github.com/google/fhir/archive/7528173fff5e29e3f3b4fa6532c4fdb813bda2f8.zip",
    ],
)

load("@com_google_fhir//bazel:dependencies.bzl", "fhirproto_dependencies")
fhirproto_dependencies()

load("@com_google_fhir//bazel:workspace.bzl", "fhirproto_workspace")
fhirproto_workspace()

load("@fhir_bazel_pip_dependencies//:requirements.bzl", "pip_install")
pip_install()

