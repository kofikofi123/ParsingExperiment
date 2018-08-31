#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <unicode/unistr.h>
#include <memory>
#include "GC.h"
#include "Stream.h"

#define TOKEN_ISUNIESC (1 << 0)


class JSFactory;

template<class T>
class GCHandle;

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
	JSFactory* factory;
	

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
	bool isHex(void);

	bool inRange(UChar32, UChar32, UChar32);

	uint8_t toHex(UChar32);

	UChar32 rewriteSpecialID(void);

	void createToken(TokenType);

	void emitToken(std::list<TokenPtr>*);
	void emitCharacterToken(std::list<TokenPtr>*, UChar32);
	void appendToken(UChar32);
	//Token* createToken(TokentType, const icu::UnicodeString*);
public:
	Tokenizer(/*JSFactory**/);
	~Tokenizer();

	std::list<TokenPtr>* tokenize(const icu::UnicodeString*);

	//std::list<GCHandle*> errors;
};

struct Token {
	TokenType type;
	icu::UnicodeString* source;
	uint32_t flag;

	Token(TokenType typ): type(typ), source(nullptr){}
	Token(TokenType typ, icu::UnicodeString* buf): type(typ), source(buf){};

	void setFlag(uint32_t);
	void clearFlag(uint32_t);
	uint8_t getFlag(uint32_t);

	//Token& operator+=(UChar32);
	friend std::ostream& operator<<(std::ostream&, const Token&);
	~Token(void){delete source;}
};


enum class TokenType {
	IDENTIFER = 0
};

#endif