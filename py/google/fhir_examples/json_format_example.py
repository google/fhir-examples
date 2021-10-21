# Lint as: python3
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
"""Example code for using the json_format API to read JSON data to FhirProtos.

To run via bazel:
```
bazel build //py/google/fhir_examples:json_format_example
bazel-bin/py/google/fhir_examples/json_format_example $WORKSPACE
```

To run using python+pip:
```
python3 -m venv venv
source venv/bin/activate
pip install google-fhir

python3 json_format_example.py $WORKSPACE
```

Where $WORKSPACE is the location of a synthea dataset. For instructions on
setting up your workspace, see the top-level README.md.
"""

import random
import re

from absl import app
from proto.google.fhir.proto.r4.core.resources import patient_pb2
from google.fhir.r4 import json_format


def main(argv):
  if len(argv) != 2:
    raise app.UsageError('Should have exactly one argument for workspace')
  workspace = argv[1]

  with open(
      workspace + '/ndjson/Patient.fhir.ndjson', 'r', encoding='utf-8') as f:

    # Read first 200 patient records.
    for i in range(0, 200):
      ndjson_line = f.readline()

      # For 5% of records, insert a spelling error in Patient.gender, which is
      # bound to a value from
      # https://www.hl7.org/fhir/valueset-administrative-gender.html
      # See: http://hl7.org/fhir/patient.html
      if random.random() < .05:
        ndjson_line = re.sub(r'"gender":"((fe)?)(male)"',
                             '"gender":\"\\2mael\"', ndjson_line)

      try:
        patient_proto = json_format.json_fhir_string_to_proto(
            ndjson_line, patient_pb2.Patient)
      except ValueError as e:
        print(f'Failed parsing record {i}: {e}')


if __name__ == '__main__':
  app.run(main)
