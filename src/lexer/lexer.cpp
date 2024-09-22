#include <string>
#include <vector>

#include "types.cpp"
#include "utils.cpp"

using namespace std;

vector<Token> tokenize(string source)
{
  vector<Token> tokens;
  int lineNumber = 1;
  // doing things line by line to preserve line numbers...
  
    // this will include words like "return;"
    vector<string> words = splitByWord(source);
    // this will resolve all of the grammar splitting. "return;" -> "return", ";"
    vector<string> rawTokens = splitByGrammar(words);

    // create the final tokens
    for (const auto &rawToken : rawTokens)
    {
      Token token;

      // grammar first, then keywords, then literals, then identifiers
      if (isGrammarSymbol(rawToken))
      {
        token.type = getTokenTypeFromGrammar((string)rawToken);
        token.value = rawToken;
        tokens.push_back(token);
        continue;
      }

      token.value = rawToken;

      if (isKeyword(rawToken))
      {
        token.type = rawToken;
        tokens.push_back(token);
        continue;
      }

      // check if it's a int, float, boolean, string, or character literal.
      LiteralConstant lc = getLiteralType(rawToken);
      if (lc.type != Unknown)
      {
        token.type = lc.type;
        token.value = lc.value;
        tokens.push_back(token);
        continue;
      }

      token.type = "identifier";
      tokens.push_back(token);
    }

  return tokens;
}

int main()
{
  const string source = readFile("../../input/example.pc");

  const vector<Token> tokens = tokenize(source);

  printTokens(tokens);
  return 0;
}
