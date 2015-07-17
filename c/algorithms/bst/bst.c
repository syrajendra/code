#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <math.h>
#include <time.h>

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

static void display_time(char *msg, uint64_t start, uint64_t stop)
{
        char precision[50];
        uint64_t result = stop - start;
        if (result > 1000000) {
                result = result/1000000;
                strcpy(precision, "seconds");                
                if(result > 60) {
                	//printf("%s %llu %s\n", msg, result, precision);                	
                	printf("%s %lumins %lusecs\n", msg, result/60, result%60);
                	return;
                }
        } else if (result > 1000) {
                result = result/1000;
                strcpy(precision, "milliseconds");
        } else {
               strcpy(precision, "microseconds");
        }
        printf("%s %lu %s\n", msg, result, precision);
}

struct bst {
	int data;
	struct bst *left;
	struct bst *right;
	int visited; // for DFS non recursive
	int depth; // to find depth of tree non recursive
	int count; 
};
typedef struct bst bst_t;

void preorder(bst_t *root) // DLR
{
	int max_nodes = 64;
	int top = 0, i;
	bst_t *curr = NULL;	
	bst_t **stack = (bst_t **) calloc(sizeof(bst_t *), max_nodes);
	stack[top++] = root;
	while(top) {		
		if(top+2 < max_nodes) {
			curr = stack[--top];
			printf(" %d ", curr->data);
			if(curr->right) stack[top++] = curr->right; // push right first
			if(curr->left) stack[top++] = curr->left;   // pop left first
		} else {
			max_nodes *= 2;
			stack = (bst_t **) realloc(stack, sizeof(bst_t *) * max_nodes); // realloc() does not initialize
			for(i=max_nodes/2; i<max_nodes;i++) stack[i] = NULL;
		}
	}
}


void preorder_recursive(bst_t *root) // DLR
{
	if(root) {
		printf(" %d ", root->data);
		preorder_recursive(root->left);
		preorder_recursive(root->right);
	}
}

// non-resursive and non-stack
// Disadvantage: Modifies tree's right link 
void morris_preorder_traversal(bst_t *root) // Preorder traversal
{
	bst_t *curr = root, *prev = NULL;	
	while(curr) {		
		if(!curr->left) { // left is NULL
			printf(" %d ", curr->data); 
			curr = curr->right;
		} else {
			prev = curr->left;
			while(prev->right && prev->right != curr) prev = prev->right; //  goto the rightmost child			
			if(!prev->right) { // right is NULL				
				printf(" %d ", curr->data);
				prev->right = curr;
				curr = curr->left;
			} else {				
				prev->right = NULL;				
				curr = curr->right;
			}
		}
	}	
}

// non-resursive and non-stack
// Disadvantage: Modifies tree's right link 
void morris_inorder_traversal(bst_t *root) // Inorder traversal
{
	bst_t *curr = root, *prev = NULL;	
	while(curr) {		
		if(!curr->left) { // left is NULL
			printf(" %d ", curr->data); 
			curr = curr->right;
		} else {
			prev = curr->left;
			while(prev->right && prev->right != curr) prev = prev->right; //  goto the rightmost child			
			if(!prev->right) { // right is NULL				
				prev->right = curr;
				curr = curr->left;
			} else {				
				prev->right = NULL;
				printf(" %d ", curr->data);
				curr = curr->right;
			}
		}
	}	
}

// Not possible unless tree is threaded binary tree
void morris_postorder_traversal(bst_t *root) 
{
	//A binary tree is threaded by making all right child pointers that would normally be 
	//null point to the inorder successor of the node, and all left child pointers that 
	//would normally be null point to the inorder predecessor of the node
}

void inorder_recursive(bst_t *root) // LDR
{
	if(root) {		
		inorder_recursive(root->left);
		printf(" %d ", root->data);
		inorder_recursive(root->right);
	}	
}

void postorder_recursive(bst_t *root) // LRD
{
	if(root) {		
		postorder_recursive(root->left);
		postorder_recursive(root->right);
		printf(" %d ", root->data);
	}
}

