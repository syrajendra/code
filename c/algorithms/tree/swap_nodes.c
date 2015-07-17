#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>

struct tree {
	int data;
	struct tree *parent;
	struct tree *left;
	struct tree *right;
	int level; // to store level of each node
};
typedef struct tree tree_t;

#define SWAP_LINK(l, r) ({		\
			typeof(l) _tmp = l;	\
			l = r;				\
			r = _tmp;			\
		})


#define SWAP_3LINKS(f, s) {					\
			SWAP_LINK(f->left, s->left);	\
			SWAP_LINK(f->right, s->right);	\
			SWAP_LINK(f->parent, s->parent);\
		}

#define CHECK_UPDATE(a, b) ({				\
				if(a->parent->left == a)	\
					a->parent->left = b;	\
				else						\
					a->parent->right = b;	\
				})

typedef void (*swap_ptr_t) (tree_t **root, tree_t *ptr1, tree_t *ptr2);

tree_t *get_new_node(int val)
{
	tree_t *node 	= (tree_t *) malloc(sizeof(tree_t));
	node->parent	= NULL;
	node->data 		= val;
	node->left 		= NULL;
	node->right 	= NULL;
	return node;
}

tree_t *insert(tree_t *root, int val)
{
	if(!root) return get_new_node(val);
	tree_t *prev = NULL, *ptr = root;
	char type = ' ';
	while(ptr) {
		prev = ptr;
		if(val < ptr->data) {
			ptr = ptr->left;
			type = 'l';
		} else {
			ptr = ptr->right;
			type = 'r';
		}
	}
	if(type == 'l') {
		prev->left  = get_new_node(val);
		prev->left->parent = prev;
	} else {
		prev->right = get_new_node(val);
		prev->right->parent = prev;
	}
	return root;
}

void print_detailed_tree(tree_t *root)
{
	if(!root) return;
	int max_nodes = 64;
	int front = 0, rear = 0, i;
	tree_t **queue = (tree_t **) calloc(sizeof(tree_t *), max_nodes);
	while(root) {
		printf("Root=%6d (%9p) Left=%6d (%9p) Right=%6d (%9p) Parent=%6d (%9p)\n",
				root->data, root,
				(root->left)?root->left->data:-99999, root->left,
				(root->right)?root->right->data:-99999, root->right,
				(root->parent)?root->parent->data:-1, root->parent);
		if(rear+2 < max_nodes) {
			if(root->left) {
				queue[rear++] = root->left;
			}
			if(root->right) {
				queue[rear++] = root->right;
			}
			root = queue[front++];
		} else {
			max_nodes *= 2;
			queue = (tree_t **) realloc(queue, sizeof(tree_t *) * max_nodes); // realloc() does not initialize
			for(i=max_nodes/2; i<max_nodes;i++) queue[i] = NULL;
		}
	}
}

int get_max_level(tree_t *root)
{
	if(!root) return 0;
	int ll = get_max_level(root->left);
	int rl = get_max_level(root->right);
	if(ll && rl) {
		if(ll > rl)
			return ll;
		else
			return rl;
	}
	return root->level;
}

int num_slashes(int max_level, int level)
{
	return (pow(2, max_level-level)/max_level);
}

void print_level(tree_t **queue, int max_level, int level,  int *points)
{
	int i, k;
	int nodes = pow(2, level);
	int front = nodes - 1;
	int links = nodes * 2;
	int prev  = 0;
	int slash = num_slashes(max_level, level);
	//printf("\nNodes:%d front:%d links:%d max_level:%d slash:%d \n", nodes, front, links, max_level, slash);

	for(i=0; i<nodes; i++) {
		if(queue[front+i])
			printf("%*d", points[front+i] - prev, queue[front+i]->data);
		else if(!queue[front+i])
			printf("%*s", points[front+i] - prev, "N");
		prev = points[front+i];
	}
	printf("\n");

	for(k=1; k<=slash; k++) {
		prev = k;
		for(i=0; i<nodes; i++) {
			printf("%*s%*s", points[front+i]-prev, "/", k*2, "\\");
			prev = points[front+i]+k*2;

		}
		if(k<slash) printf("\n");
	}
	printf("\n");
	for(i=0, k=0; i<links; i+=2, k++) {
		points[nodes+front+i]   = points[front+k] - slash;
		points[nodes+front+i+1] = points[front+k] + slash + 1;
	}
}

