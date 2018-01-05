/*******************************************************************\

Module: A Template Class for Graphs

Author: Daniel Kroening, kroening@kroening.com

\*******************************************************************/

#ifndef __GRAPH_H
#define __GRAPH_H

#include <list>
#include <stack>
#include <map>
#include <vector>

class empty_nodet
{
};

class empty_edget
{
};

template<class N=empty_nodet, class E=empty_edget>
class graph_nodet: public N
{
public:
  typedef E edget;
  typedef std::map<unsigned, edget> edgest;

  edgest in, out;
  bool visited;
  
  // for Dijkstra
  unsigned distance, previous;
  
  // for the connected subgraphs
  unsigned subgraph_nr;
  
  void add_in(unsigned n)
  {
    in.insert(std::pair<unsigned, edget>(n, edget()));
  }
  
  void add_out(unsigned n)
  {
    out.insert(std::pair<unsigned, edget>(n, edget()));
  }

  void erase_in(unsigned n)
  {
    in.erase(n);
  }
  
  void erase_out(unsigned n)
  {
    out.erase(n);
  }
};

template<class N, class E>
void intersection(
  const typename graph_nodet<N, E>::edgest &a,
  const typename graph_nodet<N, E>::edgest &b,
  typename graph_nodet<N, E>::edgest &dest)
{
  typename graph_nodet<N, E>::edgest::const_iterator
    it_a=a.begin(), 
    it_b=b.begin();

  while(it_a!=a.end() && it_b!=b.end())
  {
    if(*it_a==*it_b)
    {
      dest.insert(*it_a);
      it_a++;
      it_b++;
    }
    else if(*it_a<*it_b)
      it_a++;
    else // *it_a>*it_b
      it_b++;
  }
}
  
template<class N=empty_nodet, class E=empty_edget>
class graph
{
public:
  typedef graph_nodet<N, E> nodet;
  typedef typename nodet::edgest edgest;
  typedef std::vector<nodet> nodest;
  typedef typename nodet::edget edget;  

protected:
  nodest nodes;
  
public:
  unsigned add_node()
  {
    unsigned no=nodes.size();
    nodes.push_back(nodet());
    return no;
  }
  
  void swap(graph &other)
  {
    nodes.swap(other.nodes);
  }

  bool has_edge(unsigned i, unsigned j) const
  {
    return nodes[i].out.find(j)!=nodes[i].out.end();
  }

  inline const nodet &operator[](unsigned n) const
  {
    return nodes[n];
  }

  inline nodet &operator[](unsigned n)
  {
    return nodes[n];
  }
  
  inline void resize(unsigned s)
  {
    nodes.resize(s);
  }
  
  inline unsigned size() const
  {
    return nodes.size();
  }
  
  inline const edgest &in(unsigned n) const
  {
    return nodes[n].in;
  }

  inline const edgest &out(unsigned n) const
  {
    return nodes[n].out;
  }
  
  void add_edge(unsigned a, unsigned b)
  {
    nodes[a].add_out(b);
    nodes[b].add_in(a);
  }
  
  void remove_edge(unsigned a, unsigned b)
  {
    nodes[a].out.erase(a);
    nodes[b].in.erase(a);
  }
  
  edget &edge(unsigned a, unsigned b)
  {
    return nodes[a].out[b];
  }

  void add_undirected_edge(unsigned a, unsigned b);
  void remove_undirected_edge(unsigned a, unsigned b);
  void remove_in_edges(unsigned n);
  void remove_out_edges(unsigned n);
  
  void remove_edges(unsigned n)
  {
    remove_in_edges(n);
    remove_out_edges(n);
  }
  
  void clear_visited();
  
  void clear()
  {
    nodes.clear();
  }
  
  typedef std::list<unsigned> patht;
  
  void shortest_path(
    unsigned src,
    unsigned dest,
    patht &path);
    
  void visit_reachable(unsigned src);
  
  void make_chordal();
  
  // return value: number of connected subgraphs
  unsigned connected_subgraphs();
    
protected:
  void compute_path(unsigned dest, patht &path);
};

/*******************************************************************\

Function: graph::add_undirected_edge

  Inputs:

 Outputs:

 Purpose:

\*******************************************************************/

template<class N, class E>
void graph<N, E>::add_undirected_edge(unsigned a, unsigned b)
{
  assert(a<nodes.size());
  assert(b<nodes.size());
  nodet &na=nodes[a];
  nodet &nb=nodes[b];
  na.add_out(b);
  nb.add_out(a);
  na.add_in(b);
  nb.add_in(a);
}

/*******************************************************************\

Function: graph::remove_undirected_edge

  Inputs:

 Outputs:

 Purpose:

\*******************************************************************/
 
template<class N, class E>
void graph<N, E>::remove_undirected_edge(unsigned a, unsigned b)
{
  nodet &na=nodes[a];
  nodet &nb=nodes[b];
  na.out.erase(b);
  nb.out.erase(a);
  na.in.erase(b);
  nb.in.erase(a);
}

/*******************************************************************\

Function: graph::remove_in_edges

  Inputs:

 Outputs:

 Purpose:

\*******************************************************************/

template<class N, class E>
void graph<N, E>::remove_in_edges(unsigned n)
{
  nodet &node=nodes[n];
  
  // delete all incoming edges
  for(typename edgest::const_iterator
      it=node.in.begin();
      it!=node.in.end();
      it++)
    nodes[*it].out.erase(n);
    
  node.in.clear();
}

