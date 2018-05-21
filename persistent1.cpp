#include <iostream>
#include <cstdlib>
#include <vector>

template<typename T>
class Node {
public:
    Node* l = nullptr;
    Node* r = nullptr;
    Node* realL = nullptr;
    Node* realR = nullptr;
    Node* prevRoot1 = nullptr;
    Node* prevRoot2 = nullptr;
    bool hasRealL = false;
    bool hasRealR = false;
    bool hasRealVal = false;
    bool rev = false;
    T val;
    T realVal;
    int size = 1;
    int height = 1;
    Node(T val): val(val), realVal(val) {}
    virtual void update();
    std::pair<Node<T>*, Node<T>*> next();
    std::pair<Node<T>*, Node<T>*> prev();
    virtual void push();
    virtual ~Node() {}
};

template<typename T>
int getSize(Node<T>* n){
    if(n == nullptr){
        return 0;
    }
    return n->size;
}

template<typename T>
int getHeight(Node<T>* n){
    if(n == nullptr){
        return 0;
    }
    return n->height;
}

template<typename T>
void Node<T>::update() {
    size = 1 + getSize(l) + getSize(r);
    height = 1 + std::max(getHeight(l), getHeight(r));
}

template<typename T>
std::pair<Node<T>*, Node<T>*> Node<T>::next(){ /// returns next element only in subtree
    if(r == nullptr){
        return {nullptr, nullptr};
    }
    push();
    Node* n = r;
    Node* prv = this;
    n->push();
    while(n->l != nullptr){
        prv = n;
        n = n->l;
        n->push();
    }
    return {n, prv};
}

template<typename T>
std::pair<Node<T>*, Node<T>*> Node<T>::prev(){ /// returns prev element only in subtree
    if(l == nullptr){
        return {nullptr, nullptr};
    }
    push();
    Node* n = l;
    Node* prv = this;
    n->push();
    while(n->r != nullptr){
        prv = n;
        n = n->r;
        n->push();
    }
    return {n, prv};
}

template<typename T>
Node<T>** getPtr(Node<T>* parent, Node<T>* son){
    if(parent == nullptr || son == nullptr){
        return nullptr;
    }
    if(parent->l == son){
        return &(parent->l);
    }
    if(parent->r == son){
        return &(parent->r);
    }
    return nullptr;
}

template<typename T>
void Node<T>::push(){
    if(rev){
        Node<T>* new_l = nullptr;
        Node<T>* new_r = nullptr;
        if(l != nullptr){
            new_l = new Node<T>(l->val);
            new_l->l = l->l;
            new_l->r = l->r;
            new_l->update();
            new_l->rev = !(l->rev);
        }
        if(r != nullptr){
            new_r = new Node<T>(r->val);
            new_r->l = r->l;
            new_r->r = r->r;
            new_r->update();
            new_r->rev = !(r->rev);
        }
        std::swap(new_l, new_r);
        l = new_l;
        r = new_r;
        rev = false;
        update();
    }
}

template<typename T>
class SumMinMaxNode: public Node<T> {
public:
    T sum;
    T min;
    T max;
    T pushVal;
    int operationType = -1;
    SumMinMaxNode(T val);
    virtual void update();
    virtual void push();
    virtual ~SumMinMaxNode() {}
    void applyOperation(int op, int param);
};

template<typename T>
SumMinMaxNode<T>::SumMinMaxNode(T val): Node<T>::Node(val) {
    update();
}

template<typename T>
void SumMinMaxNode<T>::update() {
    Node<T>::update();
    sum = this->val;
    min = this->val;
    max = this->val;
    auto ptr = ((SumMinMaxNode<T>*)this->l);
    if(ptr != nullptr){
        sum += ptr->sum;
        min = std::min(min, ptr->min);
        max = std::max(max, ptr->max);
    }
    ptr = ((SumMinMaxNode<T>*)this->r);
    if(ptr != nullptr){
        sum += ptr->sum;
        min = std::min(min, ptr->min);
        max = std::max(max, ptr->max);
    }
}

