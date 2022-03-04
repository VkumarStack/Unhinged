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
			string str;
			bool end;
			Node* children[26] = { 0 };
		};
		Node* root; 
		void insert(Node* node, Node* parent, std::string key, const ValueType& value);
		//std::map<std::string, ValueType> radixTree;
};

template <typename ValueType>
RadixTree<ValueType>::RadixTree()
{
	//radixTree = std::map<std::string, ValueType > ();
	Node* rt = new Node();
	rt->str = "";
	rt->end = false;
}

template <typename ValueType>
RadixTree<ValueType>::~RadixTree()
{
	
}

template <typename ValueType>
void RadixTree<ValueType>::insert(std::string key, const ValueType& value)
{
	//radixTree.insert_or_assign(key, value);
}

template <typename ValueType>
ValueType* RadixTree<ValueType>::search(std::string key) // const
{
	typename std::map<std::string, ValueType>::iterator it;
	it = radixTree.find(key);
	if (it == radixTree.end())
		return nullptr;

	ValueType* ret = &(it->second);
	return ret;
}

template <typename ValueType>
void RadixTree<ValueType>::insert(Node* node, Node* parent, std::string key, const ValueType& value)
{
	// Loop through each character in both strings to determine matching prefixes
	int i;
	for (i = 0; i < node->str.size() && i < key.size; i++)
	{
		if (node->str.at(i) != key.at(i))
			break;
	}
	// Case #1: Both strings are equivalent - Mark the current node as being the end of a string 
	if (i == node->str.size() && i == key.size())
	{
		rt->end = true;
		rt->value = value; 
		return;
	}
	else if (i == node->str.size() && i < key.size()) // Case #2: Both strings have equivalent prefixes; traverse further down if possible or create a new node if not  
	{
		string str = key.substr(i);
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
		node->str = str.substr(i); // Cut off prefix from current node 
		node->end = true; // Since key reached its end, this is the end of the given key string 
		// Create new node with the common prefix inserted in between parent and node 
		Node* ins = new Node();
		ins->str = key; 
		ins->children[node->str.at(0) - 'a'] = node;
		parent->children[ins->str.at(0) - 'a'] = ins;
		return;
	}
	else if (i < node->str.size() && i < key.size()) // Case #4: Both strings do not have equivalent prefixes
	{

	}
}

#endif 