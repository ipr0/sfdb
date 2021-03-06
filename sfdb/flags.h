/*
 * Copyright (c) 2019 Google LLC.
 *
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 *
 */
#ifndef SFDB_FLAGS_H_
#define SFDB_FLAGS_H_

#include <string>

#include "absl/flags/flag.h"
#include "util/types/integral_types.h"

using std::string;

ABSL_DECLARE_FLAG(int32, port);
ABSL_DECLARE_FLAG(string, raft_impl);
ABSL_DECLARE_FLAG(string, raft_my_target);
ABSL_DECLARE_FLAG(string, raft_targets);

// Logging related flags
ABSL_DECLARE_FLAG(int32, log_v);
ABSL_DECLARE_FLAG(bool, log_alsologtostderr);

#endif // SFDB_FLAGS_H_