void print_detailed_tree(bst_t *root)
{
	if(!root) return;
	int max_nodes = 64;
	int front = 0, rear = 0, i;	
	bst_t **queue = (bst_t **) calloc(sizeof(bst_t *), max_nodes);	
	while(root) {		
		printf("Root=%6d Left=%6d Right=%6d Count=%6d\n", 
				root->data, 
				(root->left)?root->left->data:-99999, 
				(root->right)?root->right->data:-99999,
				root->count);
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
			queue = (bst_t **) realloc(queue, sizeof(bst_t *) * max_nodes); // realloc() does not initialize
			for(i=max_nodes/2; i<max_nodes;i++) queue[i] = NULL;
		}
	}	
}

void print_bst(bst_t *root)
{	
	printf("\nPreorder recursive (DLR) : ");
	preorder_recursive(root);
	printf("\nPreorder iterative using stack (DLR) : ");
	preorder(root);
	printf("\nPreorder Morris Traversal (DLR): ");
	morris_preorder_traversal(root);
	
	printf("\nInorder recursive (LDR)	 : ");
	inorder_recursive(root);
	printf("\nInorder Morris Traversal (LDR): ");
	morris_inorder_traversal(root);
	
	printf("\nPostorder recursive (LRD)	 : ");
	postorder_recursive(root);
	printf("\nDetailed tree: \n");
	print_detailed_tree(root);
	printf("\n");
}

bst_t *get_new_node(int val)
{
	bst_t *node = (bst_t *) malloc(sizeof(bst_t));
	node->data = val;
	node->left = NULL;
	node->right= NULL;
	node->visited = 0;
	node->depth = 0;
	node->count = 1;
	return node;
}

int bst_size(bst_t *root) 
{
	if(!root) return 0;
	return root->count;
}

bst_t *insert_recursive(bst_t *root, int val)
{
	if(!root) return get_new_node(val);	
	if(val < root->data) 
		root->left  = insert_recursive(root->left, val);
	else if(val > root->data)
		root->right = insert_recursive(root->right, val);
	root->count = 1 + bst_size(root->left) + bst_size(root->right);
	return root;	
}

void update_count(bst_t *root)
{
	if(root) {
		root->count = get_total_nodes(root);
		if(root->left) update_count(root->left);
		if(root->right) update_count(root->right); 
	}
}

bst_t *insert(bst_t *root, int val)
{	
	if(!root) return get_new_node(val);
	bst_t *prev = NULL, *ptr = root;
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
	if(type == 'l') 
		prev->left  = get_new_node(val);
	else 
		prev->right = get_new_node(val);
	update_count(root);
	return root;
}

bst_t *delete_all_nodes(bst_t *root)
{
	if(root) {
		if(root->left) delete_all_nodes(root->left);
		if(root->right) delete_all_nodes(root->right);
		free(root);
		root = NULL;
	}
	return root;
}

bst_t *init_nodes(bst_t *root)
{
	if(root) {
		root->visited = 0;
		root->depth   = 0;
		if(root->left) init_nodes(root->left);
		if(root->right) init_nodes(root->right);		
	}
	return root;
}


bst_t *find_minimum(bst_t *ptr)
{
	if(ptr) {
		while(ptr->left) ptr = ptr->left;
		return ptr;
	}
	return NULL;
}

int find_minimum_value(bst_t *ptr)
{
	if(ptr) {
		while(ptr->left) ptr = ptr->left;
		return ptr->data;
	}
	return -1;
}

int find_maximum_value(bst_t *ptr)
{
	if(ptr) {
		while(ptr->right) ptr = ptr->right;
		return ptr->data;
	}
	return -1;
}

bst_t *delete_minimum(bst_t *root)
{
	if(root) {
		if(!root->left) return root->right;
		root->left = delete_minimum(root->left);
		root->count = 1 + bst_size(root->left) + bst_size(root->right);
	}
	return root;
}

// Hibbard deletion - Sqrt(n)
bst_t *delete_recursive(bst_t *root, int val) 
{
	if(!root) return NULL;	
	if(val < root->data) // search for val
		root->left = delete_recursive(root->left, val); // replace parent link
	else if(val > root->data)
		root->right= delete_recursive(root->right, val); // replace parent link
	else {
		if(!root->right) return root->left; // no right child
		bst_t *tmp = root;
		root = find_minimum(tmp->right);
		root->right = delete_minimum(tmp->right);
		root->left = tmp->left;
	}
	root->count = 1 + bst_size(root->left) + bst_size(root->right); 
	return root;
}

