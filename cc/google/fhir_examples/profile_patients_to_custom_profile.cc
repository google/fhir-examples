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

#include "google/fhir/json_format.h"
#include "google/fhir/r4/profiles.h"
#include "proto/r4/core/datatypes.pb.h"
#include "proto/r4/uscore.pb.h"
#include "google/fhir_examples/example_utils.h"
#include "proto/google/fhir_examples//myprofile/myprofile.pb.h"

using std::string;

using ::fhirexamples::myprofile::DemoPatient;

// Example code for generating and using custom profile sets.
// This uses the "MyProfile" profile set defined in //proto/myprofile
// The Package is defined in package_info.prototxt, and a Patient profile
// is defined in profiles.prototxt
// For instructions on setting up your workspace, see the top-level README.md
//
// To generate the myprofile.proto, run
// generate_definitions_and_protos.sh //proto/myprofile:myprofile
//
// To run:
// bazel build //cc:ProfilePatientsToCustomProfile
// bazel-bin/cc/ProfilePatientsToCustomProfile $WORKSPACE

int main(int argc, char** argv) {
  if (argc == 1) {
    std::cout << "Missing workspace argument." << std::endl;
    return 1;
  }
  const std::string workspace = argv[1];

  const std::vector<DemoPatient> patients =
      fhir_examples::ReadNdJsonFile<DemoPatient>(
          absl::StrCat(workspace, "/ndjson/Patient.fhir.ndjson"));

  const fhirexamples::myprofile::DemoPatient& example_patient =
      patients.front();
  std::cout << example_patient.DebugString() << std::endl;

  std::cout << "\n\n"
            << example_patient.name(0).given(0).value() << " "
            << example_patient.name(0).family().value()
            << " has race: " << example_patient.race().text().value()
            << " and lives in " << example_patient.birth_place().city().value()
            << ", " << example_patient.birth_place().state().value()
            << " and has marital status: "
            << example_patient.marital_status().v3().display().value() << "\n\n"
            << std::endl;
}
