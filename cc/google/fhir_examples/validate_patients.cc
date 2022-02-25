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

#include <stdlib.h>

#include <fstream>
#include <iostream>
#include <random>
#include <string>
#include <vector>

#include "google/fhir/r4/json_format.h"
#include "google/fhir/r4/resource_validation.h"
#include "google/fhir/status/status.h"
#include "proto/google/fhir/proto/r4/core/resources/patient.pb.h"
#include "proto/google/fhir/proto/r4/uscore.pb.h"
#include "google/fhir_examples/example_utils.h"

using std::string;

using ::google::fhir::r4::uscore::USCorePatientProfile;

// Example code for running resource validation
//
// To run:
// bazel build //cc:ValidatePatients
// bazel-bin/cc/ValidatePatients $WORKSPACE
//
// where $WORKSPACE is the location of a synthea dataset.
// For instructions on setting up your workspace, see the top-level README.md

int main(int argc, char** argv) {
  if (argc == 1) {
    std::cout << "Missing workspace argument." << std::endl;
    return 1;
  }
  const std::string workspace = argv[1];

  // Read all the synthea patients directly into USCore Patient protos.
  std::vector<USCorePatientProfile> patients =
      fhir_examples::ReadNdJsonFile<USCorePatientProfile>(
          absl::StrCat(workspace, "/ndjson/Patient.fhir.ndjson"));

  // All of the elements of patients
  // We know this is true, because if any failed to meet the requirements of
  // that profile, they would have failed the status check in ReadNdJsonFile.
  // To demonstrate validation, let's make some of them invalid.
  std::default_random_engine generator;
  std::uniform_real_distribution<> distribution(0, 1);
  for (USCorePatientProfile& patient : patients) {
    if (distribution(generator) < .05) {
      // Gender is required by US Core
      patient.clear_gender();
    } else if (distribution(generator) < .05) {
      // Set a managingOrganization with a practitioner id.
      // This is invalid, because FHIR requires managingOrganization references
      // to refer to organizations.
      patient.mutable_managing_organization()
          ->mutable_practitioner_id()
          ->set_value("1234");
    } else if (distribution(generator) < .05) {
      // Add a contact with just an id.
      // This violates the FHIRPath for contact, which requires
      // "name.exists() or telecom.exists() or address.exists() or
      // organization.exists()"
      patient.add_contact()->mutable_id()->set_value("9876");
    }
  }

  for (USCorePatientProfile& patient : patients) {
    absl::Status status = google::fhir::r4::ValidateWithoutFhirPath(patient);
    if (!status.ok()) {
      std::cout << "Patient " << patient.identifier(0).value().value()
                << " is invalid: " << status.message() << "\n\n"
                << std::endl;
    }
  }
}