template<typename T>
void SumMinMaxNode<T>::push(){
    if(operationType == -1 && !(this->rev)){
        return;
    }
    SumMinMaxNode<T>* ptrl = (SumMinMaxNode<T>*)this->l;
    SumMinMaxNode<T>* ptrr = (SumMinMaxNode<T>*)this->r;
    SumMinMaxNode<T>* new_l = nullptr;
    SumMinMaxNode<T>* new_r = nullptr;
    if(ptrl != nullptr){
        new_l = new SumMinMaxNode<T>(ptrl->val);
        *new_l = *ptrl;
        if(this->rev){
            new_l->rev ^= 1;
        }
        if(operationType == 0){
            new_l->val += pushVal;
            if(new_l->operationType != -1){
                new_l->pushVal += pushVal;
            }else{
                new_l->operationType = 0;
                new_l->pushVal = pushVal;
            }
            new_l->sum += new_l->size * pushVal;
            new_l->min += pushVal;
            new_l->max += pushVal;
        }else if(operationType == 1){
            new_l->val = pushVal;
            new_l->operationType = 1;
            new_l->pushVal = pushVal;
            new_l->sum = new_l->size * pushVal;
            new_l->min = pushVal;
            new_l->max = pushVal;
        }
    }
    if(ptrr != nullptr){
        new_r = new SumMinMaxNode<T>(ptrr->val);
        *new_r = *ptrr;
        if(this->rev){
            new_r->rev ^= 1;
        }
        if(operationType == 0){
            new_r->val += pushVal;
            if(new_r->operationType != -1){
                new_r->pushVal += pushVal;
            }else{
                new_r->operationType = 0;
                new_r->pushVal = pushVal;
            }
            new_r->sum += new_r->size * pushVal;
            new_r->min += pushVal;
            new_r->max += pushVal;
        }else if(operationType == 1){
            new_r->val = pushVal;
            new_r->operationType = 1;
            new_r->pushVal = pushVal;
            new_r->sum = new_r->size * pushVal;
            new_r->min = pushVal;
            new_r->max = pushVal;
        }
    }
    if(this->rev){
        std::swap(new_l, new_r);
        this->rev = false;
    }
    this->l = new_l;
    this->r = new_r;
    operationType = -1;
    this->update();
}

template<typename T>
void SumMinMaxNode<T>::applyOperation(int op, int param){
    switch(op){
    case 0:
        this->val += param;
        operationType = 0;
        pushVal = param;
        sum += this->size * param;
        min += param;
        max += param;
        break;
    case 1:
        this->val = param;
        operationType = 1;
        pushVal = param;
        sum = this->size * param;
        min = param;
        max = param;
        break;
    }
}

template<typename T, template<typename U> class N = Node>
class PUA {
    std::vector<Node<T>*> roots;
    std::pair<Node<T>*, Node<T>*> find(int pos);
    void balance(Node<T>* node, Node<T>* parent);
    void rotateBL(Node<T>* n, Node<T>* parent);
    void rotateBR(Node<T>* n, Node<T>* parent);
    void rotateSL(Node<T>* n, Node<T>* parent);
    void rotateSR(Node<T>* n, Node<T>* parent);
    PUA<T, N> merge2(PUA<T, N> rhs, T t);
    PUA<T, N> merge21(Node<T>* node, Node<T>* parent, PUA<T, N> rhs, T t);
    PUA<T, N> merge22(Node<T>* node, Node<T>* parent, PUA<T, N> rhs, T t);
    void insert(Node<T>* node, Node<T>* parent, int pos, T val);
    std::pair<PUA<T, N>, PUA<T, N>> split(Node<T>* node, Node<T>* parent, int pos);
    void remove(Node<T>* node, Node<T>* parent, int pos);
    void remove2(Node<T>* node, Node<T>* parent, int pos, Node<T>* prevDelNode);
    Node<T>* begin();
public:
    Node<T>* root = nullptr;
    PUA(Node<T>* root = nullptr);
    int size();
    int height();
    void insert(int pos, T val);
    void remove(int pos);
    T operator[](int pos);
    PUA<T, N> merge(PUA<T, N> rhs);
    std::pair<PUA<T, N>, PUA<T, N>> split(int pos);
    void reverse();
    void createNewRoot();
};

