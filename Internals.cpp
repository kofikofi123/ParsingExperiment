#include "Internals.h"


/*Declarative Enviornment Record definitions*/
std::shared_ptr<CompletionRecord> DeclEnvRecord::CreateMutableBinding(ECMAString* s, ECMABool d){
	if (HasBinding(s)) return std::shared_ptr<CompletionRecord>(); //temp assert or perm
	bindings[s] = createMutableBinding(d);
	return std::shared_ptr<CompletionRecord>(new CompletionRecord(CRecordType::NORMAL, nullptr, nullptr));
}

ECMABool DeclEnvRecord::HasBinding(ECMAString* s){
	return contains(s);
}

/*Not done*/
std::shared_ptr<CompletionRecord> DeclEnvRecord::CreateImmutableBinding(ECMAString* n, ECMABool s){
	return std::shared_ptr<CompletionRecord>();
}

std::shared_ptr<CompletionRecord> DeclEnvRecord::InitializeBinding(ECMAString* s, ECMAValue* v){
	if (!HasBinding(s)) return std::shared_ptr<CompletionRecord>();
	bindingObj* obj = bindings[s];

	if (obj->isInit) return std::shared_ptr<CompletionRecord>();

	obj->value = v;
	obj->isInit = true;

	return std::shared_ptr<CompletionRecord>(new CompletionRecord(CRecordType::NORMAL, nullptr, nullptr));
}

std::shared_ptr<CompletionRecord> DeclEnvRecord::GetBindingValue(ECMAString* n, ECMABool s){
	if (!HasBinding(n)) return nullptr;
	bindingObj* obj = bindings[n];

	if (!obj->isInit){}
	return returnValue(obj->value);
}

ECMABool DeclEnvRecord::DeleteBinding(ECMAString* s){
	if (!HasBinding(s)) return false;

	bindingObj* obj = bindings[s];

	if (!obj->canDelete) return false;

	bindings[s] = nullptr;

	delete obj;

	return true;
}