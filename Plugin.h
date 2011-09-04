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

#include "Callback.h"

// class for easier embedding

class Plugin {
    public:
        void link(unordered_map<string, Callback*>& heaven);
};

class VectorPlugin : public Plugin {
	private:
		vector<Callback*> cbs;
	public:
		VectorPlugin();
		VectorPlugin(vector<Callback*>& callbacks);
        void link(unordered_map<string, Callback*>& heaven);
};
		
#endif
