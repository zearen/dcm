/*
	Zachary Weaver
	02/09/2011
	Plugin.cpp
*/

#include "Plugin.h"

void Plugin::link(unordered_map<string, Callback*>& heaven){}

VectorPlugin::VectorPlugin(){}

VectorPlugin::VectorPlugin(vector<Callback>& callbacks) {
	cbs = callbacks;
}

void VectorPlugin::link(unordered_map<string, Callback*>& heaven) {
	for (Callback *cb : cbs) {
		heaven[cb->name] = cb;
	}
}
