#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <limits.h>
#include <math.h>
#include <time.h>

/*
Red-Black binary search tree properties:
	1) Every node is either red or black.
	2) The root is black.
	3) Every leaf contains NIL and is black.
	4) If a node is red, then both its children are black.
	5) For each node x in a RBT, all paths in sub-trees rooted at x contain the same number of black nodes.

Left-leaning Red-Black binary search tree properties:
	6) No node has two red links connected to it
	7) Every path from root to null has same number of black links
	8) Red links always lean left
*/
//#define DISABLE_COLOR_PRINT
#define RED 0
#define BLACK 1

// http://stackoverflow.com/questions/17288746/red-black-nodes-struct-alignment-in-linux-kernel
// Use "parent_color" last one bit to store color (Actually last two bits will be zero if align for 4 bytes)
// Rest of "parent_color" will be used to store parent address
struct rbt {
	unsigned long parent_color; // parent link color
	struct rbt *left;
	struct rbt *right;
	int data;
	uint64_t nodes_count; // to store number of nodes present including self
	int level; // to store level of each node
}  __attribute__((aligned(sizeof(long))));
typedef struct rbt rbt_t;

rbt_t *sentinel_node;

int is_sentinel_node(rbt_t *root)
{
	return (root == sentinel_node) && (root->left == sentinel_node) && (root->right == sentinel_node);
}

int is_nil_node(rbt_t *root)
{
	return (root != sentinel_node) && (root->left == sentinel_node) && (root->right == sentinel_node);
}

rbt_t *rbt_get_parent(rbt_t *node)
{
	return ((rbt_t *)(node->parent_color & ~3));
}

rbt_t *rbt_get_sibling(rbt_t *node)
{
	rbt_t *parent = rbt_get_parent(node);
	if(node == parent->left) 
		return parent->right;
	else
		return parent->left;
}

int rbt_get_color(rbt_t *node)
{
	return (int)(node->parent_color & 1);
}

void rbt_set_red(rbt_t *node)
{
	node->parent_color &= ~1;
}

void rbt_set_black(rbt_t *node)
{
	node->parent_color |= 1;
}

void rbt_set_parent(rbt_t *node, rbt_t *parent)
{
	//printf("Node: %d Parent: %d\n", node ? node->data : 0, parent ? parent->data : 0);	
	if(parent) 
		node->parent_color = (((unsigned long)parent) & ~3) | (node->parent_color & 1);
	else
		node->parent_color = 0 | (node->parent_color & 1);	
}

void rbt_set_color(rbt_t *node, int color)
{
	if(color == RED) rbt_set_red(node);
	else if(color == BLACK) rbt_set_black(node);
}

int is_color_red(rbt_t *node)
{
	return !(node->parent_color & 1);
}

int is_color_black(rbt_t *node)
{
	return node->parent_color & 1;
}

static uint64_t micro_second_clock()
{
        struct timespec  ts;
        struct tm       *tm;
        uint64_t   ret; // micro seconds
 
        clock_gettime(CLOCK_REALTIME, &ts);
        tm = localtime(&ts.tv_sec);
 
        //printf("Clock: \t  %d:%02d:%02d:[%d nanosecons] (~%d microseconds)\n", tm->tm_hour,
        //       tm->tm_min, tm->tm_sec, ts.tv_nsec,
        //       tm->tm_hour * 3600 * 1000 * 1000 + tm->tm_min * 60 * 1000 * 1000 +
        //       tm->tm_sec * 1000 * 1000 + ts.tv_nsec / 1000);
 
        ret = tm->tm_hour * 3600 * 1000 * 1000 + tm->tm_min * 60 * 1000 * 1000 + 
                tm->tm_sec * 1000 * 1000 + ts.tv_nsec / 1000;
 
        return ret;
}

rbt_t *rbt_find_minimum(rbt_t *ptr)
{
	while(!is_nil_node(ptr->left)) ptr = ptr->left;	
	return ptr;
}

rbt_t *rbt_find(rbt_t *root, int val)
{
	rbt_t *ret = NULL;
	if(is_nil_node(root)) return NULL;	
	if(val < root->data) {
		ret = rbt_find(root->left, val);
		if(ret) return ret;
	} else if(val > root->data) {
		ret = rbt_find(root->right, val);
		if(ret) return ret;
	} else return root;	
	return ret;
}

static void display_time(char *msg, uint64_t start, uint64_t stop)
{
        char precision[50];
        uint64_t result = stop - start;
        if (result > 1000000) {
                result = result/1000000;
                strcpy(precision, "seconds");                
                if(result > 60) {
                	//printf("%s %llu %s\n", msg, result, precision);                	
                	printf("%s %llumins %llusecs\n", msg, result/60, result%60);
                	return;
                }
        } else if (result > 1000) {
                result = result/1000;
                strcpy(precision, "milliseconds");
        } else {
               strcpy(precision, "microseconds");
        }
        printf("%s %llu %s\n", msg, result, precision);
}

