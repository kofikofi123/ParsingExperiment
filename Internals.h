#ifndef INTERNALS_H
#define INTERNALS_H

#include <iostream>
#include "JSType.h"
#include "unicode/ustream.h"



enum class CRecordType {NORMAL, BREAK, CONTINUE, RETURN, THROW};

struct CompletionRecord {	
	CRecordType type;
	ECMAValue* value;
	ECMAString* target;

	CompletionRecord(CRecordType s, ECMAValue* v, ECMAString* t): type(s), value(v), target(t){}
};

/*Implict CompletionRecord calls*/
/*
CompletionRecord* returnBoolean(ECMABool);
CompletionRecord* returnString(ECMAString*);
CompletionRecord* returnNumber(ECMANumber);*/
CompletionRecord* returnValue(ECMAValue*);

enum class TBindingValue {LEXICAL, INITALIZE, UNINITALIZE};

class EnvRecord {
protected:
	//std::shared_ptr<CompletionRecord> lastError;

	//void resetLastError(){lastError = std::shared_ptr<CompletionRecord>();}
public:
	EnvRecord(): lastError(std::shared_ptr<CompletionRecord>()){}
	~EnvRecord(){}
	EnvRecord* globalEnv;

	virtual ECMABool HasBinding(ECMAString*)=0;
	virtual std::shared_ptr<CompletionRecord> CreateMutableBinding(ECMAString*, ECMABool)=0;
	virtual std::shared_ptr<CompletionRecord> CreateImmutableBinding(ECMAString*, ECMABool)=0;
	virtual std::shared_ptr<CompletionRecord> InitializeBinding(ECMAString*, ECMAValue*)=0;
	//virtual std::shared_ptr<CompletionRecord> SetMutableBinding(ECMAString*, ECMAValue*, ECMABool);
	virtual std::shared_ptr<CompletionRecord> GetBindingValue(ECMAString*, ECMABool)=0;
	virtual std::shared_ptr<CompletionRecord> DeleteBinding(ECMAString*)=0;
	//virtual ECMABool HasThisBinding();
	//virtual ECMABool HasSuperBinding();
	//virtual ECMAValue* WithBaseObject(); /*Will/Willnot change*/

	//std::shared_ptr<CompletionRecord> getLastError(){return lastError;}
	/*ECMABool errorRaised(){
		return lastError != nullptr;
	}*/
};

class DeclEnvRecord : public EnvRecord {
protected:
	struct bindingObj {
		ECMAValue* value;
		uint8_t type : 1;
		bool isInit;
		bool canDelete;
		bool strict;
	};

	bindingObj* createMutableBinding(bool canDelete){
		bindingObj* b = new bindingObj;

		b->type = 0;
		b->isInit = false;
		b->canDelete = canDelete;
		b->strict = false;

		return b;
	}

	bindingObj* createImmutableBinding(bool strict){
		bindingObj* b = new bindingObj;

		b->type = 1;
		b->isInit = false;
		b->strict = strict;
		b->canDelete = false;

		return b;
	}

	std::map<ECMAString*, bindingObj*> bindings;

	bool contains(ECMAString* str){
		auto it = bindings.find(str);

		return (it != bindings.end());
	}
public:
	DeclEnvRecord(): EnvRecord(){}
	~DeclEnvRecord(){
		for (const auto& b_pairs : bindings){
			delete b_pairs.second;
		}
	}
	ECMABool HasBinding(ECMAString*);

	std::shared_ptr<CompletionRecord> CreateMutableBinding(ECMAString*, ECMABool);

	std::shared_ptr<CompletionRecord> CreateImmutableBinding(ECMAString*, ECMABool);

	std::shared_ptr<CompletionRecord> InitializeBinding(ECMAString*, ECMAValue*);

	std::shared_ptr<CompletionRecord> GetBindingValue(ECMAString*, ECMABool);

	ECMABool DeleteBinding(ECMAString*);
};
//class ObjEnvRecord : public EnvRecord {};

/*class FuncEnvRecord : public DeclEnvRecord {
public:
	ECMAValue* ThisValue;
	TBindingValue ThisBindingStatus;
	ECMAObject* FunctionObject;//Will/Willnot to change
	ECMAValue* HomeObject;
	ECMAValue* NewTarget;
};*/

struct LexicalEnviornment {
	EnvRecord* EnvRec;
	LexicalEnviornment* OuterEnv;
};



#endif