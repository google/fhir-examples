// Copyright 2020 Google LLC
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

// Package main provides example code for using jsonformat to move back and
// forth from FHIR JSON <--> FHIR Proto.
//
// To run with bazel:
//
// 	bazel run //go/google/fhir_examples:parse_patients $WORKSPACE
//
// To run with native Go:
//
//  go run parse_patients.go $WORKSPACE
//
// where $WORKSPACE is the location of a synthea dataset.
// For instructions on setting up your workspace, see the top-level README.md
package main

import (
	"bufio"
	"bytes"
	"fmt"
	"io/ioutil"
	"log"
	"os"

	"github.com/google/fhir/go/fhirversion"
	"github.com/google/fhir/go/jsonformat"
	r4pb "github.com/google/fhir/go/proto/google/fhir/proto/r4/core/resources/bundle_and_contained_resource_go_proto"
)

const (
	timeZone = "Australia/Sydney"
)

func main() {
	path := os.Args[1] + "/ndjson/Patient.fhir.ndjson"
	testFile, err := ioutil.ReadFile(path)
	if err != nil {
		log.Fatalf("Failed to read test test file %v", err)
	}
	reader := bufio.NewReader(bytes.NewReader(testFile))
	patientJSON, err := reader.ReadBytes('\n')
	if err != nil {
		log.Fatalf("Failed reading patient %v", err)
	}

	um, err := jsonformat.NewUnmarshaller(timeZone, fhirversion.R4)
	if err != nil {
		log.Fatalf("Failed to create unmarshaller %v", err)
	}
	unmarshalled, err := um.Unmarshal(patientJSON)
	if err != nil {
		log.Fatalf("Failed to unmarshal patient %v", err)
	}

	contained := unmarshalled.(*r4pb.ContainedResource)
	patient := contained.GetPatient()
	firstName := patient.GetName()[0].GetGiven()[0].GetValue()
	familyName := patient.GetName()[0].GetFamily().GetValue()
	birthDate := patient.GetBirthDate().GetValueUs()
	fmt.Printf("%s %s was born on %d", firstName, familyName, birthDate)

	// Uncomment to see the first record converted back to FHIR JSON form.
	// m, err := jsonformat.NewPrettyMarshaller(jsonformat.R4)
	// if err != nil {
	// 	 log.Fatalf("Failed to create marshaller %v", err)
	// }
	// marshalled, err := m.Marshal(contained)
	// if err != nil {
	// 	 log.Fatalf("Failed to marshal patient %v", err)
	// }
	// fmt.Printf("Marshalled patient: %s", marshalled)
}
