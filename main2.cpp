#include <iostream>
#include <cstdlib>

template<typename T>
class Node {
public:
    Node* l = nullptr;
    Node* r = nullptr;
    bool isLeft = false;
    bool rev = false;
    T val;
    int size = 1;
    int height = 1;
    Node(T val): val(val) {}
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
std::pair<Node<T>*, Node<T>*> Node<T>::next(){ /// now returns next element only in subtree
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
std::pair<Node<T>*, Node<T>*> Node<T>::prev(){ /// now returns prev element only in subtree
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
        if(l != nullptr){
            l->rev = !l->rev;
        }
        if(r != nullptr){
            r->rev = !r->rev;
        }
        std::swap(l, r);
        if(l != nullptr){
            l->isLeft = true;
        }
        if(r != nullptr){
            r->isLeft = false;
        }
        rev = false;
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
    if(this->rev){
        if(this->l != nullptr){
            this->l->rev = !this->l->rev;
        }
        if(this->r != nullptr){
            this->r->rev = !this->r->rev;
        }
        std::swap(this->l, this->r);
        if(this->l != nullptr){
            this->l->isLeft = true;
        }
        if(this->r != nullptr){
            this->r->isLeft = false;
        }
        this->rev = false;
    }
    if(operationType != -1){
        if(operationType == 0){
            auto ptr = ((SumMinMaxNode<T>*)this->l);
            if(ptr != nullptr){
                ptr->val += pushVal;
                if(ptr->operationType != -1){
                    ptr->pushVal += pushVal;
                }else{
                    ptr->operationType = 0;
                    ptr->pushVal = pushVal;
                }
                ptr->sum += ptr->size * pushVal;
                ptr->min += pushVal;
                ptr->max += pushVal;
            }
            ptr = ((SumMinMaxNode<T>*)this->r);
            if(ptr != nullptr){
                ptr->val += pushVal;
                if(ptr->operationType != -1){
                    ptr->pushVal += pushVal;
                }else{
                    ptr->operationType = 0;
                    ptr->pushVal = pushVal;
                }
                ptr->sum += ptr->size * pushVal;
                ptr->min += pushVal;
                ptr->max += pushVal;
            }
        }else{
            auto ptr = ((SumMinMaxNode<T>*)this->l);
            if(ptr != nullptr){
                ptr->val = pushVal;
                ptr->operationType = 1;
                ptr->pushVal = pushVal;
                ptr->sum = ptr->size * pushVal;
                ptr->min = pushVal;
                ptr->max = pushVal;
            }
            ptr = ((SumMinMaxNode<T>*)this->r);
            if(ptr != nullptr){
                ptr->val = pushVal;
                ptr->operationType = 1;
                ptr->pushVal = pushVal;
                ptr->sum = ptr->size * pushVal;
                ptr->min = pushVal;
                ptr->max = pushVal;
            }
        }
        operationType = -1;
        this->update();
    }
}

template<typename T, template<typename U> class N = Node>
class UniversalArray {
    std::pair<Node<T>*, Node<T>*> find(int pos);
    void balance(Node<T>* node, Node<T>* parent);
    void rotateBL(Node<T>* n, Node<T>* parent);
    void rotateBR(Node<T>* n, Node<T>* parent);
    void rotateSL(Node<T>* n, Node<T>* parent);
    void rotateSR(Node<T>* n, Node<T>* parent);
    UniversalArray<T, N> merge2(UniversalArray<T, N> rhs, T t);
    UniversalArray<T, N> merge21(Node<T>* node, Node<T>* parent, UniversalArray<T, N> rhs, T t);
    UniversalArray<T, N> merge22(Node<T>* node, Node<T>* parent, UniversalArray<T, N> rhs, T t);
    void insert(Node<T>* node, Node<T>* parent, int pos, T val);
    std::pair<UniversalArray<T, N>, UniversalArray<T, N>> split(Node<T>* node, Node<T>* parent, int pos);
    void remove(Node<T>* node, Node<T>* parent, int pos);
    void remove2(Node<T>* node, Node<T>* parent, int pos, Node<T>* prevDelNode);
    Node<T>* begin();
public:
    Node<T>* root = nullptr;
    UniversalArray(Node<T>* root = nullptr): root(root) {}
    int size();
    int height();
    void insert(int pos, T val);

    void remove(int pos);
    T operator[](int pos);
    UniversalArray<T, N> merge(UniversalArray<T, N> rhs);
    std::pair<UniversalArray<T, N>, UniversalArray<T, N>> split(int pos);
    void reverse(int l, int r);
};

template<typename T, template<typename U> class N>
int UniversalArray<T, N>::size() {
    return getSize(root);
}

template<typename T, template<typename U> class N>
int UniversalArray<T, N>::height() {
    return getHeight(root);
}

template<typename T, template<typename U> class N>
std::pair<Node<T>*, Node<T>*> UniversalArray<T, N>::find(int pos) {
    Node<T>* node = root;
    Node<T>* prv = nullptr;
    while(1){
        if(node == nullptr) {
            throw std::invalid_argument("In UniversalArray::find(): pos out of range");
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
void UniversalArray<T, N>::rotateBL(Node<T>* n, Node<T>* parent) {
    n->push();
    rotateSR(n->r, n);
    rotateSL(n, parent);
}

template<typename T, template<typename U> class N>
void UniversalArray<T, N>::rotateBR(Node<T>* n, Node<T>* parent) {
    n->push();
    rotateSL(n->l, n);
    rotateSR(n, parent);
}

template<typename T, template<typename U> class N>
void UniversalArray<T, N>::rotateSL(Node<T>* n, Node<T>* parent) {
    n->push();
    n->r->push();
    bool isRoot = (n == root);
    Node<T>* son = n->r;
    son->isLeft = n->isLeft;
    n->isLeft = true;
    if(son->l != nullptr){
        son->l->isLeft = false;
    }
    n->r = son->l;
    son->l = n;
    if(n->r != nullptr){
        n->r->update();
    }
    if(isRoot){
        root = son;
    }
    n->update();
    son->update();
    if(parent != nullptr){
        *getPtr(parent, n) = son;
        parent->update();
    }
}

template<typename T, template<typename U> class N>
void UniversalArray<T, N>::rotateSR(Node<T>* n, Node<T>* parent) {
    n->push();
    n->l->push();
    bool isRoot = (n == root);
    Node<T>* son = n->l;
    son->isLeft = n->isLeft;
    n->isLeft = false;
    if(son->r != nullptr){
        son->r->isLeft = true;
    }
    n->l = son->r;
    son->r = n;
    if(n->l != nullptr){
        n->l->update();
    }
    if(isRoot){
        root = son;
    }
    n->update();
    son->update();
    if(parent != nullptr){
        *getPtr(parent, n) = son;
        parent->update();
    }
}

template<typename T, template<typename U> class N>
void UniversalArray<T, N>::balance(Node<T>* node, Node<T>* parent) {
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
    }else if(rh == lh + 2){
        node->r->push();
        if(getHeight(node->r->r) >= getHeight(node->r->l)){
            rotateSL(node, parent);
        }else{
            rotateBL(node, parent);
        }
    }
}

template<typename T, template<typename U> class N>
void UniversalArray<T, N>::remove(int pos) {
    return remove(root, nullptr, pos);
}

template<typename T, template<typename U> class N>
void UniversalArray<T, N>::remove(Node<T>* node, Node<T>* parent, int pos) {
    int leftSize = getSize(node->l);
    if(pos == leftSize){
        if(node->l == nullptr && node->r == nullptr){
            if(parent != nullptr){
                *getPtr(parent, node) = nullptr;
            }else{
                root = nullptr;
            }
            delete node;
            return;
        }else{
            if(node->l != nullptr){
                remove2(node->l, node, pos - 1, node);
            }else{
                remove2(node->r, node, pos + 1, node);
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
void UniversalArray<T, N>::remove2(Node<T>* node, Node<T>* parent, int pos, Node<T>* prevDelNode) {
    int leftSize = getSize(node->l);
    if(pos == leftSize){
        std::swap(node->val, prevDelNode->val);
        if(parent != nullptr){
            if(node->l != nullptr){
                *getPtr(parent, node) = node->r;
            }else{
                *getPtr(parent, node) = node->l;
            }
        }else{
            root = nullptr;
        }
        delete node;
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
void UniversalArray<T, N>::insert(int pos, T val) {
    if(pos < 0 || pos >= getSize(root) + 1){
        throw std::invalid_argument("In UniversalArray::insert(): pos out of range");
    }
    Node<T>* ans;
    if(root == nullptr){
        root = ans = new N<T>(val);
    }else{
        insert(root, nullptr, pos, val);
    }
}

template<typename T, template<typename U> class N>
void UniversalArray<T, N>::insert(Node<T>* node, Node<T>* parent, int pos, T val){
    if(node == nullptr) {
        throw;
    }
    node->push();
    int leftSize = getSize(node->l);
    if(pos > leftSize){
        if(node->r == nullptr){
            node->r = new N<T>(val);
        }else{
            insert(node->r, node, pos - leftSize - 1, val);
        }
    }else{
        if(node->l == nullptr){
            node->l = new N<T>(val);
        }else{
            insert(node->l, node, pos, val);
        }
    }
    balance(node, parent);
}

template<typename T, template<typename U> class N>
Node<T>* UniversalArray<T, N>::begin() {
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
T UniversalArray<T, N>::operator[](int pos) {
    Node<T>* node = find(pos).first;
    return node->val;
}

template<typename T, template<typename U> class N>
UniversalArray<T, N> UniversalArray<T, N>::merge21(Node<T>* node, Node<T>* parent, UniversalArray<T, N> rhs, T t){
    node->push();
    if(node->r != nullptr && node->height > 1 + rhs.height()){
        UniversalArray<T, N> ans = merge21(node->r, node, rhs, t);
        ans.balance(node, parent);
        return ans;
    }
    Node<T>* node2 = new N<T>(t);
    node2->l = node;
    node2->r = rhs.root;
    node2->isLeft = node->isLeft;
    if(node != nullptr){
        node->isLeft = true;
    }
    if(rhs.root != nullptr){
        rhs.root->isLeft = false;
    }
    if(parent != nullptr){
        *getPtr(parent, node) = node2;
    }
    if(root == node){
        root = node2;
    }
    UniversalArray<T, N> ans(root);
    ans.balance(node2, parent);
    return ans;
}

template<typename T, template<typename U> class N>
UniversalArray<T, N> UniversalArray<T, N>::merge22(Node<T>* node, Node<T>* parent, UniversalArray<T, N> rhs, T t){
    node->push();
    if(node->l != nullptr && node->height > 1 + height()){
        UniversalArray<T, N> ans = merge22(node->l, node, rhs, t);
        ans.balance(node, parent);
        return ans;
    }
    Node<T>* node2 = new N<T>(t);
    node2->r = node;
    node2->l = root;
    node2->isLeft = node->isLeft;
    if(node != nullptr){
        node->isLeft = false;
    }
    if(root != nullptr){
        root->isLeft = true;
    }
    if(parent != nullptr){
        *getPtr(parent, node) = node2;
    }
    if(rhs.root == node){
        rhs.root = node2;
    }
    UniversalArray<T, N> ans(rhs.root);
    ans.balance(node2, parent);
    return ans;
}

template<typename T, template<typename U> class N>
UniversalArray<T, N> UniversalArray<T, N>::merge2(UniversalArray<T, N> rhs, T t){
    if(rhs.size() == 0 && size() == 0){
        Node<T>* newRoot = new N<T>(t);
        return UniversalArray<T, N>(newRoot);
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
UniversalArray<T, N> UniversalArray<T, N>::merge(UniversalArray<T, N> rhs){
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
    return merge2(rhs, t);
}

template<typename T, template<typename U> class N>
std::pair<UniversalArray<T, N>, UniversalArray<T, N>> UniversalArray<T, N>::split(int pos) {
    return split(root, nullptr, pos);
}

template<typename T, template<typename U> class N>
std::pair<UniversalArray<T, N>, UniversalArray<T, N>> UniversalArray<T, N>::split(Node<T>* node, Node<T>* parent, int pos) {
    UniversalArray<T, N> L;
    UniversalArray<T, N> R;
    node->push();
    int leftSize = getSize(node->l);
    int rightSize = getSize(node->r);
    if(leftSize + rightSize < pos){
        L = UniversalArray<T, N>(node);
        return std::make_pair(L, R);
    }
    if(0 >= pos){
        R = UniversalArray<T, N>(node);
        return std::make_pair(L, R);
    }
    T t = node->val;
    if(leftSize < pos){
        auto pr = split(node->r, node, pos - leftSize - 1);
        L = pr.first;
        R = pr.second;
        UniversalArray A(node->l);
        if(parent != nullptr){
            *getPtr(parent, node) = nullptr;
        }
        delete node;
        L = A.merge2(L, t);
    }else{
        auto pr = split(node->l, node, pos);
        L = pr.first;
        R = pr.second;
        UniversalArray A(node->r);
        if(parent != nullptr){
            *getPtr(parent, node) = nullptr;
        }
        delete node;
        R = R.merge2(A, t);
    }
    return std::make_pair(L, R);
}

template<typename T, template<typename U> class N>
void UniversalArray<T, N>::reverse(int l, int r){
    if(l < 0 || l >= size()){
        throw std::invalid_argument("In UniversalArray::reverse(): l out of range");
    }
    if(r < 0 || r >= size()){
        throw std::invalid_argument("In UniversalArray::reverse(): r out of range");
    }
    if(root == nullptr){
        return;
    }
    auto p = split(l);
    auto q = p.second.split(r + 1 - l);
    q.first.root->push();
    q.first.root->rev = true;
    *this = p.first.merge(q.first).merge(q.second);
}

int main() {
    UniversalArray<int, SumMinMaxNode> a;
    int n;
    int x, y, z;
    std::string t;
    int i = 0;
    for(; 1; ++i){
        std::cin >> t;
        if(t == "exit"){
            return 0;
        }
        if(t == "="){
            std::cin >> x >> y >> z;
            auto p = a.split(x);
            auto q = p.second.split(y + 1 - x);
            auto ptr = ((SumMinMaxNode<int>*)q.first.root);
            ptr->push();
            ptr->val = z;
            ptr->operationType = 1;
            ptr->pushVal = z;
            a = p.first.merge(q.first);
            a = a.merge(q.second);
        }else if(t == "+"){
            std::cin >> x >> y >> z;
            auto p = a.split(x);
            auto q = p.second.split(y + 1 - x);
            auto ptr = ((SumMinMaxNode<int>*)q.first.root);
            ptr->push();
            ptr->val += z;
            ptr->operationType = 0;
            ptr->pushVal = z;
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
            a.reverse(x, y);
        }
    }

}
