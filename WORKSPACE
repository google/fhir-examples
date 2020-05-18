workspace(name = "com_nickjpgeorge_fhirprotoexamples")

load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

# DO NOT SUBMIT: remove local_repository once cr/312163265 is submitted
# http_archive(
#     name = "com_google_fhir",
#     sha256 = "1dbe977b7ea225be24f5b61d03b1f0ad067c7821434f93e868f4cbde075c6c36",
#     strip_prefix = "fhir-d2de420d5f6f0f72e06603308262998bd7c027bd",
#     urls = [
#         "https://github.com/google/fhir/archive/d2de420d5f6f0f72e06603308262998bd7c027bd.zip",
#     ],
# )

local_repository(
    name = "com_google_fhir",
    path = "/tmp/fhir/",
)

load("@com_google_fhir//bazel:dependencies.bzl", "fhirproto_dependencies")
fhirproto_dependencies()

load("@com_google_fhir//bazel:workspace.bzl", "fhirproto_workspace")
fhirproto_workspace()

load("@fhir_bazel_pip_dependencies//:requirements.bzl", "pip_install")
pip_install()

