//===- mlir-blas-cpu-runner.cpp - MLIR CPU Execution Driver + Blas Support ===//
//
// Copyright 2019 The MLIR Authors.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//   http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
// =============================================================================
//
// Main entry point.
//
//===----------------------------------------------------------------------===//

#include "llvm/ADT/StringRef.h"
#include "llvm/Support/DynamicLibrary.h"

#ifdef WITH_LAPACK
#include "lapack/cblas.h"
#else
extern "C" float cblas_sdot(const int N, const float *X, const int incX,
                            const float *Y, const int incY) {
  float res = 0.0f;
  for (int i = 0; i < N; ++i)
    res += X[i * incX] * Y[i * incY];
  return res;
}
#endif

extern int run(int argc, char **argv);

void addSymbols() {
  using llvm::sys::DynamicLibrary;
  DynamicLibrary::AddSymbol("cblas_sdot", (void *)(&cblas_sdot));
}

int main(int argc, char **argv) {
  addSymbols();
  return run(argc, argv);
}
