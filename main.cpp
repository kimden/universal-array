#include <iostream>
#include <cstdlib>

template<typename T>
class Node {
public:
    Node* p = nullptr;
    Node* l = nullptr;
    Node* r = nullptr;
    bool isLeft = false;
    bool rev = false;
    T val;
    int size = 1;
    int height = 1;
    Node(T val): val(val) {}
    virtual void update();
    Node<T>* next();
    Node<T>* prev();
    Node<T>** getParentPtrToSelf();
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
Node<T>* Node<T>::next(){
    if(r == nullptr){
        Node* n = this;
        while(n != nullptr && !n->isLeft){
            n = n->p;
        }
        if(n == nullptr){
            return nullptr;
        }
        return n->p;
    }
    push();
    Node* n = r;
    n->push();
    while(n->l != nullptr){
        n = n->l;
        n->push();
    }
    return n;
}

template<typename T>
Node<T>* Node<T>::prev(){
    if(l == nullptr){
        Node* n = this;
        while(n != nullptr && n->isLeft){
            n = n->p;
        }
        if(n == nullptr){
            return nullptr;
        }
        return n->p;
    }
    push();
    Node* n = l;
    n->push();
    while(n->r != nullptr){
        n = n->r;
        n->push();
    }
    return n;
}

template<typename T>
Node<T>** Node<T>::getParentPtrToSelf(){
    if(p == nullptr){
        return nullptr;
    }
    if(isLeft){
        return &(p->l);
    }
    return &(p->r);
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
    Node<T>* find(int pos);
    void balance(Node<T>* node);
    void rotateBL(Node<T>* n);
    void rotateBR(Node<T>* n);
    void rotateSL(Node<T>* n);
    void rotateSR(Node<T>* n);
    UniversalArray<T, N> merge2(UniversalArray<T, N> rhs, T t);
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
Node<T>* UniversalArray<T, N>::find(int pos) {
    Node<T>* node = root;
    while(1){
        if(node == nullptr) {
            throw std::invalid_argument("In UniversalArray::find(): pos out of range");
        }
        node->push();
        int leftSize = getSize(node->l);
        if(pos == leftSize){
            return node;
        }
        if(pos < leftSize){
            node = node->l;
        }else{
            pos -= leftSize + 1;
            node = node->r;
        }
    }
}

template<typename T, template<typename U> class N>
void UniversalArray<T, N>::rotateBL(Node<T>* n) {
    n->push();
    rotateSR(n->r);
    rotateSL(n);
}

template<typename T, template<typename U> class N>
void UniversalArray<T, N>::rotateBR(Node<T>* n) {
    n->push();
    rotateSL(n->l);
    rotateSR(n);
}

template<typename T, template<typename U> class N>
void UniversalArray<T, N>::rotateSL(Node<T>* n) {
    n->push();
    n->r->push();
    bool isRoot = (n == root);
    Node<T>* son = n->r;
    son->isLeft = n->isLeft;
    n->isLeft = true;
    if(son->l != nullptr){
        son->l->isLeft = false;
        son->l->p = n;
    }
    son->p = n->p;
    n->p = son;
    n->r = son->l;
    son->l = n;
    if(isRoot){
        root = son;
    }
    n->update();
    son->update();
    if(son->p != nullptr){
        *(son->getParentPtrToSelf()) = son;
        son->p->update();
    }
}

template<typename T, template<typename U> class N>
void UniversalArray<T, N>::rotateSR(Node<T>* n) {
    n->push();
    n->l->push();
    bool isRoot = (n == root);
    Node<T>* son = n->l;
    son->isLeft = n->isLeft;
    n->isLeft = false;
    if(son->r != nullptr){
        son->r->isLeft = true;
        son->r->p = n;
    }
    son->p = n->p;
    n->p = son;
    n->l = son->r;
    son->r = n;
    if(isRoot){
        root = son;
    }
    n->update();
    son->update();
    if(son->p != nullptr){
        *(son->getParentPtrToSelf()) = son;
        son->p->update();
    }
}

template<typename T, template<typename U> class N>
void UniversalArray<T, N>::balance(Node<T>* node) {
    while(node != nullptr){
        node->update();
        int lh = getHeight(node->l);
        int rh = getHeight(node->r);
        if(lh == rh + 2){
            node->l->push();
            if(getHeight(node->l->l) >= getHeight(node->l->r)){
                rotateSR(node);
            }else{
                rotateBR(node);
            }
        }else if(rh == lh + 2){
            node->r->push();
            if(getHeight(node->r->r) >= getHeight(node->r->l)){
                rotateSL(node);
            }else{
                rotateBL(node);
            }
        }
        node = node->p;
    }
}

template<typename T, template<typename U> class N>
void UniversalArray<T, N>::remove(int pos) {
    Node<T>* node = find(pos);
    Node<T>* prevnode = node;
    while(node->size > 1){
        if(getSize(node->l) >= getSize(node->r)){
            node = node->prev();
        }else{
            node = node->next();
        }
        std::swap(node->val, prevnode->val);
        prevnode = node;
    }
    prevnode = node->p;
    if(prevnode == nullptr){
        root = nullptr;
    }else{
        *(node->getParentPtrToSelf()) = nullptr;
        delete node;
    }
    balance(prevnode);
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
        if(pos > 0){
            ans = find(pos - 1);
            if(ans->r == nullptr){
                ans->r = new N<T>(val);
                ans->r->p = ans;
                ans->r->isLeft = false;
                ans = ans->r;
            }else{
                ans = ans->next();
                ans->l = new N<T>(val);
                ans->l->p = ans;
                ans->l->isLeft = true;
                ans = ans->l;
            }
        }else{
            ans = find(pos);
            if(ans->l == nullptr){
                ans->l = new N<T>(val);
                ans->l->p = ans;
                ans->l->isLeft = true;
                ans = ans->l;
            }else{
                ans = ans->prev();
                ans->r = new N<T>(val);
                ans->r->p = ans;
                ans->r->isLeft = false;
                ans = ans->r;
            }
        }
    }
    balance(ans);
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
    Node<T>* node = find(pos);
    return node->val;
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
        Node<T>* node = root;
        node->push();
        while(node->r != nullptr && node->height > 1 + rhs.height()){
            node = node->r;
            node->push();
        }
        Node<T>* node2 = new N<T>(t);
        node2->l = node;
        node2->r = rhs.root;
        node2->isLeft = node->isLeft;
        node2->p = node->p;
        if(node != nullptr){
            node->isLeft = true;
            node->p = node2;
        }
        if(rhs.root != nullptr){
            rhs.root->isLeft = false;
            rhs.root->p = node2;
        }
        if(node2->p != nullptr){
            *(node2->getParentPtrToSelf()) = node2;
        }
        if(root == node){
            root = node2;
        }
        UniversalArray<T, N> ans(root);
        ans.balance(node2);
        return ans;
    }else{
        Node<T>* node = rhs.root;
        node->push();
        while(node->l != nullptr && node->height > 1 + height()){
            node = node->l;
            node->push();
        }
        Node<T>* node2 = new N<T>(t);
        node2->r = node;
        node2->l = root;
        node2->isLeft = node->isLeft;
        node2->p = node->p;
        if(node != nullptr){
            node->isLeft = false;
            node->p = node2;
        }
        if(root != nullptr){
            root->isLeft = true;
            root->p = node2;
        }
        if(node2->p != nullptr){
            *(node2->getParentPtrToSelf()) = node2;
        }
        if(rhs.root == node){
            rhs.root = node2;
        }
        UniversalArray<T, N> ans(rhs.root);
        ans.balance(node2);
        return ans;
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
        t = rhs.find(0)->val;
        rhs.remove(0);
    }else{
        t = find(size() - 1)->val;
        remove(size() - 1);
    }
    return merge2(rhs, t);
}

