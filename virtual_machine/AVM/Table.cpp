

#include <cassert>
#include <vector>
#include <functional>
#include "Table.h"
#include "Memcell.h"

using namespace std;
using namespace virtual_machine;

Table::Table() {
    ref_counter = 1;
    numericMap = map<double, Memcell>();
    stringMap = map<std::string, Memcell>();
    tableMap = map<Table*, Memcell>();
    userfuncMap = map<unsigned int, Memcell>();
    libfuncMap = map<std::string, Memcell>();

    nil_pair = make_pair(Memcell(), false);
    true_pair = make_pair(Memcell(), false);
    false_pair = make_pair(Memcell(), false);
}

void Table::increase_ref_counter() {
    ref_counter++;
}

Table* Table::decrease_ref_counter() {
    assert(ref_counter>0);  //If ref_counter becomes 0 then the table must be destroyed

    ref_counter--;
    if(ref_counter == 0) {
        delete this;    //call destructor. For more info, vlepe stack overflow answer: https://stackoverflow.com/a/7039749
        return nullptr;
    } else {
        return this;
    }
}

Table::~Table() {

    //TODO: validate that this works

    //Clear all memcells from all maps
    for(auto it=numericMap.begin(); it!=numericMap.end(); it++)
        it->second.clear(); //key is it->first, value is it->second
    for(auto it=stringMap.begin(); it != stringMap.end(); it++)
        it->second.clear();
    for(auto it=tableMap.begin(); it != tableMap.end(); it++)
        it->second.clear();
    for(auto it=userfuncMap.begin(); it != userfuncMap.end(); it++)
        it->second.clear();
    for(auto it=libfuncMap.begin(); it != libfuncMap.end(); it++)
        it->second.clear();

    //Clear enumerated memcells
    if(nil_pair.second)
        nil_pair.first.clear();
    if(true_pair.second)
        true_pair.first.clear();
    if(false_pair.second)
        false_pair.first.clear();
}




