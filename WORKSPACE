workspace(name = "com_google_fhir_examples")

load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")
http_archive(
    name = "com_google_fhir",
    sha256 = "0173112fb292e3c9208f02d7efb86157ce869451b02aee62293698a2043b3b7d",
    strip_prefix = "fhir-0.7.0",
    urls = [
        "https://github.com/google/fhir/archive/v0.7.0.zip",
    ],
)

load("@com_google_fhir//bazel:dependencies.bzl", "fhirproto_dependencies")
fhirproto_dependencies()

load("@com_google_protobuf//:protobuf_deps.bzl", "protobuf_deps")
protobuf_deps()

load("@com_google_fhir//bazel:workspace.bzl", "fhirproto_workspace")
fhirproto_workspace()

load("@rules_python//python:pip.bzl", "pip3_import", "pip_repositories")
pip3_import(
    name = "fhir_bazel_pip_dependencies",
    requirements = "@com_google_fhir//bazel:requirements.txt",
)
pip_repositories()

load("@fhir_bazel_pip_dependencies//:requirements.bzl", "pip_install")
pip_install()

load("@com_google_fhir//bazel:go_dependencies.bzl", "fhir_go_dependencies")
fhir_go_dependencies()
