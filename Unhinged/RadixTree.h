#ifndef RADIXTREE_H
#define RADIXTREE_H

#include <string>

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
			ValueType* value;
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
			ins->value = new ValueType(value);
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
		if (node->end) // Check if ValueType is already at the Node - if so, free its prior value
			delete node->value;
		node->end = true;
		node->value = new ValueType(value);
		return;
	}
	else if (i == node->str.size() && i < key.size()) // Case #2: Both strings have equivalent prefixes; traverse further down if possible or create a new node if not  
	{
		std::string str = key.substr(i);
		// If the appropriate letter pointer is empty, then create a new node and connect it
		if (node->children[str.at(0)] == nullptr)
		{
			Node* ins = new Node();
			ins->value = new ValueType(value);
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
		ins->value = new ValueType(value);
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
		ins->end = false;
		ins->str = key.substr(0, i);
		ins->children[node->str.at(0)] = node; 
		parent->children[ins->str.at(0)] = ins;
		// Create a new node with the unique rest of the key and attach it to ins
		Node* ins2 = new Node();
		ins2->str = key.substr(i);
		ins2->value = new ValueType(value); 
		ins2->end = true; 
		ins->children[ins2->str.at(0)] = ins2;
		return;
	}
}

template <typename ValueType>
ValueType* RadixTree<ValueType>::search(Node* node, std::string key) const
{
	// Iterate for shared prefix
	int i;
	for (i = 0; i < node->str.size() && i < key.size(); i++)
	{
		if (node->str.at(i) != key.at(i))
			break;
	}

	// If both strings are equivalent, check if the Node is flagged as being a valid string 
	if (i == key.size() && i == node->str.size())
	{
		if (node->end == true)
			return node->value;
		else
			return nullptr;
	}
	else if (i == key.size() && i < node->str.size()) // If the key reaches the end but the Node's string does not, then it is not found
		return nullptr;
	else if (i < key.size() && i == node->str.size()) // If the Node's string reaches the end, then check if further traversals can be made 
	{
		std::string str = key.substr(i);
		if (node->children[str.at(0)] == nullptr) // If no further traversal can be made, it is not found
			return nullptr;
		else
			return search(node->children[str.at(0)], str);
	}
	else // If the prefixes do not match, then it is not found 
		return nullptr;
}

template <typename ValueType>
void RadixTree<ValueType>::deleteTree(Node* node)
{
	if (node == nullptr) // Base case
		return;

	for (int i = 0; i < 129; i++)
	{
		deleteTree(node->children[i]);
	}
	delete node->value;
	delete node;
}
#endif 