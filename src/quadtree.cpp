#include "quadtree.h"
#include "quadtreenode.h"
#include <iostream>
#include <algorithm>
#include <set>
#include <cstdlib>
#include <cstring>

using namespace std;
using namespace glm;

QuadTree::QuadTree(BBox* bbox, size_t num_points_node, int max_depth, std::vector<vertex *> p) :
  m_num_points(0),
  m_depth(0),
  m_max_depth(max_depth),
  m_max_points_node(num_points_node)
{
  m_root_node = new QuadTreeNode(bbox, num_points_node, max_depth, ROOT, p, glm::vec3(1, 0, 0));
}

QuadTree::~QuadTree()
{
  if(GetRoot())
    delete m_root_node;
  m_root_node = nullptr;
}

bool QuadTree::AddPoint(vertex *p)
{
  int r_depth = m_root_node->AddPoint(p);
  if(r_depth == -1)
    return false;
  m_depth = r_depth;
  ++m_num_points;
  return true;
}

std::vector<vertex *> QuadTree::GetPointsInRange(BBox *range)
{
  return m_root_node->GetPointsInRange(range);
}

void QuadTree::draw()
{
  m_root_node->draw();
}

std::vector<QuadTreeNode*> QuadTree::GetLeaves(int level)
{
  if(level <= -1)
    return get_all_leaves();
  return get_leaves(level);
}

std::vector<QuadTreeNode*> QuadTree::GetUnconformingLeaves(int level)
{
  vector<QuadTreeNode*> lvl_leaves = GetLeaves(level);

  if(lvl_leaves.empty())
    return lvl_leaves;

  vector<QuadTreeNode*> un_leaves;

  size_t l_sz = lvl_leaves.size();
  for(size_t i = 0; i < l_sz; ++i) {
    for(int j = 0; j < 4; ++j) {
      QuadTreeNode* nbr = lvl_leaves[i]->FindNeighbor((NBR_DIR)j);
      if(nbr == nullptr)
        continue;
      if(!nbr->IsLeaf() || nbr->GetDepth() != lvl_leaves[i]->GetDepth()) {
        un_leaves.push_back(lvl_leaves[i]);
        break;
      }
    }
  }

  return un_leaves;
}

std::vector<QuadTreeNode*> QuadTree::GetAllNodes()
{
  vector<QuadTreeNode*> nodes;

  queue<QuadTreeNode*> node_q;
  node_q.push(GetRoot());

  do {
    QuadTreeNode* curr_node = node_q.front();
    node_q.pop();

    if(!curr_node)
      continue;

    nodes.push_back(curr_node);
    if(!curr_node->IsLeaf())
      for(int i = 0; i < 4; ++i)
        node_q.push(curr_node->GetChild(i));

  } while(!node_q.empty());
  
  return nodes;
}

std::vector<QuadTreeNode*> QuadTree::get_all_leaves()
{
  vector<QuadTreeNode*> leaves;
  queue<QuadTreeNode*> bfs;
  bfs.push(GetRoot());

  do {
    QuadTreeNode* curr_node = bfs.front();
    bfs.pop();

    if(curr_node->IsLeaf()) {
      leaves.push_back(curr_node);
      continue;
    }

    for(size_t i = 0; i < 4; ++i)
      bfs.push(curr_node->GetChild(i));

  } while(!bfs.empty());

  return leaves;
}

