#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/PassManager.h"
#include "llvm/IR/CallingConv.h"
#include "llvm/IR/Verifier.h"
#include "llvm/IR/IRPrintingPasses.h"
#include "llvm/IR/IRBuilder.h"

using namespace llvm;

Module *makeLLVMModule(LLVMContext& Context);

int main(int argc, char **argv)
{
    LLVMContext Context;
    Module *Mod = makeLLVMModule(Context);
    //Prints the module IR
    ModulePass *m = createPrintModulePass(outs(), "Module IR printer");
    m->runOnModule(*Mod);

    return 0;
}

Module *makeLLVMModule(LLVMContext& Context)
{
    Module *mod = new Module("mul_add", Context);

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

    /*
    //Prints function IR. Not needed as I fixed the module printer.
    FunctionPass* fp = createPrintFunctionPass(outs(), "simple function IR printer");
    fp->runOnFunction(*mul_add);
    */

    return mod;
}
