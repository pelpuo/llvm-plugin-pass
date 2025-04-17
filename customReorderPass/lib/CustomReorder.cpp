#include "llvm/ADT/Statistic.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/Debug.h"

#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Support/raw_ostream.h"

#include "./../include/CustomReorder.h"

#include "llvm/Support/CommandLine.h"

#include <fstream>
#include <sstream>
#include <vector>
#include <map>

using namespace llvm;

#define DEBUG_TYPE "countir"

static cl::opt<std::string> FileName("inputfile", cl::desc("My argument"));

void getOrder(std::map<std::string, int> &blockOrder);

PreservedAnalyses CustomReorderPass::run(Function &F,
                                         FunctionAnalysisManager &AM) {


  // llvm::errs() << "Running CustomReorderPass on function: " << F.getName()
  //             << "\n";
  // llvm::errs() << "Input file: " << FileName.getValue() << "\n"; 
  // llvm::errs() << "Function name: " << F.getName() << "\n";
  // llvm::errs() << "Function size: " << F.size() << "\n";
  
  std::vector<BasicBlock *> BasicBlocks;
  std::map<std::string, int> blockOrder;
  std::map<int, int> blockReorder;
  getOrder(blockOrder);

  int i = 0;

  for (auto &BB : F) {
    BasicBlocks.push_back(&BB);
  }

  if (F.size() < 2) {
    llvm::errs() << "Function does not have enough basic blocks to swap.\n";
    return PreservedAnalyses::all();
  }

  // Get iterators to the first two basic blocks
  auto BBIt = F.begin();
  &*BBIt++;
  BasicBlock *FirstBB = &*BBIt++;
  BasicBlock *SecondBB = &*BBIt;

  // llvm::errs() << "Printing Basic Block Names\n";
  
  // Determine the new position for each block 
  int curPos = 0;
  int newPos = -1;
  for(auto BB: BasicBlocks){
    if(blockOrder.find(static_cast<std::string>(BB->getName())) != blockOrder.end()){
      newPos = blockOrder[static_cast<std::string>(BB->getName())];
      blockReorder[newPos] = curPos;
      // llvm::errs() << BB->getName() << " to " << newPos << "\n";
    }
      curPos++;
  }

  for(int i=0; i< BasicBlocks.size(); i++){
    int x = blockReorder[i];
    F.splice(F.end(), &F, BasicBlocks[x]->getIterator());

    // llvm::errs() << "(" << BasicBlocks[x]->getName() << " from " << x << " to " << i << ")" << "\n";
    // for(auto &BB: F){
    //   llvm::errs() << BB.getName() << ", ";
    // }
    // llvm::errs() << "\n\n";
  }


  return PreservedAnalyses::none();
}


// Extract the order for the basic blocks
void getOrder(std::map<std::string, int> &blockOrder) {
  llvm::errs() << "Reading file: " << FileName.getValue() << "\n";

  // Read the file
  std::ifstream inputFile(FileName.getValue());
  if (!inputFile) {
    llvm::errs() << "Error: Unable to open file: " << FileName.getValue()
                 << "\n";
    return;
  }

  int i = 0;
  // Read the file line by line
  std::string line;
  while (std::getline(inputFile, line)) {
    blockOrder[line] = i;
    i++;
  }
  inputFile.close();
}




// Registering pass
bool PipelineParsingCB(StringRef Name, FunctionPassManager &FPM,
                       ArrayRef<PassBuilder::PipelineElement>) {
  if (Name == "reorder-custom") {
    FPM.addPass(CustomReorderPass());
    return true;
  }
  return false;
}

void RegisterCB(PassBuilder &PB) {
  PB.registerPipelineParsingCallback(PipelineParsingCB);
}

extern "C" ::llvm::PassPluginLibraryInfo LLVM_ATTRIBUTE_WEAK
llvmGetPassPluginInfo() {
  return {LLVM_PLUGIN_API_VERSION, "ReorderCustom", "v0.1", RegisterCB};
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