template<typename T, template<typename U> class N>
PUA<T, N>::PUA(Node<T>* root): root(root) {
    roots.push_back(root);
}

template<typename T, template<typename U> class N>
int PUA<T, N>::size() {
    return getSize(root);
}

template<typename T, template<typename U> class N>
int PUA<T, N>::height() {
    return getHeight(root);
}

template<typename T, template<typename U> class N>
std::pair<Node<T>*, Node<T>*> PUA<T, N>::find(int pos) {
    Node<T>* node = root;
    Node<T>* prv = nullptr;
    while(1){
        if(node == nullptr) {
            throw std::invalid_argument("In PUA::find(): pos out of range");
        }
        node->push();
        int leftSize = getSize(node->l);
        if(pos == leftSize){
            return {node, prv};
        }
        if(pos < leftSize){
            node = node->l;
        }else{
            pos -= leftSize + 1;
            node = node->r;
        }
        prv = node;
    }
}

template<typename T, template<typename U> class N>
void PUA<T, N>::rotateBL(Node<T>* n, Node<T>* parent) {
    n->push();
    bool isRoot = (n == root);
    Node<T>* son = n->r;
    son->push();
    Node<T>* grandson = n->r->l;
    grandson->push();
    Node<T>* new_n = new N<T>(n->val);
    Node<T>* new_son = new N<T>(son->val);
    Node<T>* new_grandson = new N<T>(grandson->val);
    new_n->r = grandson->l;
    new_n->l = n->l;
    new_son->r = son->r;
    new_son->l = grandson->r;
    new_grandson->r = new_son;
    new_grandson->l = new_n;
    if(isRoot){
        roots.push_back(new_grandson);
        root = new_grandson;
    }
    new_n->update();
    new_son->update();
    new_grandson->update();
    if(parent != nullptr){
        if(parent->l == n){
            parent->l = new_grandson;
            parent->realL = n;
            parent->hasRealL = true;
        }
        if(parent->r == n){
            parent->r = new_grandson;
            parent->realR = n;
            parent->hasRealR = true;
        }
        parent->update();
    }
}

template<typename T, template<typename U> class N>
void PUA<T, N>::rotateBR(Node<T>* n, Node<T>* parent) {
    n->push();
    bool isRoot = (n == root);
    Node<T>* son = n->l;
    son->push();
    Node<T>* grandson = n->l->r;
    grandson->push();
    Node<T>* new_n = new N<T>(n->val);
    Node<T>* new_son = new N<T>(son->val);
    Node<T>* new_grandson = new N<T>(grandson->val);
    new_n->l = grandson->r;
    new_n->r = n->r;
    new_son->l = son->l;
    new_son->r = grandson->l;
    new_grandson->l = new_son;
    new_grandson->r = new_n;
    if(isRoot){
        roots.push_back(new_grandson);
        root = new_grandson;
    }
    new_n->update();
    new_son->update();
    new_grandson->update();
    if(parent != nullptr){
        if(parent->l == n){
            parent->l = new_grandson;
            parent->realL = n;
            parent->hasRealL = true;
        }
        if(parent->r == n){
            parent->r = new_grandson;
            parent->realR = n;
            parent->hasRealR = true;
        }
        parent->update();
    }
}

