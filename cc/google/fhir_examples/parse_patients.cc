// Copyright 2019 Google LLC
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "absl/strings/str_cat.h"
#include "google/fhir/r4/json_format.h"
#include "proto/google/fhir/proto/r4/core/resources/patient.pb.h"
#include "google/fhir_examples/example_utils.h"

using std::string;

using ::google::fhir::r4::JsonFhirStringToProto;
using ::google::fhir::r4::PrintFhirPrimitive;
using ::google::fhir::r4::core::Patient;

// Example code for using JsonFormat to move back and forth from
// FHIR JSON <--> FHIR Proto
//
// To run:
// bazel build //cc:ParsePatients
// bazel-bin/cc/ParsePatient $WORKSPACE
//
// where $WORKSPACE is the location of a synthea dataset.
// For instructions on setting up your workspace, see the top-level README.md

int main(int argc, char** argv) {
  if (argc == 1) {
    std::cerr << "Missing workspace argument." << std::endl;
    return 1;
  }
  const std::string workspace = argv[1];

  absl::TimeZone time_zone;
  CHECK(absl::LoadTimeZone("America/Los_Angeles", &time_zone));

  std::cout << "Reading " + absl::StrCat(workspace) << std::endl;
  const std::string file =
      absl::StrCat(workspace, "/ndjson/Patient.fhir.ndjson");
  std::ifstream read_stream(file);
  if (!read_stream.is_open()) {
    std::cerr << "Unable to open: " << file << "." << std::endl;
    return 1;
  }

  std::vector<Patient> result;
  std::string line;
  while (!read_stream.eof()) {
    std::getline(read_stream, line);
    if (!line.length()) continue;
    result.push_back(JsonFhirStringToProto<Patient>(line, time_zone).value());
    std::cout << "Parsed Patient " << result.back().id().value() << std::endl;
  }

  if (result.empty()) {
    std::cerr << "No data read from: " << file << "." << std::endl;
    return 1;
  }

  const Patient& example_patient = result.front();

  // See first record in FhirProto form
  std::cout << example_patient.DebugString() << std::endl;

  // Print the patient's name and birthdate.
  // The index 0 is given in the case of repeated fields.
  std::cout << example_patient.name(0).given(0).value() << " "
            << example_patient.name(0).family().value() << " was born on "
            << PrintFhirPrimitive(example_patient.birth_date()).value()
            << "\n\n"
            << std::endl;

  // Uncomment to see the first record converted back to FHIR JSON form
  // const std::string first_record_as_json =
  //     ::google::fhir::PrettyPrintFhirToJsonString(example_patient).value();
  // std::cout << first_record_as_json << std::endl;
}
