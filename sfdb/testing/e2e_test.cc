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
#include <string>
#include <vector>

#include "sfdb/modules.h"
#include "sfdb/service_impl.h"
#include "gtest/gtest.h"

namespace sfdb {
namespace {

class E2eTest : public ::testing::Test {
protected:
  void SetUp() override {
    modules_.reset(new Modules);
    modules_->Init();
    service_.reset(new SfdbServiceImpl(modules_.get()));

    server_ = modules_->server_builder()->BuildAndStart();
  }

  void TearDown() override {
    service_ = nullptr;
    modules_ = nullptr;

    server_ = nullptr;
  }

  // Populates rows_ with results.
  void Go(const char *sql) {
    ExecSqlRequest request;
    request.set_sql(sql);
    request.set_include_debug_strings(true);
    ::grpc::ServerContext rpc;
    ExecSqlResponse response;
    service_->ExecSql(&rpc, &request, &response);

    rows_.clear();
    for (int i = 0; i < response.debug_strings_size(); ++i)
      rows_.push_back(response.debug_strings(i));
  }

  std::unique_ptr<Modules> modules_;
  std::unique_ptr<SfdbService::Service> service_;
  std::vector<std::string> rows_;
  std::unique_ptr<::grpc::Server> server_;
};

TEST_F(E2eTest, Basic) {
  Go("CREATE TABLE People (name string, age int64);");
  EXPECT_TRUE(rows_.empty());

  Go("INSERT INTO People (name, age) VALUES ('bob', 69);");
  EXPECT_TRUE(rows_.empty());

  Go("INSERT INTO People (name, age) VALUES ('jon', 44);");
  EXPECT_TRUE(rows_.empty());

  Go("SELECT name FROM People WHERE age % 2 = 0;");
  EXPECT_EQ(1, rows_.size());
  EXPECT_EQ("_1: \"jon\"", rows_[0]);

  Go("UPDATE People SET age = age + 1 WHERE true;");
  EXPECT_TRUE(rows_.empty());

  Go("SELECT name FROM People WHERE age % 2 = 0;");
  EXPECT_EQ(1, rows_.size());
  EXPECT_EQ("_1: \"bob\"", rows_[0]);
}

} // namespace
} // namespace sfdb
