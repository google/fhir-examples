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

package com.google.fhirexamples;

import static java.nio.charset.StandardCharsets.UTF_8;

import com.google.common.io.Files;
import com.google.fhir.common.JsonFormat.Parser;
import com.google.fhir.r4.core.Patient;
import com.google.protobuf.Message;
import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.time.ZoneId;
import java.util.ArrayList;
import java.util.List;

/**
 * Example code for using JsonFormat to move back and forth from 
 * FHIR JSON <--> FHIR Proto
 *
 * To run:
 * bazel build //java:ParsePatients
 * bazel-bin/java/ParsePatient $WORKSPACE
 *
 * where $WORKSPACE is the location of a synthea dataset.
 * For instructions on setting up your workspace, see the top-level README.md
 */
public class ParsePatients {

  public static void main(String[] argv) throws IOException {
    Parser fhirParser = Parser.newBuilder()
        .withDefaultTimeZone(ZoneId.of("America/Los_Angeles"))
        .build();

    String filename = argv[0] + "/ndjson/Patient.fhir.ndjson";
    System.out.println("Reading " + filename);
    BufferedReader reader = new BufferedReader(new FileReader(filename));

    List<Patient> result = new ArrayList<>();
    String line;
    while ((line = reader.readLine()) != null) {
      Patient.Builder patient = Patient.newBuilder();
      fhirParser.merge(line, patient);
      result.add(patient.build());
      System.out.println("Parsed patient " + patient.getId().getValue());
    }

    Patient patient = result.get(0);

    // See the first record in FhirProto form
    System.out.println(patient);

    // Print the patient's name and birthdate.
    // The index 0 is given in the case of repeated fields.
    System.out.println(patient.getName(0).getGiven(0).getValue() + " "
        + patient.getName(0).getFamily().getValue() + " was born on ");
        // << PrintFhirPrimitive(example_patient.birth_date());    
  }
}