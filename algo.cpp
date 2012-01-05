#include <string>
#include "boost/intrusive/rbtree_algorithms.hpp"
using namespace std;
struct Node
{
	Node() {}
	Node(Node* p, Node* l, Node* r, char c, int v) : parent(p), left(l), right(r), color(c), val(v) {}
	Node* parent;
	Node* left;
	Node* right;
	char color;
	int val;
};

struct node_traits {
	typedef Node node;
	typedef Node* node_ptr;
	typedef const Node* const_node_ptr;
	typedef char color;
	static node_ptr get_parent(const_node_ptr n) { return n->parent; }
	static void set_parent(node_ptr n, node_ptr parent) { n->parent = parent ; }
	static node_ptr get_left(const_node_ptr n) { return n->left; }
	static void set_left(node_ptr n, node_ptr left) { n->left = left; }
	static node_ptr get_right(const_node_ptr n) { return n->right; }
	static void set_right(node_ptr n, node_ptr right) { n->right = right; }
	static color get_color(const_node_ptr n) { return n->color; }
	static void set_color(node_ptr n, color c) { n->color = c; }
	static color black() { return 'B'; }
	static color red() { return 'R'; }
};

int main()
{
	typedef boost::intrusive::rbtree_algorithms<node_traits> ralgo;
	typedef boost::intrusive::detail::tree_algorithms<node_traits> algo;

	Node header;
	ralgo::init_header(&header);
	Node n0;
	Node n1;
	Node n2;
	Node n3;
	Node n4;
	Node n5;
	Node n6;

	n0 = Node(&n1, 0, 0, 'B', 0);
	n1 = Node(&n3, &n0, &n2, 'R', 1);
	n2 = Node(&n1, 0, 0, 'B', 2);

	n3 = Node(&header, &n1, &n5, 'B', 3);
	n4 = Node(&n5, 0, 0, 'B', 4);
	n5 = Node(&n3, &n4, &n6, 'R', 5);
	n6 = Node(&n5, 0, 0, 'B', 6);

	header.parent = &n3;
	header.left = &n0;
	header.right = &n6;

	algo::data_for_rebalance info;
	algo::erase(&header, &n3, ralgo::rbtree_erase_fixup(), info);

//	int x = *((int*)0);

	ralgo::rebalance_after_erasure(&header, 0, &n5);
	return 0;
}

