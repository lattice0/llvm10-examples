#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"
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

Module *makeLLVMModule();

int main(int argc, char **argv)
{
    Module *Mod = makeLLVMModule();
    //verifyFunction(*func, &llvm::outs());
	//verifyModule(*Mod, &llvm::outs());

    ModulePass *m = createPrintModulePass(outs(), "printing module:");
    m->runOnModule(*Mod);

    return 0;
}

Module *makeLLVMModule()
{
    LLVMContext Context;

    Module *module = new Module("test", Context);

    FunctionType *functionType = FunctionType::get(Type::getInt32Ty(Context),
                                             {Type::getInt32Ty(Context),
                                              Type::getInt32Ty(Context),
                                              Type::getInt32Ty(Context)},
                                             false);

    Function *mul_add = Function::Create(functionType, Function::CommonLinkage, "mul_add", module);
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

    FunctionPass* fp = createPrintFunctionPass(outs(), "hello");
    fp->runOnFunction(*mul_add);

    return module;
}
