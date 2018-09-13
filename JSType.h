#ifndef JSTYPES_H
#define JSTYPES_H

#include <map>
#include <list>
#include <memory>
#include <unicode/unistr.h>
#include "GC.h"


//typename icu::UnicodeString* ECMAString;

//decided to model the V8 structure

//could be templated


class GC;
class JSFactory;

template<class T>
class GCHandle;

enum class ECMAValueType {Number, Undefined, String, Boolean, Object, Value, Symbol, Reference, CompletionRecord, EnviornmentRecord};
enum class CompletionRecordType {Normal, Throw};

template <class Container>
struct rangeInterface {
	auto begin(){return container->begin();}
	auto cbegin(){return container->cbegin();}

	auto end(){return container->end();}
	auto cend(){return container->cend();}

	rangeInterface(Container& c): container(&c){}
private:
	Container* container;
};

namespace std {
	template <class Container>
	auto begin(rangeInterface<Container>* v){
		return v->begin();
	}

	template <class Container>
	auto cbegin(const rangeInterface<Container>* v){
		return v->cbegin();
	}

	template <class Container>
	auto end(rangeInterface<Container>* v){
		return v->end();
	}

	template <class Container>
	auto cend(const rangeInterface<Container>* v){
		return v->cend();
	}
};	

class ECMAValue {
public:
	virtual ~ECMAValue(){}
	virtual ECMAValueType Type(){return ECMAValueType::Value;}
	virtual void* Value(){return nullptr;}
};

class ECMANumber : public ECMAValue {
	double val;
public:
	ECMANumber(): ECMANumber(0.0){}
	ECMANumber(double v): val(v){}

	ECMANumber(ECMANumber& v): ECMANumber(v.val){}

	double Value() const {return val;}
	ECMAValueType Type() override {return ECMAValueType::Number;}

	static ECMANumber* Cast(ECMAValue* v){
		return dynamic_cast<ECMANumber*>(v);
	}
};

class ECMABool : public ECMAValue {
	bool val;
public:
	ECMABool(): ECMABool(false){}
	ECMABool(bool v): val(v){}

	ECMABool(ECMABool& v): ECMABool(v.val){}

	bool Value() const {return val;}
	ECMAValueType Type() override {return ECMAValueType::Boolean;}

	static ECMABool* Cast(ECMAValue* v){
		return dynamic_cast<ECMABool*>(v);
	}
};

class ECMAString : public ECMAValue {
	icu::UnicodeString* val;
public:
	ECMAString(): ECMAString((icu::UnicodeString*)nullptr){}
	ECMAString(const char* s): ECMAString(new icu::UnicodeString(s, nullptr)){}
	ECMAString(icu::UnicodeString* s): val(s){}

	~ECMAString(){delete val;}//possible conflicts


	icu::UnicodeString* Value() const {return val;}
	ECMAValueType Type() override {return ECMAValueType::String;}

	static ECMAString* Cast(ECMAValue* v){
		return dynamic_cast<ECMAString*>(v);
	}
};

class ECMAObject : public ECMAValue {
	std::map<GCHandle<ECMAValue>*, GCHandle<ECMAValue>*> internalSlots;
public:
	ECMAObject(){
		internalRange = new rangeInterface<std::map<GCHandle<ECMAValue>*, GCHandle<ECMAValue>*>>(internalSlots);

	}
  
  ~ECMAObject(){
    delete internalRange;
  }


	ECMAObject* Value() const {return nullptr;}
	ECMAValueType Type() override {return ECMAValueType::Object;}

	rangeInterface<std::map<GCHandle<ECMAValue>*, GCHandle<ECMAValue>*>>* internalRange;
	rangeInterface<std::map<GCHandle<ECMAValue>*, GCHandle<ECMAValue>*>>* propertyRange;

	void setInternalSlot(GCHandle<ECMAValue>*, GCHandle<ECMAValue>*);
	GCHandle<ECMAValue>* getInternalSlot(GCHandle<ECMAValue>*);

	static ECMAObject* Cast(ECMAValue* v){
		return dynamic_cast<ECMAObject*>(v);
	}
};

class ECMACompletionRecord : public ECMAValue {
public:

	ECMACompletionRecord(JSFactory*);

