#pragma once

#include <stdint.h>
#include <memory>
#include "c-sigprinter.h"
#include "filewriter.h"
#include "../types/message.h"
#include "../types/outfile.h"
#include "fs-creator.h"

class CiMainGenerator {
 public:
  CiMainGenerator();

  void Generate(DbcMessageList_t& dlist, const AppSettings_t& fsd);

 private:

  void Gen_MainHeader();
  void Gen_MainSource();
  void Gen_ConfigHeader();
  void Gen_FMonHeader();
  void Gen_FMonSource();
  void Gen_CanMonUtil();
  void Gen_DbcCodeConf();

  void WriteSigStructField(const SignalDescriptor_t& sig, bool bitfield, size_t pad);

  void WriteUnpackBody(const CiExpr_t* sgs);
  void WritePackStructBody(const CiExpr_t* sgs);
  void WritePackArrayBody(const CiExpr_t* sgs);
  void PrintPackCommonText(const std::string& arrtxt, const CiExpr_t* sgs);

 private:
  std::unique_ptr<CSigPrinter> sigprt;
  std::unique_ptr<FileWriter> fwriter;
  const AppSettings_t* fdesc;
};
