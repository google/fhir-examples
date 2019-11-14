/*
 * Copyright 2019 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "absl/strings/str_cat.h"
#include "absl/time/time.h"
#include "google/fhir/json_format.h"
#include "proto/r4/core/resources/patient.pb.h"

namespace fhir_examples {

template <typename R>
std::vector<R> ReadNdJsonFile(const absl::TimeZone& time_zone, std::string filename) {
  std::ifstream read_stream;
  read_stream.open(absl::StrCat(filename));

  std::vector<R> result;
  std::string line;
  while (!read_stream.eof()) {
    std::getline(read_stream, line);
    if (!line.length()) continue;
    result.push_back(
      ::google::fhir::JsonFhirStringToProto<R>(line, time_zone)
          .ValueOrDie());
  }
  return result;
}

}  // namespace fhir_examples