#include "quadtree.h"
#include "quadtreenode.h"
#include <iostream>
#include <algorithm>
#include <set>

using namespace std;
using namespace glm;

QuadTree::QuadTree(BBox* bbox, size_t num_points_node) :
  m_num_points(0),
  depth(0),
  max_points_node(num_points_node)
{
  root_node = new QuadTreeNode(bbox, num_points_node);
}

QuadTree::~QuadTree()
{
  if(root_node)
    delete root_node;
  root_node = nullptr;
}

bool QuadTree::AddPoint(Vertex* p)
{
  int r_depth = root_node->AddPoint(p);
  if(r_depth == -1)
    return false;
  depth = r_depth;
  ++m_num_points;
  return true;
}

std::vector<Vertex*> QuadTree::GetPointsInRange(BBox *range)
{
  return root_node->GetPointsInRange(range);
}

void QuadTree::draw()
{
  root_node->draw();
}

void QuadTree::delEmptyLeaves()
{
  root_node->delEmptyLeaves();
}

std::vector<QuadTreeNode*> QuadTree::GetLeaves()
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

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void balance_tree(QuadTree* qt)
{
  if(qt == nullptr)
    return;

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

std::vector<QuadTreeNode*> get_populated_leaves(QuadTree* qt)
{
  vector<QuadTreeNode*> leaves;

  if(qt == nullptr)
    return leaves;

  leaves = qt->GetLeaves();

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

std::vector<QuadTreeNode*> get_first_nbrs(QuadTreeNode* node,
                                          std::vector<QuadTreeNode*> leaves)
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

std::vector<QuadTreeNode*> get_second_neighbors(QuadTreeNode* node,
                                                std::vector<QuadTreeNode*> leaves)
{
  vector<QuadTreeNode*> nbrs;

  if(node == nullptr || leaves.empty())
    return nbrs;

  //Getting the node's first neighbors.
  vector<QuadTreeNode*> fst_nbrs = get_first_nbrs(node, leaves);
  if(fst_nbrs.empty())
    return nbrs;

  //For each first neighbor, we get it's first neighbors. The second neighbors
  //should be in this set as well as the other first neighbors and the node.
  set<QuadTreeNode*> tmp_nbrs;
  for(size_t i = 0; i < fst_nbrs.size(); ++i) {
    vector<QuadTreeNode*> tmp_fst_nbrs = get_first_nbrs(fst_nbrs[i], leaves);
    tmp_nbrs.insert(tmp_fst_nbrs.begin(), tmp_fst_nbrs.end());
  }

  //Erasing the node and its first neighbors.
  //All the nodes left in the set are the second neighbors.
  tmp_nbrs.erase(node);
  for(size_t i = 0; i < fst_nbrs.size(); ++i) 
    tmp_nbrs.erase(fst_nbrs[i]);

  for(auto it = tmp_nbrs.begin(); it != tmp_nbrs.end(); ++it)
    nbrs.push_back(*it);

  return nbrs;
}

std::vector<QuadTreeNode*> get_third_neighbors(QuadTreeNode* node,
                                               std::vector<QuadTreeNode*> leaves)
{
  vector<QuadTreeNode*> nbrs;

  if(node == nullptr || leaves.empty())
    return nbrs;

  vector<QuadTreeNode*> fst_nbrs = get_first_nbrs(node, leaves);
  vector<QuadTreeNode*> scnd_nbrs = get_second_neighbors(node, leaves);
  if(fst_nbrs.empty() || scnd_nbrs.empty())
    return nbrs;

  set<QuadTreeNode*> tmp_nbrs;
  for(size_t i = 0; i < scnd_nbrs.size(); ++i) {
    vector<QuadTreeNode*> tmp_scnd_nbrs = get_first_nbrs(scnd_nbrs[i], leaves);
    tmp_nbrs.insert(tmp_scnd_nbrs.begin(), tmp_scnd_nbrs.end());
  }

  //Erasing the node and its first neighbors.
  //All the nodes left in the set are the second and third neighbors.
  tmp_nbrs.erase(node);
  for(size_t i = 0; i < fst_nbrs.size(); ++i) 
    tmp_nbrs.erase(fst_nbrs[i]);

  //Erasing the node's second neighbors.
  //All the nodes left in the set are the third neighbors.
  for(size_t i = 0; i < scnd_nbrs.size(); ++i) 
    tmp_nbrs.erase(scnd_nbrs[i]);

  for(auto it = tmp_nbrs.begin(); it != tmp_nbrs.end(); ++it)
    nbrs.push_back(*it);

  return nbrs;
}

bool node_depth_comp(QuadTreeNode* a, QuadTreeNode* b)
{
  return a->GetDepth() < b->GetDepth();
}

void split_nodes(std::vector<QuadTreeNode*> nodes)
{
  sort(nodes.begin(), nodes.end(), node_depth_comp);

  int max_depth = (*(nodes.end() - 1))->GetDepth();
  vector<QuadTreeNode*>::iterator node_it;

  for(node_it = nodes.begin(); node_it != nodes.end(); ++node_it)
    if((*node_it)->GetDepth() == max_depth)
      break;

  nodes.erase(node_it, nodes.end());

  queue<QuadTreeNode*> node_q;
  for(size_t i = 0; i < nodes.size(); ++i)
    node_q.push(nodes[i]);

  do {
    QuadTreeNode* curr_node = node_q.front();
    node_q.pop();

    curr_node->Split();
    int curr_depth = curr_node->GetDepth();

    //If the depth difference is too big, we must split the children as well.
    if((max_depth - curr_depth) > 1)
      for(int i = 0; i < 4; ++i)
        node_q.push(curr_node->GetChild(i));

  } while(!node_q.empty());
}

void enforce_corners(QuadTree* qt)
{
  if(qt == nullptr)
    return;

  vector<QuadTreeNode*> leaves = qt->GetLeaves();
  vector<QuadTreeNode*> pop_leaves = get_populated_leaves(qt);

  //BUG: When the refinement occurs, some populated leaf nodes will be split,
  //leading to inconsistensy in the structure and segfaults.
  for(auto it = pop_leaves.begin(); it != pop_leaves.end(); ++it) {

    //This node was a leaf, but in the neighborhood of another leaf that demanded
    //its division.
    if(!(*it)->IsLeaf()) {
      cout << "HEEEEEEEEEEEEEEEEEY!\n";
      continue;
    }

    Vertex* v = (*it)->GetVertex(0);
    vec2 v1, v2;

    Edge* e1 = v->edges[0];
    Edge* e2 = v->edges[1];
    
    v1 = normalize(e1->v->p - e1->u->p);
    v2 = normalize(e2->v->p - e2->u->p);

    if(degrees(acos(dot(v1, v2))) <= 60) {
      vector<QuadTreeNode*> fst_nbrs = get_first_nbrs(*it, leaves);
      fst_nbrs.push_back(*it);
      split_nodes(fst_nbrs);
    } else {
      vector<QuadTreeNode*> nbrs = get_first_nbrs(*it, leaves);
      nbrs.push_back(*it);

      vector<QuadTreeNode*> s_nbrs  = get_second_neighbors(*it, leaves);
      nbrs.insert(nbrs.end(), s_nbrs.begin(), s_nbrs.end());

      vector<QuadTreeNode*> t_nbrs  = get_third_neighbors(*it, leaves);
      nbrs.insert(nbrs.end(), t_nbrs.begin(), t_nbrs.end());
      split_nodes(nbrs);

//      int max_d = nbrs[0]->GetDepth();
//      for(size_t i = 1; i < nbrs.size(); ++i) {
//        int curr_d = nbrs[i]->GetDepth();
//        if(curr_d > max_d)
//          max_d = curr_d;
//      }

//      for(size_t i = 0; i < nbrs.size(); ++i) {
//        int curr_d = nbrs[i]->GetDepth();
//        if(curr_d < max_d)
//          nbrs[i]->Split();
//      }
    }
  }




}

