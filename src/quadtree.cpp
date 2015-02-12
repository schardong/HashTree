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
  //Sorting the nodes by their depths.
  sort(nodes.begin(), nodes.end(), node_depth_comp);

  int max_depth = (*(nodes.end() - 1))->GetDepth();
  vector<QuadTreeNode*>::iterator node_it;

  for(node_it = nodes.begin(); node_it != nodes.end(); ++node_it)
    if((*node_it)->GetDepth() == max_depth)
      break;

  //Erasing any nodes with the largest depth so that we may include only the
  //nodes that need to be split in a processing queue.
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

QuadTreeNode* track_pop_leaf(QuadTreeNode* root)
{
  assert(root != nullptr);

  QuadTreeNode* node = nullptr;
  queue<QuadTreeNode*> node_q;
  node_q.push(root);

  do {
    QuadTreeNode* curr_node = node_q.front();
    node_q.pop();

    if(curr_node->IsLeaf() && curr_node->GetNumPoints() > 0)
      node = curr_node;
    else if(!curr_node->IsLeaf())
      for(int i = 0; i < 4; ++i)
        node_q.push(curr_node->GetChild(i));

  } while(!node_q.empty() && !node);

  return node;
}

void enforce_corners(QuadTree* qt)
{
  assert(qt != nullptr);

  vector<QuadTreeNode*> leaves = qt->GetLeaves();
  vector<QuadTreeNode*> tmp_pop_leaves = get_populated_leaves(qt);
  queue<QuadTreeNode*> pop_leaves;

  for(size_t i = 0; i < tmp_pop_leaves.size(); ++i)
    pop_leaves.push(tmp_pop_leaves[i]);

  do {
    QuadTreeNode* curr_node = pop_leaves.front();
    pop_leaves.pop();

    //This node was a leaf, but in the neighborhood of another leaf that demanded
    //its division.
    if(!curr_node->IsLeaf()) {
      QuadTreeNode* pop_leaf = track_pop_leaf(curr_node);
      pop_leaves.push(pop_leaf);
      continue;
    }

    Vertex* v = curr_node->GetVertex(0);
    vec2 v1, v2;

    Edge* e1 = v->edges[0];
    Edge* e2 = v->edges[1];

    v1 = normalize(e1->v->p - e1->u->p);
    v2 = normalize(e2->v->p - e2->u->p);

    //If the angle between the edges of a vertex is less than 60 degrees, we
    //must check and eventualy split the node's first neighbors. Else, we must
    //do the same, but with the second and third neighbors as well.
    if(degrees(acos(dot(v1, v2))) <= 60) {
      vector<QuadTreeNode*> fst_nbrs = get_first_nbrs(curr_node, leaves);
      fst_nbrs.push_back(curr_node);
      split_nodes(fst_nbrs);
    } else {
      vector<QuadTreeNode*> nbrs = get_first_nbrs(curr_node, leaves);
      nbrs.push_back(curr_node);

      vector<QuadTreeNode*> s_nbrs  = get_second_neighbors(curr_node, leaves);
      nbrs.insert(nbrs.end(), s_nbrs.begin(), s_nbrs.end());

//      vector<QuadTreeNode*> t_nbrs  = get_third_neighbors(curr_node, leaves);
//      nbrs.insert(nbrs.end(), t_nbrs.begin(), t_nbrs.end());
      split_nodes(nbrs);
    }

  } while(!pop_leaves.empty());
}

bool pnpoly(glm::vec2 point, std::vector<glm::vec2> hull_vertices)
{
  int i, j, nvert = hull_vertices.size();
  bool c = false;

  for(i = 0, j = nvert - 1; i < nvert; j = i++)
    if(((hull_vertices[i].y >= point.y) != (hull_vertices[j].y >= point.y)) &&
        (point.x <= (hull_vertices[j].x - hull_vertices[i].x) * (point.y - hull_vertices[i].y) / (hull_vertices[j].y - hull_vertices[i].y) + hull_vertices[i].x)
      )
      c = !c;

  return c;
}

//queue<QuadTreeNode*> queue_out_leaves(vector<QuadTreeNode*> leaves,
//                                      vector<vec2> hull_vertices)
//{
//  //For each leaf node, we get its bounding box and test if any of the points
//  //intersects the polygon formed by the domain. If if does not, we mark the
//  //leaf for deletion by inserting it in a queue.
//  queue<QuadTreeNode*> to_del;
//
//  if(leaves.empty() || hull_vertices.empty())
//    return to_del;
//
//  size_t l_sz = leaves.size();
//  for(size_t i = 0; i < l_sz; ++i) {
//    BBox* bbox = leaves[i]->GetBBox();
//
//    bool a = pnpoly(bbox->GetCorner(0), hull_vertices);
//    bool b = pnpoly(bbox->GetCorner(1), hull_vertices);
//    bool c = pnpoly(bbox->GetCorner(2), hull_vertices);
//    bool d = pnpoly(bbox->GetCorner(3), hull_vertices);
//
//    if((a + b + c + d) < 3)
//      to_del.push(leaves[i]);
//    /*if(!a && !b && !c && !d)
//      to_del.push(leaves[i]);*/
//  }
//
//  return to_del;
//}

//void delete_out_nodes(QuadTree* qt, vector<Vertex*> hull_vertices)
//{
//  assert(qt != nullptr && !hull_vertices.empty());
//
//  vector<vec2> hull_coords;
//  size_t h_sz = hull_vertices.size();
//  for(size_t i = 0; i < h_sz; ++i)
//    hull_coords.push_back(hull_vertices[i]->p);
//
//  vector<QuadTreeNode*> leaves = qt->GetLeaves();
//  queue<QuadTreeNode*> to_del = queue_out_leaves(leaves, hull_coords);
//
//  cout << "Number of leaves to be deleted: " << to_del.size() << endl;
//
//  //Ideally, we should test if any of the nodes to be deleted contain a vertex,
//  //if so, we can do a couple of things:
//  // 1) Don't delete the node;
//  // 2) Find the nearest node inside the domain, assign the vertex to it and
//  //    delete the first node.
//
//  do {
//    QuadTreeNode* curr_node = to_del.front();
//    to_del.pop();
//
//    QuadTreeNode* parent = curr_node->GetParent();
//    int node_type = curr_node->GetNodeType();
//    
//    delete curr_node;
//    curr_node = nullptr;
//    parent->SetChild(node_type, nullptr);
//
//    if(parent->IsLeaf()) {
//      BBox* bbox = parent->GetBBox();
//
//      bool a = pnpoly(bbox->GetCorner(0), hull_coords);
//      bool b = pnpoly(bbox->GetCorner(1), hull_coords);
//      bool c = pnpoly(bbox->GetCorner(2), hull_coords);
//      bool d = pnpoly(bbox->GetCorner(3), hull_coords);
//
//      if((a + b + c + d) < 3)
//        to_del.push(parent);
//    }
//  } while(!to_del.empty());
//
//}
