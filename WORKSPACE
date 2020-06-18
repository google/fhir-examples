workspace(name = "com_google_fhir_examples")

load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")
http_archive(
    name = "com_google_fhir",
    sha256 = "ad8828aee2effbec79b543ff027680be3e4618339325d281e2a60af6c548ccf0",
    strip_prefix = "fhir-869537a45757be9fe0011d742873c02721aa7d97",
    urls = [
        "https://github.com/google/fhir/archive/869537a45757be9fe0011d742873c02721aa7d97.zip",
    ],
)

load("@com_google_fhir//bazel:dependencies.bzl", "fhirproto_dependencies")
fhirproto_dependencies()

load("@com_google_fhir//bazel:workspace.bzl", "fhirproto_workspace")
fhirproto_workspace()

load("@fhir_bazel_pip_dependencies//:requirements.bzl", "pip_install")
pip_install()

load("@com_google_fhir//bazel:go_dependencies.bzl", "fhir_go_dependencies")
fhir_go_dependencies()
