#include <cstring>
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
    split_aabb();
    break;
  case RHOMBUS:
  default:
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

}