int left_slash_count(int max_level)
{
	int tmp = 0;
	int count = 0;
	while(count<max_level) {
		tmp += num_slashes(max_level, count);
		count++;
	}
	return tmp;
}

void print_tree(char *msg, tree_t *root)
{
	printf("---------------------------------------------------------------------------------------\n");
	printf("%s\n", msg);
	if(root) {
		uint64_t total_nodes = 64;
		int front = 0, rear = 0, i, j, k;
		tree_t **queue = (tree_t **) calloc(sizeof(tree_t *), total_nodes);
		int max_level = get_max_level(root) + 1;
		int max_nodes = pow(2, max_level+1) - 1;
		if(root) {
			queue[rear++] = root;
			front++;
			while(rear < max_nodes) {
				if(rear+2 < total_nodes) {
					queue[rear++] = root ? root->left : NULL;
					queue[rear++] = root ? root->right: NULL;
					root = queue[front++];
				} else {
					total_nodes *= 2;
					queue = (tree_t **) realloc(queue, sizeof(tree_t *) * total_nodes); // realloc() does not initialize
					for(i=total_nodes/2; i<total_nodes;i++) queue[i] = NULL;
				}
			}

			front = 0;
			int *points = (int *) calloc(sizeof(int), total_nodes);
			int updated_level = max_level+2;
			points[0] = 1 + left_slash_count(updated_level);
			//printf("Max level: %d nodes: %d slash_count:%d leftnodes:%d middle: %d\n"
			//	, max_level, max_nodes, left_slash_count(updated_level), ((max_nodes+1)/4), points[0]);
			for(i=0; i<max_level; i++) print_level(queue, updated_level, i, points);
			//for(i=0; i<max_nodes; i++) printf("points[%d]=%d\n", i, points[i]);
			free(points);
		}
		free(queue);
	}
	printf("---------------------------------------------------------------------------------------\n");
}

tree_t *set_node_level(tree_t *root, int level)
{
	if(root) {
		root->level = level;
		set_node_level(root->left, level + 1);
		set_node_level(root->right, level + 1);
	}
	return root;
}

void swap_good(tree_t **root, tree_t *ptr1, tree_t *ptr2)
{
	if (ptr1 == ptr2) return;
	if (!ptr1->parent) { // ptr1 is root
		CHECK_UPDATE(ptr2, ptr1);
		*root = ptr2;
	} else if (!ptr2->parent) { // ptr2 is root
		CHECK_UPDATE(ptr1, ptr2);
		*root = ptr1;
	} else if (ptr1->parent == ptr2->parent) { // ptr1 & ptr2 are siblings
		SWAP_LINK(ptr1->parent->left, ptr2->parent->right);
	} else { // ptr1 & ptr2 are non root & are not siblings
		// update parents of a & b
		CHECK_UPDATE(ptr1, ptr2);
		CHECK_UPDATE(ptr2, ptr1);
	}
	// interchange parent, left & right
	SWAP_3LINKS(ptr1, ptr2);
}