/*******************************************************************\

Function: graph::remove_out_edges

  Inputs:

 Outputs:

 Purpose:

\*******************************************************************/

template<class N, class E>
void graph<N, E>::remove_out_edges(unsigned n)
{
  nodet &node=nodes[n];
  
  // delete all outgoing edges
  for(typename edgest::const_iterator
      it=node.out.begin();
      it!=node.out.end();
      it++)
    nodes[*it].in.erase(n);
  
  node.out.clear();
}

/*******************************************************************\

Function: nodet::clear_visited

  Inputs:

 Outputs:

 Purpose:

\*******************************************************************/

template<class N, class E>
void graph<N, E>::clear_visited()
{
  for(typename nodest::iterator
      it=nodes.begin();
      it!=nodes.end();
      it++)
    it->visited=false;
}

/*******************************************************************\

Function: graph::shortest_path

  Inputs:

 Outputs:

 Purpose:

\*******************************************************************/

template<class N, class E>
void graph<N, E>::shortest_path(
  unsigned src,
  unsigned dest,
  patht &path)
{
  // initialization
  for(typename nodest::iterator it=nodes.begin();
      it!=nodes.end();
      it++)
  {
    it->visited=false;
    it->distance=(unsigned)(-1);
  }

  nodes[src].distance=0;
  nodes[src].visited=true;

  // does BFS, not Dijkstra
  // we hope the graph is sparse
  std::vector<unsigned> frontier_set, new_frontier_set;
  
  frontier_set.reserve(nodes.size());

  frontier_set.push_back(src);
  
  unsigned distance=0;
  bool found=false;
  
  while(!frontier_set.empty() && !found)
  {
    distance++;
  
    new_frontier_set.clear();
    new_frontier_set.reserve(nodes.size());
  
    for(std::vector<unsigned>::const_iterator
        f_it=frontier_set.begin();
        f_it!=frontier_set.end() && !found;
        f_it++)
    {
      unsigned i=*f_it;
      nodet &n=nodes[i];
      
      // do all neighbors
      for(typename edgest::iterator
          o_it=n.out.begin();
          o_it!=n.out.end() && !found;
          o_it++)
      {
        nodet &o=nodes[o_it->first];
      
        if(!o.visited)
        {
          o.distance=distance;
          o.previous=i;
          o.visited=true;

          if(o_it->first==dest)
            found=true;
          else
            new_frontier_set.push_back(o_it->first);
        }
      }
    }
    
    frontier_set.swap(new_frontier_set);
  }
  
  // compute path
  compute_path(dest, path);
}

/*******************************************************************\

Function: graph::visit_reachable

  Inputs:

 Outputs:

 Purpose:

\*******************************************************************/

template<class N, class E>
void graph<N, E>::visit_reachable(unsigned src)
{
  // DFS

  std::stack<unsigned> s;
  s.push(src);

  while(!s.empty())
  {
    unsigned n=s.top();
    s.pop();

    nodet &node=nodes[n];
    node.visited=true;

    for(typename edgest::const_iterator
        it=node.out.begin();
        it!=node.out.end();
        it++)
      if(!nodes[*it].visited)
        s.push(*it);
  }
}

/*******************************************************************\

Function: graph::connected_subgraphs

  Inputs:

 Outputs:

 Purpose:

\*******************************************************************/

template<class N, class E>
unsigned graph<N, E>::connected_subgraphs()
{
  unsigned subgraph_nr=0;

  for(unsigned src=0; src<size(); src++)
  {
    if(nodes[src].visited) continue;

    // DFS

    std::stack<unsigned> s;
    s.push(src);

    while(!s.empty())
    {
      unsigned n=s.top();
      s.pop();

      nodet &node=nodes[n];
      node.visited=true;
      node.subgraph_nr=subgraph_nr;

      for(typename edgest::const_iterator
          it=node.out.begin();
          it!=node.out.end();
          it++)
        if(!nodes[*it].visited)
          s.push(*it);
    }

    subgraph_nr++;
  }

  return subgraph_nr;
}

/*******************************************************************\

Function: graph::compute_path

  Inputs:

 Outputs:

 Purpose:

\*******************************************************************/

template<class N, class E>
void graph<N, E>::compute_path(
  unsigned dest,
  patht &path)
{
  // walk towards 0-distance node
  path.clear();
  
  // reachable at all?
  if(nodes[dest].distance==(unsigned)(-1))
    return; // nah

  while(true)
  {
    path.push_front(dest);
    
    nodet &n=nodes[dest];
    if(n.distance==0) break; // we are there

    dest=n.previous;
  }
}

/*******************************************************************\

Function: graph::make_chordal

  Inputs:

 Outputs:

 Purpose:

\*******************************************************************/

template<class N, class E>
void graph<N, E>::make_chordal()
{
  graph tmp(*this);

  // This assumes an undirected graph.
  // 1. remove all nodes in tmp, reconnecting the remaining ones
  // 2. the chordal graph is the old one plus the new edges

  for(unsigned i=0; i<tmp.size(); i++)
  {
    const nodet &n=tmp[i];

    // connect all the nodes in n.out with each other

    for(typename edgest::const_iterator
        it1=n.out.begin();
        it1!=n.out.end();
        it1++)
      for(typename edgest::const_iterator
          it2=n.out.begin();
          it2!=n.out.end();
          it2++)
      {
        if(*it1!=*it2)
        {
          tmp.add_undirected_edge(*it1, *it2);
          this->add_undirected_edge(*it1, *it2);
        }
      }

    // remove node from tmp graph
    tmp.remove_edges(i);
  }
}

#endif
