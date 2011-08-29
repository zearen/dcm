/*
    Zachary Weaver
    27/08/2011
    Callback.cpp
*/

// Callback {
    Callback::Callback() {
        scope = NULL;
        name = "";
    }
    
    Callback::Callback(stack<Namespace*> *vars){
        scope = vars;
    }
    
    DcmType *Callback::peek(string& sym) throw (DcmStackError*) {
        stack
    }
    
    DcmType *Callback::pop(string& sym) throw (DcmStackError*) {
    }
    
    void *Callback::push(string& sym, DcmType *item);
// };


