#include "llvm/ADT/Statistic.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/Debug.h"

#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Support/raw_ostream.h"

#include "./../include/CountIR.h"

using namespace llvm;

#define DEBUG_TYPE "countir"

// STATISTIC(NumOfInst, "Number of instructions.");
// STATISTIC(NumOfBB, "Number of basic blocks.");

// PreservedAnalyses CountIRPass::run(Function &F, FunctionAnalysisManager &AM) {

//   // Collect basic blocks in a vector
//   std::vector<BasicBlock *> BasicBlocks;
//   for (auto &BB : F) {
//     BasicBlocks.push_back(&BB);
//   }

//   F.deleteBody();

//   LLVMContext &Context = F.getContext();
  
//   // for (auto *BB : llvm::reverse(BasicBlocks)) {
//   //   BasicBlock *newBB = BasicBlock::Create(Context, "new_block", &F, &F.getEntryBlock());
//   //   // Insert corresponding instructions from BB into newBB
//   //   F.splice(F.end(), &F, BB->getIterator());
//   // }

//   for (int i = 0; i <BasicBlocks.size(); i++) {
//     BasicBlock *BB = BasicBlocks[i];
//     // BasicBlock *newBB = BasicBlock::Create(Context, "new_block", &F, &F.getEntryBlock());
//     // Insert corresponding instructions from BB into newBB
//     // F.splice(F.begin(), &F, BB->getIterator());
//     F.insert(F.begin(), BB);
// }
//     // BasicBlock *BB = BasicBlocks[0];
//     // F.insert(F.begin(), BB);
//     // BasicBlock *BB2 = BasicBlocks[BasicBlocks.size()-1];
//     // F.insert(F.end(), BB2);


//   // Validate the function after modifying the order
//   if (!F.empty() && &F.getEntryBlock() != BasicBlocks.back()) {
//     llvm::errs()
//         << "Warning: Entry block was not preserved as the first block\n";
//   }

//   return PreservedAnalyses::all(); // Indicate that we modified the IR
// }

PreservedAnalyses CountIRPass::run(Function &F, FunctionAnalysisManager &AM) {
  // Collect basic blocks in a vector
  std::vector<BasicBlock *> BasicBlocks;
  for (auto &BB : F) {
    BasicBlocks.push_back(&BB);
  }

  F.deleteBody();  // Clear the function body before modifying it

  LLVMContext &Context = F.getContext();
  
  // Rebuild the function body in the desired order
  std::vector<BasicBlock *> ReorderedBasicBlocks;
  
  // Add the entry block first to preserve it at the start
  ReorderedBasicBlocks.push_back(&F.getEntryBlock());

  // Add the other basic blocks in reverse order (skip the entry block)
  for (int i = BasicBlocks.size() - 1; i >= 1; --i) {
    ReorderedBasicBlocks.push_back(BasicBlocks[i]);
  }

  // Insert the reordered basic blocks back into the function
  for (auto *BB : ReorderedBasicBlocks) {
    F.insert(F.begin(), BB);
  }

  // Validate the function after modifying the order
  if (!F.empty() && &F.getEntryBlock() != ReorderedBasicBlocks.front()) {
    llvm::errs() << "Warning: Entry block was not preserved as the first block\n";
  }

  return PreservedAnalyses::all();  // Indicate that we modified the IR
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