template<typename T, template<typename U> class N>
void PUA<T, N>::rotateSL(Node<T>* n, Node<T>* parent) {
    n->push();
    bool isRoot = (n == root);
    Node<T>* son = n->r;
    son->push();
    Node<T>* new_son = new N<T>(son->val);
    Node<T>* new_n = new N<T>(n->val);
    new_n->r = son->l;
    new_son->l = new_n;
    new_n->l = n->l;
    new_son->r = son->r;
    if(isRoot){
        roots.push_back(new_son);
        root = new_son;
    }
    new_n->update();
    new_son->update();
    if(parent != nullptr){
        if(parent->l == n){
            parent->l = new_son;
            parent->realL = n;
            parent->hasRealL = true;
        }
        if(parent->r == n){
            parent->r = new_son;
            parent->realR = n;
            parent->hasRealR = true;
        }
        parent->update();
    }
}

template<typename T, template<typename U> class N>
void PUA<T, N>::rotateSR(Node<T>* n, Node<T>* parent) {
    n->push();
    bool isRoot = (n == root);
    Node<T>* son = n->l;
    son->push();
    Node<T>* new_son = new N<T>(son->val);
    Node<T>* new_n = new N<T>(n->val);
    new_n->l = son->r;
    new_son->r = new_n;
    new_n->r = n->r;
    new_son->l = son->l;
    if(isRoot){
        roots.push_back(new_son);
        root = new_son;
    }
    new_n->update();
    new_son->update();
    if(parent != nullptr){
        if(parent->l == n){
            parent->l = new_son;
            parent->realL = n;
            parent->hasRealL = true;
        }
        if(parent->r == n){
            parent->r = new_son;
            parent->realR = n;
            parent->hasRealR = true;
        }
        parent->update();
    }
}

template<typename T, template<typename U> class N>
void PUA<T, N>::balance(Node<T>* node, Node<T>* parent) {
    node->update();
    int lh = getHeight(node->l);
    int rh = getHeight(node->r);
    if(lh == rh + 2){
        node->l->push();
        if(getHeight(node->l->l) >= getHeight(node->l->r)){
            rotateSR(node, parent);
        }else{
            rotateBR(node, parent);
        }
        if(node->hasRealL){
            node->l = node->realL;
            node->hasRealL = false;
            node->realL = nullptr;
        }
        if(node->hasRealR){
            node->r = node->realR;
            node->hasRealR = false;
            node->realR = nullptr;
        }
        if(node->hasRealVal){
            node->val = node->realVal;
            node->hasRealVal = false;
        }
        node->update();
    }else if(rh == lh + 2){
        node->r->push();
        if(getHeight(node->r->r) >= getHeight(node->r->l)){
            rotateSL(node, parent);
        }else{
            rotateBL(node, parent);
        }
        if(node->hasRealL){
            node->l = node->realL;
            node->hasRealL = false;
            node->realL = nullptr;
        }
        if(node->hasRealR){
            node->r = node->realR;
            node->hasRealR = false;
            node->realR = nullptr;
        }
        if(node->hasRealVal){
            node->val = node->realVal;
            node->hasRealVal = false;
        }
        node->update();
    }else{
        if(node->hasRealL || node->hasRealR){
            Node<T>* new_node = new N<T>(node->val);
            new_node->l = node->l;
            new_node->r = node->r;
            new_node->update();
            if(parent != nullptr){
                if(parent->l == node){
                    parent->l = new_node;
                    parent->realL = node;
                    parent->hasRealL = true;
                }
                if(parent->r == node){
                    parent->r = new_node;
                    parent->realR = node;
                    parent->hasRealR = true;
                }
                parent->update();
            }else{
                roots.push_back(new_node);
                root = new_node;
            }
            if(node->hasRealL) {
                node->l = node->realL;
                node->realL = nullptr;
                node->hasRealL = false;
            }
            if(node->hasRealR) {
                node->r = node->realR;
                node->realR = nullptr;
                node->hasRealR = false;
            }
            if(node->hasRealVal){
                node->val = node->realVal;
                node->hasRealVal = false;
            }
            node->update();
        }
    }
}

template<typename T, template<typename U> class N>
void PUA<T, N>::remove(int pos) {
    Node<T>* prevRoot = root;
    remove(root, nullptr, pos);
    if(root != nullptr){
        root->prevRoot1 = prevRoot;
    }
}

