workspace(name = "com_nickjpgeorge_fhirprotoexamples")

load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

http_archive(
    name = "com_google_fhir",
    sha256 = "68104fe1e284cb6253ae6eb5ffd0489d0dee3f0383ba61eb8e1160f440139881",
    strip_prefix = "fhir-458652c071d2e2942cec40afb771cdefa011bf09",
    urls = [
        "https://github.com/google/fhir/archive/458652c071d2e2942cec40afb771cdefa011bf09.zip",
    ],
)

load("@com_google_fhir//bazel:dependencies.bzl", "fhirproto_dependencies")
fhirproto_dependencies()

load("@com_google_fhir//bazel:workspace.bzl", "fhirproto_workspace")
fhirproto_workspace()

load("@fhir_bazel_pip_dependencies//:requirements.bzl", "pip_install")
pip_install()

