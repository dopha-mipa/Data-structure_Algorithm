/* 2017. 07. 06. 
 * B-tree header
 * Nayoun Seo (puremint09@gmail.com) */
typedef enum bool {false, true} bool;

struct datum {
  int key;
};

struct b_node {
  struct datum *data;
  int num_key;
  struct b_node **children;
};

struct b_tree {
  struct b_node *root;
  int num_data;
  /* if the maximum number of key a node can hold is 5, min_key should be 3 */
  int min_key;
  int max_key;
};

/* Init */
struct b_tree *tree_init();
struct b_node *node_init();

/* Insertion */
bool tree_insert(struct b_tree *tree, struct datum d);
bool node_check_same_key(struct b_node *node, int key);
bool node_check_full(struct b_node *node);
struct b_node *node_split(struct b_node *cur, struct b_node *parent);
struct b_node *node_copy_half(struct b_node *cur, int from, int to);
bool node_insert_datum(struct b_node *node, struct datum d, 
                       struct b_node *left, struct b_node *right);
struct b_node *tree_next_node(struct b_node *node, int key);

/* Deletion */
bool tree_delete(struct b_tree *tree, int key);
bool tree_borrow_key(struct b_node *parent, struct b_node *hungry);
struct b_node *tree_bind_node(struct b_node *parent, struct b_node *hungry);
int node_swap_key(struct b_node *node, int key);
bool node_delete_datum(struct b_node *node, int key, int known_index);
bool node_check_leaf(struct b_node *node);
bool datum_empty(struct b_node *cur, int index);

/* Search */
struct datum tree_find_datum(struct b_tree *tree, int key);
int node_find_key(struct b_node *node, int key);
int node_find_child_index(struct b_node *parent, struct b_node *child);

bool print_tree(struct b_node *cur, int depth);