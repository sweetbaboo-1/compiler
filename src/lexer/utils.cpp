#pragma once

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <regex>

#include "types.cpp"

/**
 * Splits a string into tokens without identifying them.
 *
 * for example int main() { return; } would be split into
 * ['int', 'main', '(', ')', '{', 'return', ';', '}']
 */
std::vector<std::string> splitByWord(const std::string &str)
{
  std::vector<std::string> words;
  std::string word;
  bool insideQuotes = false;

  for (size_t i = 0; i < str.length(); ++i)
  {
    char c = str[i];

    // handle quoted strings
    if (c == '"')
    {
      insideQuotes = !insideQuotes;

      if (insideQuotes)
      {
        word += c;
      }
      else
      {
        word += c;
        words.push_back(word);
        word.clear();
      }
    }
    else if (insideQuotes)
    {
      word += c;
    }
    else if (isspace(c))
    {
      if (!word.empty())
      {
        words.push_back(word);
        word.clear();
      }
    }
    else
    {
      word += c;
    }
  }

  if (insideQuotes)
  {
    throw std::runtime_error("Error: Unclosed quote detected");
  }

  if (!word.empty())
  {
    words.push_back(word);
  }

  return words;
}

bool isGrammarSymbol(const std::string &token)
{
  return std::find_if(grammarSymbols.begin(), grammarSymbols.end(),
                      [&token](const Symbol &sym)
                      {
                        return sym.type == token;
                      }) != grammarSymbols.end();
}

std::vector<std::string> splitByGrammar(const std::vector<std::string> &words)
{
  std::vector<std::string> rawTokens;
  std::string currentToken;

  for (const auto &word : words)
  {
    for (auto it = word.begin(); it != word.end(); ++it)
    {
      char ch = *it;

      // check for multi char symbols
      if (it + 1 != word.end())
      {
        std::string twoCharToken(1, ch);
        twoCharToken += *(it + 1);

        if (isGrammarSymbol(twoCharToken))
        {
          if (!currentToken.empty())
          {
            rawTokens.push_back(currentToken);
            currentToken.clear();
          }
          rawTokens.push_back(twoCharToken);
          ++it;
          continue;
        }
      }

      if (isGrammarSymbol(std::string(1, ch)))
      {
        if (!currentToken.empty())
        {
          rawTokens.push_back(currentToken);
          currentToken.clear();
        }
        rawTokens.push_back(std::string(1, ch));
      }
      else
      {
        currentToken += ch;
      }
    }

    if (!currentToken.empty())
    {
      rawTokens.push_back(currentToken);
      currentToken.clear();
    }
  }

  return rawTokens;
}

// ASSUMES that sym is known to be in grammar
std::string getTokenTypeFromGrammar(const std::string sym)
{
  int i = 0;
  while (grammarSymbols[i].type != sym && i < 100 /* obviously something has gone wrong in this case */)
  {
    i++;
  }
  if (i > 99)
  {
    return "ERROR";
  }
  return grammarSymbols[i].representation;
}

bool isKeyword(const std::string word)
{
  for (const auto *keyword : keywords)
  {
    if (keyword == word)
    {
      return true;
    }
  }
  return false;
}

std::string readFile(const std::string &filepath)
{
  std::ifstream file(filepath);

  if (!file.is_open())
  {
    std::cerr << "Error: Could not open file" << std::endl;
    return "";
  }

  std::string line;
  std::string source;

  while (std::getline(file, line))
  {
    source += line;
  }

  file.close();
  return source;
}

void printLines(const std::vector<std::string> lines)
{
  for (const auto &line : lines)
  {
    std::cout << line << std::endl;
  }
}

void printTokens(const std::vector<Token> &tokens)
{
  if (tokens.size() < 1)
  {
    return;
  }
  const int typeWidth = 20;
  const int contentWidth = 100;

  std::cout << std::left
            << std::setw(typeWidth) << "Token Type"
            << std::setw(contentWidth) << "Value"
            << std::endl;

  std::cout << std::string(typeWidth + contentWidth, '-') << std::endl;

  for (const auto &token : tokens)
  {
    std::cout << std::left
              << std::setw(typeWidth) << token.type
              << std::setw(contentWidth) << token.value
              << std::endl;
  }
  std::cout << std::string(typeWidth + contentWidth, '-') << std::endl;
}

LiteralConstant getLiteralType(const std::string &input)
{
  std::regex intRegex(R"(^\d+$)");
  std::regex floatRegex(R"(^[+-]?\d+\.\d+$)");
  std::regex boolRegex(R"(^true|false$)");
  std::regex stringRegex(R"(^\".*\"$)");
  std::regex charRegex(R"(^\'[^\']\'$)");

  if (std::regex_match(input, intRegex))
  {
    return {IntegerLiteral, input};
  }
  else if (std::regex_match(input, floatRegex))
  {
    return {FloatLiteral, input};
  }
  else if (std::regex_match(input, boolRegex))
  {
    return {BooleanLiteral, input};
  }
  else if (std::regex_match(input, stringRegex))
  {
    return {StringLiteral, input};
  }
  else if (std::regex_match(input, charRegex))
  {
    return {CharacterLiteral, input};
  }

  return {Unknown, input};
}