	ECMAValue* Value() const;
	ECMAValueType Type() override {return ECMAValueType::CompletionRecord;}


	CompletionRecordType type;
	GCHandle<ECMAValue>* value;
	GCHandle<ECMAValue>* target;

};


class ECMAReference : public ECMAValue {
	GCHandle<ECMAValue>* base;
	GCHandle<ECMAValue>* name;
	bool strict;

	friend ECMAValue* GetBase(ECMAValue*);
	friend ECMAValue* GetReferencedName(ECMAValue*);
	friend bool IsStrictReference(ECMAValue*);
	friend bool HasPrimitiveBase(ECMAValue*);
	friend bool IsPropertyReference(ECMAValue*);
	friend bool IsUnresolvableReference(ECMAValue*);
	friend bool IsSuperReference(ECMAValue*);
	friend ECMAValue* GetValue(ECMAValue*);
public:
	ECMAReference(): base(nullptr), name(nullptr), strict(false){}
	ECMAReference(GCHandle<ECMAValue>* b, GCHandle<ECMAValue>* r): ECMAReference(b, r, false){}
	ECMAReference(GCHandle<ECMAValue>* b, GCHandle<ECMAValue>* r, bool s): base(b), name(r), strict(s){}


	ECMAReference* Value() const {return nullptr;}
	ECMAValueType Type() override {return ECMAValueType::Reference;}

	static ECMAReference* Cast(ECMAValue* v){
		return dynamic_cast<ECMAReference*>(v);
	}
};

class ECMAEnvRecord : public ECMAValue {
protected:
	JSFactory* factory;
public:
	ECMAEnvRecord(JSFactory* f): factory(f){}
	virtual bool HasBinding(GCHandle<ECMAValue>*) = 0;
	virtual void CreateMutableBinding(GCHandle<ECMAValue>*, bool, ECMACompletionRecord*) = 0;
	virtual void CreateImmutableBinding(GCHandle<ECMAValue>*, bool, ECMACompletionRecord*) = 0;
	virtual void InitalizeBinding(GCHandle<ECMAValue>*, GCHandle<ECMAValue>*, ECMACompletionRecord*) = 0;
	virtual void GetBindingValue(GCHandle<ECMAValue>*, bool, ECMACompletionRecord*) = 0;
	virtual bool DeleteBinding(GCHandle<ECMAValue>*) = 0;
};


////

class ECMADeclEnv : public ECMAEnvRecord {
	struct BindingInstance {
		GCHandle<ECMAValue>* value;
		bool canDelete;
		bool isStrict;
		bool isMutable;
		bool isInit; //could be if (value->isAlive())

		BindingInstance(GCHandle<ECMAValue>* v, bool m, bool c, bool i): value(v), canDelete(c), isStrict(i), isMutable(m), isInit(false){}
	};

	std::map<GCHandle<ECMAValue>*, BindingInstance*> bindings;

	auto contains(GCHandle<ECMAValue>*);
public:
	ECMADeclEnv(JSFactory* f): ECMAEnvRecord(f){}
	bool HasBinding(GCHandle<ECMAValue>*) override;
	void CreateMutableBinding(GCHandle<ECMAValue>*, bool, ECMACompletionRecord*) override;
	void CreateImmutableBinding(GCHandle<ECMAValue>*, bool, ECMACompletionRecord*) override;
	void InitalizeBinding(GCHandle<ECMAValue>*, GCHandle<ECMAValue>*, ECMACompletionRecord*) override;
	void GetBindingValue(GCHandle<ECMAValue>*, bool, ECMACompletionRecord*) override;
	bool DeleteBinding(GCHandle<ECMAValue>*) override;
};

////

class ECMALexicalEnviornment : public ECMAValue {

public:
	ECMALexicalEnviornment* outer;

};

ECMAValue* GetBase(ECMAValue*);
ECMAValue* GetReferencedName(ECMAValue*);
bool IsStrictReference(ECMAValue*);
bool HasPrimitiveBase(ECMAValue*);
bool IsPropertyReference(ECMAValue*);
bool IsUnresolvableReference(ECMAValue*);
bool IsSuperReference(ECMAValue*);
ECMAValue* GetValue(ECMAValue*);

ECMAValueType typeof(ECMAValue*);
bool typeofIsPrimative(ECMAValue*);


#endif