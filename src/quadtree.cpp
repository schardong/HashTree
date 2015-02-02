#include "quadtree.h"
#include "quadtreenode.h"
#include <iostream>

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

bool QuadTree::AddPoint(glm::vec2 p)
{
  int r_depth = root_node->AddPoint(p);
  if(r_depth == -1)
    return false;
  depth = r_depth;
  ++m_num_points;
  return true;
}

std::vector<glm::vec2> QuadTree::GetPointsInRange(BBox *range)
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
  using namespace std;

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
  using namespace std;
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

bool leaf_comp(QuadTreeNode* a, QuadTreeNode* b)
{
  return a->GetNumPoints() > b->GetNumPoints();
}

std::vector<QuadTreeNode*> get_populated_leaves(QuadTree* qt)
{
  using namespace std;
  vector<QuadTreeNode*> leaves;

  if(qt == nullptr)
    return leaves;

  leaves = qt->GetLeaves();

  //Sorting the leaves by the number of points in them.
  sort(leaves.begin(), leaves.end(), leaf_comp);

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
  using namespace std;
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

void enforce_corners(QuadTree* qt)
{
  using namespace std;
  if(qt == nullptr)
    return;

  vector<QuadTreeNode*> leaves = qt->GetLeaves();
  vector<QuadTreeNode*> pop_leaves = get_populated_leaves(qt);

  vector<QuadTreeNode*> test_nbrs = get_first_nbrs(pop_leaves[0], leaves);
  cout << test_nbrs.size() << endl;

  cout << pop_leaves[0]->GetId() << endl << "  ";
  for(size_t i = 0; i < test_nbrs.size(); ++i) {
    cout << test_nbrs[i]->GetId() << " ";
  }

  cout << endl;
}
