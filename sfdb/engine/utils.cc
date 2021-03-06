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
#include "sfdb/engine/utils.h"

#include <memory>

#include "absl/strings/str_cat.h"
#include "google/protobuf/descriptor.h"
#include "google/protobuf/message.h"
#include "google/protobuf/empty.pb.h"
#include "sfdb/base/vars.h"
#include "sfdb/engine/expressions.h"
#include "sfdb/engine/proto_streams.h"
#include "util/task/canonical_errors.h"

namespace sfdb {
namespace {
using ::absl::StrCat;
using ::util::InternalError;
using ::util::NotFoundError;
using ::util::Status;
using ::util::StatusOr;
using ::util::InvalidArgumentError;
}  // namespace

StatusOr<std::unique_ptr<ProtoStream>> ExecuteDescribeTable(
    const TypedAst &ast, const Db *db) SHARED_LOCKS_REQUIRED(db->mu) {
  auto t = db->FindTable(ast.table_name());
  if (!t)
    return NotFoundError(StrCat("Table ", ast.table_name(),
                                " not found in database ", db->name));

  auto description = db->DescribeTable(ast.table_name());
  return std::unique_ptr<ProtoStream>(new TableProtoStream(description));
}

StatusOr<std::unique_ptr<ProtoStream>> ExecuteShowTables(
    const TypedAst &ast, const Db *db) SHARED_LOCKS_REQUIRED(db->mu) {
  auto scheme = db->GetTableList();
  return std::unique_ptr<ProtoStream>(new TableProtoStream(scheme));
}

Status ExecuteExistsCheck(const TypedAst &ast, const Db *db)
    SHARED_LOCKS_REQUIRED(db->mu) {
  bool must_exist = true;

  if(ast.lhs()) {
    // There may be only OP_NOT lhs child
    CHECK(ast.lhs()->type == Ast::OP_NOT);
    must_exist = !(ast.lhs()->type == Ast::OP_NOT);
  }

  bool object_exists = false;
  if (!ast.index_name().empty()) {
    object_exists = !!db->FindIndex(ast.index_name());
  } else if (!ast.table_name().empty()) {
    object_exists = !!db->FindTable(ast.table_name());
  } else {
    return InvalidArgumentError("EXISTS used in wrong context");
  }

  if (must_exist) {
    return object_exists ? Status::OK : Status::CANCELLED;
  } else {
    return object_exists ? Status::CANCELLED : Status::OK;
  }
}

}  // namespace sfdb
