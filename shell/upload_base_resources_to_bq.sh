# !/bin/bash
# Copyright 2018 Google LLC
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     https://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

# This example uploads the analyic data in $WORKSPACE/analytic to BigQuery
# After running this, you can run run_queries.sh to issue some quieries from the
# command line, or query from the BigQuery console at
# https://console.cloud.google.com/bigquery

if [[ $# -eq 0 ]] ; then
    echo 'Missing argument: scratch directory'
    exit 1
fi

# Note: This example requires a BigQuery project, and the bq cli tool to be
# installed.  See links in the README.md
bq mk fhirexamples

for i in $(basename -a -s.analytic.ndjson $1/analytic/*.analytic.ndjson); do
  echo "Uploading $i..."
  bq load --source_format=NEWLINE_DELIMITED_JSON --schema=$1/analytic/$i.schema.json synthea.$i $1/analytic/$i.analytic.ndjson
done