std::vector<QuadTreeNode*> QuadTree::get_leaves(int level)
{
  vector<QuadTreeNode*> leaves;
  queue<QuadTreeNode*> bfs;
  bfs.push(GetRoot());

  int curr_level = 0;
  do {
    QuadTreeNode* curr_node = bfs.front();
    bfs.pop();
    curr_level = curr_node->GetDepth();

    if(curr_node->IsLeaf()) {
      if(curr_level == level)
        leaves.push_back(curr_node);
      continue;
    }

    for(size_t i = 0; i < 4; ++i)
      bfs.push(curr_node->GetChild(i));

  } while(!bfs.empty() && curr_level <= level);

  return leaves;
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void balance_tree(QuadTree* qt)
{
  assert(qt != nullptr);

  vector<QuadTreeNode*> leaves = qt->GetLeaves();

  for(size_t i = 0; i < leaves.size(); ++i) {
    QuadTreeNode* curr_node = leaves[i];

    for(int j = 0; j < 4; ++j) {
      QuadTreeNode* nbr = curr_node->FindNeighbor((NBR_DIR)j);
      if(nbr == nullptr)
        continue;

      int diff = curr_node->GetDepth() - nbr->GetDepth();
      if(abs(diff) > 1) {
        QuadTreeNode* tmp;
        if(diff > 0)
          tmp = nbr;
        else
          tmp = curr_node;

        tmp->Split();
        for(int k = 0; k < 4; ++k)
          leaves.push_back(tmp->GetChild(k));
      }
    }
  }
}

bool leaf_numpoints_comp(QuadTreeNode* a, QuadTreeNode* b)
{
  return a->GetNumPoints() > b->GetNumPoints();
}

vector<QuadTreeNode*> get_populated_leaves(QuadTree* qt, int level)
{
  vector<QuadTreeNode*> leaves;

  if(qt == nullptr)
    return leaves;

  leaves = qt->GetLeaves(level);

  //Sorting the leaves by the number of points in them.
  sort(leaves.begin(), leaves.end(), leaf_numpoints_comp);

  //Finding the first empty leaf.
  vector<QuadTreeNode*>::iterator it;
  for(it = leaves.begin(); it != leaves.end(); ++it)
    if((*it)->GetNumPoints() == 0)
      break;

  //Removing the empty leaves from the set.
  leaves.erase(it, leaves.end());
  return leaves;
}

bool pnpoly(vertex point, std::vector<vertex> hull_vertices)
{
  int i, j, nvert = hull_vertices.size();
  bool c = false;

//  for(i = 0, j = nvert - 1; i < nvert; j = i++)
//    if(((hull_vertices[i].y >= point.y) != (hull_vertices[j].y >= point.y)) &&
//        (point.x <= (hull_vertices[j].x - hull_vertices[i].x) * (point.y - hull_vertices[i].y) / (hull_vertices[j].y - hull_vertices[i].y) + hull_vertices[i].x)
//      )
//      c = !c;

  return c;
}

vector<QuadTreeNode*> get_first_nbrs(QuadTreeNode* node,
                                     vector<QuadTreeNode*> leaves)
{
  vector<QuadTreeNode*> nbrs;

  if(node == nullptr || leaves.empty())
    return nbrs;

  BBox* node_box = node->GetBBox();
  for(size_t i = 0; i < leaves.size(); ++i) {
    if(node == leaves[i])
      continue;

    BBox* leaf_box = leaves[i]->GetBBox();
    if(leaf_box->Intersect(*node_box))
      nbrs.push_back(leaves[i]);
  }

  return nbrs;
}

vector<QuadTreeNode*> get_nbrs_vertex(QuadTreeNode* node,
                                      vector<QuadTreeNode*> leaves,
                                      int vertex_idx)
{
  vector<QuadTreeNode*> nbrs;
  if(node == nullptr || leaves.empty() || vertex_idx < 0 || vertex_idx > 3)
    return nbrs;

  vector<QuadTreeNode*> fst_nbrs = get_first_nbrs(node, leaves);
  BBox* node_box = node->GetBBox();

  for(auto it = fst_nbrs.begin(); it != fst_nbrs.end(); ++it) {
    BBox* box = (*it)->GetBBox();
    
    for(int i = 0; i < 4; ++i) {
      if(box->GetCorner(i).GetCoord() == node_box->GetCorner(vertex_idx).GetCoord()) {
        nbrs.push_back(*it);
        break;
      }
    }

  }

  return nbrs;
}
