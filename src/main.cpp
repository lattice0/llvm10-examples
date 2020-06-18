#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/raw_ostream.h"

#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"

/*
using namespace llvm;

namespace {
struct Hello : public FunctionPass {
  static char ID;
  Hello() : FunctionPass(ID) {}

  bool runOnFunction(Function &F) override {
    errs() << "Hello: ";
    errs().write_escaped(F.getName()) << '\n';
    return false;
  }
}; // end of struct Hello
}  // end of anonymous namespace

char Hello::ID = 0;
static RegisterPass<Hello> X("hello", "Hello World Pass",
                             false ,
                             false );

static RegisterStandardPasses Y(
    PassManagerBuilder::EP_EarlyAsPossible,
    [](const PassManagerBuilder &Builder,
       legacy::PassManagerBase &PM) { PM.add(new Hello()); });
*/

#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/PassManager.h"
#include "llvm/IR/CallingConv.h"
#include "llvm/IR/Verifier.h"
#include "llvm/IR/IRPrintingPasses.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"
#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/DebugLoc.h"
#include "llvm/IR/Instruction.h"
#include "llvm/ADT/DenseMap.h"
#include "llvm/ADT/FoldingSet.h"
#include "llvm/ADT/SmallPtrSet.h"
#include "llvm/ADT/StringMap.h"
#include "llvm/IR/DebugInfo.h"
#include "llvm/Support/Allocator.h"
#include <iostream>

using namespace llvm;

namespace
{
    struct PrintPass : public ModulePass
    {
        static char ID;
        PrintPass() : ModulePass(ID) {}

        virtual bool runOnModule(Module &M)
        {
            //errs() << "I saw a function called " << F.getName() << "!\n";
            for (Function &F : M.functions())
            {
                std::cout << "f" << std::endl;
                for (BasicBlock &BB : F)
                {
                    for (Instruction &II : BB)
                    {
                        Instruction *I = &II;
                        if (CallInst *CI = dyn_cast<CallInst>(I))
                        {
                            if (DILocation *Loc = I->getDebugLoc())
                            {
                                unsigned Line = Loc->getLine();
                                StringRef File = Loc->getFilename();
                                StringRef Dir = Loc->getDirectory();
                                errs() << Dir << "/" << File << ":" << Line << "\n";
                            }
                        }
                    }
                }
            }
            errs() << "Module passed"
                   << "!\n";
            return false;
        }
    };
} // namespace

char PrintPass::ID = 0;

// Register the pass so `opt -skeleton` runs it.
static RegisterPass<PrintPass> X("printpass", "a pass that just prints the IR");

namespace
{
    struct DummyPass : public FunctionPass
    {
    public:
        static char ID;
        DummyPass() : FunctionPass(ID) {}
        virtual bool runOnFunction(Function &F) override;
    };
} // namespace

bool DummyPass::runOnFunction(Function &F)
{
    errs() << "Visiting function " << F.getName();

    for (BasicBlock &BB : F)
    {
        std::cout << "basicblock" << std::endl;
        for (Instruction &II : BB)
        {
            std::cout << "instruction" << std::endl;
            Instruction *I = &II;
            std::cout << I->getOpcodeName() << std::endl;
            if (CallInst *CI = dyn_cast<CallInst>(I))
            {
                std::cout << "CallInst *CI = dyn_cast<CallInst>(I)" << std::endl;
                if (DILocation *Loc = I->getDebugLoc())
                {
                    unsigned Line = Loc->getLine();
                    StringRef File = Loc->getFilename();
                    StringRef Dir = Loc->getDirectory();
                    errs() << Dir << "/" << File << ":" << Line << "\n";
                }
            }
        }
    }

    return false;
}

// Register the pass with llvm, so that we can call it with dummypass
char DummyPass::ID = 0;
static RegisterPass<DummyPass> X2("dummypass", "Example LLVM pass printing each function it visits");

using namespace llvm;

Module *makeLLVMModule();

int main(int argc, char **argv)
{
    Module *Mod = makeLLVMModule();
    //PrintPass pp;
    ModulePass *m = createPrintModulePass(outs(), "printing module:");
    m->runOnModule(*Mod);
    //pp.runOnModule(*Mod);

    return 0;
}

Module *makeLLVMModule()
{
    // Module Construction
    LLVMContext Context;

    Module *mod = new Module("test", Context);

    FunctionType *FibFTy = FunctionType::get(Type::getInt32Ty(Context),
                                             {Type::getInt32Ty(Context),
                                              Type::getInt32Ty(Context),
                                              Type::getInt32Ty(Context)},
                                             false);

    Function *mul_add = Function::Create(FibFTy, Function::ExternalLinkage, "mul_add", mod);
    mul_add->setCallingConv(CallingConv::C);
    Function::arg_iterator args = mul_add->arg_begin();
    Value *x = args++;
    x->setName("x");
    Value *y = args++;
    y->setName("y");
    Value *z = args++;
    z->setName("z");

    BasicBlock *block = BasicBlock::Create(Context, "entry", mul_add);
    IRBuilder<> builder(block);
    Value *tmp = builder.CreateBinOp(Instruction::Mul, x, y, "tmp");
    Value *tmp2 = builder.CreateBinOp(Instruction::Add, tmp, z, "tmp2");
    builder.CreateRet(tmp2);

    //DummyPass dp;
    //dp.runOnFunction(*mul_add);
    FunctionPass* fp = createPrintFunctionPass(outs(), "hello");
    fp->runOnFunction(*mul_add);

    return mod;
}
