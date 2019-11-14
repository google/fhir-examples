# Example Custom Profile

This directory contains an example profile set, containing a single profile of Patient, that extends from the [US-Core patient](http://hl7.org/fhir/us/core/StructureDefinition/us-core-patient).

There is currently no .proto file in this directory - that is because generating the proto has been left as an exercise to the reader!  Before you can do this, however, the first step is to ensure that you have the proto-generating scripts in your bin:

```
wget -O ~/bin/generate_protos_utils.sh https://raw.githubusercontent.com/google/fhir/v0.5.0/bazel/generate_protos_utils.sh && \
  wget -O ~/bin/generate_protos.sh https://raw.githubusercontent.com/google/fhir/v0.5.0/bazel/generate_protos.sh && \
  wget -O ~/bin/generate_definitions_and_protos.sh https://raw.githubusercontent.com/google/fhir/v0.5.0/bazel/generate_definitions_and_protos.sh && \
  chmod +x ~/bin/generate_protos.sh && chmod +x ~/bin/generate_definitions_and_protos.sh
```
Now, just invoke the generate_definitions_and_protos.sh script on the `gen_fhir_definitions_and_protos` rule in the BUILD file in this directory:
```
generate_definitions_and_protos.sh //proto:demo
```

You will see 3 files get generated: 
* demo.json
* demo_extensions.json
* demo.proto.

The JSON files contain the newly-defined structure defintions - demo_extensions.json is empty because we haven't defined any extensions.  demo.proto contains the newly-generated DemoPatient proto.  If we'd specified any extensions to generate, there would also be a demo_extensions.proto file.
