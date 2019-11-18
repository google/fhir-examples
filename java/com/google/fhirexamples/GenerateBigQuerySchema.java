//    Copyright 2018 Google Inc.
//
//    Licensed under the Apache License, Version 2.0 (the "License");
//    you may not use this file except in compliance with the License.
//    You may obtain a copy of the License at
//
//        https://www.apache.org/licenses/LICENSE-2.0
//
//    Unless required by applicable law or agreed to in writing, software
//    distributed under the License is distributed on an "AS IS" BASIS,
//    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//    See the License for the specific language governing permissions and
//    limitations under the License.

package com.google.fhirexamples;

import com.mycompany.fhirexamples.myprofile.DemoPatient;
import com.google.api.client.json.gson.GsonFactory;
import com.google.api.services.bigquery.model.TableSchema;
import com.google.fhir.common.BigQuerySchema;
import com.google.protobuf.Descriptors.Descriptor;
import java.io.File;
import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.nio.file.Paths;
import java.util.HashMap;
import java.util.Map;

/**
 * Utility for generating BigQuery schema from descriptors.
 * Generated schema will be located in $WORKSPACE/analytic/[Resource].schema,json
 */
public class GenerateBigQuerySchema {

  private static final Descriptor[] DESCRIPTORS = {DemoPatient.getDescriptor()};

  public static void main(String[] args) throws IOException {
    for (Descriptor descriptor : DESCRIPTORS) {
      String resourceName = descriptor.getName();
      String filename = Paths.get(args[0], "analytic", resourceName + ".schema.json").toString();

      System.out.println("Generating " + filename + "...");
      TableSchema schema = BigQuerySchema.fromDescriptor(descriptor);

      GsonFactory gsonFactory = new GsonFactory();
      com.google.common.io.Files.asCharSink(
          new File(filename), StandardCharsets.UTF_8)
              .write(gsonFactory.toPrettyString(schema.getFields()));
    }
  }
}