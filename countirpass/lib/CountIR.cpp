#include "llvm/ADT/Statistic.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/Debug.h"

#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Support/raw_ostream.h"

#include "./../include/CountIR.h"

#include <vector>

using namespace llvm;

#define DEBUG_TYPE "countir"

// STATISTIC(NumOfInst, "Number of instructions.");
// STATISTIC(NumOfBB, "Number of basic blocks.");

PreservedAnalyses CountIRPass::run(Function &F, FunctionAnalysisManager &AM) {
    // Ensure the function has at least two basic blocks
    if (F.size() < 2) {
      errs() << "Function does not have enough basic blocks to swap.\n";
      return PreservedAnalyses::all();
    }

    // Get iterators to the first two basic blocks
    auto BBIt = F.begin();
    &*BBIt++;
    BasicBlock *FirstBB = &*BBIt++;
    BasicBlock *SecondBB = &*BBIt;

    // Output the basic block names for clarity
    errs() << "Swapping Basic Blocks: " << FirstBB->getName() << " and " << SecondBB->getName() << "\n";

    // Perform the swap by reordering basic blocks in the parent function
    F.splice(FirstBB->getIterator(), &F, SecondBB->getIterator());
    // F.splice(F.end(), &F, FirstBB->getIterator());

    return PreservedAnalyses::none();
}

// Registering pass
bool PipelineParsingCB(StringRef Name, FunctionPassManager &FPM,
                       ArrayRef<PassBuilder::PipelineElement>) {
  if (Name == "countir") {
    FPM.addPass(CountIRPass());
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

// extern "C" ::llvm::PassPluginLibraryInfo LLVM_ATTRIBUTE_WEAK
// llvmGetPassPluginInfo() {
// return {LLVM_PLUGIN_API_VERSION, "CountIR", "v0.1", [](PassBuilder &PB) {
//     PB.registerPipelineParsingCallback([](StringRef Name, FunctionPassManager
//         &FPM,ArrayRef<PassBuilder::PipelineElement>){
//         if (Name == "countir") {
//             FPM.addPass(CountIRPass());
//             return true;
//         }
//         return false;
//     });
// }};
// }