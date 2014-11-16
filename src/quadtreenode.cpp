#include <cstring>
#include "quadtreenode.h"

static size_t g_quad_node_id = 0;

QuadTreeNode::QuadTreeNode(size_t max_npoints,
                           AABB* box,
                           std::vector<glm::vec2> p) :
  bbox(box), points(p), max_points(max_npoints)
{
  id = g_quad_node_id++;
  depth = 0;
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
}

void QuadTreeNode::Split()
{
  using std::vector;
  using glm::vec2;

  if(!IsLeaf() || points.size() < max_points)
    return;

  vector<vec2> p_quads[4];
  AABB* bbox_quads[4];
  double e = bbox->edge_sz / 2;
  double cx = bbox->bl_corner.x;
  double cy = bbox->bl_corner.y;

  bbox_quads[0] = new AABB(bbox->bl_corner, e);
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
    children[i] = new QuadTreeNode(max_points, bbox_quads[i], p_quads[i]);
    children[i]->SetDepth(GetDepth() + 1);
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

std::vector<glm::vec2> QuadTreeNode::GetPointsInRange(AABB* range)
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
