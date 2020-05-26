# Examples Guide

This is a guide to the __FhirProto Examples__ in this repository.  For instructions on setting up a working environment, see [README.md](https://github.com/google/fhir-examples/blob/master/README.md).

## C++
### Example 1: Parsing and Printing
The first example is found in [parse_patients.cc](https://github.com/google/fhir-examples/blob/master/cc/parse_patients.cc).  This shows how to parse FhirProtos from an NDJSON file containing FHIR JSON records.  It parses the 1000 patients in the synthea workspace.  It then prints out the first one to proto text, and shows simple manipulation by printing some information about the patient.

To run:
```
bazel build //cc:ParsePatients
bazel-bin/cc/ParsePatient $WORKSPACE
```
Finally, you can uncomment the final lines in this file in order to see how printing a FhirProto to FHIR JSON works.

### Example 2: Profiling to US Core Patient Profile
In Example Two, [profile_patients_to_uscore.cc](https://github.com/google/fhir-examples/blob/master/cc/profile_patients_to_uscore.cc), we add an additional step after parsing: we covert the Patient protos from Core FHIR Patient protos into [protos generated from the US Core profiles](https://github.com/google/fhir/blob/master/proto/r4/uscore.proto).  This will automatically detect extensions defined by the [US Core Profile definitions](https://www.hl7.org/fhir/us/core/StructureDefinition-us-core-patient.html), and convert them in to [strongly-typed fields](https://github.com/google/fhir/blob/master/proto/r4/uscore.proto#L4612) on the UsCore Patient Proto.

To run:
```
bazel build //cc:ProfilePatientsToUsCore
bazel-bin/cc/ProfilePatientsToUsCore $WORKSPACE
```
As before, we print out the first record in its entirety, and then show some direct field access, this time looking at profile field `Patient.race`.  We originally read the JSON into Core FHIR Patients so that we can demonstrate the process of converting the resource - if all we wanted was to get USCore Patient Protos, we could have just parsed directly into the USCore Patient proto.  See Example 3 for a demonstration of this.

In the printout of the proto, notice how the profiled extensions have been turned into messages.  For instance, the extension `http://hl7.org/fhir/us/core/StructureDefinition/us-core-race` from Example One looks like:
```
extension {
  url {
    value: "http://hl7.org/fhir/us/core/StructureDefinition/us-core-ethnicity"
  }
  extension {
    url {
      value: "ombCategory"
    }
    value {
      coding {
        system {
          value: "urn:oid:2.16.840.1.113883.6.238"
        }
        code {
          value: "2186-5"
        }
        display {
          value: "Not Hispanic or Latino"
        }
      }
    }
  }
  extension {
    url {
      value: "text"
    }
    value {
      string_value {
        value: "Not Hispanic or Latino"
      }
    }
  }
}
```
Because the US Core Patient proto contains this as a profiled field, the conversion process will convert this into the [PatientUSCoreRaceExtension](https://github.com/google/fhir/blob/master/proto/r4/uscore.proto#L5126)
```
race {
  omb_category {
    code {
      value: WHITE
    }
    display {
      value: "White"
    }
  }
  text {
    value: "White"
  }
}
```
Note that the `Patient.race.omb_category.code` field is typed as [OmbRaceCategoriesValueSet](https://github.com/google/fhir/blob/master/proto/r4/uscore_codes.proto), meaning it's impossible to have an incorrect code.  As an interesting note, at the time of writing this guide, there are a small number of records that fail to profile - if you scroll to just before the example record prints, you may see something like
```
Patient [...] is invalid for US Core profile: Failed to convert 2131-1 to google.fhir.r4.uscore.OmbRaceCategoriesValueSet.Value: No matching enum found.
```
This is due to a bug in Synthea - it gives some patients the omb category code "2131-1", which corresponds to the [US Core Race code](https://www.hl7.org/fhir/us/core/CodeSystem-cdcrec.html) for "Other Race".  But this code does not exist in the [OMB ValueSet](http://hl7.org/fhir/us/core/ValueSet/omb-race-category)!  This is exactly the kind of bug that is immediately detected by FhirProto.


## Example 3: Custom Profile Generation
In this example, [profile_patients_to_custom_profile.cc](https://github.com/google/fhir-examples/blob/master/cc/profile_patients_to_custom_profile.cc), we will generate our own Extensions and Profiles - imagine it to be the world's smallest Implementation Guide.  There are three config files in [//proto/myprofile]() that define our new profiles:
* [`package_info.prototxt`](https://github.com/google/fhir-examples/blob/master/proto/myprofile/package_info.prototxt) is a [PackageConfig](https://github.com/google/fhir/blob/master/proto/profile_config.proto#L14) that defines some package-wide metadata.
* [`extensions.prototxt`](https://github.com/google/fhir-examples/blob/master/proto/myprofile/extensions.prototxt) is a [Extensions](https://github.com/google/fhir/blob/master/proto/profile_config.proto#L80) proto that defines two new extensions: one simple and one complex.
* [`profiles.prototxt`](https://github.com/google/fhir-examples/blob/master/proto/myprofile/profiles.prototxt)is a [Profiles](https://github.com/google/fhir/blob/master/proto/profile_config.proto#L76) proto that defines a profile for DemoPatient.  This extends from USCore patient, adds several new extension fields, and a slice onto the `CodeableConcept` at `Patient.maritalStatus`.

In the `BUILD`file in that directory, there is a `gen_fhir_definitions_and_protos` named `myprofile`.  This will be the target of our generation script.  To generate the JSON Structure Definitions and FhirProtos, run:
```
generate_protos_from_config.sh //proto/myprofile:myprofile
```
This will generate 3 files:
* `myprofile.json` containing the StructureDefinitions for new profile.  In our case this is just DemoPatient.
* `myprofile_extensions.json` containing the StructureDefinnitions for new extensions.
* `myprofile.proto` containing the newly-created FhirProtos.

DemoPatient adds three new extension fields on top of the US-Core extension fields:
* `birth_place`, defined by the Core FHIR [Birth Place](http://hl7.org/fhir/StructureDefinition/patient-birthPlace) extension, is inlined as an `Address` field.
* `likes_pie`is a boolean extension defined  in `myprofile_extensions.json` as generated from `extensions.prototxt`
* `favorites` is a complex extension also generated from `extensions.prototxt`

Obviously our new extensions don't show up in Synthea data, but Birth Place does, as you can see from the printed proto:
```
birth_place {
  city {
    value: "Westwood"
  }
  state {
    value: "Massachusetts"
  }
  country {
    value: "US"
  }
}
```
The last change is that DemoProfile adds a slice to the CodeableConcept field `Patient.maritalStatus`.  This tells the FhirProto Generator to expect a Coding with the url `http://terminology.hl7.org/CodeSystem/v3-MaritalStatus`.  So, when generating the proto, it will generate a custom  version of codeable concept with an extra field for v3 MaritalStatus, that is strongly typed to the correct code enum.  This makes the maritalStatu field go from
```
marital_status {
  coding {
    system {
      value: "http://terminology.hl7.org/CodeSystem/v3-MaritalStatus"
    }
    code {
      value: "M"
    }
    display {
      value: "M"
    }
  }
  text {
    value: "M"
  }
}
```
to

```
marital_status {
  text {
    value: "M"
  }
  v3 {
    code {
      value: M
    }
    display {
      value: "M"
    }
  }
}
```
This means instead of having to iterate through all the codings to find the v3 code, you can just call `patient.marital_status().v3()`
## Example 4: Validate Patients
This one, [validate_patients.cc](https://github.com/google/fhir-examples/blob/master/cc/validate_patients.cc), is a relatively straight-forward example for demonstrating the ValidateResource api.
##