workspace(name = "com_google_fhir_examples")

load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")
http_archive(
    name = "com_google_fhir",
    sha256 = "def823d697511f4f02dcba368fa99282588cd96b2d765657db8242301d0688df",
    strip_prefix = "fhir-d7b450f1179f7b3fb6edec91511004dbc196f394",
    urls = [
        "https://github.com/google/fhir/archive/d7b450f1179f7b3fb6edec91511004dbc196f394.zip",
    ],
)

load("@com_google_fhir//bazel:dependencies.bzl", "fhirproto_dependencies")
fhirproto_dependencies()

load("@com_google_protobuf//:protobuf_deps.bzl", "protobuf_deps")
protobuf_deps()

load("@com_google_fhir//bazel:workspace.bzl", "fhirproto_workspace")
fhirproto_workspace()

load("@com_google_fhir//bazel:go_dependencies.bzl", "fhir_go_dependencies")
fhir_go_dependencies()