int get_max_level(rbt_t *root)
{	
	if(is_sentinel_node(root)) return 0;
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

#ifdef DISABLE_COLOR_PRINT
#define CRED     "" 
#define CBLACK   "" 
#define CNONE    ""
#define CBLUE    ""
#define CGREEN   ""
#else
// 0 -> normal ; 1 -> bold ; 4 -> underlin ; 9 -> strike ;
// 30 -> BLACK ; 31 -> RED ; 32 -> GREEN ; 33 -> BROWN ; 34 -> BLUE ; 35 -> MAGENTA  ; 36 -> CYAN ; 37 -> GRAY
#define CRED     "\033[0;31m" 
#define CBLACK   "\033[0;30m" 
#define CNONE    "\033[0m"
#define CBLUE    "\033[0;34m"
#define CGREEN   "\033[0;32m"
#endif

int num_slashes(int max_level, int level)
{
	return (pow(2, max_level-level)/max_level);
}

void print_level(rbt_t **queue, int max_level, int level,  int *points)
{
	int i, k;
	int nodes = pow(2, level);
	int front = nodes - 1;
	int links = nodes * 2;
	int prev  = 0;
	int slash = num_slashes(max_level, level);
	//printf("\nNodes:%d front:%d links:%d max_level:%d slash:%d \n", nodes, front, links, max_level, slash);

	for(i=0; i<nodes; i++) {		
		if(!is_nil_node(queue[front+i]) && !is_sentinel_node(queue[front+i])) 
			printf("%s%*d%s", is_color_red(queue[front+i]) ? CRED : CBLACK, points[front+i] - prev, queue[front+i]->data, CNONE);
		else if(is_nil_node(queue[front+i])) 
			printf("%s%*s%s", CBLACK, points[front+i] - prev, "Nil", CNONE);
		else if(is_sentinel_node(queue[front+i])) 
			printf("%s%*s%s", CGREEN, points[front+i] - prev, "ST", CNONE);
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

void print_tree(char *msg, rbt_t *root)
{
	printf("---------------------------------------------------------------------------------------\n");
	printf("%s\n", msg);
	if(!is_nil_node(root)) {
		uint64_t total_nodes = 64;
		int front = 0, rear = 0, i, j, k;	
		rbt_t **queue = (rbt_t **) calloc(sizeof(rbt_t *), total_nodes);		
		int max_level = get_max_level(root);
		int max_nodes = pow(2, max_level+1) - 1;
		if(root) {
			queue[rear++] = root;
			front++;	
			while(rear < max_nodes) {		
				if(rear+2 < total_nodes) {
					queue[rear++] = root->left;
					queue[rear++] = root->right;			
					root = queue[front++];
				} else {			
					total_nodes *= 2;
					queue = (rbt_t **) realloc(queue, sizeof(rbt_t *) * total_nodes); // realloc() does not initialize
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

void print_detailed_tree(char *msg, rbt_t *root)
{
	printf("--------------------------------------------------------------------------------\n");
	printf("%s\n", msg);
	if(!is_nil_node(root)) {
		int max_nodes = 64;
		int front = 0, rear = 0, i;	
		rbt_t **queue = (rbt_t **) calloc(sizeof(rbt_t *), max_nodes);	
		while(front <= rear) {
			if(!is_nil_node(root))
				printf("Node=%5d ", root->data);
			else
				printf("Leaf=%5s ", "   --");
			
			if(!is_nil_node(root->left))
				if(!is_sentinel_node(root->left))
					printf("Left=%5d ", root->left->data);
				else
					printf("Left=%5s ", "  STN");
			else 
				printf("Left=%5s ", "  NIL");

			if(!is_nil_node(root->right))
				if(!is_sentinel_node(root->right))
					printf("Right=%5d ", root->right->data);
				else
					printf("Right=%5s ", "  STN");
			else
				printf("Right=%5s ", "  NIL");
			rbt_t *parent = rbt_get_parent(root);
			if(parent)
				printf("Parent=%5d ", parent->data);
			else
				printf("Parent=%5s ", "   --");			
			printf("Color=%5s NodeCount=%3llu Level=%d\n", is_color_red(root) ? "RED" : "BLACK"
								, root->nodes_count
								, root->level);
			if(rear+2 < max_nodes) {
				if(!is_sentinel_node(root->left)) {				
					queue[rear++] = root->left;
				}
				if(!is_sentinel_node(root->right)) {				
					queue[rear++] = root->right;
				}
				root = queue[front++];			
			} else {			
				max_nodes *= 2;
				queue = (rbt_t **) realloc(queue, sizeof(rbt_t *) * max_nodes); // realloc() does not initialize
				for(i=max_nodes/2; i<max_nodes;i++) queue[i] = NULL;
			}
		}
		free(queue);
	}
	printf("--------------------------------------------------------------------------------\n");	
}

rbt_t *rbt_get_nil_node(rbt_t *parent)
{
	rbt_t *node = (rbt_t *) malloc(sizeof(rbt_t));
	node->data  = -9999;
	node->level = 0;
	node->nodes_count = 0;
	node->left = sentinel_node;
	node->right= sentinel_node;		
	rbt_set_parent(node, parent);			
	rbt_set_black(node); // leaf or root nodes are always black
	return node;
}

rbt_t *rbt_get_new_node(int val, rbt_t *parent)
{
	rbt_t *node = (rbt_t *) malloc(sizeof(rbt_t));
	node->data = val;
	node->left = rbt_get_nil_node(node);
	node->right= rbt_get_nil_node(node);	
	if(parent) 
		rbt_set_parent(node, parent);
	else
		node->parent_color= 0;
	//printf("Node: %d Parent: %d\n", node->data, (parent) ? parent->data : 0);
	// color the new node always red because there is a possibility that 
	// insertion won't violate RBT rules at all	
	rbt_set_red(node); // not needed of this because default parent address's last two bits are ZERO always
	node->nodes_count = 1;
	node->level = 0;
	return node;
}

uint64_t rbt_size(rbt_t *root) 
{
	if(is_nil_node(root)) return 0;
	return root->nodes_count;
}

void rbt_swap_level(rbt_t *t1, rbt_t *t2)
{
	int level = t1->level;
	t1->level = t2->level;
	t2->level = level;
}

rbt_t *rbt_set_node_level(rbt_t *root, int level)
{
	if(!is_sentinel_node(root)) {
		root->level = level;
		rbt_set_node_level(root->left, level + 1);
		rbt_set_node_level(root->right, level + 1);
	}
	return root;
}

void rbt_recount_upward_nodes(rbt_t *child)
{
	//printf("Child: %d\n", child->data);
	rbt_t *parent = rbt_get_parent(child);
	while(parent) {
		//printf("Data:%d Count:%llu\n", parent->data, parent->nodes_count);
		if(parent->nodes_count > 1) parent->nodes_count--;
		parent = rbt_get_parent(parent);
	}
}

int rbt_check_black_height(rbt_t *root)
{
	if(is_nil_node(root)) return 1;
	else {
		int left_height = rbt_check_black_height(root->left);
		int right_height = rbt_check_black_height(root->right);
		//printf("left_height=%d right_height=%d\n", left_height, right_height);
		if (left_height && right_height && left_height != right_height) {
			printf("RBT black violation - Number of black nodes along the path are not same @level=%d left-bh=%d right-bh=%d\n"
							, root->level
							, left_height
							, right_height);
			//exit(1);
		}
		if (left_height && right_height)
			return is_color_red(root) ? left_height : left_height + 1;
		else
			return 0;
	}
}

void llrbt_verify_properties(rbt_t *root)
{
	// LL-RB-BST property 6: No node has two red links connected to it
	if(is_color_red(root->left) && is_color_red(root->left->left)) {
		printf("LLRBT red violation - Red cannot be on two left children's @levels = %d,%d\n"
							, root->left->level, root->left->left->level);
		//exit(1);
	}

	// LL-RB-BST property 6: No node has two red links connected to it
	if(is_color_red(root->left) && is_color_red(root->right)) {
		printf("LLRBT red violation - Red cannot be on both left & right children's @levels= %d,%d\n"
							, root->left->level, root->right->level);
		//exit(1);
	}

	// LL-RB-BST property 8: Red links always lean left
	if(is_color_red(root->right) && !is_color_red(root->left)) {
		printf("LLRBT red violation - Red cannot be on right side @level = %d\n", root->right->level);
		//exit(1);
	}
}

void rbt_verify_properties(rbt_t *root, int llrbt)
{	
	int max_nodes = 64;
	int front = 0, rear = 0, i;
	rbt_t **queue = (rbt_t **) calloc(sizeof(rbt_t *), max_nodes);
	
	// RB-BST property 2: The root is black
	if(!is_color_black(root)) {
		printf("RBT root is not black @level = %d\n", root->level);
		//exit(1);
	}
	while(front <= rear) {
		//printf("Root: %d %s ", root->data, rbt_get_color(root) == RED? "RED" : "BLACK");
		//printf("RootLeft: %d %s ", root->left->data, rbt_get_color(root->left) == RED? "RED" : "BLACK");
		//printf("RootRight: %d %s\n", root->right->data, rbt_get_color(root->right) == RED? "RED" : "BLACK");			
		
		if(is_nil_node(root)) {
			// RB property 3: Every leaf contains NIL and is black.
			if(!is_color_black(root)) {
				printf("RBT: Leaf node is not nil or black\n");
				//exit(1);
			}
		} else {
			// RB-BST property 1: Every node is either red or black.	
			if(!is_color_red(root) && !is_color_black(root)) {
				printf("RBT: Node is neither RED nor BLACK @data = %d @level = %d\n", root->data, root->level);
				//exit(1);
			}
					
			// RB-BST property 4: If a node is red, then both its children are black.			
			if(is_color_red(root))
				if(!is_nil_node(root->left) && !is_nil_node(root->right))
					if(is_color_red(root->left) || is_color_red(root->right)) {				
						printf("RBT: Red node must have both its children as black @level = %d\n", root->level);
						exit(1);
					}	

			if(llrbt) llrbt_verify_properties(root);

			// BST property
			if ((!is_nil_node(root) && !is_nil_node(root->left) && root->left->data >= root->data) || 
				(!is_nil_node(root) && !is_nil_node(root->right) && root->right->data <= root->data)) {
				printf("RB binary search tree violation\n");
				//exit(1);
			}			
			
			// RB-BST property 5: For each node x in a RBT, all paths in sub-trees rooted at x contain the same number of black nodes
			rbt_check_black_height(root);
		}
		if(rear+2 < max_nodes) {			
			if(!is_sentinel_node(root->left))  queue[rear++] = root->left;			
			if(!is_sentinel_node(root->right)) queue[rear++] = root->right;
			root = queue[front++];			
		} else {
			max_nodes *= 2;
			queue = (rbt_t **) realloc(queue, sizeof(rbt_t *) * max_nodes); // realloc() does not initialize
			for(i=max_nodes/2; i<max_nodes;i++) queue[i] = NULL;
		}			
	}
	free(queue);	
}

rbt_t *llrbt_rotate_left(rbt_t *root) 
{	
	rbt_t *tmp = root->right;
	root->right= tmp->left;	
	rbt_set_parent(tmp->left, root);
	rbt_set_parent(tmp, rbt_get_parent(root));
	tmp->left = root;	
	rbt_set_parent(root, tmp);
	rbt_set_color(tmp, rbt_get_color(root));
	rbt_set_red(root);	
	tmp->nodes_count = root->nodes_count;
	rbt_swap_level(tmp, root);
	root->nodes_count = 1 + rbt_size(root->left) + rbt_size(root->right);
	return tmp;
}

rbt_t *llrbt_rotate_right(rbt_t *root) 
{
	rbt_t *tmp = root->left;
	root->left = tmp->right;
	rbt_set_parent(tmp->right, root);
	tmp->right = root;
	rbt_set_parent(tmp, rbt_get_parent(root));
	rbt_set_parent(root, tmp);
	rbt_set_color(tmp, rbt_get_color(root));
	rbt_set_red(root);
	tmp->nodes_count = root->nodes_count;
	rbt_swap_level(tmp, root);
	root->nodes_count = 1 + rbt_size(root->left) + rbt_size(root->right);
	return tmp;
}

void llrbt_flip_color(rbt_t *root) 
{	
	rbt_set_color(root, !rbt_get_color(root));
	rbt_set_color(root->left, !rbt_get_color(root->left));
	rbt_set_color(root->right, !rbt_get_color(root->right));
}


rbt_t *llrbt_fixup(rbt_t *root)
{
	if(!is_nil_node(root->right) && is_color_red(root->right))  root = llrbt_rotate_left(root);
	if(!is_nil_node(root->left)  && is_color_red(root->left)  && 
		!is_nil_node(root->left->left) && is_color_red(root->left->left)) root = llrbt_rotate_right(root);	
	if(!is_nil_node(root->left)  && is_color_red(root->left)  && 
		!is_nil_node(root->right) && is_color_red(root->right)) llrbt_flip_color(root);
	return root;
}

rbt_t *llrbt_move_red_right(rbt_t *root)
{	
	//print_detailed_tree("llrbt_move_red_right", root);
	llrbt_flip_color(root);
	if(!is_nil_node(root->left) && is_color_red(root->left->left)) {
		root = llrbt_rotate_right(root);
		llrbt_flip_color(root);
	}
	return root;
}

rbt_t *llrbt_move_red_left(rbt_t *root)
{
	/*  
		1. If parent & children 1 is NOT red ==> Color flip
		
			r|		       |			
		         G 		       G 			
		       /   \		    r/   \r 			
		      P     U 		    P     U 			
		     / \   r/ \	  ==>      / \  r/ \			
		    1   2  3   4	  1   2 3   4 			


		2. If Uncle's right child is RED ==> Rotate right from Uncle + rotate left from grand parent + flip colors

		        |		      |			     |		    r|	
		        G 		      G 		     3 		     3 				
		     r/   \r 		   r/   \r 		  r/   \r 	   /   \ 	
		     P     U 	  ==>	   P     3 		  G     U    +	  G     U 					
		    / \  r/ \		  / \   / \r	 + 	 /     / \	 / \   / \	
		   1   2 3   4 		 1   2     U 		P         4 	P   2     4				
		      			 	    \			   		   		
		      			 	     4			   		
		 
	*/
	//print_detailed_tree("llrbt_move_red_left", root);
	llrbt_flip_color(root);
	if(!is_nil_node(root->right) &&	is_color_red(root->right->left)) {
		root->right = llrbt_rotate_right(root->right);
		root = llrbt_rotate_left(root);
		llrbt_flip_color(root);
	}
	return root;
}

rbt_t *llrbt_delete_max1(rbt_t *root)
{
	if(is_color_red(root->left)) root = llrbt_rotate_right(root);
	if(is_nil_node(root->right)) {		
		free(root->left);
		rbt_t *tmp = root->right;
		free(root);
		return tmp;
	}
	if(!is_color_red(root->right) && !is_color_red(root->right->left)) root = llrbt_move_red_right(root);
	
	root->left = llrbt_delete_max1(root->left);
	return llrbt_fixup(root);
}

rbt_t *llrbt_delete_max(rbt_t *root)
{
	root = llrbt_delete_max1(root);
	rbt_set_black(root);
	return root;
}

rbt_t *llrbt_delete_min1(rbt_t *root)
{
	if(is_nil_node(root->left)) {
		free(root->right);
		rbt_t *tmp = root->left;
		free(root);
		return tmp;
	}
	if(!is_color_red(root->left) && !is_color_red(root->left->left)) root = llrbt_move_red_left(root);
	
	root->left = llrbt_delete_min1(root->left);	
	return llrbt_fixup(root); 
}

rbt_t *llrbt_delete_min(rbt_t *root)
{
	root = llrbt_delete_min1(root);
	rbt_set_black(root);
	return root;
}

rbt_t *llrbt_delete1(rbt_t *root, int val)
{		
	if(val < root->data) { // left subtree
		if(!is_nil_node(root->left)) { 
			if(!is_color_red(root->left) && 
				!is_color_red(root->left->left)) root = llrbt_move_red_left(root);

			root->left = llrbt_delete1(root->left, val);
			rbt_set_parent(root->left, root);			
		}
	} else { // right subtree
		if(!is_nil_node(root->left) && is_color_red(root->left)) root = llrbt_rotate_right(root);

		if(val == root->data && is_nil_node(root->right)) {
			free(root->left);
			rbt_t *tmp = root->right;
			free(root);
			return tmp;
		}
		if(!is_nil_node(root->right)) { 
			if(!is_color_red(root->right) && 
				!is_color_red(root->right->left)) root = llrbt_move_red_right(root);
				
			if(val == root->data) {				
				//print_tree("After move red right 1", root);
				rbt_t *replaced = rbt_find_minimum(root->right);				
				root->data  = replaced->data;				
				root->right = llrbt_delete_min1(root->right);
				//print_tree("After move red right 2", root);
			} else {
				root->right = llrbt_delete1(root->right, val);				
			}
			rbt_set_parent(root->right, root);
		}
	}
	//print_tree("At end", root);
	//exit(1);
	return llrbt_fixup(root);
}

rbt_t *llrbt_delete(rbt_t *root, int val)
{
	if(!is_nil_node(root)) {
		root = llrbt_delete1(root, val);
		if(!is_nil_node(root)) rbt_set_black(root);
	}
	return root;
}

// http://en.wikipedia.org/wiki/Left-leaning_red-black_tree
rbt_t *llrbt_insert(rbt_t *root, int val, rbt_t *parent)
{
	if(is_nil_node(root)) return rbt_get_new_node(val, parent);	
	if(val < root->data) root->left = llrbt_insert(root->left, val, root);
	else if(val > root->data) root->right = llrbt_insert(root->right, val, root);
	else if(val == root->data) root->data = val;		
	if(is_color_red(root->right) && !is_color_red(root->left)) 	root = 	llrbt_rotate_left(root);
	if(is_color_red(root->left)  && is_color_red(root->left->left))	root = 	llrbt_rotate_right(root);	
	if(is_color_red(root->left)  && is_color_red(root->right)) 	llrbt_flip_color(root);
	root->nodes_count = 1 + rbt_size(root->left) + rbt_size(root->right);
	return root;
}

void rbt_rotate_left(rbt_t **root, rbt_t *parent) 
{
	rbt_t *tmp = parent->right;
	parent->right= tmp->left;	
	rbt_set_parent(tmp->left, parent);
	rbt_set_parent(tmp, rbt_get_parent(parent));
	if(!rbt_get_parent(parent))
		*root = tmp;
	else {
		if(parent == rbt_get_parent(parent)->left)
			rbt_get_parent(parent)->left = tmp;
		else
			rbt_get_parent(parent)->right = tmp;	
	}
	tmp->left = parent;	
	rbt_set_parent(parent, tmp);	
	tmp->nodes_count = parent->nodes_count;
	rbt_swap_level(tmp, parent);
	parent->nodes_count = rbt_size(parent->left) + rbt_size(parent->right);	
}

rbt_t *rbt_rotate_right(rbt_t **root, rbt_t *parent) 
{	
	rbt_t *tmp = parent->left;
	parent->left = tmp->right;
	rbt_set_parent(tmp->right, parent);
	rbt_set_parent(tmp, rbt_get_parent(parent));
	if(!rbt_get_parent(parent))
		*root = tmp;
	else {
		if(parent == rbt_get_parent(parent)->left)
			rbt_get_parent(parent)->left = tmp;
		else
			rbt_get_parent(parent)->right = tmp;	
	}
	tmp->right = parent;
	rbt_set_parent(parent, tmp);	
	tmp->nodes_count = parent->nodes_count;
	rbt_swap_level(tmp, parent);
	parent->nodes_count = rbt_size(parent->left) + rbt_size(parent->right);
}

// http://en.wikipedia.org/wiki/Red-black_tree
rbt_t *rbt_insert_fixup(rbt_t **root, rbt_t* new_node)
{
	rbt_t *parent = NULL;
	while((parent = rbt_get_parent(new_node)) && is_color_red(parent)) { // Case 2: Fix up if new node parent is RED
		rbt_t *grand_parent = rbt_get_parent(parent); // root's children's does not need fix up, because root is always black
		if(parent == grand_parent->left) { // parent is on left link
			rbt_t *uncle = grand_parent->right;
			// Uncle is RED
			if(!is_nil_node(uncle) && is_color_red(uncle)) { // Parent & Uncle are RED
				/* 
				G - Grant-parent, P - Parent, U - Uncle, r - Red
				Case 3: If parent & uncle of new node is red => Flip colors

					|		       | r
				        G 		       G 					
				     r/   \ r		     /   \
				     P     U 		    P     U 				
				   r/ \   / \ 	  ==>     r/ \   / \
				   N   3 4   5 		  N   3 4   5 				
				  / \			 / \
				 1   2 			1   2

				*/
				rbt_set_black(uncle);
				rbt_set_black(parent);
				rbt_set_red(grand_parent);
				new_node = grand_parent;
				continue;
			}

			// Uncle is BLACK
			if(new_node == parent->right) {
				/* Case 4: If parent is red & new node is right link => Left rotate from parent

					|		       |
				        G 		       G 					
				     r/   \		    r/   \
				     P     U 		    N     U 				
				    / \r  / \ 	  ==>     r/ \   / \
				   1   N 4   5 		  P   3 4   5 				
				      / \		 / \
				     2   3 		1   2
				 
				*/
				rbt_rotate_left(root, parent);
				rbt_t *tmp = parent;
				parent = new_node;
				new_node = tmp;
			}
			/* Case 5: If parent is red & new node is left link => Right rotate from grandparent

					|		       |
				        G 		       P 					
				     r/   \		    r/   \r  						
				     P     U 		    N     G 				
				   r/ \   / \ 	  ==>      / \   / \
				   N   3 4   5 		  1   2 3   U 				
				  / \		 	 	   / \
				 1   2 			          4   5 
				 
			*/
			rbt_set_black(parent);
			rbt_set_red(grand_parent);
			rbt_rotate_right(root, grand_parent);
		} else { // parent is on right link 
			rbt_t *uncle = grand_parent->left;
			// Uncle is RED
			if(!is_nil_node(uncle) && is_color_red(uncle)) {
				rbt_set_black(uncle);
				rbt_set_black(parent);
				rbt_set_red(grand_parent);
				new_node = grand_parent;
				continue;
			}
			// Uncle is BLACK
			if(new_node == parent->left) {				
				rbt_rotate_right(root, parent);
				rbt_t *tmp = parent;
				parent = new_node;
				new_node = tmp;
			}
			rbt_set_black(parent);
			rbt_set_red(grand_parent);
			rbt_rotate_left(root, grand_parent);
		}
	}
	rbt_set_black(*root); // Case 1: Always root is black 
}

// http://en.wikipedia.org/wiki/Red-black_tree
void rbt_insert(rbt_t **root, int val)
{
	rbt_t *node = *root;
	while(!is_nil_node(node)) {
		if(val < node->data) node = node->left;
		else if (val > node->data) node = node->right;
		else return;		
	}	
	rbt_t *parent   = rbt_get_parent(node);
	rbt_t *new_node = rbt_get_new_node(val, parent);
	if(!parent) *root = new_node;		
	else {
		if(node == parent->left)
			parent->left = new_node;
		else
			parent->right = new_node;	
		while(parent) {
			parent->nodes_count++; //OR parent->nodes_count = 1 + rbt_size(parent->left) + rbt_size(parent->right);
			parent = rbt_get_parent(parent);
		}		
	}
	rbt_insert_fixup(root, new_node);
}

rbt_t *rbt_find_successor(rbt_t *root)
{
	rbt_t *succ = sentinel_node;
	if(!is_nil_node(root->right)) {
		succ = rbt_find_minimum(root->right);
	} else {
		succ = rbt_get_parent(root);
		while(succ && root == succ->right) {
			root = succ;
			succ = rbt_get_parent(succ);
		}
	}
	return succ;
}

// http://en.wikipedia.org/wiki/Red-black_tree
// Push extra black up tree until
// - reaching a RED node that can be colored BLACK or
// - reaching the root, in which case the extra black can be discarded
void rbt_delete_fixup(rbt_t **root, rbt_t *child, rbt_t *parent)
{	
	rbt_t *sibling = sentinel_node;
	while((is_nil_node(child) || is_color_black(child)) && child != *root) {		
		//printf("Child:%d Sibling:%d Parent:%d \n", child->data, sibling->data, parent->data);		
		if(child == parent->left) { // if child is left link
			sibling = parent->right;
			//printf("Parent right sibling:%d \n", sibling->data);			
			
			if(is_color_red(sibling)) {
				/* CASE 2:
				   C - Child, S - Sibling, P - Parent, r - Red 
				   C could be { deleted->right OR deleted->left OR replaced->right }
				   If sibling is red  => Left rotate from parent

					|		       |
				        P 		       S 					
				      /   \r		    r/   \
				     C     S 		    P     R 				
				          / \ 	  ==>      / \
				         L   R 	  	  C   L

				*/
				rbt_set_black(sibling);
				rbt_set_red(parent);
				rbt_rotate_left(root, parent);				
				sibling = parent->right;
			}
			/* CASE 3: If parent, sibling & sibling children's are black => Recolor sibling

					|		       |		
				        P 		       P 				
				      /   \		     /   \r								
				     C     S 		    C     S 					
				          / \ 	  ==>            / \
				         L   R 	  	        L   R	

			*/
			if(is_nil_node(parent) || is_color_black(parent) && 
				is_nil_node(sibling->left) || is_color_black(sibling->left) && 
				is_nil_node(sibling->right) || is_color_black(sibling->right)) {
				rbt_set_red(sibling);
				child = parent;
				parent = rbt_get_parent(child);
			} else {
				/* CASE 4: If parent is red & sibling, sibling children's are black => Recolor sibling

					|r		       |		
				        P 		       P 				
				      /   \		     /   \r								
				     C     S 		    C     S 					
				          / \ 	  ==>            / \
				         L   R 	  	        L   R	

				*/
				if(is_nil_node(parent) || is_color_red(parent) && 
					is_nil_node(sibling->left) || is_color_black(sibling->left) && 
					is_nil_node(sibling->right) || is_color_black(sibling->right)) {
					rbt_set_red(sibling);
					child = parent;
					parent = rbt_get_parent(child);
				} else {
					/* CASE 5: If sibling is black & sibling left child is red => RRight rotate from sibling

						|		    	|		
					        S 		       	L 				
					     r/   \		      /   \r								
					     L     R 		     1     S 					
					    / \   / \ 	  ==>             / \
					   1   2 L   R 	  	         2   R

					*/
					if(is_nil_node(sibling->right) || is_color_black(sibling->right)) {
						rbt_set_black(sibling->left);
						rbt_set_red(sibling);
						rbt_rotate_right(root, sibling);					
						sibling = parent->right;
					}
					/* CASE 6: If sibling is black & sibling right child is red => Left rotate from parent

						|		        |		
					        P 		 	S 				
					      /   \		      /   \r								
					     C     S 		     P     R 					
					    / \   / \r 	  ==>       / \
					   1   2 L   R 	  	   C   L  
						 		  / \
						 		 1   2  
  					*/
					rbt_set_color(sibling, rbt_get_color(parent));
					rbt_set_black(parent);
					rbt_set_black(sibling->right);
					rbt_rotate_left(root, parent);
					child = *root;
					break;
				}				
			}
		} else { // if child is right link
			sibling = parent->left;
			//printf("Parent left sibling:%d \n", sibling->data); 
			// CASE 2
			if(is_color_red(sibling)) {
				rbt_set_black(sibling);
				rbt_set_red(parent);
				rbt_rotate_right(root, parent);				
				sibling = parent->left;				
			}
			// CASE 3 + CASE 4
			if(is_nil_node(sibling->left) || is_color_black(sibling->left) && 
				is_nil_node(sibling->right) || is_color_black(sibling->right)) {
				rbt_set_red(sibling);
				child = parent;
				parent = rbt_get_parent(child);
			} else {
				// CASE 5
				if(is_nil_node(sibling->left) || is_color_black(sibling->left)) {
					rbt_set_black(sibling->right);
					rbt_set_red(sibling);
					rbt_rotate_left(root, sibling);					
					sibling = parent->left;
				}
				// CASE 6
				rbt_set_color(sibling, rbt_get_color(parent));
				rbt_set_black(parent);
				rbt_set_black(sibling->left);
				rbt_rotate_right(root, parent);					
				child = *root;
				break;				
			}
		}			
		//print_detailed_tree(*root);		
	}
	if(!is_nil_node(child)) rbt_set_black(child);
}

// http://en.wikipedia.org/wiki/Red-black_tree
void rbt_delete(rbt_t **root, rbt_t *deleted)
{
	rbt_t *child, *parent;
	int color;
	int single = 0;
	if(deleted) {
		if(is_nil_node(deleted->left)) { // no left child
			child = deleted->right;
			single = 1;
		} else if(is_nil_node(deleted->right)) { // no right child
			child = deleted->left;
			single = 1;
		} else {		
			rbt_t *replaced = rbt_find_minimum(deleted->right);
			rbt_recount_upward_nodes(replaced);
			rbt_t *parent_deleted = rbt_get_parent(deleted);
			if(parent_deleted) {
				if(parent_deleted->left == deleted)
					parent_deleted->left = replaced;
				else
					parent_deleted->right = replaced;

			} else *root = replaced;
			
			child  = replaced->right;
			parent = rbt_get_parent(replaced);
			color  = rbt_get_color(replaced);

			if(parent == deleted)
				parent = replaced;
			else {
				if(!is_nil_node(child)) rbt_set_parent(child, parent);
				parent->left = child;
				replaced->right = deleted->right;
				rbt_set_parent(deleted->right, replaced);
			}
			replaced->parent_color = deleted->parent_color; // copy both color & parent
			replaced->left = deleted->left;
			rbt_set_parent(deleted->left, replaced);
			replaced->level = deleted->level;
			replaced->nodes_count = deleted->nodes_count;			
		}
		if(single) { // at most one non-null children
			rbt_recount_upward_nodes(child);
			parent = rbt_get_parent(deleted);
			color  = rbt_get_color(deleted);
			if(!is_sentinel_node(child)) rbt_set_parent(child, parent);
			if(parent) {
				if(parent->left == deleted)
					parent->left = child;
				else
					parent->right = child;
			} else *root = child;
			child->level = deleted->level;
			child->nodes_count = deleted->nodes_count;			
			if(is_color_black(deleted) && is_color_red(child)) {
				rbt_set_black(child);
				free(deleted); 
				return;
			}
		}
		
		if(color == BLACK) rbt_delete_fixup(root, child, parent);
		free(deleted);
	}
}

void init_sentinel_node()
{
	sentinel_node = (rbt_t *) malloc(sizeof(rbt_t));
	sentinel_node->data  = 9999;	
	sentinel_node->left  = sentinel_node;
	sentinel_node->right = sentinel_node;
}

void test_llrbt(int argc, char *argv[])
{
	int i;
	rbt_t *llroot = rbt_get_nil_node(NULL);		
	uint64_t start = micro_second_clock();			
	for(i=1; i<argc; i++) {
		int val = atoi(argv[i]);
		//printf("Left-leaning red black tree insert %d \n", val);		
		llroot = llrbt_insert(llroot, val, NULL);
		rbt_set_black(llroot);
		//rbt_set_node_level(llroot, 0);
		//char msg[100];
		//sprintf(msg, "Left-leaning red black tree insert %d", val);
		//print_detailed_tree(msg, llroot);
		//print_tree(msg, llroot);
		//rbt_verify_properties(llroot, 1); //  1 - check RBT properties + Left leaning properties
	}
	display_time("Time taken to complete LLRBT insert", start, micro_second_clock());		
	rbt_set_node_level(llroot, 0);
	rbt_verify_properties(llroot, 1); //  1 - check RBT properties + Left leaning properties
	print_detailed_tree("Left-leaning red black tree insert detailed", llroot);
	print_tree("Left-leaning red black tree insert ", llroot);	

	start = micro_second_clock();
	for(i=1; i<argc; i++) {
		int val = atoi(argv[i]);
		//printf("Deleting %d from LLRBT\n", val);
		llroot = llrbt_delete(llroot, val);
		//rbt_set_node_level(llroot, 0);		
		//char msg[100];
		//sprintf(msg, "Delete %d from LLRBT", val);
		//print_detailed_tree(msg, llroot);
		//print_tree(msg, llroot);
		//rbt_verify_properties(llroot, 1);
		if(i==8) break;				
	}
	display_time("Time taken to complete LLRBT delete", start, micro_second_clock());		
	rbt_set_node_level(llroot, 0);
	rbt_verify_properties(llroot, 1);
	print_detailed_tree("Left-leaning red black tree delete detailed", llroot);
	print_tree("Left-leaning red black tree delete", llroot);
}

void test_rbt(int argc, char *argv[])
{
	int i;
	rbt_t *root   = rbt_get_nil_node(NULL);
	
	uint64_t start = micro_second_clock();	
	for(i=1; i<argc; i++) {
		int val = atoi(argv[i]);
		//printf("Red black tree insert %d \n", val);	
		rbt_insert(&root, val);		
		//char msg[100];
		//sprintf(msg, "Red black tree insert %d", val);
		//print_detailed_tree(msg, root);
		//print_tree(msg, root);
		//rbt_verify_properties(root, 0);
	}
	display_time("Time taken to complete RBT insert", start, micro_second_clock());			
	rbt_set_node_level(root, 0);							
	rbt_verify_properties(root, 0);	// 0 - check RBT properties only
	print_detailed_tree("Red black tree detailed insert", root);
	print_tree("Red black tree insert", root);

	start = micro_second_clock();
	for(i=1; i<argc; i++) {
		int val = atoi(argv[i]);
		printf("Deleting %d from RBT\n", val);
		rbt_delete(&root, rbt_find(root, val));
		//char msg[100];
		//sprintf(msg, "Red black tree delete %d", val);
		//print_detailed_tree(msg, root);
		//print_tree(msg, root);
		//rbt_verify_properties(root, 0);
		if(i==8) break;
	}
	display_time("Time taken to complete RBT delete", start, micro_second_clock());	
	rbt_set_node_level(root, 0);
	rbt_verify_properties(root, 0);	
	print_detailed_tree("Red black tree detailed delete", root);
	print_tree("Red black tree delete", root);
}

void dummy_rbt(int argc, char *argv[])
{
	int i;
	rbt_t *root   = rbt_get_nil_node(NULL);
	rbt_t *llroot = rbt_get_nil_node(NULL);	
	uint64_t start = micro_second_clock();	
	for(i=1; i<argc; i++) {
		int val = atoi(argv[i]);	
		rbt_insert(&root, val);
		rbt_delete(&root, rbt_find(root, val));
		llroot = llrbt_insert(llroot, val, NULL);
		llroot = llrbt_delete(llroot, val);
	}
}

int main(int argc, char *argv[])
{
	
	printf("Red black tree demo\nINFO:\n NIL - Nil black node\n STN - Sentinel Node\n");
	init_sentinel_node();
	if(argc > 1) {		
		dummy_rbt(argc, argv); // to avoid time taken by loader to load this code
		test_rbt(argc, argv);
		test_llrbt(argc, argv);				
	}
	return 0;
}
// ./rbt 50 70 30 90 20 10 39 40 100 60 80 5 85 15

