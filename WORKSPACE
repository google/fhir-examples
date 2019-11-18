workspace(name = "com_nickjpgeorge_fhirprotoexamples")

load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

http_archive(
    name = "com_google_fhir",
    sha256 = "18e90a4cfd84512e635c04c28e988a06020691edaba98e6694fb128abb93d7ea",
    strip_prefix = "fhir-8a253d84ffc65dcea9c013f8e73d5897614c237f",
    urls = [
        "https://github.com/google/fhir/archive/8a253d84ffc65dcea9c013f8e73d5897614c237f.zip",
    ],
)

load("@com_google_fhir//bazel:dependencies.bzl", "fhirproto_dependencies")
fhirproto_dependencies()

load("@com_google_fhir//bazel:workspace.bzl", "fhirproto_workspace")
fhirproto_workspace()

load("@fhir_bazel_pip_dependencies//:requirements.bzl", "pip_install")
pip_install()

