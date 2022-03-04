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
		ValueType* search(std::string key); //const;
	private:
		struct Node
		{
			ValueType value;
			std::string str;
			bool end;
			Node* children[26] = { 0 };
		};
		Node* root; 
		void insert(Node* node, Node* parent, std::string key, const ValueType& value);
		ValueType* search(Node* node, std::string key) const;
		//std::map<std::string, ValueType> radixTree;
};

template <typename ValueType>
RadixTree<ValueType>::RadixTree()
{
	//radixTree = std::map<std::string, ValueType > ();
	Node* rt = new Node();
	rt->str = "";
	rt->end = false;

	root = rt; 
}

template <typename ValueType>
RadixTree<ValueType>::~RadixTree()
{
	
}

template <typename ValueType>
void RadixTree<ValueType>::insert(std::string key, const ValueType& value)
{
	//radixTree.insert_or_assign(key, value);
	if (key != "")
	{
		if (root->children[key.at(0) - 'a'] == nullptr)
		{
			Node* ins = new Node();
			ins->str = key;
			ins->value = value;
			ins->end = true;

			root->children[key.at(0) - 'a'] = ins;
		}
		else
			insert(root->children[key.at(0) - 'a'], root, key, value);
	}
}

template <typename ValueType>
ValueType* RadixTree<ValueType>::search(std::string key) // const
{
	/*typename std::map<std::string, ValueType>::iterator it;
	it = radixTree.find(key);
	if (it == radixTree.end())
		return nullptr;

	ValueType* ret = &(it->second);
	return ret;
	*/
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
		if (node->children[str.at(0) - 'a'] == nullptr)
		{
			Node* ins = new Node();
			ins->value = value;
			ins->str = key;
			ins->end = true;
			return;
		}
		else // Otherwise, traverse further
			return insert(node->children[str.at(0) - 'a'], node, str, value);
	}
	else if (i < node->str.size() && i == key.size()) // Case #3: Both strings have equivalent prefixes but the current node is longer than the key
	{
		node->str = node->str.substr(i); // Cut off prefix from current node 
		// Create new node with the common prefix inserted in between parent and node 
		Node* ins = new Node();
		ins->str = key; 
		ins->value = value;
		ins->end = true;
		ins->children[node->str.at(0) - 'a'] = node;
		parent->children[ins->str.at(0) - 'a'] = ins;
		return;
	}
	else if (i < node->str.size() && i < key.size()) // Case #4: Both strings do not have equivalent prefixes
	{
		node->str = node->str.substr(i); // Cut off shared prefix 

		// Create new node with shared prefix inserted between parent and node  
		Node* ins = new Node();
		ins->str = key.substr(0, i);
		ins->children[node->str.at(0) - 'a'] = node; 
		parent->children[ins->str.at(0) - 'a'] = ins;
		// Create a new node with the unique rest of the key and attach it to ins
		Node* ins2 = new Node();
		ins2->str = key.substr(i);
		ins2->value = value; 
		ins2->end = true; 
		ins->children[ins2->str.at(0) - 'a'] = ins2;
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
		if (node->children[str.at(0) - 'a'] == nullptr)
			return nullptr;
		else
			return search(node->children[str.at(0) - 'a'], str);
	}
	else
		return nullptr;
}

#endif 