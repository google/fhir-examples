workspace(name = "com_google_fhir_examples")

load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")
http_archive(
    name = "com_google_fhir",
    sha256 = "5c745a489469db26150f4bb975e1b171d9d3e219d574514bc5c86f292bbe4a70",
    strip_prefix = "fhir-30f5cde7c5cdeb6f2f9f4d54757f46f31ca5c55f",
    urls = [
        "https://github.com/google/fhir/archive/30f5cde7c5cdeb6f2f9f4d54757f46f31ca5c55f.zip",
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
