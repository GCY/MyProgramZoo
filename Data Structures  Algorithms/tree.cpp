#include <thread>
#include <mutex>
#include <chrono>
#include <condition_variable>
#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <sstream>

const static uint32_t DEFAULT_SIZE = 10;

template<typename T>
class Node
{
   public: 
      Node(){};
      Node(T data){this->data = data;}   

      T GetData(){return data;}
      Node<T>* GetAddress(){return this;}

      Node<T> *parent{ [](){return nullptr;}() };
      Node<T> *left_child{ [](){return nullptr;}() };
      Node<T> *right_child{ [](){return nullptr;}() };

      T data{ []()->T{return T();}() };
};

template<typename T>
class Tree
{
   public:
      //Tree(Node<T> *root):root{root}{}

      Tree(std::stringstream ss){
	 this->root = new Node<T>();
	 Node<T> *current = this->root;
	 
	 char data;
	 ss >> data;
	 current->data = std::string(1,data);

	 std::queue<Node<T>* > fifo;
	 while(ss >> data){
	    if(data >= 'A' && data <= 'Z'){
	       Node<T> *node = new Node<T>();
	       node->data = std::string(1,data);
	       node->parent = current;
	       current->left_child = node;
	       fifo.push(node);
	    }
	    if(!(ss >> data)){
	       break;
	    }
	    if(data >= 'A' && data <= 'Z'){
	       Node<T> *node = new Node<T>();
	       node->data = std::string(1,data);
	       node->parent = current;
	       current->right_child = node;
	       fifo.push(node);	       
	    }

	    current = fifo.front();
	    fifo.pop();
	 }
      }

      void InsertLevelorder(std::string data){
	 Node<T> *current = this->root;
	 Node<T> *node = new Node<T>();
	 node->data = data;

	 std::queue<Node<T>* > fifo;

	 while(current != nullptr){
	    if(current->left_child != nullptr){
	       fifo.push(current->left_child);
	    }
	    else{
	       current->left_child = node;
	       node->parent = current;
	       break;
	    }
	    if(current->right_child != nullptr){
	       fifo.push(current->right_child);
	    }
	    else{
	       current->right_child = node;
	       node->parent = current;	 
	       break;
	    }
	    current = fifo.front();
	    fifo.pop();
	 }
      }

      void Preorder(Node<T> *current){	//VLR
	 if(current != nullptr){
	    std::cout << current->GetData() << std::endl;
	    Preorder(current->left_child);
	    Preorder(current->right_child);
	 }
      }
      void Inorder(Node<T> *current){	//LVR
	 if(current != nullptr){
	    Inorder(current->left_child);
	    std::cout << current->GetData() << std::endl;
	    Inorder(current->right_child);
	 }
      };
      void Postorder(Node<T> *current){	//LRV
	 if(current != nullptr){
	    Postorder(current->left_child);
	    Postorder(current->right_child);
	    std::cout << current->GetData() << std::endl;
	 }
      };
      void Levelorder(){
	 std::queue<Node<T>* > fifo;
	 fifo.push(this->root);

	 while(!fifo.empty()){
	    Node<T> *current = fifo.front();
	    fifo.pop();
	    std::cout << current->GetData() << " ";

	    if(current->left_child != nullptr){
	       fifo.push(current->left_child);
	    }
	    if(current->right_child != nullptr){
	       fifo.push(current->right_child);
	    }
	 }

	 std::cout << std::endl;
      }

      Node<T>* Leftmost(Node<T> *root){
	 Node<std::string> *current = root;

	 while(current->left_child != nullptr){
	    current = current->left_child;
	 }
	 return current;
      }
      Node<T>* Rightmost(Node<T> *root){
	 Node<std::string> *current = root;

	 while(current->right_child != nullptr){
	    current = current->right_child;
	 }
	 return current;
      }

      Node<T>* InorderSuccessor(Node<T> *current){
	 if (current->right_child != NULL){
	    return Leftmost(current->right_child);
	 }

	 Node<T> *successor = current->parent;   
	 while (successor != NULL && current == successor->right_child) {
	    current = successor;
	    successor = successor->parent;
	 }
	 return successor;	 
      }

      void Inorder_by_parent(){
	 Node<T> *current = Leftmost(this->root);
	 while(current){
	    std::cout << current->GetData() << " ";
	    current = InorderSuccessor(current);
	 }
	 std::cout <<std::endl;
      }

      Node<T>* InorderPredecessor(Node<T> *current){
	 if (current->left_child != NULL){
	    return Rightmost(current->left_child);
	 }

	 Node<T> *predecessor = current->parent;

	 while (predecessor != NULL && current == predecessor->left_child) {
	    current = predecessor;
	    predecessor = predecessor->parent;
	 }

	 return predecessor;
      }

      void Inorder_Reverse(){
	 Node<T> *current = Rightmost(this->root);

	 while(current){
	    std::cout << current->GetData() << " ";
	    current = InorderPredecessor(current);
	 }
	 std::cout <<std::endl;
      }      

      Node<T>* GetRoot(){
	 return root;
      }
   private:
      Node<T> *root{ [](){return nullptr;}() };
};


int main( int argc, char** argv )
{
   std::ios::sync_with_stdio(false);
   std::cin.tie(0);

   /*
   Node<std::string> *a{new Node<std::string>(std::string("A"))};
   Node<std::string> *b{new Node<std::string>(std::string("B"))};
   Node<std::string> *c{new Node<std::string>(std::string("C"))};
   Node<std::string> *d{new Node<std::string>(std::string("D"))};
   Node<std::string> *e{new Node<std::string>(std::string("E"))};
   Node<std::string> *f{new Node<std::string>(std::string("F"))};
   Node<std::string> *g{new Node<std::string>(std::string("G"))};
   Node<std::string> *h{new Node<std::string>(std::string("H"))};
   Node<std::string> *i{new Node<std::string>(std::string("I"))};

   a->left_child = b; a->right_child = c;

   b->left_child = d; b->right_child = e;

   c->left_child = f; c->right_child = g;

   d->left_child = h; d->right_child = i;

   b->parent = a;
   c->parent = a;
   d->parent = b;
   e->parent = b;
   f->parent = c;
   g->parent = c;
   h->parent = d;
   i->parent = d;

   Tree<std::string> tree(a);
*/
   Tree<std::string> tree(std::stringstream(std::string("ABCDEFGHI")));
   Node<std::string> *a = tree.GetRoot();

   std::cout << "================" << std::endl;

   tree.Preorder(a);

   std::cout << "================" << std::endl;

   tree.Inorder(a);

   std::cout << "================" << std::endl;

   tree.Postorder(a);

   std::cout << "================" << std::endl;

   tree.Levelorder();
   //std::cout << tree.Leftmost()->GetData() << "," << tree.Rightmost()->GetData() << std::endl;

   std::cout << "================" << std::endl;
   tree.Inorder_by_parent();

   std::cout << "================" << std::endl;
   tree.Inorder_Reverse();

   std::cout << "================" << std::endl;
   tree.InsertLevelorder(std::string("J"));
   tree.Inorder_by_parent();

   return 0;
}
