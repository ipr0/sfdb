package(default_visibility = ["//visibility:public"])

load("@io_bazel_rules_go//go:def.bzl", "go_binary", "go_library")

cc_proto_library(
    name = "data",
    deps = [":data_proto"],
)

proto_library(
    name = "data_proto",
    srcs = ["data.proto"],
)

cc_test(
    name = "big_test",
    size = "medium",
    srcs = ["big_test.cc"],
    deps = [
        "//server:grpc_modules",
        "//server:grpc_sfdb_service_impl",
        "//util/net:port",
        "@com_google_absl//absl/memory",
        "@com_google_absl//absl/strings",
        "@com_google_absl//absl/strings:str_format",
        "@com_google_googletest//:gtest",
        "@com_google_googletest//:gtest_main",
    ],
)

cc_test(
    name = "e2e_test",
    size = "small",
    srcs = ["e2e_test.cc"],
    deps = [
        "//sfdb:flags",
        "//server:grpc_modules",
        "//server:grpc_sfdb_service_impl",
        "//util/net:port",
        "@com_google_absl//absl/flags:flag",
        "@com_google_absl//absl/strings",
        "@com_google_googletest//:gtest",
        "@com_google_googletest//:gtest_main",
    ],
)

go_binary(
    name = "loadtest",
    embed = [":go_default_library"],
    importpath = "github.com/googlegsa/sfdb/sfdb/testing",
)

go_library(
    name = "go_default_library",
    srcs = ["loadtest.go"],
    importpath = "github.com/googlegsa/sfdb/sfdb/testing",
    deps = [
        "//server:grpc_sfdb_service_go_proto",
        "//sfdb:api_go_proto",
        "@com_github_golang_glog//:go_default_library",
        "@org_golang_google_grpc//:go_default_library",
    ],
)
