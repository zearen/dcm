// We'll use later if we need 
#ifndef NULL
#define NULL 0
#endif

class NotFoundException {};

template <class K, class V>
class Tree {
    protected:
        struct Node {
            K key;
            V val;
            // true is black
            bool col;
            Node *left, *right;
            Node (Node node, Node *lft, Node *rgt) {
                key = node.key;
                val = node.val;
                col = node.col;
                left = lft;
                right = rgt;
            }
        };
        
        Node *tree;
        
        Tree(Node *newTree) {
            tree = newTree;
        }
        
    public:
        Tree () {
            tree = NULL;
        }
        
        ~Tree () {
            clear();
        }
        
        void insert(K key, V val) {
            Lineage * history;
        }
        
        V lookup(K key) {
            return p_lookup(key, tree);
        }
        
        bool elem(K key) {
            try {
                lookup(key);
                return true;
            }
            catch (NotFoundException) {
                return false;
            }
        }
        
        void remove(K key) {}
        
        void clear() {
            p_clear(tree);
            tree = NULL;
        }
        
        Tree filter(bool (*pred)(K, V)) {
            Tree ret = Tree();
            p_filter(pred, tree, ret);
            return ret;
        }
        
        void map(void (*f)(K, V&)) {
            p_map(f, tree);
        }
        
        Tree copy() {
            return Tree(p_copy(tree));
        }
        
    private:
        //used to keep track of parents on insert
        struct Lineage {
            Node *node;
            Lineage *mother;
            Lineage (Node *n, Lineage *next) {
                node = n;
                mother = next;
            }
        };
        Node *granny(Lineage *parent) {
            if (parent && parent->mother) {
                return parent->mother->node;
            }
            else
                return NULL;
        }
        Node *aunt(Lineage *parent) {
            Node *g = granny(parent);
            if (g) {
                if (parent->node == g->left)
                    return g->right;
                else
                    return g->left;
            }
            else 
                return NULL;
        }
        
        // Begin inner functions
        V p_lookup(K key, Node *tre) {
            if (!tre) 
                throw NotFoundException();
            else if (key == tre->key) 
                return tre->val;
            else if (key < tre->key) 
                return p_lookup(key, tre->left);
            else 
                return p_lookup(key, tre->right);
        }
        
        void p_clear(Node *tre) {
            if (tre){
                p_clear(tre->left);
                p_clear(tre->right);
                delete tre;
            }
        }
        
        void p_filter(bool (*pred)(K, V), Node *tre, Tree& ret) { 
            if (tre) {
                if ((*pred)(tre->key, tre->val))
                    ret.insert(tre->key, tre->val);
                p_filter(pred, tre->left, ret);
                p_filter(pred, tre->right, ret);
            }
        }
        
        void p_map(void (*f)(K, V&), Node *tre) {
            if (tre) {
                (*f)(tre->key, tre->val);
                p_map(f, tre->left);
                p_map(f, tre->right);
            }
        }
        
        Node *p_copy(Node *tre) {
            if (tre)
                return new Node(*tre, p_copy(tre->left, tre->right));
            else return NULL;
        }
    
};
