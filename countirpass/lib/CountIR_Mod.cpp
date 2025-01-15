#include "llvm/ADT/Statistic.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/Debug.h"

#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Support/raw_ostream.h"

#include "llvm/IR/Module.h"

#include "./../include/CountIR.h"

#include <vector>

using namespace llvm;

#define DEBUG_TYPE "countir"

// PreservedAnalyses CountIRPass::run(Module &M, ModuleAnalysisManager &MAM) {
//   std::vector<Function *> FunctionsToDelete;

//   for(auto &F : M){
//     errs() << F.getName() << "\n";
//   }

//   // Duplicate each function in the module

//   // Delete the original function;

//   return PreservedAnalyses::none(); // Indicate that we modified the IR
// }

PreservedAnalyses CountIRPass::run(Module &M, ModuleAnalysisManager &MAM) {
    // Collect functions to duplicate and delete
    std::vector<Function *> FunctionsToDelete;

    for (Function &F : M) {
        // Skip external declarations (functions without a body)
        if (F.isDeclaration()) {
            continue;
        }

        errs() << "Duplicating function: " << F.getName() << "\n";

        // Create a new function with the same signature
        FunctionType *FT = F.getFunctionType();
        Function *NewF = Function::Create(FT, F.getLinkage(), F.getName() + "_copy", M);

        // Map old arguments to new arguments
        ValueToValueMapTy VMap;
        auto NewArgIter = NewF->arg_begin();
        for (const Argument &Arg : F.args()) {
            VMap[&Arg] = &*NewArgIter++;
        }

        // Deep copy basic blocks and instructions
        for (const BasicBlock &BB : F) {
            BasicBlock *NewBB = BasicBlock::Create(M.getContext(), BB.getName() + "_copy", NewF);
            VMap[&BB] = NewBB;

            for (const Instruction &I : BB) {
                Instruction *NewI = I.clone();
                NewI->insertInto(NewBB, NewBB->end());
                // NewBB->getInstList().push_back(NewI);
                VMap[&I] = NewI;
            }
        }

        // Fix references in the new function
        for (BasicBlock &NewBB : *NewF) {
            for (Instruction &NewI : NewBB) {
                RemapInstruction(&NewI, VMap, RF_NoModuleLevelChanges | RF_IgnoreMissingLocals);
            }
        }

        // Mark the original function for deletion
        FunctionsToDelete.push_back(&F);
    }

    // Delete the original functions
    for (Function *F : FunctionsToDelete) {
        errs() << "Deleting original function: " << F->getName() << "\n";
        F->eraseFromParent();
    }

    return PreservedAnalyses::none(); // Indicate that we modified the IR
}


// Registering pass
bool PipelineParsingCB(StringRef Name, ModulePassManager &MPM,
                       ArrayRef<PassBuilder::PipelineElement>) {
  if (Name == "countir") {
    MPM.addPass(CountIRPass());
    return true;
  }
  return false;
}

void RegisterCB(PassBuilder &PB) {
  PB.registerPipelineParsingCallback(PipelineParsingCB);
}

extern "C" ::llvm::PassPluginLibraryInfo LLVM_ATTRIBUTE_WEAK
llvmGetPassPluginInfo() {
  return {LLVM_PLUGIN_API_VERSION, "CountIR", "v0.1", RegisterCB};
}
