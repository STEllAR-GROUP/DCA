//-*-C++-*-

/* 
 * 	 This is a C++ file generated by write_union_domains.py 
 *
 * 	 author: Peter Staar  
 */ 


#ifndef UNION_1_H_
#define UNION_1_H_

#include "domain.h"   
#include "type_list.h"   
#include "type_list_definitions.h"   

using namespace TL; 


template< typename domain_0_t>
class union_1 : public domain 
{

public:
 
 	typedef typename domain_0_t::this_type domain_typelist_0;
	
	typedef domain_typelist_0 this_type;
 
 	union_1();
 
 	void reset(); 
 
 	int operator()(int l);

protected:

	domain_0_t domain_0;

};



template< typename domain_0_t>
union_1<domain_0_t>::union_1():
	domain(),
	domain_0()
{

	size = domain_0.get_size();
	
	branch_domain_sizes.push_back(size);
	leaf_domain_sizes.push_back(size);
}

template< typename domain_0_t>
void union_1<domain_0_t>::reset()
{
domain::reset();

domain_0.reset();


	size = domain_0.get_size();
	
branch_domain_sizes.resize(0);
branch_domain_sizes.push_back(size);

leaf_domain_sizes.resize(0);
leaf_domain_sizes.push_back(size);
}

template< typename domain_0_t>
int union_1<domain_0_t>::operator()(int l)
{
	 assert(l>=0 && l<size);
	 return l;
}

#endif