///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// lib_share_helper.h                                                        //
// Copyright (C) Microsoft Corporation. All rights reserved.                 //
// This file is distributed under the University of Illinois Open Source     //
// License. See LICENSE.TXT for details.                                     //
//                                                                           //
// Helper header for lib_shaer.                                              //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <memory>
#include "llvm/ADT/StringRef.h"
#include <vector>
#include <string>
#include <functional>

namespace llvm {
  class Twine;
}

namespace dxcutil {
bool IsAbsoluteOrCurDirRelative(const llvm::Twine &T);
}

namespace libshare {

struct CompileInput {
  std::vector<DxcDefine> &defines;
  std::vector<LPCWSTR> &arguments;
};

class LibCacheManager {
public:
  virtual ~LibCacheManager() {}
  virtual HRESULT AddLibBlob(IDxcBlob *pSource, CompileInput &compiler, size_t &hash,
                     IDxcBlob **pResultLib,
                     std::function<void(void)> compileFn) = 0;
  virtual bool GetLibBlob(IDxcBlob *pSource, CompileInput &compiler, size_t &hash,
                  IDxcBlob **pResultLib) = 0;
  static LibCacheManager &GetLibCacheManager();
  static void ReleaseLibCacheManager();
};

class IncludeToLibPreprocessor {
public:
  virtual ~IncludeToLibPreprocessor() {}

  virtual void SetupDefines(const DxcDefine *pDefines, unsigned defineCount) = 0;
  virtual void AddIncPath(llvm::StringRef path) = 0;
  virtual HRESULT Preprocess(IDxcBlob *pSource, LPCWSTR pFilename) = 0;

  virtual const std::vector<std::string> &GetHeaders() const = 0;
  static std::unique_ptr<IncludeToLibPreprocessor>
  CreateIncludeToLibPreprocessor(IDxcIncludeHandler *handler);
};

}