bst_t *delete(bst_t *root, int val) 
{
	bst_t *prev = NULL, *ptr = root;
	char type = ' ';	
	while(ptr) {
		if(ptr->data == val) {			
			if(!ptr->left && !ptr->right) { // node to be removed has no children's
				if(ptr != root && prev) { // delete leaf node
					if(type == 'l') 
						prev->left = NULL;
					else
						prev->right = NULL;
				} else root = NULL; // deleted node is root
			} else if (ptr->left && ptr->right) { // node to be removed has two children's				
				ptr->data = find_minimum_value(ptr->right); // find minimum value from right subtree					
				val = ptr->data;
				prev = ptr;
				ptr = ptr->right; // continue from right subtree delete min node
				type = 'r';
				continue;				
			} else { // node to be removed has one children				
				if(ptr == root)  { // root with one child					
					root = root->left ? root->left : root->right;	
				} else { // subtree with one child
					if(type == 'l') 
						prev->left = ptr->left ? ptr->left : ptr->right;
					else
						prev->right = ptr->left ? ptr->left : ptr->right;	
				}			
			}
			free(ptr);
		}
		prev = ptr;	
		if(val < ptr->data) {			
			ptr = ptr->left;
			type = 'l';
		} else {
			ptr = ptr->right;
			type = 'r';
		}
	}
	update_count(root);
	return root;
}

int get_total_nodes(bst_t *root) 
{
	if(!root) return 0;
	int max_nodes = 64;
	int front = 0, rear = 0, i;
	bst_t **queue = (bst_t **) calloc(sizeof(bst_t *), max_nodes);	
	while(root) {
		if(rear+2 < max_nodes) {
			if(root->left)  queue[rear++] = root->left;			
			if(root->right) queue[rear++] = root->right;
			root = queue[front++];
		} else {			
			max_nodes *= 2;
			queue = (bst_t **) realloc(queue, sizeof(bst_t *) * max_nodes); // realloc() does not initialize
			for(i=max_nodes/2; i<max_nodes;i++) queue[i] = NULL;
		}
	}
	return rear + 1;
}

int get_total_nodes_recursive(bst_t *root) 
{
	if(!root) return 0;
	return get_total_nodes_recursive(root->left) + get_total_nodes_recursive(root->right) + 1;
}

int get_tree_depth_recursive(bst_t *root)
{	
	if(!root) return 0;	
	int ldepth = get_tree_depth_recursive(root->left);	
	int rdepth = get_tree_depth_recursive(root->right);
	//printf("Data=%d ldepth=%d rdepth=%d\n", root->data, ldepth, rdepth);
	return (ldepth > rdepth) ? ldepth + 1 : rdepth + 1;
}

int dfs(bst_t *root, int val) // Implement stack using iterative algorithm
{
	printf("DFS iterative traversal: ");
	init_nodes(root);
	bst_t *ptr = root;
	while(ptr && !ptr->visited) {
		if(ptr->left && !ptr->left->visited) {			
			ptr = ptr->left;
		} else if(ptr->right && !ptr->right->visited) {			
			ptr = ptr->right;
		} else {
			printf(" %d ", ptr->data);
			if(ptr->data == val) return 1;
			ptr->visited = 1;
			ptr = root;			
		}
	}
	return 0;
}

int dfs_recursive(bst_t *root, int val) // Depth Fist search : Preorder traversal uses stack
{	
	if(root) {		
		if(dfs_recursive(root->left, val)) return 1;
		if(dfs_recursive(root->right, val)) return 1;
		printf(" %d ", root->data);
		if(val == root->data) return 1;
	}	
	return 0;
}

