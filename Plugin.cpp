/*
	Zachary Weaver
	02/09/2011
	Plugin.cpp
*/

#include "Plugin.h"

using namespace std;
using namespace Dcm;

void Plugin::link(Namespace& heaven){}
Plugin::~Plugin(){}

VectorPlugin::VectorPlugin(){}

VectorPlugin::VectorPlugin(vector<NamedCB> callbacks) {
	cbs = callbacks;
}

void VectorPlugin::link(Namespace& heaven) {
	for (NamedCB ncb : cbs) {
		heaven[ncb.first].push(new DcmPrimFun(ncb.second));
	}
}
