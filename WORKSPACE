workspace(name = "com_google_fhir_examples")

load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")
http_archive(
    name = "com_google_fhir",
    sha256 = "64e6a1ed624dcf68556761a04e1096119bfc52db1e0956fbcdabf3fa1478fdbb",
    strip_prefix = "fhir-bba915b82dacb4d1b9571a134a4049ec3a24181c",
    urls = [
        "https://github.com/google/fhir/archive/bba915b82dacb4d1b9571a134a4049ec3a24181c.zip",
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
