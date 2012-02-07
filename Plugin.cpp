/*
	Zachary Weaver
	02/09/2011
	Plugin.cpp
*/

#include "Plugin.h"

void Plugin::link(Namespace& heaven){}
Plugin::~Plugin(){}

VectorPlugin::VectorPlugin(){}

VectorPlugin::VectorPlugin(vector<Callback*> callbacks) {
	cbs = callbacks;
}

void VectorPlugin::link(Namespace& heaven) {
	for (Callback *cb : cbs) {
		heaven[cb->name].push(new DcmPrimFun(cb));
	}
}
