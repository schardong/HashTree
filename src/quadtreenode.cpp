#include "quadtreenode.h"

static size_t g_quad_node_id = 0;

QuadTreeNode::QuadTreeNode(size_t max_npoints, AABB* box,
                           std::vector<glm::vec2> p) :
  bbox(box), points(p), max_points(max_npoints)
{
  id = g_quad_node_id++;
  depth = 0;
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

//  vector<vec2> v_nw, v_ne, v_sw, v_se;
//  AABB *b_nw, *b_ne, *b_sw, *b_se;
//  double e = bbox->edge_sz / 2;
//  double cx = bbox->bl_corner.x;
//  double cy = bbox->bl_corner.y;

//  b_sw = new AABB(bbox->bl_corner, e);
//  b_se = new AABB(vec2(cx + e, cy), e);
//  b_ne = new AABB(vec2(cx  + e, cy + e), e);
//  b_nw = new AABB(vec2(cx, cy + e), e);

//  for(auto it = points.begin(); it != points.end(); ++it) {
//    if(b_sw->PointInBox(*it)) v_sw.push_back(*it);
//    else if(b_se->PointInBox(*it)) v_se.push_back(*it);
//    else if(b_ne->PointInBox(*it)) v_ne.push_back(*it);
//    else if(b_nw->PointInBox(*it)) v_nw.push_back(*it);
//  }

//  children[0] = new QuadTreeNode(max_points, b_sw, v_sw);
//  children[1] = new QuadTreeNode(max_points, b_se, v_se);
//  children[2] = new QuadTreeNode(max_points, b_ne, v_ne);
//  children[3] = new QuadTreeNode(max_points, b_nw, v_nw);

//  for(size_t i = 0; i < 4; i++) {
//    children[i]->SetDepth(GetDepth() + 1);
//  }

  points.clear();
}

void QuadTreeNode::AddPoint(glm::vec2 p)
{
  if(!bbox->PointInBox(p))
    return;

  if(IsLeaf()) {
    if(points.size() < max_points) {
      points.push_back(p);
      return;
    } else {
      Split();
    }
  }

  for(size_t i = 0; i < 4; ++i) {
    if(children[i]->GetBBox()->PointInBox(p)) {
      children[i]->AddPoint(p);
      break;
    }
  }
}
