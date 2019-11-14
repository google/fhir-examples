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
#include "absl/time/time.h"
#include "google/fhir/json_format.h"
#include "proto/r4/core/resources/patient.pb.h"
#include "cc/example_utils.h"

using std::string;

using ::google::fhir::r4::core::Patient;
// using ::google::fhir::PrintFhirPrimitive;
using ::google::fhir::JsonFhirStringToProto;

int main(int argc, char** argv) {
  absl::TimeZone time_zone;
  CHECK(absl::LoadTimeZone("America/Los_Angeles", &time_zone));

  std::ifstream read_stream;
  std::cout << "Reading " + absl::StrCat(argv[1]) << std::endl;
  read_stream.open(absl::StrCat(argv[1], "/ndjson/Patient.fhir.ndjson"));

  std::vector<Patient> result;
  std::string line;
  while (!read_stream.eof()) {
    std::getline(read_stream, line);
    if (!line.length()) continue;
    result.push_back(
      JsonFhirStringToProto<Patient>(line, time_zone).ValueOrDie());
    std::cout << "Parsed Patient " + result.back().id().value() << std::endl;
  }

  const Patient& example_patient = result.front();
   
  // See first record in FhirProto form
  std::cout << example_patient.DebugString() << std::endl;

  // Print the patient's name and birthdate.
  // The index 0 is given in the case of repeated fields.
  std::cout << example_patient.name(0).given(0).value() << " "
            << example_patient.name(0).family().value()
            << " was born on ";
            // << PrintFhirPrimitive(example_patient.birth_date());


  // Uncomment to see the first record converted back to FHIR JSON form
  // const std::string first_record_as_json = 
  //     ::google::fhir::PrettyPrintFhirJsonString().ValueOrDie();
  // std::cout << first_record_as_json
}
