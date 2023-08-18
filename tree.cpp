//your AVL tree implementation here
#include <iostream>
#include <sstream>
#include <string>
#include <cmath>
#include <vector>
using namespace std;
struct Node{
unsigned int number;
string name;
int height;
Node* left;
Node* right;
Node(string nam, int x) : number(x), name(nam), height (1), left(nullptr), right(nullptr) {}
};

class Tree{
Node* root;
unsigned int counter;
    
public: // start of public functions
unsigned int inOrderTracker;
Tree() : counter(0), root(nullptr), inOrderTracker(0){}
Node* insert(Node* node, string nam, unsigned int number);
int GetHeight(Node* node);
Node* rotateLeft(Node* node);
Node* rotateRight(Node* node);
unsigned int GetCounter();
void searchID(Node* node, unsigned int key);
bool searchString(Node* node, string key);
Node* GetRoot();
void SetRoot(Node* node);
void inOrderPrint(Node* node);
void preOrderPrint(Node* node);
void postOrderPrint(Node* node);
void printLevelCount();
Node* removeID(Node* root, unsigned int key);
void inOrderFind(Node* node, vector<int>& vec);
void deleteAll(Node* node);
~Tree();
};

void searchName(Tree& tree, string key);
bool traverse(Tree& tree, unsigned int key);
void deleteID(Tree& tree, unsigned int key);
void deleteInOrder(Tree& tree, unsigned int index);

int main()
{
    //your code to invoke the respective commands
    Tree tree;
    string line;
    getline(cin, line);
    int inputs = stoi(line);
    string word;
    for(int i = 0; i < inputs; i++)
    {
        getline(cin, line);
        stringstream s(line);
        s >> word; // grab the first search word
        
        if(word == "search")
        {
            int forPass = 0;
            bool searchFlag = true;
            string checker;
            s >> checker;
            try // check to see whether the term is an int or a string
            {
                forPass = stoi(checker);
            }
            catch(invalid_argument& e)
            {
               searchFlag = false;
            }
            if(searchFlag) //search term is an integer print all the names with that ID call search ID function
            {
                tree.searchID(tree.GetRoot(), forPass);
                
            }
            else 
            {
                /* 
                search term is a name. find associated id(s) call search name 
                funcation and that takes care of unsuccessfull */
                checker = checker.substr(1, checker.size() - 2);
                searchName(tree, checker);
            }
        } // end of search if
        else if(word == "insert")
        {
            string checker;
            string checkerInt;
            s >> checker;
            checker = checker.substr(1, checker.size() - 2);
            s >> checkerInt;
            // check to make sure it is a valid name
            bool flag = true;
            for(char c : checker)
            {
                if(!isalpha(c) && !isdigit(c))
                {
                    flag = false;
                    break;
                }
            }
            // check to see if ID has 8 values
            if(checkerInt.size() != 8)
            {
                flag = false;
            }
            // check if all chars are digits
            for(char c : checkerInt)
            {
                if(!isdigit(c))
                {
                    flag = false;
                    break;
                }
            }
            
            if(flag) // do insert stuff
            {
		int val = stoi(checkerInt);
                tree.SetRoot(tree.insert(tree.GetRoot(), checker, val));
            }
            else
            {
                cout << "unsuccessful" << endl;
            }
        } //end of insert else if
        
        else if(word == "printLevelCount") // have tree call print level count function
        {
            tree.printLevelCount();
        } // end of print level count else if
        
        else if(word == "printInorder") //prints the tree in order
        { 
            tree.inOrderPrint(tree.GetRoot());
            cout << endl;
        }
        
        else if(word == "remove") // removes the ID from the tree
        {
            string checkerInt;
            s >> checkerInt;
            int val = stoi(checkerInt);
            // call deleteID with tree and val as the ID value
            deleteID(tree, val);
        }
        
        else if(word == "removeInorder") // finds the value of the Nth order then calls the remove function
        {
            string checkerInt;
            s >> checkerInt;
            int val = stoi(checkerInt);
            // call deleteInOrder passing in tree and val
            deleteInOrder(tree, val);
        }
        
        else if(word == "printPreorder") // prints the preordered tree
        {
            // call tree.printPreOrder
            tree.preOrderPrint(tree.GetRoot());
            cout << endl;
        }
        
        else if(word == "printPostorder") // prints the post order list
        {
            // call tree.printPostOrder
            tree.postOrderPrint(tree.GetRoot());
            cout << endl;
        }
    } // end of the for loop
    // call delete function
      tree.deleteAll(tree.GetRoot());
      tree.SetRoot(nullptr);
    return 0;
}

