#include "llvm/Analysis/MyAnalysis.h"

#include "llvm/IR/PassManager.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Support/raw_ostream.h"

#include <iostream>

using namespace llvm;

AnalysisKey MyAnalysis::Key;

// Definition of the run function of the analysis.
// Here the actual stuff happens!!!
std::string MyAnalysis::run(Function &F, FunctionAnalysisManager &FAM) {

  // This pass just iterates over all instructions in all the
  // basic blocks of the function and appends their opcodes to
  // the output string.

  std::string output;
  for (Function::iterator BB = F.begin(); BB != F.end(); BB++) {
    for (BasicBlock::iterator I = BB->begin(); I != BB->end(); I++) {
      output += I->getOpcodeName();
      output += "\n";
    }
  }
  return output;
}

// Definition of the run function of the analysis pass that
// will be invocable via opt. It uses the getResult<Analysis>()
// method of the FunctionAnalysisManager. This result will be an
// std::string as we have defined the Result of MyAnalysis above.
// The result string is piped into the raw_ostream member
// of the MyAnalysisPass.
PreservedAnalyses MyAnalysisPass::run(Function &F,
                                      FunctionAnalysisManager &FAM) {
  OS << "Printing analysis results of MyAnalysis for function "
     << "'" << F.getName() << "':"
     << "\n";
  OS << FAM.getResult<MyAnalysis>(F);

  // Analysis should never change the LLVM IR code so all
  // results of other analyses are still valid!
  return PreservedAnalyses::all();
}