#include "llvm/IR/PassManager.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Support/raw_ostream.h"

#include <iostream>

namespace llvm {

// This is the actual analysis that will perform some operation
class MyAnalysis : public AnalysisInfoMixin<MyAnalysis> {
  // needed so that AnalysisInfoMixin<MyAnalysis> can access
  // private members of MyAnalysis
  friend AnalysisInfoMixin<MyAnalysis>;

  static AnalysisKey Key;

public:
  // You need to define a result. This can also be some other class.
  using Result = std::string;
  Result run(Function &F, FunctionAnalysisManager &AM);
};

// This is the analysis pass that will be invocable via opt
class MyAnalysisPass : public AnalysisInfoMixin<MyAnalysisPass> {
  raw_ostream &OS;

public:
  explicit MyAnalysisPass(raw_ostream &OS) : OS(OS) {}
  PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM);
};

} // namespace llvm