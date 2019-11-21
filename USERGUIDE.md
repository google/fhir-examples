# FhirProto Users’ Guide

# Introduction

FhirProto is Google’s implementation of the [FHIR Standard](http://hl7.org/fhir/) for Health Care data using [Protocol Buffers](https://developers.google.com/protocol-buffers).  By leveraging Google’s core data storage format, FhirProto provides a type-safe, strongly validated FHIR format with cross-language support at a fraction of the size on disk, making it a great data model to develop your Health Care application on.  Structured Codes and Extensions guarantee that your data will be in the correct format.  Support for generating and validating against custom Implementation Guides allow you to customize FhirProto to your dataset and requirements.  Parsing and Printing libraries make it easy to go back and forth between FhirProto format and JSON.


# Using this Guide

This guide describes how to use the FhirProto github repository at [https://github.com/google/fhir/](https://github.com/google/fhir/).  However, unless you want to actively tweak the implementation of FhirProto, you don’t need to clone this repository, and instead should include it as an external dependency.  To demonstrate how this works, this guide makes use of a separate repository, [https://github.com/google/fhir-example](https://github.com/google/fhir-example).  This repo contains a script for using [Synthea](https://github.com/synthetichealth/synthea) to create a synthetic FHIR JSON dataset, and then shows some examples of parsing, printing, validating, profiling and querying.  Some of these examples are left intentionally incomplete, to leave exercises to go along with this guide.

This guide represents a reference with in-depth descriptions of different concepts, but if you just want to jump in and try working with FHIR, head over to the [Examples](https://github.com/google/fhir-examples/blob/master/EXAMPLES.md) document.


## Setting Up

FhirProto uses [Bazel](https://bazel.build/) as its dependency management/build tool. This is a declarative build system used by Google, Tensorflow, and many others. Installation is pretty simple, but there is one wrinkle: __Tensorflow does not yet support Bazel 1.X, so you need to use a 0.X version.  We recommend [0.29.1](https://github.com/bazelbuild/bazel/releases/tag/0.29.1), the latest 0.X release__.  Other than that, follow the steps [here](https://docs.bazel.build/versions/master/install.html) to download and run the install script. Pro-tip: make sure not to drop the `--user` flag when running the script.  Verify that bazel is installed correctly by running `bazel --version`.

__Important:__ Remember, you cannot use Bazel 1.X yet - if you do, you will get very strange error messages like "This attribute was removed".  You can always check what version of Bazel you have by running `bazel --version`


## Add Proto Generation scripts to your bin

Generating custom profiles and protos makes use of a couple of scripts defined by the FhirProto library. To add these to your `bin`, run

```
curl https://raw.githubusercontent.com/google/fhir/v0.5.0/bazel/generate_protos_utils.sh > ~/bin/generate_protos_utils.sh     && \
  curl https://raw.githubusercontent.com/google/fhir/v0.5.0/bazel/generate_protos.sh > ~/bin/generate_protos.sh && \
  curl https://raw.githubusercontent.com/google/fhir/v0.5.0/bazel/generate_definitions_and_protos.sh > ~/bin/generate_definitions_and_protos.sh && \
  chmod +x ~/bin/generate_protos.sh && chmod +x ~/bin/generate_definitions_and_protos.sh
```


# FHIR on Protocol Buffers

## What is a Protocol Buffer

[Protocol Buffers](https://developers.google.com/protocol-buffers) (or “Protos” for short) are a language-agnostic way of defining strongly-typed, space efficient data structures.  A `.proto` file consists of a definition of a struct.  For example, one might define a (non-FHIR) “Person” data struct like this:

```
message Person {
  string name = 1;
  int32 id = 2;  // Unique ID number for this person.
  string email = 3;

  enum PhoneType {
    MOBILE = 0;
    HOME = 1;
    WORK = 2;
  }

  message PhoneNumber {
    string number = 1;
    PhoneType type = 2;
  }
  repeated PhoneNumber phones = 4;

  .other.package.ExternallyDefinedType other_data = 5;
}
```

It’s beyond the scope of this doc to do an exhaustive explanation on protos.  For that you can take a look at the [Official Guide](https://developers.google.com/protocol-buffers).  But, if you squint at the above definition, most of what’s going on should be fairly obvious.  We define a new struct, or **`message`**, called `Person`, which has a handful of primitive fields, and defines a sub-type `PhoneNumber`, which has two fields, one of which is a locally-defind `enum`.  The final field pulls in the definition of a message defined in a different `.proto` file.  Fields marked as “Repeated” are list-types that can have multiple values

The only thing that might seem strange is the `= 3` at the end of each field.  This is used in serialization.  Instead of storing data as (fieldname -> value), it’s stored as (field number -> value).  In addition to resulting in a big space saving, this also makes the binary representation robust to changes in the name of fields, or the order that they appear in the file.  As long as the field type and number stay the same.

These proto files can then be compiled into structures in nearly any language you might want to use, including C++, Java, JavaScript, Go, Python, etc.  Each language has its own syntax for working with protos, but all languages provide type-safe, programmatic ways to read and write to fields, and to serialize to binary or text formats.


## How FHIR is represented in Protos


### Resources

Take a look at the Proto definition of the [Patient](https://github.com/google/fhir/blob/master/proto/r4/core/resources/patient.proto#L32) message.  The proto fields correspond pretty closely to the fields in the [StructureDefinition defined by FHIR](https://www.hl7.org/fhir/patient.html). It references FHIR primitive types and datatypes defined in [datatypes.proto](https://github.com/google/fhir/blob/master/proto/r4/core/datatypes.proto).



### Primitives and Datatypes

FHIR Primitives are represented in FhirProto with small messages that wrap proto primitive types.  For example, consider the `Boolean` type:

```
message Boolean {
  bool value = 1;

  String id = 2;
  repeated Extension extension = 3;
}
```

This has three fields: the boolean `value` field itself, and two fields that are present on all FHIR elements, `id` and `extensions`.  This pattern of wrapping all primitives as messages is a bit cumbersome, since you end up calling `primitive.value()` a lot, but the trade off is that you can avoid having to deal with the `_field` grammar of FHIR JSON.  Additionally, it allows us to impose more structure than the JSON primitives, such as in Date-like Primitives.


### Date-like Primitives

Representing primitives like DateTime with messages allows us to store the value in a more useful way.  For instance, DateTime in FHIR JSON is represented as a string, like `2015-02-07T13:28:17-05:00`.  This is pretty inconvenient to work with, because any meaningful interaction with it requires parsing first.  In FhirProto, that parsing is done up-front when read from JSON, so it can be represented internally with structure:

```
message DateTime {
  int64 value_us = 1;
  string timezone = 2;

  enum Precision {
    PRECISION_UNSPECIFIED = 0;
    YEAR = 1;
    MONTH = 2;
    DAY = 3;
    SECOND = 4;
    MILLISECOND = 5;
    MICROSECOND = 6;
  }
  Precision precision = 3;

  String id = 4;
  repeated Extension extension = 5;
}

```

The precise moment indicated by the datetime is stored in the first field as microseconds since the UNIX Epoch.  The timezone is parsed into its own field, and the prescision is detected from the date string format, and stored as an enum.  This is sufficient data to fully reconstruct the original primitive string, while being more convenient to work with in code.  Additionally, this makes it a lot harder to make invalid dates, since validity is backed by the structure, rather than a validation regex.


### Codes, CodeSystems and ValueSets

In FHIR JSON, codes are represented as strings, even in places where the code is bound to a set of possible values.  This makes bindings hard to enforce, and makes it easy for incorrect codes to slip in.  FhirProto, on the other hand, can comprehend [Code System](https://github.com/google/fhir/blob/master/proto/r4/core/codes.proto) and [Value Sets](https://github.com/google/fhir/blob/master/proto/r4/core/valuesets.proto) and generate enums for them, for instance:

```
message AdministrativeGenderCode {
  enum Value {
    option (.google.fhir.proto.fhir_code_system_url) =
        "http://hl7.org/fhir/administrative-gender";

    INVALID_UNINITIALIZED = 0;
    MALE = 1;
    FEMALE = 2;
    OTHER = 3;
    UNKNOWN = 4;
  }
}
```

Then, when generating the gender field on Patient, FhirProto detects the binding, and replaces the Code type with an inline-generated Code message that has the value bound to the enum:

```
message Patient {
  message GenderCode {
    option (.google.fhir.proto.structure_definition_kind) = KIND_PRIMITIVE_TYPE;
    option (.google.fhir.proto.fhir_valueset_url) =
        "http://hl7.org/fhir/ValueSet/administrative-gender";
    option (.google.fhir.proto.fhir_profile_base) =
        "http://hl7.org/fhir/StructureDefinition/code";

    AdministrativeGenderCode.Value value = 1;
    String id = 2;
    repeated Extension extension = 3;
  }
  GenderCode gender = 14;
}
```

This means that it is literally impossible to have a patient with an invalid gender code!

**Note:** You may have noticed that the binding on the gender field is to a **ValueSet**, but is inlined with an enum from `codes.proto` rather than `valuesets.proto`.  This is because the FhirProto generator detected that `http://hl7.org/fhir/ValueSet/administrative-gender` is simply all the codes in `http://hl7.org/fhir/administrative-gender`, so it doesn’t bother generating a new enum and just reuses the one in `codes.proto`.  The valuesets defined in `valuesets.proto` are only the ones that don’t identically map to a CodeSystem.


### References

[References](https://github.com/google/fhir/blob/master/proto/r4/core/datatypes.proto#L3281) are represented in proto form as a struct with many fields representing the different kinds of references that are possible in a FHIR reference.  Note that since these are in a proto `oneof`, only one can be populated at any given time.  In addition to the `uri` and `fragment` fields, there is a field for each resource type that the reference could refer to.  For example, when parsing from JSON, a reference of the type `Patient/1234` will have `1234` in the `patient_id` field.

Currently, there is no compile-time guarantee that only valid reference fields are populated - in other words, it’s possible to accidentally populate the `patient_id` field of `[Patient.practitioner](https://github.com/google/fhir/blob/master/proto/r4/core/resources/patient.proto#L203)`, even though it is invalid to refer to a patient in that field.  Instead, validation is handled by a `valid_reference_type` annotation on the field, which will cause incorrect references to get flagged by the Resource Validation APIs.  There is a plan for future versions of FhirProto to generate custom Reference protos per field with only the valid reference type fields.  This will remove the need for validation,  since it will be impossible to populate the incorrect reference field.


## Validation


## Parsing and Printing

Since there is not a one-to-one relationship between FHIR JSON fields and FhirProto fields, we need custom parsers and printers to move between JSON and proto form.  FhirProto library provides these in Java and C++, with Go and Python to come.  One key point to note in all languages is that parsing FHIR JSON to Proto requires a **default timezone**.   This is because unlike JSON FHIR, which stores some timelike primitives without timezones, FHIR Proto represents all timelike primitives as milliseconds since the UNIX Epoch, and so needs a timezone in order to convert back and forth.  For cases where a timezone is not present in the FHIR date string, the default timezone will be used.


### C++ Parsing and Printing API

In C++, parsing and printing is handled by **[json_format.h](https://github.com/google/fhir/blob/master/cc/google/fhir/json_format.h)**.  This contains the following core API functions:



*   **`JsonFhirStringToProto<R>`**: This is the primary function for converting FHIR JSON to FhirProto.  It takes two arguments and a template type:
    *   `const std::string& raw_json` is the JSON string to parse
    *   `const absl::TimeZone default_timezone` is the default timezone to use.
    *   Template type **`<R>`** is the FHIR Resource to parse into.
    *   This function runs the resource validator on the generated proto.  This means it will return a status failure if the JSON string is not valid for the template proto.  This will not only validate field names and primitive types, but also validate FHIR requirements, like fields that must be set according to the given Resource or Profile.
    *   If invoked with a **Profile** proto, this will automatically convert any extensions it finds that were declared in the profile into the correct, typed extension field (See **Profiles** section for more info on profiled extensions).
*   **`JsonFhirStringToProtoWithoutValidating<R>`**: This is identical to `JsonFhirStringToProto` but does not validate for FHIR resource/profile requirements.
    *   **Important:**  This will not check that required fields are set, but it will still fail if JSON data is invalid - e.g., it encounters a field in the JSON that doesn’t have a corresponding field in the Proto, or it encounters a malformed primitive, like `-1` for a `PositiveInt`, or a bound `Code` field with a string that is not in the bound value set.
*   **`PrintFhirToJsonString`**: This converts a FHIR proto to a single-line JSON string, suitable for use as a row of NDJSON.  This has a **`PrettyFhirToJsonString`** variant, that prints multi-line, formatted JSON.
*   **`PrintFhirToJsonStringForAnalytics`**: This converts a FHIR proto to a single-line [Analytic FHIR JSON](https://github.com/FHIR/sql-on-fhir/blob/master/sql-on-fhir.md).  See section **Analytic Printing and Big Query **for more information.  Once again, this has a “pretty print” variant that prints multi-line, formatted JSON.
*   **`PrintFhirPrimitive`**: converts a FHIR primitive to the string representation it would have in FHIR.  E.g., a proto **Date** would be converted to “1905-08-23”.


### C++ Example

The FhirProto-Examples library contains an example of using the C++ parser/printer: [ParsePatients](https://github.com/google/fhir-examples/blob/master/cc/parse_patients.cc).  This example reads all the Patient resources in the ndjson directory of the synthea workspace, and parses them into [Patient](https://github.com/google/fhir/blob/master/proto/r4/core/resources/patient.proto) protos.  To show what this looks like, it then prints the first patient proto to a string.  Finally, it prints an info sentence about the patient, showing what data access looks like.

To run the example, first build with

```
bazel build //cc:ParsePatients
```

This will generate a c++ binary in bazel-bin/.  Assuming you still have your workspace in $WORKSPACE (see **Setting Up an Example Repository**), run using

```
bazel-bin/cc/ParsePatients $WORKSPACE
```

To avoid too much output spam, the last step is commented out, but if you’d like to see an example of the JSON printing API, you can uncomment and rerun.


### Java Parsing and Printing

In Java, parsing and printing is handled by **[JsonFormat.java](https://github.com/google/fhir/blob/master/java/src/main/java/com/google/fhir/common/JsonFormat.java)**, which exposes two classes: **JsonParser** and **JsonPrinter**.



*   **JsonParser#merge** takes a CharSequence (e.g., String), and a proto builder of the desired type.  It then parses


## Analytic Printing and Big Query
Description coming soon, for now see [PrintToAnalyticJson](https://github.com/google/fhir-examples/blob/master/cc/print_to_analytic_ndjson.cc) example.


# Profiles


## Outline

In this section, we will describe what Profiles are, what Profiled Protos look like, and how to use them.  Then we’ll about how to generate new Profiled Protos given a profile set.  Finally, we’ll tie it all together by showing how to generate your own profiles and extensions, and populating data in the new profiled protos.


## Introduction

[Definition in Spec](https://www.hl7.org/fhir/profiling.html)

**Profiles** are a mechanism defined by the FHIR spec that allows organizations to **tailor** FHIR resource definitions to their specific usage and datasets, as well as defining **custom extensions**.  They allow for **custom validations**, as well indicating what extensions/code systems to expect or even require on resources.  For example, the [US-Core spec](https://www.hl7.org/fhir/us/core/index.html) defines the minimum conformance requirements for interoperability and application development in US-based healthcare systems. Some examples from the US-Core spec:



*   **`Patient.name`** has field size **`0..*`** (i.e., no size restrictions, zero-to-infinitely many) in the [main resource definition](http://hl7.org/fhir/patient.html).  However, to conform to the [US-Core profile](https://www.hl7.org/fhir/us/core/StructureDefinition-us-core-patient.html) for patient, the Patient.name field must have size **`1..*`** (i.e., “at least one”).  If a patient has zero names, it will be considered **invalid** by US-Core profile.
*   The US-Core Patient resource specifies three extensions to expect: **`us-core-race`**, **`us-core-ethnicity`**, and **`us-core-birthsex`.  **Each of these is sized to be 0..1 (at most one), and provides the defining url for these extensions.

Profiles like these can be thought of as serving three primary purposes:



1.  **Define** extensions, to create new data structures to attach to resources.  This essentially allows adding entirely new fields to resources, while still being spec-compliant
1.  **Validate** custom requirements
1.  **Publish** these extensions and requirements as a public interoperability API.

**FhirProto** has a deep understanding of profiles and extensions that allows it to generate **custom protos** based off of profiles.  This gives a programmer-friendly interface and compile-time guarantees for things described by profiles.  This guarantees, for example, that data in known extensions is correct the correct type, even for arbitrarily complex extensions.  It is literally impossible to incorrectly fill in extensions!

**Note**: It is an explicit goal of the FhirProto platform to avoid forcing users to have detailed understanding of FHIR StructureDefinitions - that stuff is hard.  Therefore, some of the following discussion will be intentionally vague - the goal is to cover enough to give an intuitive understanding of the concepts of profiling and slicing.  For readers seeking more detail, the FHIR [Profiling](https://www.hl7.org/fhir/profiling.html) and [Extensibility](https://www.hl7.org/fhir/extensibility.html) definitions are great resources.


## Profiled Protos

Given a profile of a resource, FhirProto will generate a customized version of the resource proto based on the profile.  There are three main categories of supported changes: **Size Changes**, **Extension Slicing**, and **Code Slicing.**


### Size Changes

**Size Changes** are the most straight forward.  If a field changes from accepting multiple values (a JSON array) to accepting only a single element, the proto field goes from **repeated** to **singular**.  If the field goes from a lower bound of zero to a lower bound of one, it gains the **validation_requirement =** **REQUIRED_BY_FHIR** annotation.  Thus, for example, the field **Patient.name **is marked as required in the [US-Core proto](https://github.com/google/fhir/blob/33702125b2310140401a6ff29d4a14ffd9181da9/proto/r4/uscore.proto#L4466)


### Extension Slicing

Extensions allow defining custom data that can be added to resources and datatypes.  All extensions consist of a url-payload pair, where the url defines the meaning as well as the format of the payload.  Extensions come in two forms: **Simple **and **Complex**.  In both cases, the FhirProto Generator is capable of generating data structures out of extension definitions.

**Simple** extensions are extensions where the payload consists entirely of a single FHIR datatype, e.g, string, decimal, or code.  Example: the **[US-Core Birthsex](http://hl7.org/fhir/us/core/StructureDefinition/us-core-birthsex)** extension is defined to be a **code**, bound to the [Birthsex ValueSet](http://hl7.org/fhir/us/core/STU3/ValueSet-birthsex.html).  In this case, the [generated extension](https://github.com/google/fhir/blob/master/proto/r4/uscore.proto#L834) has a value field with the appropriate code type.  This is a slightly more complicated case because it uses a code type with a bound value set, meaning that the definition also defines a custom submessage (INSERT LINK TO PREVIOUS DISCUSSION OF CODE HANDLING) - in cases with more straightforward datatypes no submessage is needed.  For example, the [Allergy Intolerance Asserted Date](http://hl7.org/fhir/extension-allergyintolerance-asserteddate.html) extension definition just generates a [proto with a DateTime value field](https://github.com/google/fhir/blob/33702125b2310140401a6ff29d4a14ffd9181da9/proto/r4/core/extensions.proto#L3734).

**Complex** extensions consist of extensions where the value field is empty, but the extension itself has extensions.  In this case, the entire structure is defined by the url in the top-level extension, and in the sub-extension, instead of the url field being the url to a payload definition, it’s a simple field name.  This allows extensions to encode arbitrarily complicated data.  An example of this is the [Questionnaire Constraint](http://hl7.org/fhir/StructureDefinition/questionnaire-constraint) extension, which defines several fields, each of which turn into fields on the [proto definition](https://github.com/google/fhir/blob/33702125b2310140401a6ff29d4a14ffd9181da9/proto/r4/core/extensions.proto#L3794).  Returning to our US-Core patient extension example, we see the [Ethnicity](http://hl7.org/fhir/us/core/StructureDefinition/us-core-ethnicity) extension, which similarly generates a [proto structure](https://github.com/google/fhir/blob/33702125b2310140401a6ff29d4a14ffd9181da9/proto/r4/uscore.proto#L2716) with several fields.  Again, this is a slightly more complicated example, because the bound codings require generating custom data structures, but the principle is the same - we read the Extension definition, and generate a corresponding data structure that is as strongly typed as possible.

Now, we’ve seen what it looks like to generate a proto from an Extension - the next question is how they are used in profiles. The key here is **Extension Slicing**.  This is a mechanism defined by FHIR that allows us to define on a profile what extensions we can expect to see on the resource, along with imposing restrictions on those extensions.  Let’s again return to our example of the [US-Core Patient](https://www.hl7.org/fhir/us/core/StructureDefinition-us-core-patient.html).  We see three slices of extension, two that are complex, and one that is a simple extension of type “code”:



<p id="gdcalert1" ><span style="color: red; font-weight: bold">>>>>  GDC alert: inline image link here (to images/FhirProto-UG0.png). Store image on your image server and adjust path/filename if necessary. </span><br>(<a href="#">Back to top</a>)(<a href="#gdcalert2">Next alert</a>)<br><span style="color: red; font-weight: bold">>>>> </span></p>


![alt_text](images/FhirProto-UG0.png "image_tooltip")


FhirProto can read and understand this, and creates a profile of Patient that turns these into strongly-typed fields on the [US-Core profile of patient](https://github.com/google/fhir/blob/v0.5.0/proto/r4/uscore.proto#L4613).



<p id="gdcalert2" ><span style="color: red; font-weight: bold">>>>>  GDC alert: inline image link here (to images/FhirProto-UG1.png). Store image on your image server and adjust path/filename if necessary. </span><br>(<a href="#">Back to top</a>)(<a href="#gdcalert3">Next alert</a>)<br><span style="color: red; font-weight: bold">>>>> </span></p>


![alt_text](images/FhirProto-UG1.png "image_tooltip")


The two complex extensions have been turned into fields typed as the extension we saw generated above.  The simple extension is inlined directly as the value of the extension - there’s no point in having a “extension type” wrapper around it.  If the extension were string, or decimal, or some other FHIR datatype, the field would be that type instead.

This means we’ll be able to set or get these extensions in nice, programmatic ways, and have guarantees that they will be the correct structure and type.  In the next section, we’ll talk about how to take JSON or proto data that we know to be US-Core, and convert it from a basic FHIR proto to a profiled proto.


## Moving between Profiles

As we saw above, FhirProto turns things like profiled extensions directly into fields.  At this point, it’s worth stressing that this is just a different **view** of the **same data**.  As long as FHIR data fits a given profile, moving between profiles is **isomorphic**, and represents **no gain or loss **of information.  It’s just a change of form that makes the data **easier to work with**.

The “how” here is easy - FhirProto provides a straightforward API for moving between proto representations: **`[ConvertToProfileR4](https://github.com/google/fhir/blob/v0.5.0/cc/google/fhir/r4/profiles.h#L45)`**.  This takes a message in one profile (or base resource), and attempts to convert it to the target profile (or base resource).  If this fails (e.g., an extension is not the correct type, or some validation fails), you’ll get an error status.  It looks like this:

```
google::fhir::ConvertToProfileR4(source_proto, target_proto_ptr);

```

At this point, an example is useful.  **[ProfilePatientsToUsCore](https://github.com/google/fhir-examples/blob/master/cc/profile_patients_to_uscore.cc)** demonstrates the API for converting from one profile to another.  In this case, we read the Patient NDJSON resources into core Patient protos, and convert them to the US-Core profile of Patient.  After converting all the patients, it prints out one as an example.  What you should pay attention to is the last three fields it prints: **race**, **ethnicity**, and **birthsex**.  These have been converted from the cumbersome, untyped key-value Extensions to the strongly-typed structures we talked about above.  This means instead of having to iterate over all extensions, we can say things like

```
patient.race().omb_category().code()
```

as seen in the example.

Note that the API makes no distinction between whether we’re going from a “super type” to a “sub type” or vice versa.  This means, e.g., that you can take a core Patient and convert it to a USCorePatientProfile with the same API call that you could turn a USCorePatientProfile into a core Patient.


## Generating Protos from Profiles

Now we know how to work with already generated Profiled Protos, let’s move a step down the stack: **Generating Profiled Protos** from existing Implementation Guides.  FhirProto defines a concept of a **Fhir Package** that corresponds a package of protos that correspond to an Implementation Guide, or set of profiles.  This is done with four steps:

1.  Define a [PackageInfo](https://github.com/google/fhir/blob/master/proto/profile_config.proto) proto with some metadata about the Fhir Package such as FHIR version, publisher, and package namespace.
1.  Make a `fhir_package` bazel rule with the JSON definitions, and a PackageInfo.  This packages the spec into a form that bazel understands.
1.  Make a `gen_fhir_protos` rule with the `fhir_package` and any `fhir_package`s it depends on
1.  Run `generate_protos.sh` on the `gen_fhir_proto` rule.  See the instructions in **Setting Up** on how to download these scripts and add them to your bin.

This will generate 2 files: a profiles file containing all the profiles and extensions defined by StructureDefinition resources in the package, and a codes file containing all the enums defined by CodeSystem and ValueSet resources in the package

As an example of this, consider the **US Core** protos:

1.  A [US Core PackageInfo](https://github.com/google/fhir/blob/master/spec/uscore_r4_package_info.prototxt)
1.  A [fhir_package](https://github.com/google/fhir/blob/master/spec/BUILD#L81) defines all the inputs for the package as `//spec:uscore_r4_package`
1.  A [gen_fhir_protos](https://github.com/google/fhir/blob/master/proto/r4/BUILD#L22) rule makes a target for the generation script at `//proto/r4:uscore`.  Note that since this package does not depend on any other profiles, we don’t need to declare any dependencies on other fhir packages.
1.  The protos can be regenerated using `generate_protos.sh //proto/r4:uscore`

One more example, the **QI Core** which extends from **US Core:**

1.  A [QI Core PackageInfo](https://github.com/google/fhir/blob/master/spec/qicore_r4_package_info.prototxt)
1.  A [fhir_package](https://github.com/google/fhir/blob/master/spec/BUILD#L101) defines all the inputs for the package
1.  A [gen_fhir_protos](https://github.com/google/fhir/blob/master/proto/r4/qicore/BUILD#L28) rule makes a target for the generation script at `//proto/r4:uscore`.  In this case, `//spec:uscore_r4_package` is declared as a dependency, because the QI Core profiles are sub-profiles of US Core
1.  The protos can be regenerated using `generate_protos.sh //proto/r4:uscore`


## Generating Custom Profiles

Now we’ve seen how to take an existing Implementation Guide, or set of profiles and codes.  There are a lot of existing Implementation Guides like US Core, and the ability to understand them at a deep level is a powerful part of FhirProto.  However, any complex application will want to define their own profiles and extensions.  There are a lot of existing tools for generating Implementation Guides, and anything that generates valid FHIR will work with FhirProto.

However, we wanted to provide an end-to-end solution for this.  FhirProto provides powerful tools for defining profiles and extensions with a minimum of boilerplate, and a generation rule that will generate both JSON StructureDefinitions and FhirProtos.

In FhirProto, you define profiles and extensions by writing protos Profiles and Extensions protos, which are defined in [profile_config.proto](https://github.com/google/fhir/blob/master/proto/profile_config.proto).  The most common way to do this is to define a `profiles.prototxt` file and an `extensions.prototxt` file.  The overall process is actually very similar to the process for generating from profiles, just using slightly different rules.

1.  Like above, define a [PackageInfo](https://github.com/google/fhir/blob/master/proto/profile_config.proto) proto.  This time, however, it should also include metadata like Publisher, and a base url for the structure definitions.
1.  There are no external resources to bundle into a `fhir_package`, so instead we skip directly to the gen rule, in this case `gen_fhir_definitions_and_protos`, because we want to generate JSON definitions in addition to protos.  We pass this rule our  `profiles.prototxt` and `extensions.prototxt` config file, along with any other proto `fhir_package`s this package depends on.  **Note:** this rule also defines a `fhir_package`, so you can use list `gen_fhir_definitions_and_protos` as a dependency for other packages that want to inherit from this.
1.  The protos are then generated using a slightly different command, that points to the `gen_fhir_definitions_and_protos` rule: `generate_definitions_and_protos.sh`

The easiest way to get a sense of how these work is to take a look at the [examples in the fhir-examples repo](https://github.com/google/fhir-examples/tree/master/proto/myprofile), which defines a single profile for Patient.  Looking at `[profiles.prototxt](https://github.com/google/fhir-examples/blob/master/proto/myprofile/profiles.prototxt)`, some things to note:
1) The profile extends from base url `http://hl7.org/fhir/us/core/StructureDefinition/us-core-patient`.  This means it will have all the new slices and requirements outlined by US Core.
2) The profile adds an extension slice called `birthPlace` for extension `http://hl7.org/fhir/StructureDefinition/patient-birthPlace`.  Since this is a core FHIR extension, the proto generator already knows what this looks like.
3) The profile adds a slice to the `CodeableConcept` field `Patient.maritalStatus` for the Code System `http://terminology.hl7.org/CodeSystem/v3-MaritalStatus` with the name `v3`.
4) The profile marks the `Patient.birthDate` as **required**.
5) The profile makes two slices for extensions `https://mycompany.com/fhir/myprofile/Favorites` and `https://mycompany.com/fhir/myprofile/LikesPie`.  These are new extensions, which we define in `extensions.prototxt`.

The `gen_fhir_definitions_and_protos` rule can be found in the [BUILD](https://github.com/google/fhir-examples/blob/master/proto/myprofile/BUILD#L10) file.

## Profiles and Analytic Printing
Description coming soon, for now see [PrintToAnalyticJson](https://github.com/google/fhir-examples/blob/master/cc/print_to_analytic_ndjson.cc) example.

## Machine Learning
Description coming soon