template<typename T, template<typename U> class N>
std::pair<UniversalArray<T, N>, UniversalArray<T, N>> UniversalArray<T, N>::split(int pos) {
    UniversalArray<T, N> L;
    UniversalArray<T, N> R;
    Node<T>* n = root;
    Node<T>* son;
    int idx = 0;
    int leftSize;
    int rightSize;
    Node<T>* prev;
    bool wasLeft;
    bool isLeft;
    while(1){
        n->push();
        leftSize = getSize(n->l);
        rightSize = getSize(n->r);
        idx += leftSize;
        if(idx < pos){
            if(idx + rightSize < pos){
                isLeft = true;
                break;
            }
            n = n->r;
            ++idx;
        }else{
            if(idx - leftSize >= pos){
                isLeft = false;
                break;
            }
            n = n->l;
            idx -= leftSize;
        }
    }

    prev = n->p;
    wasLeft = !(n->isLeft);
    n->p = nullptr;
    if(prev != nullptr){
        if(wasLeft){
            prev->r = nullptr;
        }else{
            prev->l = nullptr;
        }
    }
    UniversalArray<T, N> A(n);
    if(isLeft){
        L = A.merge(L);
    }else{
        R = R.merge(A);
    }
    n = prev;
    isLeft = wasLeft;
    while(n != nullptr){
        prev = n->p;
        wasLeft = !(n->isLeft);
        n->p = nullptr;
        if(prev != nullptr){
            if(wasLeft){
                prev->r = nullptr;
            }else{
                prev->l = nullptr;
            }
        }
        Node<T>* son;
        if(isLeft){
            son = n->l;
            n->l = nullptr;
        }else{
            son = n->r;
            n->r = nullptr;
        }
        if(son != nullptr){
            son->p = nullptr;
        }
        UniversalArray<T, N> B(son);
        T t = n->val;
        delete n;
        if(isLeft){
            L = B.merge2(L, t);
        }else{
            R = R.merge2(B, t);
        }
        n = prev;
        isLeft = wasLeft;
    }

    root = nullptr;
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