template<typename T, template<typename U> class N>
void PUA<T, N>::remove(Node<T>* node, Node<T>* parent, int pos) {
    node->push();
    int leftSize = getSize(node->l);
    if(pos == leftSize){
        if(node->l == nullptr && node->r == nullptr){
            if(parent != nullptr){
                if(parent->l == node){
                    parent->realL = node;
                    parent->hasRealL = true;
                    parent->l = nullptr;
                }else{
                    parent->realR = node;
                    parent->hasRealR = true;
                    parent->r = nullptr;
                }
            }else{
                roots.push_back(nullptr);
                root = nullptr;
            }
            return;
        }else{
            if(node->l != nullptr){
                remove2(node->l, node, pos - 1, node);
            }else{
                remove2(node->r, node, 0, node);
            }
        }
    }else{
        if(pos < leftSize){
            remove(node->l, node, pos);
        }else{
            remove(node->r, node, pos - leftSize - 1);
        }
    }
    balance(node, parent);
}

template<typename T, template<typename U> class N>
void PUA<T, N>::remove2(Node<T>* node, Node<T>* parent, int pos, Node<T>* prevDelNode) {
    node->push();
    int leftSize = getSize(node->l);
    if(pos == leftSize){
        prevDelNode->realVal = prevDelNode->val;
        prevDelNode->hasRealVal = true;
        prevDelNode->val = node->val;
        if(parent != nullptr){
            Node<T>* replacement;
            if(node->l != nullptr){
                replacement = node->l;
            }else{
                replacement = node->r;
            }
            if(parent->l == node){
                parent->realL = node;
                parent->hasRealL = true;
                parent->l = replacement;
            }else{
                parent->realR = node;
                parent->hasRealR = true;
                parent->r = replacement;
            }
        }else{
            roots.push_back(nullptr);
            root = nullptr;
        }
        return;
    }else{
        if(pos < leftSize){
            remove2(node->l, node, pos, prevDelNode);
        }else{
            remove2(node->r, node, pos - leftSize - 1, prevDelNode);
        }
    }
    balance(node, parent);
}

template<typename T, template<typename U> class N>
void PUA<T, N>::insert(int pos, T val) {
    if(pos < 0 || pos >= getSize(root) + 1){
        throw std::invalid_argument("In PUA::insert(): pos out of range");
    }
    Node<T>* prevRoot = root;
    Node<T>* ans;
    if(root == nullptr){
        root = ans = new N<T>(val);
        roots.push_back(root);
    }else{
        insert(root, nullptr, pos, val);
    }
    if(root != nullptr){
        root->prevRoot1 = prevRoot;
    }
}

template<typename T, template<typename U> class N>
void PUA<T, N>::insert(Node<T>* node, Node<T>* parent, int pos, T val){
    if(node == nullptr) {
        throw;
    }
    node->push();
    int leftSize = getSize(node->l);
    if(pos > leftSize){
        if(node->r == nullptr){
            node->realR = nullptr;
            node->hasRealR = true;
            node->r = new N<T>(val);
        }else{
            insert(node->r, node, pos - leftSize - 1, val);
        }
    }else{
        if(node->l == nullptr){
            node->realL = nullptr;
            node->hasRealL = true;
            node->l = new N<T>(val);
        }else{
            insert(node->l, node, pos, val);
        }
    }
    balance(node, parent);
}

template<typename T, template<typename U> class N>
Node<T>* PUA<T, N>::begin() {
    Node<T>* n = root;
    if(n == nullptr){
        return nullptr;
    }
    n->push();
    while(n->l != nullptr){
        n = n->l;
        n->push();
    }
    return n;
}

template<typename T, template<typename U> class N>
T PUA<T, N>::operator[](int pos) {
    Node<T>* node = find(pos).first;
    return node->val;
}

