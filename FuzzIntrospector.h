/* Copyright 2021 Fuzz Introspector Authors
Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at
      http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#ifndef _INSPECTOR_INCLUDES_
#define _INSPECTOR_INCLUDES_

// LLVM include
// #include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/PassManager.h"
#include "llvm/Transforms/IPO.h"
#include "llvm/Transforms/Scalar.h"

// Namespace
using namespace std;

namespace llvm {
Pass *createFuzzIntrospectorPass();

enum class ConstraintType {
  EXACT_MATCH,     // 精确匹配类 (CMPLOG 极强)
  ARITHMETIC,      // 算术运算类 (符号执行极强)
  FLOATING_POINT,  // 浮点运算类 (梯度下降极强)
  COMPLEX_BITWISE, // 位操作/哈希类 (集体短板)
  UNKNOWN          // 无法确定的复杂类型
};

class FuzzIntrospectorPass : public PassInfoMixin<FuzzIntrospectorPass> {
public:
  PreservedAnalyses run(Module &M, ModuleAnalysisManager &AM);
};

} // namespace llvm

#endif
