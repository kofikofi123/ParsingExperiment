#include <iostream>
#include "Tokenizer.h"
#include "unicode/uchar.h"
#include "unicode/ustream.h"

static const char* _getTokenType(const Token&);
static const char* _tokenTypes[] = {
	"<UNKNOWN>", "<IDENTIFER>"
};

Tokenizer::Tokenizer(){
	stream = new Stream();
}

Tokenizer::~Tokenizer(){
	delete stream;
}

void Tokenizer::reset(void){
	currentState = TokenizerStates::NORMAL;
	reconsume = false;
	if (stream != nullptr){
		stream->reset();
		grabCurrent();
	}
}

std::list<std::shared_ptr<Token>>* Tokenizer::tokenize(const icu::UnicodeString* buffer){
	stream->reload(buffer);

	reset();
	std::list<TokenPtr>* tokenList = new std::list<TokenPtr>(0);

	while (!stream->isFinished()){
		if (currentState == TokenizerStates::NORMAL){

			if (isIDStart()){
				reconsume = true;
				currentState = TokenizerStates::IDENTIFER_START;
				createToken(TokenType::IDENTIFER);
			}else if (isWhitepace()){
				doNothing();
			}else{
				doNothing(); //temp
			}
		}else if (currentState == TokenizerStates::IDENTIFER_START){
			if (isIDStart()){
				appendToken(currentInput);
			}else if (isIDContinue()){
				reconsume = true;
				currentState = TokenizerStates::IDENTIFER_PART;
			}else{
				emitToken(tokenList);
				currentState = TokenizerStates::NORMAL;
				reconsume = true;
			}
			
		}else if (currentState == TokenizerStates::IDENTIFER_PART){
			if (isIDContinue()){
				appendToken(currentInput);
			}else{
				emitToken(tokenList);
				currentState = TokenizerStates::NORMAL;
				reconsume = true;
			}
		}

		if (!reconsume)
			advance();
		else
			reconsume = false;
	}

	return tokenList;
}


void Tokenizer::next(void){
	if (stream == nullptr) return;

	stream->next();
}
void Tokenizer::grabCurrent(void){
	if (stream == nullptr) return;

	currentInput = stream->getCurrent();
}

UChar32 Tokenizer::rewriteSpecialID(void){
	skipForward(2);
	if (currentInput == 0x7B){/*{*/
		advance();
	}

	return 0;
}

bool Tokenizer::isWhitepace(void){
	if (currentInput == 0x09 || currentInput == 0x0B || currentInput == 0x0C || currentInput == 0x20 || currentInput == 0xA0 || currentInput == 0xFEFF)
		return true;

	return u_isblank(currentInput);
}

bool Tokenizer::isIDStart(void){
	if (currentInput == 0x24 || currentInput == 0x5F || currentInput == 0x5C)
		return true;
	return u_isIDStart(currentInput);
}

bool Tokenizer::isIDContinue(void){
	if (currentInput == 0x24 || currentInput == 0x5C || currentInput == 0x200C || currentInput == 0x200D)
		return true;
	return u_hasBinaryProperty(currentInput, UCHAR_ID_CONTINUE);
}

void Tokenizer::advance(void){
	/*possibly add pre mature exit*/
	next();
	grabCurrent();
}

void Tokenizer::skipForward(std::size_t s){
	stream->next(s);
}

void Tokenizer::skipBackward(std::size_t s){
	stream->prev(s);
}

void Tokenizer::doNothing(void){
	advance();
	reconsume = true;
}

void Tokenizer::createToken(TokenType type){
	currentToken.reset(new Token(type));
	currentToken->source = new icu::UnicodeString();
}

void Tokenizer::emitToken(std::list<TokenPtr>* tokenList){
	tokenList->push_back(currentToken);
	currentToken.reset();
}


void Tokenizer::appendToken(UChar32 unit){
	currentToken->source->append(unit);
}

std::ostream& operator<<(std::ostream& stream, const Token& token){
	icu::UnicodeString* str = token.source;
	stream << "{\n\tTokenType: " << _getTokenType(token) << "\n\tToken: \"" << *str << "\"\n}";
	return stream;
}

const char* _getTokenType(const Token& tok){
	TokenType type = tok.type;
	switch(type){
		case TokenType::IDENTIFER:
			return _tokenTypes[1];
			break;
		default:
			return _tokenTypes[0];
			break;
	}
}