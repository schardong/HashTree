#include <cstring>
#include <array>
#include <stack>
#include <iostream>
#include <GL/glut.h>
#include "quadtreenode.h"

static size_t g_quad_node_id = 0;

QuadTreeNode::QuadTreeNode()
{
  max_points = 64;
}

QuadTreeNode::QuadTreeNode(BBox* box,
                           size_t max_npoints,
                           NODE_TYPE nt,
                           glm::vec3 color,
                           std::vector<glm::vec2> p) :
  bbox(box), node_type(nt), points(p), max_points(max_npoints)
{
  id = g_quad_node_id++;
  depth = 0;
  m_color = color;
  parent = nullptr;
  memset(children, 0, sizeof(QuadTreeNode*) * 4);
}

QuadTreeNode::~QuadTreeNode()
{
  if(!IsLeaf()) {
    for(int i = 0; i < 4; i++) {
      delete children[i];
      children[i] = nullptr;
    }
  }
  else
    points.clear();

  delete bbox;
  bbox = nullptr;
  parent = nullptr;
}

void QuadTreeNode::Split()
{
  using namespace std;
  using namespace glm;

  if(!IsLeaf())
    return;

  vector<vec2> p_quads[4];
  array<BBox*, 4> bbox_quads;
  BBox* box = (BBox*) bbox;

  vec2 e0_midp = 0.5f * (box->GetCorner(1) - box->GetCorner(0)) + box->GetCorner(0);
  vec2 e1_midp = 0.5f * (box->GetCorner(2) - box->GetCorner(1)) + box->GetCorner(1);
  vec2 e2_midp = 0.5f * (box->GetCorner(3) - box->GetCorner(2)) + box->GetCorner(2);
  vec2 e3_midp = 0.5f * (box->GetCorner(0) - box->GetCorner(3)) + box->GetCorner(3);
  vec2 midd = 0.5f * (box->GetCorner(2) - box->GetCorner(0)) + box->GetCorner(0);

  array<vec2, 4> v0 = {box->GetCorner(0), e0_midp, midd, e3_midp};
  array<vec2, 4> v1 = {e0_midp, box->GetCorner(1), e1_midp, midd};
  array<vec2, 4> v2 = {e3_midp, midd, e2_midp, box->GetCorner(3)};
  array<vec2, 4> v3 = {midd, e1_midp, box->GetCorner(2), e2_midp};

  bbox_quads[SW] = new BBox(v0);
  bbox_quads[SE] = new BBox(v1);
  bbox_quads[NW] = new BBox(v2);
  bbox_quads[NE] = new BBox(v3);

  for(auto it = points.begin(); it != points.end(); ++it) {
    for(size_t i = 0; i < 4; ++i) {
      if(bbox_quads[i]->PointInBox(*it)) {
        p_quads[i].push_back(*it);
        break;
      }
    }
  }

  for(int i = 0; i < 4; ++i) {
    children[i] = new QuadTreeNode(bbox_quads[i], max_points, (NODE_TYPE)i, m_color, p_quads[i]);
    children[i]->SetDepth(GetDepth() + 1);
    children[i]->SetParent(this);
  }
  points.clear();
}

int QuadTreeNode::AddPoint(glm::vec2 p)
{
  if(!bbox->PointInBox(p))
    return -1;

  if(IsLeaf()) {
    if(points.size() < max_points) {
      points.push_back(p);
      return GetDepth();
    } else {
      Split();
    }
  }

  int res_depth = GetDepth();
  for(size_t i = 0; i < 4; ++i) {
    if(children[i]->GetBBox()->PointInBox(p)) {
      int d = children[i]->AddPoint(p);
      res_depth = d > res_depth? d : res_depth;
    }
  }

  return res_depth;
}

std::vector<glm::vec2> QuadTreeNode::GetPointsInRange(BBox* range)
{
  using std::vector;
  using glm::vec2;

  vector<vec2> p_range;

  if(!range->Intersect(*GetBBox()))
    return p_range;

  if(IsLeaf()) {
    for(auto it = points.begin(); it != points.end(); it++)
      if(range->PointInBox(*it))
        p_range.push_back(*it);
    return p_range;
  }

  for(size_t i = 0; i < 4; ++i) {
    vector<vec2> tmp = children[i]->GetPointsInRange(range);
    if(!tmp.empty())
      p_range.insert(p_range.end(), tmp.begin(), tmp.end());
  }

  return p_range;
}

void QuadTreeNode::draw()
{
  std::array<glm::vec2, 4> corners;

  glColor3f(m_color.r, m_color.g, m_color.b);

  for(int i = 0; i < 4; ++i)
    corners[i] = bbox->GetCorner(i);

  glBegin(GL_QUADS);
    glVertex2f(corners[0].x, corners[0].y);
    glVertex2f(corners[1].x, corners[1].y);
    glVertex2f(corners[2].x, corners[2].y);
    glVertex2f(corners[3].x, corners[3].y);
  glEnd();

  if(!IsLeaf())
    for(int i = 0; i < 4; ++i)
      if(children[i])
        children[i]->draw();
}

void QuadTreeNode::delEmptyLeaves()
{
  if(IsLeaf())
    return;
  for(int i = 0; i < 4; ++i) {
    if(children[i]->IsLeaf()) {
      if(children[i]->GetNumPoints() == 0) {
        delete children[i];
        children[i] = nullptr;
      }
    } else {
      children[i]->delEmptyLeaves();
    }
  }
}

QuadTreeNode* QuadTreeNode::FindNeighbor(NBR_DIR dir)
{
  using namespace std;

  if(!IsLeaf() || node_type == ROOT)
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

QuadTreeNode *QuadTreeNode::north_nbr(QuadTreeNode* node)
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

QuadTreeNode *QuadTreeNode::south_nbr(QuadTreeNode *node)
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

QuadTreeNode *QuadTreeNode::east_nbr(QuadTreeNode *node)
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

QuadTreeNode *QuadTreeNode::west_nbr(QuadTreeNode *node)
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
