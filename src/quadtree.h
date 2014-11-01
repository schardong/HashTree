#ifndef QUADTREE_H
#define QUADTREE_H

class QuadTreeNode;

class QuadTree
{
public:
  QuadTree();
  virtual ~QuadTree();

  virtual void AddPoint(glm::vec2 p);

private:
  const int maxPoints = 64;
  QuadTreeNode* root_node;
};

#endif // QUADTREE_H
