#include <iostream>

std::string to_print = "";

class Node
{
public:
  int value;
  int height = 1;
  Node *parent;
  Node *left;
  Node *right;

  Node(int value, Node *parent, Node *left, Node *right)
  {
    this->value = value;
    this->parent = parent;
    this->left = left;
    this->right = right;
  }

  int balance_factor()
  {
    if (this->right != NULL && this->left != NULL)
    {
      return this->right->height - this->left->height;
    }
    else if (this->right != NULL)
    {
      return this->right->height;
    }
    else if (this->left != NULL)
    {
      return -this->left->height;
    }

    return 0;
  }
};

class Tree
{
public:
  Node *root;

  Tree(Node *root)
  {
    this->root = root;
  }

  Node *rightRotate(Node *node)
  {
    Node *initial_left = node->left; // 18
    Node *initial_parent = node->parent;

    initial_left->parent = initial_parent; // 67
    if (initial_parent != NULL)
    {
      if (initial_left->value > initial_parent->value)
      {
        initial_parent->right = initial_left;
      }
      else
      {
        initial_parent->left = initial_left;
      }
    }
    node->parent = initial_left; // 18
    node->left = initial_left->right;
    if (node->left != NULL)
    {
      node->left->parent = node;
    }
    initial_left->right = node;

    this->update_node_height(node);
    this->update_node_height(initial_left);
    return node->parent;
  }

  Node *leftRotate(Node *node) // 73239
  {
    Node *initial_right = node->right;   // 83379
    Node *initial_parent = node->parent; // 57824

    initial_right->parent = initial_parent;
    if (initial_parent != NULL)
    {
      if (initial_right->value < initial_parent->value)
      {
        initial_parent->left = initial_right;
      }
      else
      {
        initial_parent->right = initial_right;
      }
    }
    node->parent = initial_right;
    node->right = initial_right->left;
    if (node->right != NULL)
    {
      node->right->parent = node;
    }
    initial_right->left = node;

    this->update_node_height(node);
    this->update_node_height(initial_right);
    return node->parent;
  }

  Node *search(Node *search_node, int value)
  {
    while (search_node != NULL and value != search_node->value)
    {
      // std::cout << search_node->value << std::endl;
      if (value < search_node->value)
      {
        search_node = search_node->left;
      }
      else
      {
        search_node = search_node->right;
      }
    }

    return search_node;
  }

  Node *minimum(Node *node)
  {
    while (node->left != NULL)
    {
      node = node->left;
    }

    return node;
  }

  Node *maximum(Node *node)
  {
    while (node->right != NULL)
    {
      node = node->right;
    }

    return node;
  }

  int level(int value)
  {
    int level = 0;
    Node *search_node = this->root;
    search_node = this->balance(search_node);
    while (search_node != NULL && value != search_node->value)
    {
      search_node = this->balance(search_node);
      level++;
      if (value < search_node->value)
      {
        search_node = search_node->left;
      }
      else
      {
        search_node = search_node->right;
      }
    }

    if (search_node != NULL && search_node->value == value)
    {
      return level;
    }
    else
    {
      return -1;
    }
  }

  Node *insert(int value, Node *current_node, Node *current_node_parent)
  {
    if (current_node == NULL)
    {
      if (current_node_parent != NULL)
      {
        return new Node(value, current_node_parent, NULL, NULL);
      }
      this->root = new Node(value, NULL, NULL, NULL);
      return this->root;
    }

    if (value < current_node->value)
    {
      current_node->left = this->insert(value, current_node->left, current_node);
    }
    else if (value > current_node->value)
    {
      current_node->right = this->insert(value, current_node->right, current_node);
    }
    else
    {
      return this->balance(current_node);
    }
    this->update_node_height(current_node);
    current_node = this->balance(current_node);

    if (current_node->parent != NULL)
    {
      this->update_node_height(current_node->parent);
    }
    else
    {
      this->root = current_node;
    }

    return current_node;
  }

  void transplant(Node *old_subtree, Node *new_subtree)
  {
    if (old_subtree->parent == NULL)
    {
      this->root = new_subtree;
    }
    else if (old_subtree == old_subtree->parent->left)
    {
      old_subtree->parent->left = new_subtree;
    }
    else
    {
      old_subtree->parent->right = new_subtree;
    }

    if (new_subtree != NULL)
    {
      new_subtree->parent = old_subtree->parent;
    }
  }

  Node *balance(Node *current_node, bool recursive = true)
  {
    if (current_node == NULL)
    {
      return NULL;
    }

    int balance_factor = current_node->balance_factor();
    if (balance_factor == 2)
    {
      if (current_node->right != NULL)
      {
        if (current_node->right->balance_factor() == 1)
        {
          current_node = leftRotate(current_node);
        }
        else if (current_node->right->balance_factor() == -1)
        {
          current_node->right = rightRotate(current_node->right);
          current_node = leftRotate(current_node);
        }
        else if (current_node->right->balance_factor() == 0)
        {
          current_node = leftRotate(current_node);
        }
      }
    }
    else if (balance_factor == -2)
    {
      if (current_node->left != NULL)
      {
        if (current_node->left->balance_factor() == -1)
        {
          current_node = rightRotate(current_node);
        }
        else if (current_node->left->balance_factor() == 1)
        {
          current_node->left = leftRotate(current_node->left);
          current_node = rightRotate(current_node);
        }
        else if (current_node->left->balance_factor() == 0)
        {
          current_node = rightRotate(current_node);
        }
      }
    }

    this->update_node_height(current_node);
    if (recursive)
    {
      current_node->right = this->balance(current_node->right, recursive);
      current_node->left = this->balance(current_node->left, recursive);
    }

    return current_node;
  }

