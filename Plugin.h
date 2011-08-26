#include <string>
#include <stack>
#include <unordered_map>

#include "Callback.h"
#include "DcmType/DcmType.h"

// class for easier embedding

class Plugin {
    protected:
        int len;
        struct {string name; Callback cb} *funs;
    public:
        Plugin();
        Plugin(struct {string name; Callback cb} callbacks);
        ~Plugin();
        void link(Namespace &ns);
        void addCallback(string& name, Callback cb);
        void removeCallback(string& name);
        bool hasCallback(string&);
};

