#ifndef __NODEINDEX_H__
#define __NODEINDEX_H__

#include "defines.h"

struct Node {
	size_t size[NODE_HASH]; //current size of capacity used on the first list_node
	size_t offset[NODE_HASH]; //offset of the first list_node
	size_t count; //number of buckets for this node
};

class NodeIndex {
	size_t capacity; //capacity of table
	Node* table; //offset of Buffer line
public:
	NodeIndex(); //NodeIndex* createNodeIndex();
	~NodeIndex(); //OK_SUCCESS destroyNodeIndex(NodeIndex*);
	Node& operator[](size_t index);
	void insertNode(uint32_t nodeId);
	size_t getCapacity(void);
};

#endif