int bfs(bst_t *root, int val) // Breadth First Search (Level by Level)
{
	if(!root) return 0;
	printf("BFS iterative traversal: ");
	int max_nodes = 64;
	int front = 0, rear = 0, i;
	bst_t **queue = (bst_t **) calloc(sizeof(bst_t *), max_nodes);	
	while(root) {
		printf(" %d ", root->data);
		if(root->data == val) return 1;
		if(rear+2 < max_nodes) {
			if(root->left)  queue[rear++] = root->left;			
			if(root->right) queue[rear++] = root->right;
			root = queue[front++];			
		} else {			
			max_nodes *= 2;
			queue = (bst_t **) realloc(queue, sizeof(bst_t *) * max_nodes); // realloc() does not initialize
			for(i=max_nodes/2; i<max_nodes;i++) queue[i] = NULL;
		}
	}
	return 0;
}

// largest number <= to given value
bst_t *bst_floor_recursive(bst_t *root, int val)
{
	if(!root) return NULL;
	if(root->data == val) return root;
	if(val < root->data) return bst_floor_recursive(root->left, val);
	bst_t *tmp = bst_floor_recursive(root->right, val);
	if(tmp) 
		return tmp;
	else
		return root;
}

// smallest number >= to given value
bst_t *bst_ceiling_recursive(bst_t *root, int val)
{
	if(!root) return NULL;
	if(root->data == val) return root;
	if(val > root->data) return bst_ceiling_recursive(root->right, val);
	bst_t *tmp = bst_ceiling_recursive(root->left, val);
	if(tmp) 
		return tmp;
	else
		return root;
}

// how many nodes < value
int bst_rank_recursive(bst_t *root, int val)
{
	if(!root) return 0;
	if(val < root->data) return bst_rank_recursive(root->left, val);
	else if(val > root->data) return 1 + bst_size(root->left) + bst_rank_recursive(root->right, val);
	else return bst_size(root->left);
}

int get_tree_depth(bst_t *root) // Iterative way to find tree depth
{
	int max_nodes = 64;
	int top = 0, i, depth;	
	bst_t *curr = NULL;	
	bst_t **stack = (bst_t **) calloc(sizeof(bst_t *), max_nodes);
	init_nodes(root);
	stack[top++] = root;
	depth = root->depth;
	while(top) {		
		if(top+2 < max_nodes) {
			curr = stack[--top];			
			//printf("%d - %d \n", curr->data, curr->depth);
			if(!curr->left && !curr->right) {
				if(curr->depth > depth) 
					depth = curr->depth;
			}
			if(curr->right) {				
				curr->right->depth = curr->depth + 1;
				stack[top++] = curr->right; 
			}			
			if(curr->left) {
				curr->left->depth = curr->depth + 1;
				stack[top++] = curr->left;   
			}						
		} else {
			max_nodes *= 2;
			stack = (bst_t **) realloc(stack, sizeof(bst_t *) * max_nodes); // realloc() does not initialize
			for(i=max_nodes/2; i<max_nodes;i++) stack[i] = NULL;
		}
	}	
	return depth + 1;
}

int find_minimum_value_recursive(bst_t *root)
{
	if(root) {
		if(!root->left) return root->data;
		return find_minimum_value_recursive(root->left);		
	}
	return 0;
}

int is_same_trees_recursive(bst_t *root1, bst_t *root2)
{
	if(!root1 && !root2) return 1;
	else if(root1 && root2) {
		return ((root1->data == root2->data) && 
			 is_same_trees_recursive(root1->left, root2->left) &&
			 is_same_trees_recursive(root1->right, root2->right));
	} else return 0;
}

int is_binary_search_tree_recursive(bst_t *root)
{
	if(!root) return 1;
	if(root->left && find_minimum_value(root->left) > root->data) return 0;
	if(root->right && find_maximum_value(root->right) <= root->data) return 0;
	if(!is_binary_search_tree_recursive(root->left) || !is_binary_search_tree_recursive(root->right)) return 0;
	return 1;
}

void diplay_result(char *msg, bst_t *root)
{
	printf("--------------- %s ------------------\n", msg);	
	printf("Number of nodes recursive: %d\n", get_total_nodes_recursive(root));
	printf("Number of nodes iterative: %d\n", get_total_nodes(root));
	printf("Number of levels recursive: %d\n", get_tree_depth_recursive(root));	
	printf("Number of levels iterative: %d\n", get_tree_depth(root));	
	print_bst(root);
	printf("------------------------------------------\n");
}

