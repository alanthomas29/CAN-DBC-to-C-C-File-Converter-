#include <stdlib.h>
#include "testapi.h"
#include "parser/dbclineparser.h"
#include "helpers/formatter.h"


TEST(TestSigLineParsing, test1)
{
  DbcLineParser parser;

  const std::string t2 = " SG_ FLT4_TEST_1 : 39|4@0+ (2.01,1E-002) [-0.01|30.14] \"\"  BCM";

  expect_true(parser.IsSignalLine(t2));

  SignalDescriptor_t dsc;

  parser.ParseSignalLine(&dsc, t2);

  expect_true(dsc.IsDoubleSig);
  expect_true(dsc.Offset == 0.01);

  const std::string t3 = " SG_ FLT4_TEST_1 : 39|4@0+ (2, 1) [-0.01|30.14] \"\"  BCM";

  parser.ParseSignalLine(&dsc, t3);

  expect_false(dsc.IsDoubleSig);
  expect_true(dsc.Offset == 1.0);

  // last supported double values
  const std::string t2_ok_norm = " SG_ FLT4_TEST_1 : 39|4@0+ (2, 0.000000001) [-0.01|30.14] \"\"  BCM";
  const std::string t2_ok_scie = " SG_ FLT4_TEST_1 : 39|4@0+ (2, 1e-9) [-0.01|30.14] \"\"  BCM";
  // next values (and less than) are not currently supported
  const std::string t2_nok_norm = " SG_ FLT4_TEST_1 : 39|4@0+ (2, 0.00000000099) [-0.01|30.14] \"\"  BCM";
  const std::string t2_nok_scie = " SG_ FLT4_TEST_1 : 39|4@0+ (2, 1e-10) [-0.01|30.14] \"\"  BCM";
  const std::string t3_nok_scie = " SG_ FLT4_TEST_1 : 39|4@0+ (1e-10, 44) [-0.01|30.14] \"\"  BCM";
  const std::string t3_nok_norm = " SG_ FLT4_TEST_1 : 39|4@0+ (0.00000000099, 2) [-0.01|30.14] \"\"  BCM";

  parser.ParseSignalLine(&dsc, t2_ok_norm);

  expect_true(dsc.IsDoubleSig);
  expect_eq(dsc.Offset, 0.000000001);
  expect_eq(dsc.Factor, 2.0);

  parser.ParseSignalLine(&dsc, t2_ok_scie);

  expect_true(dsc.IsDoubleSig);
  expect_eq(dsc.Offset, 0.000000001);
  expect_eq(dsc.Factor, 2.0);

  parser.ParseSignalLine(&dsc, t3_nok_norm);

  expect_false(dsc.IsDoubleSig);
  expect_eq(dsc.Offset, 0.0);
  expect_eq(dsc.Factor, 1.0);

  parser.ParseSignalLine(&dsc, t3_nok_scie);

  expect_false(dsc.IsDoubleSig);
  expect_eq(dsc.Offset, 0.0);
  expect_eq(dsc.Factor, 1.0);

}

TEST(TestSigLineParsing, test_02)
{
  const std::string t3_ok = " SG_ FLT4_TEST_1 : 39|4@0+ (0.99, 0) [-0.01|30.14] \"\"  BCM";
  const std::string t4_ok = " SG_ FLT4_TEST_1 : 39|4@0+ (0, -0.11) [-0.01|30.14] \"\"  BCM";
  const std::string t5_notok = " SG_ FLT4_TEST_1 : 39|4@0+ (0.00000000099, 0) [-0.01|30.14] \"\"  BCM";
  const std::string t6_ok = " SG_ FLT4_TEST_1 : 39|4@0+ (0, 0.000000000000) [-0.01|30.14] \"\"  BCM";

  DbcLineParser parser;

  SignalDescriptor_t dsc;

  parser.ParseSignalLine(&dsc, t3_ok);

  expect_true(dsc.IsDoubleSig);

  parser.ParseSignalLine(&dsc, t4_ok);

  expect_true(dsc.IsDoubleSig);
  expect_eq(dsc.Factor, 0.0);
  expect_eq(dsc.Offset, -0.11);

  parser.ParseSignalLine(&dsc, t5_notok);

  expect_false(dsc.IsDoubleSig);
  expect_eq(dsc.Factor, 1.0);
  expect_eq(dsc.Offset, 0.0);

  parser.ParseSignalLine(&dsc, t6_ok);

  expect_true(dsc.IsDoubleSig);
  expect_eq(dsc.Factor, 0.0);
  expect_eq(dsc.Offset, 0.0);

}

TEST(TestSigLineParsing, test_prt_double)
{
  constexpr double v = -124.10001110002220;

  expect_eq((std::string)prt_double(v, 0, false), (std::string)"-124");
  expect_eq((std::string)prt_double(v, 1, false), (std::string)"-124.1");
  expect_eq((std::string)prt_double(v, 2, false), (std::string)"-124.1");
  expect_eq((std::string)prt_double(v, 3, false), (std::string)"-124.1");
  expect_eq((std::string)prt_double(v, 4, false), (std::string)"-124.1");
  expect_eq((std::string)prt_double(v, 5, false), (std::string)"-124.10001");
  expect_eq((std::string)prt_double(v, 6, false), (std::string)"-124.100011");
  expect_eq((std::string)prt_double(v, 7, false), (std::string)"-124.1000111");
  expect_eq((std::string)prt_double(v, 8, false), (std::string)"-124.1000111");
  expect_eq((std::string)prt_double(v, 9, false), (std::string)"-124.1000111");

  constexpr double vint = 123.0000;

  expect_eq((std::string)prt_double(vint, 3), (std::string)"123.0");
  expect_eq((std::string)prt_double(vint, 2), (std::string)"123.0");
  expect_eq((std::string)prt_double(vint, 1), (std::string)"123.0");
  expect_eq((std::string)prt_double(vint, 0), (std::string)"123.0");
  expect_eq((std::string)prt_double(vint, 100), (std::string)"123.0");
  expect_eq((std::string)prt_double(vint, 1000), (std::string)"123.0");

  constexpr double v2 = 0.0110022;

  expect_eq((std::string)prt_double(v2, 0), "0.0");
}
