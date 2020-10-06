#
# Copyright 2020 Google LLC
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
"""Example code for using the resource_validation API to validate FhirProtos.

To run using bazel:
```
bazel build //py/google/fhir_examples:validation_example
bazel-bin/py/google/fhir_examples:validation_example $WORKSPACE
```

To run using python+pip:
```
python3 -m venv venv
source venv/bin/activate
pip install google-fhir

python3 validation_example.py $WORKSPACE
```

Where $WORKSPACE is the location of a synthea dataset. For instructions on
setting up your workspace, see the top-level README.md.
"""

import random
from absl import app
from proto.google.fhir.proto.r4.core.resources import patient_pb2
from google.fhir import fhir_errors
from google.fhir.r4 import json_format
from google.fhir.r4 import resource_validation


def main(argv):
  if len(argv) != 2:
    raise app.UsageError('Should have exactly one argument for workspace')
  workspace = argv[1]

  with open(
      workspace + '/ndjson/Patient.fhir.ndjson', 'r', encoding='utf-8') as f:

    patients = []

    # Read first 200 patient records.
    for i in range(0, 200):
      ndjson_line = f.readline()

      try:
        patient_proto = json_format.json_fhir_string_to_proto(
            ndjson_line, patient_pb2.Patient)
        patients.append(patient_proto)
      except ValueError as e:
        print(f'Failed parsing record {i}: {e}')

    for i, patient in enumerate(patients):
      # Insert a validation error into 5% of the records.
      # Patient.communication is an optional BackboneElement field, but if one
      # is added, it *MUST* contain a populated Patient.communication.language
      # field.
      # See: http://hl7.org/fhir/patient.html
      communication = patient.communication.add()
      communication.preferred.value = True

      # Add the required language field to 95% of records
      if random.random() < .95:
        language = communication.language.coding.add()
        language.system.value = 'http://hl7.org/fhir/valueset-all-languages.html'
        language.code.value = 'fr'

      try:
        resource_validation.validate_resource(patient)
      except fhir_errors.InvalidFhirError as e:
        print(f'Failed validating record {i}: {e}')


if __name__ == '__main__':
  app.run(main)
