/* 2017. 07. 06. 
 * B-tree header
 * Nayoun Seo (puremint09@gmail.com)
 * 
 * TODO : node.h와 tree.h로 나눠야 할까?
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
  int num_key;
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

bool tree_insert(struct b_tree *tree, struct datum d);
bool check_leaf(struct b_node *node);
bool check_full(struct b_node *node);
bool check_same_key(struct b_node *node, int key);

bool split(struct b_node *cur, struct b_node *parent);
struct b_node *copy_node_half(struct b_node *cur, int from, int to);

struct b_node *next_node(struct b_node *node, int key);

struct datum *find(struct b_tree *tree, int key);
bool print_tree(struct b_tree *tree);

bool node_insert_datum(struct b_node *node, struct datum d, 
                       struct b_node *left, struct b_node *right);
bool node_delete_datum(struct b_node *node, int key);

bool replace_empty(struct b_node *cur, int index);