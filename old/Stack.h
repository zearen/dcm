#ifndef NULL
#define NULL 0
#endif

class StackEmptyException {};

template <class V>
class Stack {
    protected:
        struct Node {
            V val;
            Node *next;
            Node (V value, Node *nxt) {
                val = value;
                next = nxt;
            }
        };
        Node * stack;
    public:
        Stack() {
            stack = NULL;
        }
        
        ~Stack() {
            clear();
        }
        
        void push(V item) {
            Node *newNode = new Node (item, stack);
            stack = newNode;
        }
        
        V pop() {
            if (empty()) throw StackEmptyException();
            Node *zombie = stack;
            V ret = stack->val;
            stack = stack->next;
            delete zombie;
            return ret;
        }
        
        V peek() {
            if (empty()) throw StackEmptyException();
            return stack->val;
        }
        
        void clear() {
            Node *zombie, *scan = stack;
            while (scan) {
                zombie = scan;
                scan = scan->next;
                delete zombie;
            }
            stack = NULL;
        }
        
        bool empty() {
            return stack == NULL;
        }

        int length () {
            Node *scan = stack;
            int len = 0;
            while (scan) {
                len++;
                scan = scan->next;
            }
            return len;
        }
        
        void swapStacks(Stack<V> stk2) {
            Node *tmp;
            tmp = stack;
            stack = stk2.stack;
            stk2.stack = tmp;
        }
        
        void map(void (*f)(V&)) {
            Node *scan=stack;
            while (scan) {
                (*f)(scan->val);
                scan = scan->next;
            }
        }
};
