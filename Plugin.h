/*
	Zachary Weaver
	02/09/2011
	Plugin.h
*/

#ifndef _PLUGIN_H
#define _PLUGIN_H
#include <string>
#include <vector>
#include <unordered_map>
#include <utility>

class Plugin;

#include "Callback.h"

// class for easier embedding

typedef pair<string, Callback*> NamedCB;

class Plugin {
    public:
        virtual void link(Namespace& heaven);
        virtual ~Plugin();
};

class VectorPlugin : public Plugin {
	private:
		vector<NamedCB> cbs;
	public:
		VectorPlugin();
		VectorPlugin(vector<NamedCB> callbacks);
        virtual void link(Namespace& heaven);
};
		
#endif