void swap_bad(tree_t **root, tree_t *ptr1, tree_t *ptr2)
{
	if (ptr1 == ptr2) return;
	if (ptr1->parent && ptr2->parent) {
		if (ptr1->parent == ptr2->parent) { // ptr1 & ptr2 are siblings
			SWAP_LINK(ptr1->parent->left, ptr2->parent->right);
			SWAP_3LINKS(ptr1, ptr2);
			return;
		} else {
			if (ptr1->parent != ptr2 || ptr2->parent != ptr1) {
				if (ptr1->parent->left == ptr1) {
					if (ptr2->parent->left == ptr2)
						SWAP_LINK(ptr1->parent->left, ptr2->parent->left);
					else
						SWAP_LINK(ptr1->parent->left, ptr2->parent->right);
				} else {
					if (ptr2->parent->left == ptr2)
						SWAP_LINK(ptr1->parent->right, ptr2->parent->left);
					else
						SWAP_LINK(ptr1->parent->right, ptr2->parent->right);
				}
			} else {
				if (ptr1->parent == ptr2) {
					if (ptr2->parent->left == ptr2)
						SWAP_LINK(ptr2->parent->left, ptr2);
					else
						SWAP_LINK(ptr2->parent->right, ptr2);
					if (ptr2->left == ptr1)
						SWAP_LINK(ptr1->left, ptr2);
					else
						SWAP_LINK(ptr2->right, ptr2);
				} else {
					if (ptr1->parent->left == ptr1)
						SWAP_LINK(ptr1->parent->left, ptr1);
					else
						SWAP_LINK(ptr1->parent->right, ptr1);
					if (ptr1->left == ptr2)
						SWAP_LINK(ptr2->left, ptr1);
					else
						SWAP_LINK(ptr1->right, ptr1);
				}
			}
		}
		SWAP_3LINKS(ptr1, ptr2);
	} else {
		if (ptr1->left != ptr2 && ptr1->right != ptr2 &&
			ptr2->left != ptr1 && ptr2->right != ptr1) {
			SWAP_LINK(ptr1->left,  ptr2->left);
			SWAP_LINK(ptr1->right, ptr2->right);
			if (!ptr1->parent) { // ptr1 is root
				if (ptr2->parent->left == ptr2)
					SWAP_LINK(ptr2->parent->left, ptr1);
				else
					SWAP_LINK(ptr2->parent->right, ptr1);
				*root = ptr2;
			} else { // ptr2 is root
				if (ptr1->parent->left == ptr1)
					SWAP_LINK(ptr1->parent->left, ptr2);
				else
					SWAP_LINK(ptr1->parent->right, ptr2);
				*root = ptr1;
			}
			SWAP_LINK(ptr1->parent, ptr2->parent);
		} else { //
			if (!ptr1->parent)
				*root = ptr2;
			else
				*root = ptr1;
			SWAP_LINK(ptr1->parent, ptr2->parent);
			if (ptr1->left == ptr2) {
				ptr1->left = ptr2->left;
				ptr2->left = ptr1;
				SWAP_LINK(ptr2->right, ptr1->right);
			} else if (ptr1->right == ptr2) {
				ptr1->right = ptr2->right;
				ptr2->right = ptr1;
				SWAP_LINK(ptr2->left, ptr1->left);
			} else if (ptr2->left == ptr1 ) {
				ptr2->left = ptr1->left;
				ptr1->left = ptr2;
				SWAP_LINK(ptr1->right, ptr2->right);
			} else if (ptr2->right == ptr1) {
				ptr2->right = ptr1->right;
				ptr1->right = ptr2;
				SWAP_LINK(ptr1->left, ptr2->left);
			}
		}
	}
}

tree_t *get_node(tree_t *root, int val)
{
	tree_t *ret;
	if(root) {
		if(val == root->data) return root;
		if(ret = get_node(root->left, val)) return ret;
		if(ret = get_node(root->right, val)) return ret;
	}
	return NULL;
}

void inorder_recursive(tree_t *root) // DLR
{
	if(root) {
		inorder_recursive(root->left);
		printf(" %d ", root->data);
		inorder_recursive(root->right);
	}
}

tree_t *delete_all_nodes(tree_t *root)
{
	if(root) {
		if(root->left) delete_all_nodes(root->left);
		if(root->right) delete_all_nodes(root->right);
		free(root);
		root = NULL;
	}
	return root;
}

