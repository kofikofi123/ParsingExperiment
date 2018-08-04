#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <unicode/unistr.h>
#include <memory>
#include "Stream.h"

struct Token;
enum class TokenType;

typedef std::shared_ptr<Token> TokenPtr;

class Tokenizer {
	Stream* stream;
	UChar32 currentInput;
	bool reconsume;
	//Token* currentToken;
	TokenPtr currentToken;
	enum class TokenizerStates{
		NORMAL = 0,
		IDENTIFER_START = 1,
		IDENTIFER_PART = 2,
	} currentState;

	void reset(void);
	void next(void);
	void prev(void);
	void skipForward(std::size_t);
	void skipBackward(std::size_t);
	void grabCurrent(void);
	void advance(void);
	void doNothing(void);

	void syntaxError(const char*);

	bool isWhitepace(void);
	bool isIDStart(void);
	bool isIDContinue(void);
	bool isSpecialID(void);

	UChar32 rewriteSpecialID(void);

	void createToken(TokenType);

	void emitToken(std::list<TokenPtr>*);
	void emitCharacterToken(std::list<TokenPtr>*, UChar32);
	void appendToken(UChar32);
	//Token* createToken(TokentType, const icu::UnicodeString*);
public:
	Tokenizer();
	~Tokenizer();

	std::list<TokenPtr>* tokenize(const icu::UnicodeString*);
};

struct Token {
	TokenType type;
	icu::UnicodeString* source;

	Token(TokenType typ): type(typ), source(nullptr){}
	Token(TokenType typ, icu::UnicodeString* buf): type(typ), source(buf){};

	Token& operator+=(UChar32);
	friend std::ostream& operator<<(std::ostream&, const Token&);
	~Token(void){delete source;}
};


enum class TokenType {
	IDENTIFER = 0
};

#endif