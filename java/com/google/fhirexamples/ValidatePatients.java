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

import com.google.fhir.common.InvalidFhirException;
import com.google.fhir.common.ResourceValidator;
import com.google.fhir.r4.core.Patient;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

/**
 * Example code for using JsonFormat to move back and forth from FHIR JSON <--> FHIR Proto
 *
 * <p>To run: bazel build //java:ValidatePatients bazel-bin/java/ValidatePatients $WORKSPACE
 *
 * <p>where $WORKSPACE is the location of a synthea dataset. For instructions on setting up your
 * workspace, see the top-level README.md
 */
public class ValidatePatients {

  public static void main(String[] argv) throws IOException {
    List<Patient> patients =
        ExampleUtils.loadNdJson(
            Patient.getDefaultInstance(), argv[0] + "/ndjson/Patient.fhir.ndjson");

    List<Patient> modifiedPatients = new ArrayList<>();
    //  Make some of the patients invalid.
    for (Patient patient : patients) {
      if (Math.random() < .05) {
        Patient.Builder modifiedPatient = patient.toBuilder();
        modifiedPatient
            .getManagingOrganizationBuilder()
            .getPractitionerIdBuilder()
            .setValue("1234");
        modifiedPatients.add(modifiedPatient.build());
      } else {
        modifiedPatients.add(patient);
      }
    }

    ResourceValidator validator = new ResourceValidator();
    for (Patient patient : modifiedPatients) {
      try {
        validator.validateResource(patient);
      } catch (InvalidFhirException e) {
        System.out.println(
            "Patient "
                + patient.getIdentifier(0).getValue().getValue()
                + " is invalid: "
                + e.getMessage());
      }
    }
  }
}
