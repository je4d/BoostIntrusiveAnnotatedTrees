#include "boost/intrusive/options.hpp"
#include "boost/intrusive/rbtree_algorithms.hpp"
#include "boost/intrusive/monoid_annotation.hpp"
#include "boost/intrusive/detail/generic_annotation_list.hpp"
#include "boost/intrusive/detail/generic_annotated_node.hpp"
#include "boost/intrusive/detail/rbtree_node.hpp"
#include "boost/intrusive/set_hook.hpp"
#include "boost/intrusive/rbtree.hpp"
#include <cstddef>
#include <type_traits>
#include <iostream>

struct value_count_monoid
{
	typedef std::size_t              type;
	typedef std::plus<type>          operation;
	static const type                identity = 0;
};

struct subtree_count_monoid
{
	typedef std::size_t              type;
	typedef std::plus<type>          operation;
	static const type                identity = 0;
	static const type                value = 1;
};

typedef boost::intrusive::fixed_value_monoid_annotation<subtree_count_monoid> subtree_count_annotation;

typedef boost::intrusive::annotations<subtree_count_annotation> my_annotation_list;

typedef boost::intrusive::detail::generic_annotation_list_traits<void*,my_annotation_list> annotation_list_traits;

typedef boost::intrusive::detail::generic_annotation_list_traits<void*,my_annotation_list> annotation_list_traits;
typedef boost::intrusive::detail::generic_annotated_node_traits<boost::intrusive::rbtree_node_traits<void*,false>, annotation_list_traits> my_annotated_node_traits;

typedef my_annotated_node_traits::annotated_node MonoidNode;

struct Value : public MonoidNode
{
	static int a;
	Value() : val(a++) {}
	int val;
};

int Value::a = 999;

bool operator<(const my_annotated_node_traits::node& a, const my_annotated_node_traits::node& b) { return static_cast<const Value&>(a).val < static_cast<const Value&>(b).val; }

struct Compare
{
	bool operator()(my_annotated_node_traits::const_node_ptr a, my_annotated_node_traits::const_node_ptr b) { return *a < *b; }
};

typedef boost::intrusive::annotated_rbtree_algorithms<my_annotated_node_traits,my_annotation_list> ralgo;
typedef boost::intrusive::detail::annotated_tree_algorithms<my_annotated_node_traits,my_annotation_list> algo;

bool check(my_annotated_node_traits::node_ptr header, MonoidNode* node)
{
	size_t count = 0;
	while (node != header) {
		if (ralgo::count(node) != my_annotated_node_traits::annotation_list_traits::get_annotation_value<subtree_count_annotation>(
					my_annotated_node_traits::to_annotation_list_ptr(node)))
			std::cout << "oops!" << std::endl;
		node = static_cast<MonoidNode*>(ralgo::next_node(node));
	}
}

void test_algo()
{
	my_annotated_node_traits::node header;
	ralgo::init_header(&header);

	for (int i = 0; i < 100; ++i) {
		ralgo::insert_equal(&header, &header, new Value(), Compare());
		check(&header, static_cast<MonoidNode*>(my_annotated_node_traits::node_traits::get_left(&header)));
		std::cout << i << " ";
	}
	std::cout << std::endl;
	Value::a = -100;
	for (int i = 0; i < 100; ++i) {
		ralgo::insert_equal(&header, &header, new Value(), Compare());
		check(&header, static_cast<MonoidNode*>(my_annotated_node_traits::node_traits::get_left(&header)));
		std::cout << i << " ";
	}
	std::cout << std::endl;
	Value::a = 0;
	for (int i = 0; i < 50; ++i) {
		// XXX the explicit temporary pointer is a workaround for bug #6978
		//ralgo::erase(&header, my_annotated_node_traits::node_ptr(my_annotated_node_traits::node_traits::get_left(&header)));
		ralgo::erase(&header, my_annotated_node_traits::node_traits::get_left(&header));
		check(&header, static_cast<MonoidNode*>(my_annotated_node_traits::node_traits::get_left(&header)));
		std::cout << i << " ";
	}
	std::cout << std::endl;
	for (int i = 0; i < 50; ++i) {
		ralgo::erase(&header, my_annotated_node_traits::node_ptr(my_annotated_node_traits::node_traits::get_right(&header)));
		check(&header, static_cast<MonoidNode*>(my_annotated_node_traits::node_traits::get_right(&header)));
		std::cout << i << " ";
	}
	std::cout << std::endl;
}

typedef boost::intrusive::set_base_hook<my_annotation_list> my_hook;

struct ValueUsingHook : public my_hook
{
	static int a;
	ValueUsingHook() : val(a++) {}
	int val;
};

