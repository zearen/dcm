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

namespace Dcm {
class Plugin;
}

#include "Callback.h"

// class for easier embedding
namespace Dcm {
typedef std::pair<std::string, Callback*> NamedCB;

class Plugin {
    public:
        virtual void link(Namespace& heaven);
        virtual ~Plugin();
};

class VectorPlugin : public Plugin {
	private:
	std::vector<NamedCB> cbs;
	public:
		VectorPlugin();
		VectorPlugin(std::vector<NamedCB> callbacks);
        virtual void link(Namespace& heaven);
};
}
#endif