void testcase0(tree_t *root, swap_ptr_t swap)
{
	print_tree("Tree", root);
	//print_detailed_tree(root);
	//inorder_recursive(root);
	printf("\n");
	printf("Swap root's left & right child\n");
	tree_t *ptr1 = get_node(root, 2);
	tree_t *ptr2 = get_node(root, 12);
	printf("Swap Node1: %d Node2: %d\n", ptr1->data, ptr2->data);
	swap(&root, ptr1, ptr2);
	set_node_level(root, 0);
	print_tree("Swaped Tree", root);
	//print_detailed_tree(root);
	delete_all_nodes(root);
}

void testcase1(tree_t *root, swap_ptr_t swap)
{
	print_tree("Tree", root);
	printf("Swap nodes which are not connected\n");
	tree_t *ptr1 = get_node(root, 2);
	tree_t *ptr2 = get_node(root, 21);
	printf("Swap Node1: %d Node2: %d\n", ptr1->data, ptr2->data);
	swap(&root, ptr1, ptr2);
	set_node_level(root, 0);
	print_tree("Swaped Tree", root);
	delete_all_nodes(root);
}

void testcase2(tree_t *root, swap_ptr_t swap)
{
	print_tree("Tree", root);
	printf("Swap nodes which are connected\n");
	tree_t *ptr1 = get_node(root, 2);
	tree_t *ptr2 = get_node(root, 4);
	printf("Swap Node1: %d Node2: %d\n", ptr1->data, ptr2->data);
	swap(&root, ptr1, ptr2);
	set_node_level(root, 0);
	print_tree("Swaped Tree", root);
	delete_all_nodes(root);
}

void testcase3(tree_t *root, swap_ptr_t swap)
{
	print_tree("Tree", root);
	printf("Swap root with its right child\n");
	tree_t *ptr1 = get_node(root, 5);
	tree_t *ptr2 = get_node(root, 12);
	printf("Swap Node1: %d Node2: %d\n", ptr1->data, ptr2->data);
	swap(&root, ptr1, ptr2);
	set_node_level(root, 0);
	print_tree("Swaped Tree", root);
	delete_all_nodes(root);
}

void testcase4(tree_t *root, swap_ptr_t swap)
{
	print_tree("Tree", root);
	printf("Swap root with its left child\n");
	tree_t *ptr1 = get_node(root, 5);
	tree_t *ptr2 = get_node(root, 2);
	printf("Swap Node1: %d Node2: %d\n", ptr1->data, ptr2->data);
	swap(&root, ptr1, ptr2);
	set_node_level(root, 0);
	print_tree("Swaped Tree", root);
	delete_all_nodes(root);
}

void testcase5(tree_t *root, swap_ptr_t swap)
{
	print_tree("Tree", root);
	printf("Swap root with non child\n");
	tree_t *ptr1 = get_node(root, 5);
	tree_t *ptr2 = get_node(root, 3);
	printf("Swap Node1: %d Node2: %d\n", ptr1->data, ptr2->data);
	swap(&root, ptr1, ptr2);
	set_node_level(root, 0);
	print_tree("Swaped Tree", root);
	delete_all_nodes(root);
}

tree_t *create_tree()
{
	tree_t *root = NULL;
	root = insert(root, 5);
	root = insert(root, 2);
	root = insert(root, 12);
	root = insert(root, 4);
	root = insert(root, 3);
	root = insert(root, 9);
	root = insert(root, 21);
	root = insert(root, 19);
	root = insert(root, 25);
	set_node_level(root, 0);
	return root;
}

int main(int argc, char *argv[])
{
	swap_ptr_t func_bad = swap_bad;
	swap_ptr_t func_good = swap_good;
	//testcase0(create_tree(), func_bad);
	testcase0(create_tree(), func_good);

	//testcase1(create_tree(), func_bad);
	//testcase1(create_tree(), func_good);

	//testcase2(create_tree(), func_bad);
	//testcase2(create_tree(), func_good);

	//testcase3(create_tree(), func_bad);
	//testcase3(create_tree(), func_good);

	//testcase4(create_tree(), func_bad);
	//testcase4(create_tree(), func_good);

	//testcase5(create_tree(), func_bad);
	//testcase5(create_tree(), func_good);
	return 0;
}
