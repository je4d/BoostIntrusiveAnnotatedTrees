#include "boost/intrusive/options.hpp"
#include "boost/intrusive/rbtree_algorithms.hpp"
#include "boost/intrusive/monoid_annotation.hpp"
#include "boost/intrusive/detail/generic_annotation_list.hpp"
#include "boost/intrusive/detail/generic_annotated_node.hpp"
#include "boost/intrusive/detail/rbtree_node.hpp"
#include "boost/intrusive/set_hook.hpp"
#include "boost/intrusive/rbtree.hpp"
#include <cstddef>
#include <cassert>
#include <iostream>

class ValueMemberHook;
class ValueBaseHook;

struct subtree_count_monoid
{
	typedef std::size_t              type;
	typedef std::plus<type>          operation;
	static const type                identity = 0;
	static const type                value = 1;
	struct get_input_value {
		type operator()(ValueMemberHook&);
		type operator()(ValueBaseHook&);
	};
};

typedef boost::intrusive::function_monoid_annotation<subtree_count_monoid> subtree_count_annotation;
//typedef boost::intrusive::fixed_value_monoid_annotation<subtree_count_monoid> subtree_count_annotation;

template <class ValueTraits, class Hook, class Algo>
int check(typename Algo::node_traits::node_ptr header, typename ValueTraits::value_type& value)
{
	int ret = 0;
	Hook* hook = static_cast<Hook*>(ValueTraits::to_node_ptr(value));
	size_t count = 0;
	while (hook != header) {
		if (Algo::count(hook) != hook->template get_annotation_value<subtree_count_annotation>())
			++ret;
		hook = static_cast<Hook*>(Algo::next_node(hook));
	}
	return ret;
}

template <class Tree, class Hook>
int test_tree()
{
	int ret = 0;
	Tree tree;

	typedef typename Tree::node_algorithms node_algorithms;
	typedef typename Tree::node_traits node_traits;
	typedef typename Tree::real_value_traits value_traits;
	typename node_algorithms::node* header(value_traits::to_node_ptr(*tree.end()));

	value_traits::value_type::a = 0;
	for (int i = 0; i < 100; ++i) {
		tree.insert_equal(*(new typename value_traits::value_type));
		ret += check<value_traits, Hook, node_algorithms>(header, *tree.begin());
	}
	value_traits::value_type::a = -100;
	for (int i = 0; i < 100; ++i) {
		tree.insert_equal(*(new typename value_traits::value_type));
		ret += check<value_traits, Hook, node_algorithms>(header, *tree.begin());
	}
	value_traits::value_type::a = 0;
	for (int i = 0; i < 50; ++i) {
		tree.erase(tree.begin());
		ret += check<value_traits, Hook, node_algorithms>(header, *tree.begin());
	}
	for (int i = 0; i < 50; ++i) {
		typename Tree::iterator endIt = tree.end();
		tree.erase(--endIt);
		ret += check<value_traits, Hook, node_algorithms>(header, *tree.begin());
	}
	return ret;
}

typedef boost::intrusive::set_base_hook<boost::intrusive::annotations<subtree_count_annotation> > my_hook;

struct ValueBaseHook : public my_hook
{
	static int a;
	ValueBaseHook() : val(a++) {}
	int val;
};

int ValueBaseHook::a = 998;

bool operator<(const ValueBaseHook& a, const ValueBaseHook& b) { return a.val < b.val; }

typedef boost::intrusive::set_member_hook<boost::intrusive::annotations<subtree_count_annotation> > member_hook;

struct ValueMemberHook
{
	static int a;
	ValueMemberHook() : val(a++) {}
	member_hook hook;
	int val;
};

int ValueMemberHook::a = 998;

subtree_count_monoid::type subtree_count_monoid::get_input_value::operator()(ValueMemberHook& val)
{
	return 1;
}

subtree_count_monoid::type subtree_count_monoid::get_input_value::operator()(ValueBaseHook& val)
{
	return 1;
}

bool operator<(const ValueMemberHook& a, const ValueMemberHook& b) { return a.val < b.val; }

int main()
{
//	test_algo();
//	test_hook();
	assert((test_tree<boost::intrusive::rbtree<ValueBaseHook, boost::intrusive::base_hook<my_hook> >, my_hook>() == 0));
	assert((test_tree<boost::intrusive::rbtree<ValueBaseHook, boost::intrusive::base_hook<my_hook>, boost::intrusive::annotations<> >, my_hook>() != 0));

	assert((test_tree<boost::intrusive::rbtree<ValueMemberHook, boost::intrusive::member_hook<ValueMemberHook, member_hook, &ValueMemberHook::hook> >, member_hook>() == 0));
	assert((test_tree<boost::intrusive::rbtree<ValueMemberHook, boost::intrusive::member_hook<ValueMemberHook, member_hook, &ValueMemberHook::hook>, boost::intrusive::annotations<> >, member_hook>() != 0));

//	int x = *((int*)0); 

	return 0;
}
