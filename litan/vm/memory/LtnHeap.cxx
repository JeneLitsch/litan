#include "LtnHeap.hxx"

ltn::Heap::Heap() {
	this->nextID = 0;
	this->objects.clear();
}

void ltn::Heap::clear() {
	this->nextID = 0;
	this->objects.clear();
	this->resuseableIDs = {};
}

std::uint64_t ltn::Heap::allocateArray()  {
	std::uint64_t addr;

	// determine id
	// new id
	if(this->resuseableIDs.empty()){
		addr = this->nextID;
		this->nextID++;
	}
	// reuse id
	else{
		addr = this->resuseableIDs.front();
		this->resuseableIDs.pop();
	}

	this->objects.insert({addr, HeapObject(HeapObject::Type::ARRAY)});
	// push pointer
	return addr;
}

void ltn::Heap::destroy(std::uint64_t ptr) {
	if(this->objects.contains(ptr)){
		this->resuseableIDs.push(ptr);
		this->objects.erase(ptr);
	}
	else{
		throw std::runtime_error("Access Violation at id: " + std::to_string(ptr));
	}
}

std::vector<std::uint64_t> & ltn::Heap::accessArray(std::uint64_t ptr) {
	if(this->objects.contains(ptr)){
		return std::get<std::vector<std::uint64_t>>(this->objects.at(ptr).data);
	}
	else{
		throw std::runtime_error("Access Violation at ptr: " + std::to_string(ptr));
	}
}
