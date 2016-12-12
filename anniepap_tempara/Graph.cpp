#include "Graph.h"
#include "QueueSet.h"

void Graph::addEdge(uint32_t from, uint32_t to) {

	if (in.getCapacity() > to && out.getCapacity() > from) {
		if (in.getCount(to) < out.getCount(from)) {
			if (in.find(from, to)) return;
		}
		else {
			if (out.find(to, from)) return;
		}
	}

	out.addEdge(from, to);
	in.addEdge(to, from);
}

long Graph::question(uint32_t from, uint32_t to) {
	if (to == from) return 0;

	size_t max = (out.getCapacity() > in.getCapacity()) ? out.getCapacity() : in.getCapacity();

	QueueSet start(max);
	QueueSet target(max);

	long lvl = 1;

	start.push(from);
	target.push(to);
/*
	while (!start.empty() && !target.empty()) {
		if (out.bfs(start, target)) return (lvl<<1)-1;
		if (in.bfs(target, start)) return lvl<<1;
		lvl++;
	}
*/
	while (!start.empty() && !target.empty()) {
		if (target.size() < start.size()) {
			if (in.bfs(target, start)) return lvl;
		}
		else {
			if (out.bfs(start, target)) return lvl;
		}
		lvl++;
	}

	return -1;
}

Pair& Graph::getOut() {
	return out;
}
Pair& Graph::getIn() {
	return in;
}

void Graph::test() {
	SCC* scc = out.getIndex().estimateStronglyConnectedComponents(out.getBuffer());
	scc->print();
	delete scc;
}

/////////////////////////////////////////////////

void Pair::addEdge(uint32_t from, uint32_t to) {
	insertNode(from);

	Node& node = index[from];

	if (node.size == LIST_NODE_CAPACITY) {
		node.offset = buffer.allocNewNode(node.offset);
		node.size = 0;
		node.count++;
	}

	buffer[node.offset].neighbor[node.size] = to;
	node.size++;
}

bool Pair::bfs(QueueSet& start, QueueSet& target) {
	size_t size = start.size();
	for (size_t i = 0; i < size; ++i) {
		uint32_t id = start.pop();
		
		Node& node = index[id];
		size_t size = node.size;
		size_t offset = node.offset;
		while (offset != NONE) {
			list_node& bucket = buffer[offset];
			for (size_t i = 0; i < size; ++i) {
				id = bucket.neighbor[i];
				if (start.visited(id) == false) {
					if (target.visited(id) == true) return true;
					start.push(id);
				}
			}
			offset = bucket.nextListNode;
			size = LIST_NODE_CAPACITY;
		}
	}
	return false;
}

void Pair::insertNode(uint32_t id) {
	index.insertNode(id);
}

size_t Pair::getCapacity() {
	return index.getCapacity();
}

size_t Pair::getCount(uint32_t id) {
	return index[id].count;
}

bool Pair::find(uint32_t to, uint32_t from) {
	Node& node = index[from];
	return buffer.find(node.offset, to, node.size);
}

Buffer& Pair::getBuffer() {
	return buffer;
}

NodeIndex& Pair::getIndex() {
	return index;
}