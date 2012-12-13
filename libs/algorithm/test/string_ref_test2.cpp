/* 
   Copyright (c) Marshall Clow 2012-2012.

   Distributed under the Boost Software License, Version 1.0. (See accompanying
   file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

    For more information, see http://www.boost.org
*/

#include <iostream>
#include <cstring>    // for std::strchr

#include <boost/algorithm/string_ref.hpp>

#include <boost/test/included/test_exec_monitor.hpp>

typedef boost::string_ref string_ref;

void ends_with ( const char *arg ) {
    const size_t sz = strlen ( arg );
    string_ref sr ( arg );
    string_ref sr2 ( arg );
    const char *p = arg;

    while ( !*p ) {
        BOOST_CHECK ( sr.ends_with ( p ));
        ++p;
        }

    while ( !sr2.empty ()) {
        BOOST_CHECK ( sr.ends_with ( sr2 ));
        sr2.remove_prefix (1);
        }

    sr2 = arg;
    while ( !sr2.empty ()) {
        BOOST_CHECK ( sr.ends_with ( sr2 ));
        sr2.remove_prefix (1);
        }

    char ch = sz == 0 ? '\0' : arg [ sz - 1 ];
    sr2 = arg;
    if ( sz > 0 )
      BOOST_CHECK ( sr2.ends_with ( ch ));
    BOOST_CHECK ( !sr2.ends_with ( ++ch ));
    BOOST_CHECK ( sr2.ends_with ( string_ref ()));
    }
    
void starts_with ( const char *arg ) {
    const size_t sz = strlen ( arg );
    string_ref sr  ( arg );
    string_ref sr2 ( arg );
    const char *p = arg + std::strlen ( arg ) - 1;
    while ( p >= arg ) {
        std::string foo ( arg, p + 1 );
        BOOST_CHECK ( sr.starts_with ( foo ));
        --p;
        }

    while ( !sr2.empty ()) {
        BOOST_CHECK ( sr.starts_with ( sr2 ));
        sr2.remove_suffix (1);
        }

    char ch = *arg;
    sr2 = arg;
  if ( sz > 0 )
    BOOST_CHECK ( sr2.starts_with ( ch ));
    BOOST_CHECK ( !sr2.starts_with ( ++ch ));
    BOOST_CHECK ( sr2.starts_with ( string_ref ()));
    }

void reverse ( const char *arg ) {
//  Round trip
    string_ref sr1 ( arg );
    std::string string1 ( sr1.rbegin (), sr1.rend ());
    string_ref sr2 ( string1 );
    std::string string2 ( sr2.rbegin (), sr2.rend ());

    BOOST_CHECK ( std::equal ( sr2.rbegin (), sr2.rend (), arg ));
    BOOST_CHECK ( string2 == arg );
    BOOST_CHECK ( std::equal ( sr1.begin (), sr1.end (), string2.begin ()));
    }


