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

class Plugin;

#include "Callback.h"

// class for easier embedding

class Plugin {
    public:
        virtual void link(Namespace& heaven);
        virtual ~Plugin();
};

class VectorPlugin : public Plugin {
	private:
		vector<Callback*> cbs;
	public:
		VectorPlugin();
		VectorPlugin(vector<Callback*> callbacks);
        virtual void link(Namespace& heaven);
};
		
#endif
