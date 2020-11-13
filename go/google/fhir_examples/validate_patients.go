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

// Package main provides example code for running resource validation.
//
// To run:
//
// 	bazel run //go/google/fhir_examples:validate_patients -- $WORKSPACE
//
// where $WORKSPACE is the location of a synthea dataset.
// For instructions on setting up your workspace, see the top-level README.md
package main

import (
	"bufio"
	"bytes"
	"errors"
	"fmt"
	"io"
	"io/ioutil"
	"log"
	"math/rand"
	"os"

	"github.com/google/fhir/go/jsonformat/fhirvalidate"
	"github.com/google/fhir/go/jsonformat"

	d4pb "github.com/google/fhir/go/proto/google/fhir/proto/r4/core/datatypes_go_proto"
	r4pb "github.com/google/fhir/go/proto/google/fhir/proto/r4/core/resources/bundle_and_contained_resource_go_proto"
	p4pb "github.com/google/fhir/go/proto/google/fhir/proto/r4/core/resources/patient_go_proto"
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
	um, err := jsonformat.NewUnmarshaller(timeZone, jsonformat.R4)
	if err != nil {
		log.Fatalf("Failed to create unmarshaller %v", err)
	}

	// Read all the synthea patients directly into Patient protos.
	var patients []*p4pb.Patient
	for i := 0; ; i++ {
		patientJSON, err := reader.ReadBytes('\n')
		if err != nil {
			if !errors.Is(err, io.EOF) {
				log.Fatalf("Failed reading patient %d: %v", i, err)
			}
			break
		}

		unmarshalled, err := um.Unmarshal(patientJSON)
		if err != nil {
			log.Fatalf("Failed reading patient %d: %v", i, err)
		}
		patients = append(patients, unmarshalled.(*r4pb.ContainedResource).GetPatient())
	}

	// All of the elements of patients are valid. We know this is true, because if
	// any failed to meet the FHIR requirements, they would have failed to parse.
	// To demonstrate validation, let's make some of them invalid.
	for _, patient := range patients {
		if rand.Float64() < 0.05 {
			// Patient links have two required fields, "other" and "type".
			patient.Link = []*p4pb.Patient_Link{{}}
		} else if rand.Float64() < 0.05 {
			// Set a managingOrganization with a practitioner id.
			// This is invalid, because FHIR requires managingOrganization references
			// to refer to organizations.
			patient.ManagingOrganization = &d4pb.Reference{
				Reference: &d4pb.Reference_PractitionerId{
					PractitionerId: &d4pb.ReferenceId{Value: "1234"},
				},
			}
		} else if rand.Float64() < 0.05 {
			// Add an extension that contains an invalid OID value. OIDs must conform
			// to a regex.
			patient.Extension = append(patient.Extension, &d4pb.Extension{
				Url: &d4pb.Uri{Value: "https://example.com/invalid-extension"},
				Value: &d4pb.Extension_ValueX{
					Choice: &d4pb.Extension_ValueX_Oid{
						Oid: &d4pb.Oid{Value: "123"},
					},
				},
			})
		}
	}

	for _, patient := range patients {
		err := fhirvalidate.Validate(patient)
		if err != nil {
			fmt.Printf("Patient %s is invalid: %v\n\n", patient.GetIdentifier()[0].GetValue().GetValue(), err)

		}
	}
}
