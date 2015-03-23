#include <cstring>
#include <array>
#include <stack>
#include <iostream>
#include <GL/glut.h>

#include "quadtreenode.h"
#include "vertex.h"
#include "bbox.h"

static size_t g_quad_node_id = 0;

QuadTreeNode::QuadTreeNode()
{
  id = g_quad_node_id++;
  m_max_points = 64;
  m_max_depth = -1;
  m_depth = 0;
  m_node_type = ROOT;
}

QuadTreeNode::QuadTreeNode(BBox* box,
                           size_t max_npoints,
                           int max_depth,
                           NODE_TYPE nt,
                           std::vector<vertex *> p,
                           glm::vec3 color) :
  m_box(box), m_max_depth(max_depth), m_node_type(nt), m_points(p), m_max_points(max_npoints)
{
  id = g_quad_node_id++;
  m_depth = 0;
  m_color = color;
  m_parent = nullptr;
  memset(m_children, 0, sizeof(QuadTreeNode*) * 4);
}

QuadTreeNode::~QuadTreeNode()
{
  if(!IsLeaf()) {
    for(int i = 0; i < 4; i++) {
      delete m_children[i];
      m_children[i] = nullptr;
    }
  }
  else
    m_points.clear();

  delete m_box;
  m_box = nullptr;
  m_parent = nullptr;
}

void QuadTreeNode::Split(PATTERN_TYPE tp)
{
  //Instead of creating new vertices for the children's boxes, use the existing ones
  //and create only the ones that don't exist yet.
  using namespace std;
  using namespace glm;

  if(!IsLeaf())
    return;

  vector<vertex*> p_quads[4];
  array<BBox*, 4> bbox_quads;
  BBox* box = (BBox*) m_box;
  
  vertex v0 = box->GetCorner(0);
  vertex v1 = box->GetCorner(1);
  vertex v2 = box->GetCorner(2);
  vertex v3 = box->GetCorner(3);

  vertex e0_midp = vertex(0.5f * (v1.GetCoord() - v0.GetCoord()) + v0.GetCoord());
  vertex e1_midp = vertex(0.5f * (v2.GetCoord() - v1.GetCoord()) + v1.GetCoord());
  vertex e2_midp = vertex(0.5f * (v3.GetCoord() - v2.GetCoord()) + v2.GetCoord());
  vertex e3_midp = vertex(0.5f * (v0.GetCoord() - v3.GetCoord()) + v3.GetCoord());
  vertex midd = vertex(0.5f * (v2.GetCoord() - v0.GetCoord()) + v0.GetCoord());

  array<vertex, 4> vv0 = {v0, e0_midp, midd, e3_midp};
  array<vertex, 4> vv1 = {e0_midp, v1, e1_midp, midd};
  array<vertex, 4> vv2 = {e3_midp, midd, e2_midp, v3};
  array<vertex, 4> vv3 = {midd, e1_midp, v2, e2_midp};

  bbox_quads[SW] = new BBox(vv0);
  bbox_quads[SE] = new BBox(vv1);
  bbox_quads[NW] = new BBox(vv2);
  bbox_quads[NE] = new BBox(vv3);

  for(auto it = m_points.begin(); it != m_points.end(); ++it) {
    for(size_t i = 0; i < 4; ++i) {
      if(bbox_quads[i]->PointInBox((*it)->GetCoord())) {
        p_quads[i].push_back(*it);
        break;
      }
    }
  }

  for(int i = 0; i < 4; ++i) {
    m_children[i] = new QuadTreeNode(bbox_quads[i], m_max_points, m_max_depth, (NODE_TYPE)i, p_quads[i], m_color);
    m_children[i]->SetDepth(GetDepth() + 1);
    m_children[i]->SetParent(this);
  }
  m_points.clear();
}

int QuadTreeNode::AddPoint(vertex *p)
{
  if(!m_box->PointInBox(p->GetCoord()))
    return -1;

  if(IsLeaf()) {
    //If there is a depth bound to be respected, we test if the node's depth is
    //at such bound or if the number of points is bellow the threshold. If any
    //of these conditions hold, we add the point, if not, we split the node.
    if(GetMaxDepth() != -1) {
      if(GetDepth() == GetMaxDepth() || GetNumPoints() < GetMaxPoints()) {
        m_points.push_back(p);
        return GetDepth();
      } else if(GetDepth() < GetMaxDepth()) {
        Split();
      }
    } else {
      if(GetNumPoints() < GetMaxPoints()) {
        m_points.push_back(p);
        return GetDepth();
      } else {
        Split();
      }
    }    
  }

  //If the node is not a leaf, we try to add the point into one of its children.
  //The resulting depth is returned.
  int res_depth = GetDepth();
  for(size_t i = 0; i < 4; ++i) {
    if(m_children[i]->GetBBox()->PointInBox(p->GetCoord())) {
      int d = m_children[i]->AddPoint(p);
      res_depth = d > res_depth? d : res_depth;
    }
  }

  return res_depth;
}

