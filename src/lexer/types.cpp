#pragma once
#include <string>

struct Token
{
  std::string type;
  std::string value;
};

struct Symbol
{
  std::string type;
  std::string representation;
};

struct LiteralConstant
{
  std::string type;
  std::string value;
};

// Initialize grammar symbols and their types
const std::vector<Symbol> grammarSymbols = {
    {";", "semi"},
    {"+", "plus"},
    {"-", "minus"},
    {"*", "mult"},
    {"/", "divide"},
    {"%", "mod"},
    {"<", "lt"},
    {">", "gt"},
    {"(", "l_paren"},
    {")", "r_paren"},
    {"{", "l_bracket"},
    {"}", "r_bracket"},
    {"==", "double_equals"},
    {"<=", "lte"},
    {">=", "gte"},
    {"=", "equals"},
    {"!", "not"},
    {"!=", "not_equals"},
    {"||", "logical_or"},
    {"&&", "logical_and"},
    // {"|", "logical_or"},
    // {"&", "binary_and"}
};

std::string getSymbolType(const std::string &symbol)
{
  for (const auto &sym : grammarSymbols)
  {
    if (sym.representation == symbol)
    {
      return sym.type;
    }
  }
  return "unknown";
}

const char *keywords[] = {"function", "int", "char", "float", "string", "bool", "if", "while", "return", "void"};

const std::string IntegerLiteral = "integerLiteral";
const std::string FloatLiteral = "floatLiteral";
const std::string BooleanLiteral = "booleanLiteral";
const std::string StringLiteral = "stringLiteral";
const std::string CharacterLiteral = "characterLiteral";
const std::string Unknown = "unknown";
