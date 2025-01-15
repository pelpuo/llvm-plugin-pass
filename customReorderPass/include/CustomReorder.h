#ifndef COUNTIR_H
#define COUNTIR_H

#include "llvm/IR/PassManager.h"

class CustomReorderPass : public llvm::PassInfoMixin<CustomReorderPass> {
public:
  llvm::PreservedAnalyses run(llvm::Function &F,
                              llvm::FunctionAnalysisManager &AM);

};
#endif