Node* Tree::insert(Node* node, string nam, unsigned int number)
{ 
// code taken from module 3
  //insert into the tree
  if(node == nullptr) // adds a new node and updates the counter
  { 
      Node* treeNode = new Node(nam, number);
      counter++;
    cout << "successful" << endl;
    return treeNode;
  }
  if(number < node->number) // number is less than the current value
  {
    node->left = insert(node->left, nam, number);
  }
  else if(number > node->number) // number is greater than the current value
  {
    node->right = insert(node->right, nam, number);
  }
  else{ // number is the same as a value and kicks it back out
    cout << "unsuccessful" << endl;
    return node;
  }
  
  // update height of node;
  int leftH = GetHeight(node->left);
    int rightH = GetHeight(node->right);
   
    if(leftH > rightH)
    {
        node->height = 1 + leftH;
    }
    else
    {
        node->height = 1 + rightH;
    }
  
// check balance factor
  int BF = 0;
  if(node == nullptr)
  {
    BF = 0;
  }
  else{
    BF = GetHeight(node->left) - GetHeight(node->right);
  }
  //check for rotations
  if(BF <= -2){ // right heavy tree
    // get balance factor for node->right.
    int BFchild = 0;
    Node* child = node->right;
    if(child == nullptr)
    {
      BFchild = 0;
    }
    else
    {
      BFchild = GetHeight(child->left) - GetHeight(child->right);
    }
    //check if child is left heavy ( Bf >= 1)
    if(BFchild >= 1) // left heavy case so do right left rotation
    {
      node->right = rotateRight(node->right);
      return rotateLeft(node);
    }
    else{
      return rotateLeft(node);
    }
  }
  else if(BF >= 2) // left heavy tree
  {
    int BFchild = 0;
    Node* child = node->left;
    if(child == nullptr)
    {
      BFchild = 0;
    }
    else
    {
      BFchild = GetHeight(child->left) - GetHeight(child->right);
    }
    if(BFchild <= -1)
    {
      node->left = rotateLeft(node->left);
      return rotateRight(node);
    }
    else
    {
      return rotateRight(node);
    }
    
  }
  
    return node;
/*
code citing 
Author: N/A
website: GeeksforGeeks.com
URL: https://www.geeksforgeeks.org/avl-tree-set-1-insertion/
  */
  
}

int Tree::GetHeight(Node* node) // return the height of the tree 
{
  if(node == nullptr)
  {
    return 0;
  }
  else
  {
    return node->height;
  }
}

Node* Tree::rotateLeft(Node* node) //rotates the tree to the left
{
  Node* b = node->right;
    Node* y = b->left;
    
    b->left = node;
    node->right = y;
    // update height of node node
    int leftH = GetHeight(node->left);
    int rightH = GetHeight(node->right);
   
    if(leftH > rightH)
    {
        node->height = 1 + leftH;
    }
    else
    {
        node->height = 1 + rightH;
    }
  // update height of node b
    leftH = GetHeight(b->left);
    rightH = GetHeight(b->right);
   
    if(leftH > rightH)
    {
        b->height = 1 + leftH;
    }
    else
    {
        b->height = 1 + rightH;
    }
    return b;
  // from module 4 and stepik 5.1
}

Node* Tree::rotateRight(Node* node) // rotates the tree to the right
{
  Node* b = node->left;
    Node* y = b->right;
    
    b->right = node;
    node->left = y;
  //update height node node
  int leftH = GetHeight(node->left);
    int rightH = GetHeight(node->right);
   
    if(leftH > rightH)
    {
        node->height = 1 + leftH;
    }
    else
    {
        node->height = 1 + rightH;
    }
    //update height node b
  leftH = GetHeight(b->left);
    rightH = GetHeight(b->right);
   
    if(leftH > rightH)
    {
        b->height = 1 + leftH;
    }
    else
    {
        b->height = 1 + rightH;
    }
  
    return b;
// from module 4 and stepik 5.1
  
}

unsigned int Tree::GetCounter() //returns the number of nodes in the tree
{
  return counter;
}

void Tree::searchID(Node* node, unsigned int key) //iterates through the tree to find the ID that matches input
{
  while(node != nullptr){
    if(key < node->number)
    {
      node = node->left;
    }
    else if (key > node->number)
    {
      node = node->right;
    }
    else 
    {
      std::cout << node->name << std::endl;
      return;
    }
  }
  cout << "unsuccessful" << endl;
  
}

bool Tree::searchString(Node* node, string key) // recursively checks each node for a specific name
{
  //preorder recursive function
  bool flag = false;
  bool binL = false;
  bool binR = false;
  if(node == nullptr)
  {
    flag = false;
    return flag;
  }
  if(node->name == key)
  {
    flag = true;
    std::cout << node->number << std::endl;
  }
  binL = searchString(node->left, key); // stores bool value from left tree
  binR = searchString(node->right, key); // stores bool value from right tree
  if(flag == true || binL == true || binR == true) // if any of the three are true return a true value
  {
    flag = true;
  }
  else{
    flag = false;
  }
  return flag; // main needs to handle printing true or false
}

Node* Tree::GetRoot()
{
  return root;
}
void Tree::SetRoot(Node* node)
{
  root = node;
}

