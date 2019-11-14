# Copyright 2019 Google LLC
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

if [[ $# -eq 0 ]] ; then
    echo 'Missing argument: scratch directory'
    exit 1
fi

mkdir -p $1
pushd $1

# build synthea according to https://github.com/synthetichealth/synthea
git clone https://github.com/synthetichealth/synthea.git
cd synthea

# generate 1000 valid R4 bundles in output/fhir/
./run_synthea Massachusetts -p 1000

popd


mkdir $1/bundles
cp $1/synthea/output/fhir/*.json $1/bundles/

mkdir $1/ndjson/
bazel build @com_google_fhir//java:SplitBundle
bazel-bin/external/com_google_fhir/java/SplitBundle $1/ndjson $1/bundles/*.json
