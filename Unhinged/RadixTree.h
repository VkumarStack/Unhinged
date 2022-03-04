#ifndef RADIXTREE_H
#define RADIXTREE_H

#include <string>
#include <map>

template <typename ValueType>
class RadixTree
{
	public:
		RadixTree();
		~RadixTree();
		void insert(std::string key, const ValueType& value);
		ValueType* search(std::string key) const;
	private:
		struct Node // To be used for the RadixTree
		{
			ValueType value;
			std::string str;
			bool end; // Flag for if the Node can be a valid end to a string 
			Node* children[129] = { 0 }; // Initialize all children to nullptr by default 
		};
		Node* root; 
		void insert(Node* node, Node* parent, std::string key, const ValueType& value); // Recursive insert
		ValueType* search(Node* node, std::string key) const; // Recursive search
		void deleteTree(Node* node); // Recursive destructor
};

template <typename ValueType>
RadixTree<ValueType>::RadixTree()
{
	Node* rt = new Node();
	rt->str = ""; // The very first node of the RadixTree is to be the empty string
	rt->end = false;

	root = rt; 
}

template <typename ValueType>
RadixTree<ValueType>::~RadixTree()
{
	deleteTree(root);
}

template <typename ValueType>
void RadixTree<ValueType>::insert(std::string key, const ValueType& value)
{
	if (key != "") // Do nothing if an empty string is being inserted 
	{
		// If a string is being inserted that is not already pathed from the root, just insert it normally 
		if (root->children[key.at(0)] == nullptr)
		{
			Node* ins = new Node();
			ins->str = key;
			ins->value = value;
			ins->end = true;

			root->children[key.at(0)] = ins;
		}
		else // Otherwise, call the recursive insert 
			insert(root->children[key.at(0)], root, key, value);
	}
}

template <typename ValueType>
ValueType* RadixTree<ValueType>::search(std::string key) const
{
	return search(root, key);
}

template <typename ValueType>
void RadixTree<ValueType>::insert(Node* node, Node* parent, std::string key, const ValueType& value)
{
	// Loop through each character in both strings to determine matching prefixes
	int i;
	for (i = 0; i < node->str.size() && i < key.size(); i++)
	{
		if (node->str.at(i) != key.at(i))
			break;
	}
	// Case #1: Both strings are equivalent - Mark the current node as being the end of a string and insert value
	if (i == node->str.size() && i == key.size())
	{
		node->end = true;
		node->value = value; 
		return;
	}
	else if (i == node->str.size() && i < key.size()) // Case #2: Both strings have equivalent prefixes; traverse further down if possible or create a new node if not  
	{
		std::string str = key.substr(i);
		// If the appropriate letter pointer is empty, then create a new node 
		if (node->children[str.at(0)] == nullptr)
		{
			Node* ins = new Node();
			ins->value = value;
			ins->str = str;
			ins->end = true;
			node->children[str.at(0)] = ins;
			return;
		}
		else // Otherwise, traverse further
			return insert(node->children[str.at(0)], node, str, value);
	}
	else if (i < node->str.size() && i == key.size()) // Case #3: Both strings have equivalent prefixes but the current node is longer than the key
	{
		node->str = node->str.substr(i); // Cut off prefix from current node 
		// Create new node with the common prefix inserted in between parent and node 
		Node* ins = new Node();
		ins->str = key; 
		ins->value = value;
		ins->end = true;
		ins->children[node->str.at(0)] = node;
		parent->children[ins->str.at(0)] = ins;
		return;
	}
	else if (i < node->str.size() && i < key.size()) // Case #4: Both strings do not have equivalent prefixes
	{
		node->str = node->str.substr(i); // Cut off shared prefix 

		// Create new node with shared prefix inserted between parent and node  
		Node* ins = new Node();
		ins->str = key.substr(0, i);
		ins->children[node->str.at(0)] = node; 
		parent->children[ins->str.at(0)] = ins;
		// Create a new node with the unique rest of the key and attach it to ins
		Node* ins2 = new Node();
		ins2->str = key.substr(i);
		ins2->value = value; 
		ins2->end = true; 
		ins->children[ins2->str.at(0)] = ins2;
		return;
	}
}

template <typename ValueType>
ValueType* RadixTree<ValueType>::search(Node* node, std::string key) const
{
	int i;
	for (i = 0; i < node->str.size() && i < key.size(); i++)
	{
		if (node->str.at(i) != key.at(i))
			break;
	}

	if (i == key.size() && i == node->str.size())
	{
		if (node->end == true)
			return &(node->value);
		else
			return nullptr;
	}
	else if (i == key.size() && i < node->str.size())
		return nullptr;
	else if (i < key.size() && i == node->str.size())
	{
		std::string str = key.substr(i);
		if (node->children[str.at(0)] == nullptr)
			return nullptr;
		else
			return search(node->children[str.at(0)], str);
	}
	else
		return nullptr;
}

template <typename ValueType>
void RadixTree<ValueType>::deleteTree(Node* node)
{
	for (int i = 0; i < 129; i++)
	{
		if (node->children[i] != nullptr)
			deleteTree(node->children[i]);
	}
	delete node;
}
#endif 