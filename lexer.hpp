#include <string>
class Lexer {
public:
	Lexer(const std::string& source);
	void getNextToken();
private:
	std::string source;
	size_t pos;
};
