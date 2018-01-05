/*******************************************************************\

Module: A Template Class for Graphs

Author: Daniel Kroening, kroening@kroening.com

\*******************************************************************/

#include "graph.h"

class test_nodet
{
public:
  int test;
};

class test_edget
{
public:
  int test2;
};

void test_graph()
{
  graph<> G1;
  
  graph<test_nodet> G2;
  G2.add_node();
  G2[0].test=2;
  
  graph<test_nodet, test_edget> G3;
  G3.add_node();
  G3.edge(0, 0).test2=3;
}
