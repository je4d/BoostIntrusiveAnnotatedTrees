#include "boost/intrusive/options.hpp"
#include "boost/intrusive/rbtree_algorithms.hpp"
#include "boost/intrusive/monoid_annotation.hpp"
#include <cstddef>
#include <type_traits>
#include <iostream>

class Node;

struct value_count_monoid
{
	typedef std::size_t              type;
	typedef std::plus<type>          operation;
	static const type                identity = 0;
	static type                      value(const Node*);
};

struct subtree_count_monoid
{
	typedef std::size_t              type;
	typedef std::plus<type>          operation;
	static const type                identity = 0;
	static const type                value = 1;
};

typedef boost::intrusive::fixed_value_monoid_annotation<subtree_count_monoid> subtree_count_annotation;

/*struct tree_depth_monoid
{
	typedef std::size_t                   type;
	typedef std::plus<type>          operation;
	static const type                identity = 0;
	struct operation { type operator()(type a, type b) { type subtree_depth = identity; if (
	static type                      value(const Node*) { return 1; }
};*/

struct Node
{
	Node() {}
	Node(Node* p, Node* l, Node* r, char c) : parent(p), left(l), right(r), color(c) {}
	Node* parent;
	Node* left;
	Node* right;
	char color;
};

struct MonoidNode : public Node
{
	subtree_count_annotation::type subtree_count;
};

struct Value : public MonoidNode
{
	static int a;
	Value() : val(a++) {}
	int val;
};

int Value::a = 999;

bool operator<(const Node& a, const Node& b) { return static_cast<const Value&>(a).val < static_cast<const Value&>(b).val; }

struct my_node_traits {
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

struct my_annotated_node_traits {
	typedef my_node_traits                                                 node_traits;
	typedef node_traits::node_ptr                                          node_ptr;
	typedef node_traits::const_node_ptr                                    const_node_ptr;
	typedef MonoidNode                                                     annotated_node;
	typedef boost::pointer_to_other<node_ptr, annotated_node>::type        annotated_node_ptr;
	typedef boost::pointer_to_other<node_ptr, const annotated_node>::type  const_annotated_node_ptr;

	static node_ptr                  to_node_ptr                   (annotated_node_ptr mn)
	{ return static_cast<node_ptr>(mn); }
	static const_node_ptr            to_node_ptr                   (const_annotated_node_ptr mn)
	{ return static_cast<const_node_ptr>(mn); }
	static annotated_node_ptr        to_annotated_node_ptr         (node_ptr n)
	{ return static_cast<annotated_node_ptr>(n); }
	static const_annotated_node_ptr  to_const_annotated_node_ptr   (const_node_ptr n)
	{ return static_cast<const_annotated_node_ptr>(n); }

	template <class Annotation> static typename Annotation::type get_annotation_value(const_annotated_node_ptr n)
	{ static_assert(std::is_class<Annotation>::value && false, "get_annotation_value called for unsupported annotation"); }
	template <class Annotation> static void set_annotation_value(annotated_node_ptr n, typename Annotation::type value)
	{ static_assert(std::is_class<Annotation>::value && false, "set_annotation_value called for unsupported annotation"); }
};

typedef boost::intrusive::annotations<subtree_count_annotation> my_annotation_list;

template <> subtree_count_annotation::type my_annotated_node_traits::get_annotation_value<subtree_count_annotation>(const_annotated_node_ptr n)
{ return n->subtree_count; }
template <> void my_annotated_node_traits::set_annotation_value<subtree_count_annotation>(annotated_node_ptr n, subtree_count_annotation::type value)
{ n->subtree_count = value; }

struct Compare
{
	bool operator()(const Node* a, const Node* b) { return *a < *b; }
};

typedef boost::intrusive::annotated_rbtree_algorithms<my_annotated_node_traits,my_annotation_list> ralgo;
typedef boost::intrusive::detail::annotated_tree_algorithms<my_annotated_node_traits,my_annotation_list> algo;
//typedef boost::intrusive::rbtree_algorithms<my_node_traits> ralgo;
//typedef boost::intrusive::detail::tree_algorithms<my_node_traits> algo;

bool check(Node* header, MonoidNode* node)
{
	size_t count = 0; while (node != header) {
		if (ralgo::count(node) != node->subtree_count)
			std::cout << "oops!" << std::endl;
		node = static_cast<MonoidNode*>(ralgo::next_node(node));
	}
}

int main()
{

	Node header;
	ralgo::init_header(&header);

/*	Node n0;
	ralgo::init(&n0);
	Node n1;
	ralgo::init(&n1);
	Node n2;
	ralgo::init(&n2);
	Node n3;
	ralgo::init(&n3);
	Node n4;
	ralgo::init(&n4);
	Node n5;
	ralgo::init(&n5);
	Node n6;
	ralgo::init(&n6);*/

/*	n0 = Node(&n1, 0, 0, 'B', 0);
	n1 = Node(&n3, &n0, &n2, 'R', 1);
	n2 = Node(&n1, 0, 0, 'B', 2);

	n3 = Node(&header, &n1, &n5, 'B', 3);
	n4 = Node(&n5, 0, 0, 'B', 4);
	n5 = Node(&n3, &n4, &n6, 'R', 5);
	n6 = Node(&n5, 0, 0, 'B', 6);

	header.parent = &n3;
	header.left = &n0;
	header.right = &n6;*/

//	algo::data_for_rebalance info;
//	algo::erase(&header, &n3, ralgo::rbtree_erase_fixup(), info);

//	node_traits::set_monoid_value<subtree_count_annotation>(&n0, 1);
//	node_traits::set_monoid_value<value_count_monoid>(&n0, 1); // should static assert

/*	ralgo::insert_equal(&header, &header, &n0, Compare());
	ralgo::insert_equal(&header, &header, &n1, Compare());
	ralgo::insert_equal(&header, &header, &n2, Compare());
	ralgo::insert_equal(&header, &header, &n3, Compare());
	ralgo::insert_equal(&header, &header, &n4, Compare());
	ralgo::insert_equal(&header, &header, &n5, Compare());
	ralgo::insert_equal(&header, &header, &n6, Compare());*/
	for (int i = 0; i < 100; ++i) {
		ralgo::insert_equal(&header, &header, new Value(), Compare());
		check(&header, static_cast<MonoidNode*>(header.left));
		std::cout << i << std::endl;
	}
	Value::a = -100;
	for (int i = 0; i < 100; ++i) {
		ralgo::insert_equal(&header, &header, new Value(), Compare());
		check(&header, static_cast<MonoidNode*>(header.left));
		std::cout << i << std::endl;
	}
	Value::a = 0;
	for (int i = 0; i < 50; ++i) {
		ralgo::erase(&header, header.left);
		check(&header, static_cast<MonoidNode*>(header.left));
		std::cout << i << std::endl;
	}
	for (int i = 0; i < 50; ++i) {
		ralgo::erase(&header, header.right);
		check(&header, static_cast<MonoidNode*>(header.right));
		std::cout << i << std::endl;
	}

//	int x = *((int*)0); 

//	ralgo::rebalance_after_erasure(&header, 0, &n5);
	return 0;
}
