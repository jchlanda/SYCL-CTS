/*******************************************************************************
//
//  SYCL 2020 Conformance Test Suite
//
//  Copyright (c) 2023 The Khronos Group Inc.
//
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
//  Unless required by applicable law or agreed to in writing, software
//  distributed under the License is distributed on an "AS IS" BASIS,
//  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//  See the License for the specific language governing permissions and
//  limitations under the License.
//
*******************************************************************************/

#include "../common/common.h"
#include "../common/disabled_for_test_case.h"
#include "type_coverage.h"
#if !SYCL_CTS_COMPILING_WITH_HIPSYCL
#include "group_scan.h"

using ScanTypes = Types;

using HalfType = unnamed_type_pack<sycl::half>;
using HalfExtendedTypes = concatenation<ScanTypes, sycl::half>::type;

static const auto Dims = integer_pack<1, 2, 3>::generate_unnamed();
#endif  // !SYCL_CTS_COMPILING_WITH_HIPSYCL
// FIXME: known_identity is not impemented yet for hipSYCL.
DISABLED_FOR_TEST_CASE(hipSYCL)
("Group and sub-group joint scan functions",
 "[group_func][type_list][fp16][dim]")({
#if defined(SYCL_CTS_COMPILING_WITH_HIPSYCL)
  WARN(
      "hipSYCL cannot handle cases of different types for InPtr and OutPtr. "
      "Skipping such test cases.");
  WARN(
      "hipSYCL joint_exclusive_scan and joint_inclusive_scan cannot process "
      "over several sub-groups simultaneously. Using one sub-group only.");
  WARN("hipSYCL does not support sycl::known_identity_v yet.");
#endif

  auto queue = once_per_unit::get_queue();
  if (queue.get_device().has(sycl::aspect::fp16)) {
    // FIXME: hipSYCL cannot handle cases of different types
#if defined(SYCL_CTS_COMPILING_WITH_HIPSYCL)
    for_all_combinations<invoke_joint_scan_group_same_type>(Dims, HalfType{},
                                                            queue);
#else
    for_all_combinations_with<invoke_joint_scan_group, sycl::half>(
        Dims, HalfExtendedTypes{}, HalfExtendedTypes{}, queue);
#endif
  } else {
    WARN("Device does not support half precision floating point operations.");
  }
});

// FIXME: known_identity is not impemented yet for hipSYCL.
DISABLED_FOR_TEST_CASE(hipSYCL)
("Group and sub-group joint scan functions with init",
 "[group_func][type_list][fp16][dim]")({
#if defined(SYCL_CTS_COMPILING_WITH_HIPSYCL)
  WARN(
      "hipSYCL cannot handle cases of different types for T, *InPtr and "
      "*OutPtr. Skipping such test cases.");
  WARN(
      "hipSYCL joint_exclusive_scan and joint_inclusive_scan with init values "
      "cannot process over several sub-groups simultaneously. Using one "
      "sub-group only.");
#endif

  auto queue = once_per_unit::get_queue();
  if (queue.get_device().has(sycl::aspect::fp16)) {
    // FIXME: hipSYCL cannot handle cases of different types
#if defined(SYCL_CTS_COMPILING_WITH_HIPSYCL)
    for_all_combinations<invoke_init_joint_scan_group_same_type>(
        Dims, HalfType{}, queue);
#else
    for_all_combinations_with<invoke_init_joint_scan_group, sycl::half>(
        Dims, HalfExtendedTypes{}, HalfExtendedTypes{}, HalfExtendedTypes{},
        queue);
#endif
  } else {
    WARN("Device does not support half precision floating point operations.");
  }
});

// FIXME: known_identity is not impemented yet for hipSYCL.
DISABLED_FOR_TEST_CASE(hipSYCL)
("Group and sub-group scan functions", "[group_func][fp16][dim]")({
  auto queue = once_per_unit::get_queue();
  if (queue.get_device().has(sycl::aspect::fp16)) {
    for_all_combinations<invoke_scan_over_group>(Dims, HalfType{}, queue);
  } else {
    WARN("Device does not support half precision floating point operations.");
  }
});

// FIXME: hipSYCL has wrong arguments order for inclusive_scan_over_group: init
// and op are interchanged. known_identity is not impemented yet.
DISABLED_FOR_TEST_CASE(hipSYCL)
("Group and sub-group scan functions with init",
 "[group_func][type_list][fp16][dim]")({
  auto queue = once_per_unit::get_queue();
  if (queue.get_device().has(sycl::aspect::fp16)) {
    for_all_combinations_with<invoke_init_scan_over_group, sycl::half>(
        Dims, HalfExtendedTypes{}, HalfExtendedTypes{}, queue);
  } else {
    WARN("Device does not support half precision floating point operations.");
  }
});
