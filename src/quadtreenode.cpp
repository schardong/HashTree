#include <cstring>
#include <array>
#include <GL/glut.h>
#include "quadtreenode.h"

static size_t g_quad_node_id = 0;

QuadTreeNode::QuadTreeNode()
{
  max_points = 64;
  box_type = AXIS_ALIGNED;
}

QuadTreeNode::QuadTreeNode(BBox* box,
                           BBOX_TYPE t,
                           size_t max_npoints,
                           std::vector<glm::vec2> p) :
  bbox(box), box_type(t), points(p), max_points(max_npoints)
{
  id = g_quad_node_id++;
  depth = 0;
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
  } else {
    points.clear();
  }

  delete bbox;
  bbox = nullptr;
  parent = nullptr;
}

void QuadTreeNode::Split()
{
  if(!IsLeaf() || points.size() < max_points)
    return;

  switch(box_type) {
  case AXIS_ALIGNED:
  default:
    split_aabb();
    break;
  case RHOMBUS:
    split_rhombus();
    break;
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

void QuadTreeNode::split_aabb()
{
  using std::vector;
  using std::array;
  using glm::vec2;

  vector<vec2> p_quads[4];
  array<AABB*, 4> bbox_quads;
  AABB* box = (AABB*) bbox;

  double e = box->GetEdgeSz() / 2;
  double cx = box->GetCorner().x;
  double cy = box->GetCorner().y;

  bbox_quads[0] = new AABB(box->GetCorner(), e);
  bbox_quads[1] = new AABB(vec2(cx + e, cy), e);
  bbox_quads[2] = new AABB(vec2(cx  + e, cy + e), e);
  bbox_quads[3] = new AABB(vec2(cx, cy + e), e);

  for(auto it = points.begin(); it != points.end(); ++it) {
    for(size_t i = 0; i < 4; ++i) {
      if(bbox_quads[i]->PointInBox(*it)) {
        p_quads[i].push_back(*it);
        break;
      }
    }
  }

  for(size_t i = 0; i < 4; ++i) {
    children[i] = new QuadTreeNode(bbox_quads[i], box_type, max_points, p_quads[i]);
    children[i]->SetDepth(GetDepth() + 1);
    children[i]->SetParent(this);
  }
}

void QuadTreeNode::split_rhombus()
{
  using std::vector;
  using std::array;
  using glm::vec2;

  vector<vec2> p_quads[4];
  array<Rhombus*, 4> bbox_quads;
  Rhombus* box = (Rhombus*) bbox;

  vec2 e0_midp = 0.5f * (box->GetCorner(1) - box->GetCorner(0)) + box->GetCorner(0);
  vec2 e1_midp = 0.5f * (box->GetCorner(2) - box->GetCorner(1)) + box->GetCorner(1);
  vec2 e2_midp = 0.5f * (box->GetCorner(3) - box->GetCorner(2)) + box->GetCorner(2);
  vec2 e3_midp = 0.5f * (box->GetCorner(0) - box->GetCorner(3)) + box->GetCorner(3);
  vec2 midd = 0.5f * (box->GetCorner(2) - box->GetCorner(0)) + box->GetCorner(0);

  array<vec2, 4> v0 = {box->GetCorner(0), e0_midp, midd, e3_midp};
  array<vec2, 4> v1 = {e0_midp, box->GetCorner(1), e1_midp, midd};
  array<vec2, 4> v2 = {midd, e1_midp, box->GetCorner(2), e2_midp};
  array<vec2, 4> v3 = {e3_midp, midd, e2_midp, box->GetCorner(3)};

  bbox_quads[0] = new Rhombus(v0);
  bbox_quads[1] = new Rhombus(v1);
  bbox_quads[2] = new Rhombus(v2);
  bbox_quads[3] = new Rhombus(v3);

  for(auto it = points.begin(); it != points.end(); ++it) {
    for(size_t i = 0; i < 4; ++i) {
      if(bbox_quads[i]->PointInBox(*it)) {
        p_quads[i].push_back(*it);
        break;
      }
    }
  }

  for(size_t i = 0; i < 4; ++i) {
    children[i] = new QuadTreeNode(bbox_quads[i], box_type, max_points, p_quads[i]);
    children[i]->SetDepth(GetDepth() + 1);
    children[i]->SetParent(this);
  }
}

void QuadTreeNode::draw_aabb()
{
}

void QuadTreeNode::draw_rhombus()
{
  Rhombus* box = (Rhombus*)bbox;

  std::array<glm::vec2, 4> corners;

  for(int i = 0; i < 4; ++i)
    corners[i] = box->GetCorner(i);

  glBegin(GL_QUADS);
    glVertex2f(corners[0].x, corners[0].y);
    glVertex2f(corners[1].x, corners[1].y);
    glVertex2f(corners[2].x, corners[2].y);
    glVertex2f(corners[3].x, corners[3].y);
  glEnd();
}

void QuadTreeNode::draw()
{
  switch(box_type) {
  case AXIS_ALIGNED:
  default:
    draw_aabb();
    break;
  case RHOMBUS:
    draw_rhombus();
    break;
  }

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