void test_delete(char *msg, bst_t *root, int argc, char *argv[])
{
	char tmp[64];
	int num;

	num = atoi(argv[1]);
	sprintf(tmp, "%s%s:%d", "Delete", msg, num);
	root = delete(root, num);
	diplay_result(tmp, root);
	
	if(argc > 3) {
		num = atoi(argv[3]);
		sprintf(tmp, "%s%s:%d", "Delete", msg, num);
		root = delete(root, num);
		diplay_result(tmp, root);
	}

	num = atoi(argv[argc-1]);
	sprintf(tmp, "%s%s:%d", "Delete", msg, num);
	root = delete(root, num);
	diplay_result(tmp, root);

	num = find_minimum_value(root);
	sprintf(tmp, "%s%s:%d", "Delete", msg, num);
	root = delete(root, num);
	diplay_result(tmp, root);

}

void test_bst(bst_t *root1, bst_t *root2)
{
	printf("--------------- %s ------------------\n", "BST");	
	printf("Trees are%ssame\n", is_same_trees_recursive(root1, root2) ? " " : " not ");
	printf("Tree-1 is%sBST\n", is_binary_search_tree_recursive(root1) ? " " : " not ");
	printf("Tree-2 is%sBST\n", is_binary_search_tree_recursive(root2) ? " " : " not ");
	printf("------------------------------------------\n");
}

void test_search(bst_t *root, int argc, char *argv[])
{
	printf("--------------- %s ------------------\n", "Search");	
	int val = atoi(argv[argc-2]);
	printf("DFS recursive traversal: ");
	printf("\nDFS recursive for %d %s\n", val, (dfs_recursive(root, val) ? "found" : "not found"));	
	printf("\nDFS iterative for %d %s\n", val, (dfs(root, val) ? "found" : "not found"));
	printf("\nBFS for %d %s\n", val, (bfs(root, val) ? "found" : "not found"));
	printf("------------------------------------------\n");
}

void test_minimum(bst_t *root)
{
	printf("--------------- %s ------------------\n", "Minimum");
	printf("Minimum iterative %d \n",  find_minimum_value(root));
	printf("Minimum recursive %d \n",  find_minimum_value_recursive(root));
	printf("------------------------------------------\n");
}

void test_operations(bst_t *root, int argc, char *argv[])
{
	bst_t *tmp;
	int val = atoi(argv[argc/2]);
	printf("--------------- %s ------------------\n", "Operations");
	printf("Floor recursive of %d = %d\n",  val, (tmp=bst_floor_recursive(root, val))  ? tmp->data : 0);
	printf("Ceil recursive  of %d = %d\n",  val, (tmp=bst_ceiling_recursive(root, val))? tmp->data : 0);
	printf("Rank recursive  of %d = %d\n",  val, bst_rank_recursive(root, val));
	val = 8;
	printf("Floor recursive of %d = %d\n",  val, (tmp=bst_floor_recursive(root, val))  ? tmp->data : 0);
	printf("Ceil recursive  of %d = %d\n",  val, (tmp=bst_ceiling_recursive(root, val))? tmp->data : 0);
	printf("Rank recursive  of %d = %d\n",  val, bst_rank_recursive(root, val));	
	printf("------------------------------------------\n");
}

void test_insert(int argc, char *argv[])
{
	int i;
	bst_t *root_rec = NULL, *root = NULL;
	for(i=1; i<argc; i++) {
		root_rec = insert_recursive(root_rec, atoi(argv[i]));		
		root = insert(root, atoi(argv[i]));
	}	
	diplay_result("InsertRecursive", root_rec);
	diplay_result("InsertIterative", root);
	
	test_bst(root, root_rec);
	test_operations(root_rec, argc, argv);	
	test_search(root, argc, argv);
	test_minimum(root);
	
	test_delete("Iterative", root, argc, argv);
	test_delete("Recursive", root_rec, argc, argv);
	
	test_minimum(root);	
	test_bst(root, root_rec);
}

int main(int argc, char *argv[])
{		
	uint64_t start = micro_second_clock();
	if(argc > 1) {		
		test_insert(argc, argv);						
	}
	display_time("\nTime taken to complete execution", start, micro_second_clock());
	return 0;
}

// $ ./bst 5 2 12 4 3 9 21 19 25