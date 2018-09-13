#include <iostream>
#include <unicode/ustream.h>
#include "JSType.h"

static const char* getType(ECMAValueType type){
	switch(type){
		case ECMAValueType::Number:
			return "<NUMBER>";
			break;
		case ECMAValueType::String:
			return "<String>";
			break;
		case ECMAValueType::Boolean:
			return "<Bool>";
			break;
		case ECMAValueType::Object:
			return "<Object>";
			break;
		case ECMAValueType::Symbol:
			return "<Symbol>";
			break;
		case ECMAValueType::Reference:
			return "<Internal_Reference>";
			break;
		default:
			return "<?Unknown?>";
			break;
	}
}

std::ostream& operator<<(std::ostream& stream, ECMAValueType val){
  stream << getType(val);
  return stream;
}
//Will rewrite to fit with GC

static void SetCompletion(ECMACompletionRecord* c, CompletionRecordType t, GCHandle<ECMAValue>* newVal){
	c->type = t;

	GCHandle<ECMAValue>* _c = c->value;

	if (newVal == nullptr)
		_c->empty();
	else
		_c->operator=(*newVal);

}


static void SetNormalCompletion(ECMACompletionRecord* c, GCHandle<ECMAValue>* newVal){
	return SetCompletion(c, CompletionRecordType::Normal, newVal);

}
////////////////

ECMACompletionRecord::ECMACompletionRecord(JSFactory* js){
	value = js->createEmptyHandler();
	target = js->createEmptyHandler();
}


ECMAValue* ECMACompletionRecord::Value() const {
	return value->get();
}


auto ECMADeclEnv::contains(GCHandle<ECMAValue>* N){
	auto start = bindings.begin();
	auto end = bindings.end();

	if (typeof(N->get()) != ECMAValueType::String) return end;

	icu::UnicodeString* temp = ECMAString::Cast(N->get())->Value();
	icu::UnicodeString* temp2 = nullptr;

	for (auto i = start; i != end; i++){
		temp2 = ECMAString::Cast((*i).first->get())->Value(); // :(

		if (*temp == *temp2)
			return i;
	}

	return end;
}

bool ECMADeclEnv::HasBinding(GCHandle<ECMAValue>* N){
	if (contains(N) == bindings.end()) return false;

	return true;
}

void ECMADeclEnv::CreateMutableBinding(GCHandle<ECMAValue>* N, bool D, ECMACompletionRecord* C){
	if (HasBinding(N)){
		return SetNormalCompletion(C, nullptr);
	}

	bindings[factory->createCloneHandle(N)] = new BindingInstance(factory->createEmptyHandler(), true, D, false);


	return SetNormalCompletion(C, nullptr);
}

void ECMADeclEnv::CreateImmutableBinding(GCHandle<ECMAValue>* N, bool S, ECMACompletionRecord* C){
	if (HasBinding(N)){
		return SetNormalCompletion(C, nullptr);
	}

	bindings[factory->createCloneHandle(N)] = new BindingInstance(factory->createEmptyHandler(), false, true, S);


	return SetNormalCompletion(C, nullptr);
}

void ECMADeclEnv::InitalizeBinding(GCHandle<ECMAValue>* N, GCHandle<ECMAValue>* V, ECMACompletionRecord* C){
	auto it = contains(N);

	if (it == bindings.end())
		return SetNormalCompletion(C, nullptr);

	BindingInstance* b = (*it).second;

	if (b->isInit) 
		return SetNormalCompletion(C, nullptr);

	b->isInit = true;
	b->value = factory->createCloneHandle(V);

	return SetNormalCompletion(C, nullptr);
}

void ECMADeclEnv::GetBindingValue(GCHandle<ECMAValue>* N, bool S, ECMACompletionRecord* C){
	auto it = contains(N);

	if (it == bindings.end()) return SetNormalCompletion(C, nullptr);

	BindingInstance* b = (*it).second;

	if (!b->isInit) 
		return SetCompletion(C, CompletionRecordType::Throw, factory->createNumber(32));///for now

	return SetNormalCompletion(C, b->value);
} 

bool ECMADeclEnv::DeleteBinding(GCHandle<ECMAValue>* N){
	auto it = contains(N);

	if (it == bindings.end()) return false;

	bindings.erase(it);

	return true;
}


///////////
///////////
inline ECMAValueType typeof(ECMAValue* v){
	return v->Type();
}

bool typeofIsPrimative(ECMAValue* v){
	if (v == nullptr) return false; //  :(
	bool result = false;

	switch (typeof(v)){
		case ECMAValueType::String:
		case ECMAValueType::Number:
		case ECMAValueType::Boolean:
		case ECMAValueType::Symbol:
			result = true;
			break;
		default:
			break;
	}

	return result;
}


//could do some checkng here
ECMAValue* GetBase(ECMAValue* v){
	return ECMAReference::Cast(v)->base->get();
}

ECMAValue* GetReferencedName(ECMAValue* v){
	return ECMAReference::Cast(v)->name->get();
}

bool IsStrictReference(ECMAValue* v){
	return ECMAReference::Cast(v)->strict;
}

bool HasPrimitiveBase(ECMAValue* v){
	return typeofIsPrimative(GetBase(v));
}

bool IsPropertyReference(ECMAValue* v){
	return (HasPrimitiveBase(v) || typeof(GetBase(v)) == ECMAValueType::Object);
}

bool IsUnresolvableReference(ECMAValue* v){
	return (typeof(GetBase(v)) == ECMAValueType::Undefined);
}

ECMAValue* GetValue(ECMAValue* v){
	if (typeof(v) != ECMAValueType::Reference)
		return v;

	ECMAValue* base = GetBase(v);

	//if unresolved 
	if (IsUnresolvableReference(v)){

	}

	if (IsPropertyReference(v)){
		if (HasPrimitiveBase(v)){
		}
	}

	return nullptr;
}


//////////////
void ECMAObject::setInternalSlot(GCHandle<ECMAValue>* key, GCHandle<ECMAValue>* value){
  internalSlots[key->clone()] = value->clone();
}

GCHandle<ECMAValue>* ECMAObject::getInternalSlot(GCHandle<ECMAValue>* key){
	for (auto i : internalSlots){
		GCHandle<ECMAValue>* handle = i.first;
    ECMAValue* val = handle->get();

		if (val->Value() == val->Value())
			return i.second;
	}
  
  return nullptr;
}