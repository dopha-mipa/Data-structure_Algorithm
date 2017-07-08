/* 2017. 07. 06. 
 * B-tree header
 * Nayoun Seo (puremint09@gmail.com)
 * 
 * TODO : 메소드 명명법 통일하기, 함수 순서 정리하기 (.c)
 * (위상 정렬 써볼까 데헿)
 */
#define MIN_KEY 3  // 5차 B-tree
#define MAX_KEY 2 * MIN_KEY - 1

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
  int max_order;  /* order of B-tree */
};

struct b_tree *tree_init();
bool tree_insert(struct b_tree *tree, struct datum d);
struct b_node *tree_next_node(struct b_node *node, int key);

struct b_node *node_init();
bool node_insert_datum(struct b_node *node, struct datum d, 
                       struct b_node *left, struct b_node *right);
bool node_split(struct b_node *cur, struct b_node *parent);
struct b_node *node_copy_half(struct b_node *cur, int from, int to);
bool node_check_leaf(struct b_node *node);
bool node_check_full(struct b_node *node);
bool node_check_same_key(struct b_node *node, int key);

/* -------------- 미구현 -------------- */
struct datum tree_find_datum(struct b_tree *tree, int key);
int node_find_key(struct b_node *node, int key);
bool print_tree(struct b_tree *tree);

bool node_delete_datum(struct b_node *node, int key);

bool datum_empty(struct b_node *cur, int index);