std::vector<vertex *> QuadTreeNode::GetPointsInRange(BBox* range)
{
  using std::vector;
  using glm::vec2;

  vector<vertex*> p_range;

  if(!range->Intersect(*GetBBox()))
    return p_range;

  if(IsLeaf()) {
    for(auto it = m_points.begin(); it != m_points.end(); it++)
      if(range->PointInBox((*it)->GetCoord()))
        p_range.push_back(*it);
    return p_range;
  }

  for(size_t i = 0; i < 4; ++i) {
    vector<vertex*> tmp = m_children[i]->GetPointsInRange(range);
    if(!tmp.empty())
      p_range.insert(p_range.end(), tmp.begin(), tmp.end());
  }

  return p_range;
}

void QuadTreeNode::draw()
{
  glColor3f(m_color.r, m_color.g, m_color.b);
  m_box->draw();

  if(!IsLeaf())
    for(int i = 0; i < 4; ++i)
      if(m_children[i])
        m_children[i]->draw();
}

QuadTreeNode* QuadTreeNode::FindNeighbor(NBR_DIR dir)
{
  using namespace std;

  if(!IsLeaf() || m_node_type == ROOT)
    return nullptr;

  switch(dir) {
  case S:
    return south_nbr(this);
    break;
  case E:
    return east_nbr(this);
    break;
  case N:
    return north_nbr(this);
    break;
  case W:
    return west_nbr(this);
    break;
  }

  return nullptr;
}

QuadTreeNode* QuadTreeNode::north_nbr(QuadTreeNode* node)
{
  if(node->GetParent() == nullptr) //We arrived at the root.
    return nullptr;

  if(node->GetNodeType() >> 1 == 0) //SOUTH-(WEST/EAST) node
    return ((node->GetNodeType() == SE) ? node->GetParent()->GetChild(NE) : node->GetParent()->GetChild(NW));

  QuadTreeNode* tmp = north_nbr(node->GetParent());
  if(tmp == nullptr || tmp->IsLeaf())
    return tmp;

  return ((node->GetNodeType() == NW) ? tmp->GetChild(SW) : tmp->GetChild(SE));
}

QuadTreeNode* QuadTreeNode::south_nbr(QuadTreeNode *node)
{
  if(node->GetParent() == nullptr) //We arrived at the root.
    return nullptr;

  if(node->GetNodeType() >> 1 == 1) //NORTH-(WEST/EAST) node
    return (node->GetNodeType() == NE ? node->GetParent()->GetChild(SE) : node->GetParent()->GetChild(SW));

  QuadTreeNode* tmp = south_nbr(node->GetParent());
  if(tmp == nullptr || tmp->IsLeaf())
    return tmp;

  return (node->GetNodeType() == SW ? tmp->GetChild(NW) : tmp->GetChild(NE));
}

QuadTreeNode* QuadTreeNode::east_nbr(QuadTreeNode *node)
{
  if(node->GetParent() == nullptr) //We arrived at the root.
    return nullptr;

  if((node->GetNodeType() & 0x1) == 0) //(NORTH/SOUTH)-WEST node
    return (node->GetNodeType() == NW ? node->GetParent()->GetChild(NE) : node->GetParent()->GetChild(SE));

  QuadTreeNode* tmp = east_nbr(node->GetParent());
  if(tmp == nullptr || tmp->IsLeaf())
    return tmp;

  return (node->GetNodeType() == NE ? tmp->GetChild(NW) : tmp->GetChild(SW));
}

QuadTreeNode* QuadTreeNode::west_nbr(QuadTreeNode *node)
{
  if(node->GetParent() == nullptr) //We arrived at the root.
    return nullptr;

  if((node->GetNodeType() & 0x1) == 1) //(NORTH/SOUTH)-EAST node
    return (node->GetNodeType() == NE ? node->GetParent()->GetChild(NW) : node->GetParent()->GetChild(SW));

  QuadTreeNode* tmp = west_nbr(node->GetParent());
  if(tmp == nullptr || tmp->IsLeaf())
    return tmp;

  return (node->GetNodeType() == NW ? tmp->GetChild(NE) : tmp->GetChild(SE));
}