  void update_node_height(Node *current_node)
  {
    if (current_node != NULL)
    {
      int height = 1;

      if (current_node->left != NULL && current_node->right == NULL)
      {
        height = current_node->left->height + 1;
      }
      else if (current_node->right != NULL && current_node->left == NULL)
      {
        height = current_node->right->height + 1;
      }
      else if (current_node->right != NULL && current_node->left != NULL)
      {
        height = std::max(current_node->right->height, current_node->left->height) + 1;
      }
      else
      {
        height = 1;
      }

      current_node->height = height;
    }
  }

  Node *remove(Node *current_node, int value)
  {
    Node *new_node = NULL;

    if (current_node == NULL)
    {
      return new_node;
    }
    else if (value < current_node->value)
    {
      current_node->left = this->remove(current_node->left, value);
      new_node = current_node;
    }
    else if (value > current_node->value)
    {
      current_node->right = this->remove(current_node->right, value);
      new_node = current_node;
    }
    else
    { // Should remove this one

      if (current_node->left == NULL && current_node->right == NULL)
      {
        if (current_node->parent == NULL)
        {
          this->root = NULL;
          delete current_node;
          return NULL;
        }
        if (current_node->parent->right == current_node)
        {
          current_node->parent->right = NULL;
        }
        else
        {
          current_node->parent->left = NULL;
        }
      }
      else if (current_node->left == NULL)
      {
        if (current_node->parent == NULL)
        {
          this->root = current_node->right;
          this->root->parent = NULL;
          delete current_node;
          return NULL;
        }
        if (current_node->parent->right == current_node)
        {
          current_node->parent->right = current_node->right;
        }
        else
        {
          current_node->parent->left = current_node->right;
        }
        this->update_node_height(current_node->parent);

        current_node->right->parent = current_node->parent;

        // current_node->right = this->balance(current_node->right);

        new_node = current_node->right;
      }
      else if (current_node->right == NULL)
      {
        if (current_node->parent == NULL)
        {
          this->root = current_node->left;
          this->root->parent = NULL;
          delete current_node;
          return NULL;
        }
        if (current_node->parent->right == current_node)
        {
          current_node->parent->right = current_node->left;
        }
        else
        {
          current_node->parent->left = current_node->left;
        }
        this->update_node_height(current_node->parent);
        current_node->left->parent = current_node->parent;

        // current_node->left = this->balance(current_node->left);

        new_node = current_node->left;
      }
      else
      {
        Node *successor_node = this->minimum(current_node->right);

        if (successor_node->parent != current_node)
        {
          this->transplant(successor_node, successor_node->right);
          successor_node->right = current_node->right;
          successor_node->right->parent = successor_node;
          successor_node->right = this->balance(successor_node->right);
        }

        this->transplant(current_node, successor_node);
        successor_node->left = current_node->left;
        successor_node->left->parent = successor_node;

        // successor_node = this->balance(successor_node);

        new_node = successor_node;
        delete current_node;
      }
    }

    if (new_node != NULL)
    {
      this->update_node_height(new_node);

      if (new_node->parent != NULL)
      {
        new_node = this->balance(new_node);
      }
    }

    if (new_node != NULL && new_node->parent == NULL)
    {
      this->root = new_node;
    }

    return new_node;
  }

  void print_pre_order(Node *node)
  {
    if (node == NULL)
    {
      return;
    }

    to_print += std::to_string(node->value);
    to_print += ",";
    print_pre_order(node->left);
    print_pre_order(node->right);
  }

  void print_in_order(Node *node)
  {
    if (node == NULL)
    {
      return;
    }

    print_in_order(node->left);
    to_print += std::to_string(node->value);
    to_print += ",";
    print_in_order(node->right);
  }

  void print_post_order(Node *node)
  {
    if (node == NULL)
    {
      return;
    }

    print_post_order(node->left);
    print_post_order(node->right);
    to_print += std::to_string(node->value);
    to_print += ",";
  }
};

int main()
{
  std::string buffer;
  bool tree_started = false;
  Node *root = new Node(-1029, NULL, NULL, NULL);
  Tree *tree = new Tree(root);

  while (buffer != "FIM")
  {
    to_print = "";
    std::cin >> buffer;
    if (buffer == "ADICIONA")
    {
      int value;
      std::cin >> value;
      if (!tree_started)
      {
        tree_started = true;
        root = new Node(value, NULL, NULL, NULL);
        tree = new Tree(root);
      }
      else
      {
        Node *node = new Node(value, NULL, NULL, NULL);
        tree->insert(value, tree->root, NULL);
      }
    }
    else if (buffer == "REMOVE")
    {
      int value;
      std::cin >> value;
      if (tree->search(tree->root, value) != NULL)
      {
        tree->remove(tree->root, value);
      }
      else
      {
        std::cout << "Valor " << value << " inexistente" << std::endl;
      }
    }
    else if (buffer == "NIVEL")
    {
      int value;
      std::cin >> value;

      int level = tree->level(value);
      if (level != -1)
      {
        std::cout << "Nivel de " << value << ": " << level << std::endl;
      }
      else
      {
        std::cout << "Valor " << value << " inexistente" << std::endl;
      }
    }
    else if (buffer == "PRINT")
    {
      std::string print_type;
      std::cin >> print_type;

      if (tree->root == NULL || !tree_started)
      {
        std::cout << "[]" << std::endl;
        continue;
      }

      std::cout << "[";
      if (print_type == "PREORDEM")
      {
        tree->print_pre_order(tree->root);
      }
      else if (print_type == "EMORDEM")
      {
        tree->print_in_order(tree->root);
      }
      else if (print_type == "POSORDEM")
      {
        tree->print_post_order(tree->root);
      }
      to_print.pop_back();
      std::cout << to_print;
      std::cout << "]" << std::endl;
    }
  }
}