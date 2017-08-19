/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Microsoft Corporation. All rights reserved.
 *  Licensed under the MIT License. See License.txt in the project root for license information.
 *--------------------------------------------------------------------------------------------*/

#include "worker.h"

Worker::Worker(
    ProcessInfo* process_info,
    uint32_t* process_count,
    Nan::Callback* callback) 
      : AsyncWorker(callback),
        process_count(process_count),
        process_info(process_info) {
}

Worker::~Worker() {
}

void Worker::Execute() {
  GetRawProcessList(process_info, process_count);
}

void Worker::HandleOKCallback() {
  Nan::HandleScope scope;
  // Transfer results into actual result object
  v8::Local<v8::Array> result = Nan::New<v8::Array>(*process_count);
  for (uint32_t i = 0; i < *process_count; i++) {
    v8::Local<v8::Object> object = Nan::New<v8::Object>();
    Nan::Set(object, Nan::New<v8::String>("name").ToLocalChecked(),
              Nan::New<v8::String>(process_info[i].name).ToLocalChecked());
    Nan::Set(object, Nan::New<v8::String>("pid").ToLocalChecked(),
              Nan::New<v8::Number>(process_info[i].pid));
    Nan::Set(object, Nan::New<v8::String>("ppid").ToLocalChecked(),
              Nan::New<v8::Number>(process_info[i].ppid));

    Nan::Set(result, i, Nan::New<v8::Value>(object));
  }
  v8::Local<v8::Value> argv[] = { result };
  callback->Call(1, argv);
}