int ValueUsingHook::a = 998;


struct CompareUsingHook
{
	bool operator()(my_hook::boost_intrusive_tags::node_traits::const_node_ptr a, my_hook::boost_intrusive_tags::node_traits::const_node_ptr b) { return static_cast<const ValueUsingHook&>(*a).val < static_cast<const ValueUsingHook&>(*b).val; }
};

bool check_hook(my_hook::boost_intrusive_tags::node_traits::node_ptr header, my_hook* node)
{
	size_t count = 0;
	while (node != header) {
		if (ralgo::count(node) != node->get_annotation_value<subtree_count_annotation>())
			std::cout << "oops!" << std::endl;
		node = static_cast<my_hook*>(ralgo::next_node(node));
	}
}

void test_hook()
{
	typedef boost::intrusive::annotated_rbtree_algorithms<my_hook::boost_intrusive_tags::annotated_node_traits, my_annotation_list> node_algorithms;
//	typedef boost::intrusive::annotated_rbtree_algorithms<my_hook::boost_intrusive_tags::annotated_node_traits, boost::intrusive::annotations<>> node_algorithms;
	node_algorithms::node header;
	node_algorithms::init_header(&header);

	typedef node_algorithms ralgo;
	typedef ralgo::node_traits node_traits;

	for (int i = 0; i < 100; ++i) {
		ralgo::insert_equal(&header, &header, new ValueUsingHook(), CompareUsingHook());
		check_hook(&header, static_cast<my_hook*>(node_traits::get_left(&header)));
		std::cout << i << " ";
	}
	std::cout << std::endl;
	ValueUsingHook::a = -100;
	for (int i = 0; i < 100; ++i) {
		ralgo::insert_equal(&header, &header, new ValueUsingHook(), Compare());
		check_hook(&header, static_cast<my_hook*>(node_traits::get_left(&header)));
		std::cout << i << " ";
	}
	std::cout << std::endl;
	ValueUsingHook::a = 0;
	for (int i = 0; i < 50; ++i) {
		ralgo::erase(&header, my_annotated_node_traits::node_ptr(node_traits::get_left(&header)));
		check_hook(&header, static_cast<my_hook*>(node_traits::get_left(&header)));
		std::cout << i << " ";
	}
	std::cout << std::endl;
	for (int i = 0; i < 50; ++i) {
		ralgo::erase(&header, my_annotated_node_traits::node_ptr(node_traits::get_right(&header)));
		check_hook(&header, static_cast<my_hook*>(node_traits::get_right(&header)));
		std::cout << i << " ";
	}
	std::cout << std::endl;
}

bool operator<(const ValueUsingHook& a, const ValueUsingHook& b) { return a.val < b.val; }

void test_tree()
{
	typedef boost::intrusive::rbtree<ValueUsingHook, boost::intrusive::base_hook<my_hook>> my_tree;
//	typedef boost::intrusive::rbtree<ValueUsingHook, boost::intrusive::base_hook<my_hook>, boost::intrusive::annotations<>> my_tree;

	my_tree tree;

	typedef my_tree::node_algorithms node_algorithms;
	typedef my_tree::node_traits node_traits;
	node_algorithms::node& header(*tree.end());

	ValueUsingHook::a = 0;
	for (int i = 0; i < 100; ++i) {
		tree.insert_equal(*(new ValueUsingHook));
		check_hook(&header, static_cast<my_hook*>(node_traits::get_left(&header)));
		std::cout << i << " ";
	}
	std::cout << std::endl;
	ValueUsingHook::a = -100;
	for (int i = 0; i < 100; ++i) {
		tree.insert_equal(*(new ValueUsingHook));
		check_hook(&header, static_cast<my_hook*>(node_traits::get_left(&header)));
		std::cout << i << " ";
	}
	std::cout << std::endl;
	ValueUsingHook::a = 0;
	for (int i = 0; i < 50; ++i) {
		tree.erase(tree.begin());
		check_hook(&header, static_cast<my_hook*>(node_traits::get_left(&header)));
		std::cout << i << " ";
	}
	std::cout << std::endl;
	for (int i = 0; i < 50; ++i) {
		auto endIt = tree.end();
		tree.erase(--endIt);
		check_hook(&header, static_cast<my_hook*>(node_traits::get_right(&header)));
		std::cout << i << " ";
	}
	std::cout << std::endl;
}

int main()
{
	test_algo();
	test_hook();
	test_tree();

//	int x = *((int*)0); 

//	ralgo::rebalance_after_erasure(&header, 0, &n5);
	return 0;
}