void Tree::inOrderPrint(Node* node)
{
  if(node != nullptr)
  {
    inOrderPrint(node->left);
    inOrderTracker++; 
    // in_order_tracker keeps track of how many times print was called so the last one doesn't get a comma
    if(inOrderTracker == counter) 
    {
      cout << node->name;
      inOrderTracker = 0;
    }
    else
    {
      cout << node->name << ", ";
    }
    inOrderPrint(node->right);
  }
}

void Tree::preOrderPrint(Node* node)
{
  if(node != nullptr)
  {
    if(node->number == root->number) // if the node is the same as the root just print out the name
    {
      cout << node->name;
    }
    else
    {
      cout <<", " << node->name; // else print the comma first then the name
    }
    preOrderPrint(node->left);
    preOrderPrint(node->right);
  }
}

void Tree::postOrderPrint(Node* node)
{
  if(node != nullptr)
  {
    postOrderPrint(node->left);
    postOrderPrint(node->right);
    if(node->number == root->number){ // reverse of preorder print
      cout << node->name;
    }
    else{
      cout <<node->name << ", ";
    }
  }

  
}

void Tree::printLevelCount() // height is equal to level count so just print the height of the root
{
  if(root == nullptr)
  {
    cout << "0" << endl;
  }
  else{
    cout << root->height << endl;
  }
}

void searchName(Tree& tree, string key) // function that handles the bool from search string
{
  bool flag = tree.searchString(tree.GetRoot(), key);
  if(!flag)
  {
    cout << "unsuccessful" << endl;
  }
}

Node* Tree::removeID(Node* node, unsigned int key)
{
  //assume it found the node make check in main
  if(key < node->number)
  {
    node->left = removeID(node->left, key);
  }
  else if(key > node->number)
  {
    node->right = removeID(node->right, key);
  }
  else // found the node it was looking for
  {
    //case with 0 children
    if(node->right == nullptr && node->left == nullptr)
    {
      delete node;
      counter--;
      return nullptr;
    }
      // case with 2 children
    else if(node->left != nullptr && node->right != nullptr)
    {
      // find in order predicessor
      Node* inorderPredcess = node->left;
      Node* parent = node->left;
      if(inorderPredcess->right == nullptr) // node->left is the inorder predicessor return node->left
      {
        inorderPredcess->right = node->right;
        delete node;
        counter--;
        return inorderPredcess;
      }
      else
      {
        while(inorderPredcess->right != nullptr)
          {
            
            if(inorderPredcess->right->right != nullptr)
            {
              parent = parent->right;
            }
            inorderPredcess = inorderPredcess->right;
          }
        
        node->number = inorderPredcess->number;
        node->name = inorderPredcess->name;
        //parent->left = inorderPredcess->left;
        parent->right = inorderPredcess->left;
        delete inorderPredcess;
        counter--;
        return node;
      }
      
    }
    // case with 1 child
    else
    {
      // figure out which one it is
      Node* child = nullptr;
      if(node->left == nullptr)
      {
        child = node->right;
      }
      else
      {
        child = node->left;
      }
      // delete node and update data
      delete node;
      counter--;
      return child;
    }
  }
  // code used from example in module 4
  return node;
}


bool traverse(Tree& tree, unsigned int key) // used to check if the node exsists for removal function
{
  Node* node = tree.GetRoot();

  
  while(node != nullptr){
    if(key < node->number)
    {
      node = node->left;
    }
    else if (key > node->number)
    {
      node = node->right;
    }
    else 
    {
      //std::cout << node->name << std::endl;
      return true;
    }
  }
  return false;
}

void deleteID(Tree& tree, unsigned int key) //main function that calls the delete function
{
  if(traverse(tree, key))
  {
    tree.SetRoot(tree.removeID(tree.GetRoot(), key));
    cout << "successful" << endl;
   }
  else
  {
    cout << "unsuccessful" << endl;
  }
}

void Tree::inOrderFind(Node* node, vector<int>& vec) 
{ // takes all the node numbers and puts them in order into a vector
  if(node != nullptr){
  inOrderFind(node->left, vec);
  vec.push_back(node->number);
  inOrderFind(node->right, vec);
  }
}

void deleteInOrder(Tree& tree, unsigned int index)
{ //checks to see if the index is greater than the number of nodes
  //if index is less than number of nodes it gets all the numbers and places them into a vector in order
  vector<int> vec;
  if(index > tree.GetCounter())
  {
    cout <<"unsuccessful" << endl;
    return;
  }
  else
  {
    tree.inOrderFind(tree.GetRoot(), vec);
    tree.SetRoot(tree.removeID(tree.GetRoot(), vec.at(index))); 
    // uses the index passed it to find the key that is
      // passed to the removeID function and it treats it like a regular delete
    cout << "successful" << endl;
  }

  vec.resize(0);
}

void Tree::deleteAll(Node* node) // deletes all nodes. Called at end of main
{
  if (node != nullptr)
    {
        deleteAll(node->left);
        deleteAll(node->right);
        delete node;
    }
}

Tree::~Tree()
{
  
}