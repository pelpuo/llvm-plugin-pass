#include "llvm/ADT/Statistic.h"
#include "llvm/Support/Debug.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/BasicBlock.h"

#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Support/raw_ostream.h"

#include "./../include/CountIR.h"

using namespace llvm;

#define DEBUG_TYPE "countir"

STATISTIC(NumOfInst, "Number of instructions.");
STATISTIC(NumOfBB, "Number of basic blocks.");

PreservedAnalyses CountIRPass::run(Function &F, FunctionAnalysisManager &AM) {
  for (auto &BB : F) {
    ++NumOfBB;
    for (auto &I : BB) {
      (void)I;
      ++NumOfInst;
    }
  }
  return PreservedAnalyses::all();
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