template<typename T, template<typename U> class N>
PUA<T, N> PUA<T, N>::merge21(Node<T>* node, Node<T>* parent, PUA<T, N> rhs, T t){
    node->push();
    if(node->r != nullptr && node->height > 1 + rhs.height()){
        PUA<T, N> ans = merge21(node->r, node, rhs, t);
        ans.balance(node, parent);
        return ans;
    }
    Node<T>* node2 = new N<T>(t);
    node2->l = node;
    node2->r = rhs.root;
    if(parent != nullptr){
        if(parent->l == node){
            parent->realL = node;
            parent->hasRealL = true;
            parent->l = node2;
        }else{
            parent->realR = node;
            parent->hasRealR = true;
            parent->r = node2;
        }
    }
    if(root == node){
        root = node2;
        roots.push_back(root);
    }
    PUA<T, N> ans(root);
    ans.balance(node2, parent);
    return ans;
}

template<typename T, template<typename U> class N>
PUA<T, N> PUA<T, N>::merge22(Node<T>* node, Node<T>* parent, PUA<T, N> rhs, T t){
    node->push();
    if(node->l != nullptr && node->height > 1 + height()){
        PUA<T, N> ans = merge22(node->l, node, rhs, t);
        ans.balance(node, parent);
        return ans;
    }
    Node<T>* node2 = new N<T>(t);
    node2->r = node;
    node2->l = root;
    if(parent != nullptr){
        if(parent->l == node){
            parent->realL = node;
            parent->hasRealL = true;
            parent->l = node2;
        }else{
            parent->realR = node;
            parent->hasRealR = true;
            parent->r = node2;
        }
    }
    if(rhs.root == node){
        rhs.root = node2;
        rhs.roots.push_back(rhs.root);
    }
    PUA<T, N> ans(rhs.root);
    ans.balance(node2, parent);
    return ans;
}

template<typename T, template<typename U> class N>
PUA<T, N> PUA<T, N>::merge2(PUA<T, N> rhs, T t){
    if(rhs.size() == 0 && size() == 0){
        Node<T>* newRoot = new N<T>(t);
        return PUA<T, N>(newRoot);
    }
    if(size() == 0){
        rhs.insert(0, t);
        return rhs;
    }
    if(rhs.size() == 0){
        insert(size(), t);
        return *this;
    }
    root->push();
    rhs.root->push();
    if(height() >= rhs.height()){
        return merge21(root, nullptr, rhs, t);
    }else{
        return merge22(rhs.root, nullptr, rhs, t);
    }
}

template<typename T, template<typename U> class N>
PUA<T, N> PUA<T, N>::merge(PUA<T, N> rhs){
    Node<T>* prevRoot1 = root;
    Node<T>* prevRoot2 = rhs.root;
    if(rhs.size() == 0){
        return *this;
    }
    if(size() == 0){
        return rhs;
    }
    root->push();
    rhs.root->push();
    T t;
    if(rhs.height() >= height()){
        t = rhs.find(0).first->val;
        rhs.remove(0);
    }else{
        t = find(size() - 1).first->val;
        remove(size() - 1);
    }
    auto res = merge2(rhs, t);
    if(res.root != nullptr){
        res.root->prevRoot1 = prevRoot1;
        res.root->prevRoot2 = prevRoot2;
    }
    return res;
}

template<typename T, template<typename U> class N>
std::pair<PUA<T, N>, PUA<T, N>> PUA<T, N>::split(int pos) {
    Node<T>* prevRoot = root;
    auto pr = split(root, nullptr, pos);
    if(pr.first.root != nullptr){
        pr.first.root->prevRoot1 = prevRoot;
    }
    if(pr.second.root != nullptr){
        pr.second.root->prevRoot1 = prevRoot;
    }
    return pr;
}