void find ( const char *arg ) {
    string_ref sr1;
    string_ref sr2;
    const char *p;

//  Look for each character in the string(searching from the start)
  p = arg;
  sr1 = arg;
  while ( *p ) {
        string_ref::size_type pos = sr1.find(*p);
    BOOST_CHECK ( pos != string_ref::npos && ( pos <= p - arg ));
    ++p;
    }
  
//  Look for each character in the string (searching from the end)
    p = arg;
    sr1 = arg;
  while ( *p ) {
        string_ref::size_type pos = sr1.rfind(*p);
    BOOST_CHECK ( pos != string_ref::npos && pos < sr1.size () && ( pos >= p - arg ));
    ++p;
    }

  sr1 = arg;
  p = arg;
//  for all possible chars, see if we find them in the right place.
//  Note that strchr will/might do the _wrong_ thing if we search for NULL
  for ( int ch = 1; ch < 256; ++ch ) {
        string_ref::size_type pos = sr1.find(ch);
        const char *strp = std::strchr ( arg, ch );
        BOOST_CHECK (( strp == NULL ) == ( pos == string_ref::npos ));
        if ( strp != NULL )
          BOOST_CHECK (( strp - arg ) == pos );
    }

  sr1 = arg;
  p = arg;
//  for all possible chars, see if we find them in the right place.
//  Note that strchr will/might do the _wrong_ thing if we search for NULL
  for ( int ch = 1; ch < 256; ++ch ) {
        string_ref::size_type pos = sr1.rfind(ch);
        const char *strp = std::strrchr ( arg, ch );
        BOOST_CHECK (( strp == NULL ) == ( pos == string_ref::npos ));
        if ( strp != NULL )
          BOOST_CHECK (( strp - arg ) == pos );
    }


//  Find everything at the start
    p = arg;
    sr1 = arg;
    while ( !sr1.empty ()) {
        string_ref::size_type pos = sr1.find(*p);
        BOOST_CHECK ( pos == 0 );
        sr1.remove_prefix (1);
        ++p;
        }

//  Find everything at the end
    sr1  = arg;
    p    = arg + strlen ( arg ) - 1;
    while ( !sr1.empty ()) {
        string_ref::size_type pos = sr1.rfind(*p);
        BOOST_CHECK ( pos == sr1.size () - 1 );
        sr1.remove_suffix (1);
        --p;
        }
        
//  Find everything at the start
    sr1  = arg;
    p    = arg;
    while ( !sr1.empty ()) {
        string_ref::size_type pos = sr1.find_first_of(*p);
        BOOST_CHECK ( pos == 0 );
        sr1.remove_prefix (1);
        ++p;
        }


//  Find everything at the end
    sr1  = arg;
    p    = arg + strlen ( arg ) - 1;
    while ( !sr1.empty ()) {
        string_ref::size_type pos = sr1.find_last_of(*p);
        BOOST_CHECK ( pos == sr1.size () - 1 );
        sr1.remove_suffix (1);
        --p;
        }
        
//  Basic sanity checking for "find_first_of / find_first_not_of"
  sr1 = arg;
  sr2 = arg;
  while ( !sr1.empty() ) {
    BOOST_CHECK ( sr1.find_first_of ( sr2 )     == 0 );
    BOOST_CHECK ( sr1.find_first_not_of ( sr2 ) == string_ref::npos );
    sr1.remove_prefix ( 1 );
    }

  p = arg;
  sr1 = arg;
  while ( *p ) {
        string_ref::size_type pos1 = sr1.find_first_of(*p);
        string_ref::size_type pos2 = sr1.find_first_not_of(*p);
        BOOST_CHECK ( pos1 != string_ref::npos && pos1 < sr1.size () && pos1 <= ( p - arg ));
        if ( pos2 != string_ref::npos ) {
          for ( size_t i = 0 ; i < pos2; ++i )
            BOOST_CHECK ( sr1[i] == *p );
          BOOST_CHECK ( sr1 [ pos2 ] != *p );
          }

        BOOST_CHECK ( pos2 != pos1 );
        ++p;
        }
        
//  Basic sanity checking for "find_last_of / find_last_not_of"
  sr1 = arg;
  sr2 = arg;
  while ( !sr1.empty() ) {
    BOOST_CHECK ( sr1.find_last_of ( sr2 )     == ( sr1.size () - 1 ));
    BOOST_CHECK ( sr1.find_last_not_of ( sr2 ) == string_ref::npos );
    sr1.remove_suffix ( 1 );
    }

  p = arg;
  sr1 = arg;
  while ( *p ) {
        string_ref::size_type pos1 = sr1.find_last_of(*p);
        string_ref::size_type pos2 = sr1.find_last_not_of(*p);
        BOOST_CHECK ( pos1 != string_ref::npos && pos1 < sr1.size () && pos1 >= ( p - arg ));
        BOOST_CHECK ( pos2 == string_ref::npos || pos1 < sr1.size ());
        if ( pos2 != string_ref::npos ) {
          for ( size_t i = sr1.size () -1 ; i > pos2; --i )
            BOOST_CHECK ( sr1[i] == *p );
          BOOST_CHECK ( sr1 [ pos2 ] != *p );
          }
         
        BOOST_CHECK ( pos2 != pos1 );
        ++p;
        }
        
    }

const char *test_strings [] = {
    "",
    "0",
    "abc",
    "AAA",  // all the same
    "adsfadadiaef;alkdg;aljt;j agl;sjrl;tjs;lga;lretj;srg[w349u5209dsfadfasdfasdfadsf",
    "abc\0asdfadsfasf",
    NULL
    };
    
int test_main( int , char* [] ) {
    const char **p = &test_strings[0];
    
    while ( *p != NULL ) {
        starts_with ( *p );
        ends_with ( *p );
        reverse ( *p );
        find ( *p );

        p++;
        }

    return 0;
    }
