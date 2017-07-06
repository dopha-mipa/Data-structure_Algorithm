/* 2017. 07. 06. 
 * B-tree header
 * Nayoun Seo (puremint09@gmail.com)
 */
#define MIN_KEY 3  // 5차 B-tree
#define MAX_KEY 2 * MIN_KEY - 1

typedef enum bool {false, true} bool;

struct datum {
  int key;
  // int value;
};

struct b_node {
  struct datum *data;
  int num_data;
  struct b_node **children;
};

struct b_tree {
  struct b_node *root;
  int num_data;
  // int depth;  /* if tree is empty = -1, only root = 0. ++ when splitted */
  // int min_data;
  int max_order;  /* order of B-tree */
};

struct b_tree *tree_init();
struct b_node *realloc_node(struct b_node *cur, int dim);
// struct datum *create_datum(int key);

bool insert(struct b_tree *tree, struct datum d);

bool check_leaf(struct b_node *node);
bool check_full(struct b_node *node);
bool split(struct b_node *cur, struct b_node *parent);

struct datum *find(struct b_tree *tree, int key);
bool print_tree(struct b_tree *tree);