template<typename T, template<typename U> class N>
std::pair<PUA<T, N>, PUA<T, N>> PUA<T, N>::split(Node<T>* node, Node<T>* parent, int pos) {
    PUA<T, N> L;
    PUA<T, N> R;
    node->push();
    int leftSize = getSize(node->l);
    int rightSize = getSize(node->r);
    if(leftSize + rightSize < pos){
        Node<T>* new_node = new N<T>(node->val);
        new_node->l = node->l;
        new_node->r = node->r;
        new_node->update();
        L = PUA<T, N>(new_node);
        return std::make_pair(L, R);
    }
    if(0 >= pos){
        Node<T>* new_node = new N<T>(node->val);
        new_node->l = node->l;
        new_node->r = node->r;
        new_node->update();
        R = PUA<T, N>(new_node);
        return std::make_pair(L, R);
    }
    T t = node->val;
    if(leftSize < pos){
        auto pr = split(node->r, node, pos - leftSize - 1);
        L = pr.first;
        R = pr.second;
        PUA A(node->l);
        L = A.merge2(L, t);
    }else{
        auto pr = split(node->l, node, pos);
        L = pr.first;
        R = pr.second;
        PUA A(node->r);
        R = R.merge2(A, t);
    }
    return std::make_pair(L, R);
}

template<typename T, template<typename U> class N>
void PUA<T, N>::createNewRoot() {
    if(root == nullptr){
        return;
    }
    root->push();
    Node<T>* new_root = new N<T>(root->val);
    new_root->l = root->l;
    new_root->r = root->r;
    new_root->update();
    roots.push_back(new_root);
    root = new_root;
};

template<typename T, template<typename U> class N>
void PUA<T, N>::reverse(){
    if(root == nullptr){
        return;
    }
    Node<T>* prevRoot = root;
    createNewRoot();
    root->rev = true;
    root->prevRoot1 = prevRoot;
}

int main() {
    PUA<int, SumMinMaxNode> a;
    int n;
    int x, y, z;
    std::string t;
    int i = 0;
    std::vector<SumMinMaxNode<int>*> versions;
    versions.push_back((SumMinMaxNode<int>*)a.root);
    for(; 1; ++i){
        std::cerr << i << ">> ";
        std::cerr.flush();
        std::cin >> t;
        if(t == "exit"){
            return 0;
        }
        if(t == "="){
            std::cin >> x >> y >> z;
            auto p = a.split(x);
            auto q = p.second.split(y + 1 - x);
            q.first.createNewRoot();
            ((SumMinMaxNode<int>*)q.first.root)->applyOperation(1, z);
            a = p.first.merge(q.first);
            a = a.merge(q.second);
        }else if(t == "+"){
            std::cin >> x >> y >> z;
            auto p = a.split(x);
            auto q = p.second.split(y + 1 - x);
            q.first.createNewRoot();
            ((SumMinMaxNode<int>*)q.first.root)->applyOperation(0, z);
            a = p.first.merge(q.first);
            a = a.merge(q.second);
        }else if(t == "?"){
            std::cin >> x >> y;
            auto p = a.split(x);
            auto q = p.second.split(y + 1 - x);
            auto ptr = ((SumMinMaxNode<int>*)q.first.root);
            ptr->push();
            std::cout << ptr->sum << " " << ptr->min << " " << ptr->max << "\n";
            a = p.first.merge(q.first);
            a = a.merge(q.second);
        }else if(t == "v"){
            std::cin >> x >> z;
            a.insert(x, z);
        }else if(t == "x"){
            std::cin >> x;
            a.remove(x);
        }else if(t == "."){
            std::cin >> x;
            std::cout << a[x] << "\n";
        }else if(t == "r"){
            std::cin >> x >> y;
            auto p = a.split(y + 1);
            auto q = p.first.split(x);
            q.second.reverse();
            p.first = q.first.merge(q.second);
            a = p.first.merge(p.second);
        }else if(t == "c"){
            std::cin >> x;
            if(x < -1 || x >= i){
                std::cout << "Invalid version" << std::endl;
            }else{
                a = PUA<int, SumMinMaxNode>(versions[x + 1]);
            }
        }
        versions.push_back((SumMinMaxNode<int>*)a.root);
    }

}
