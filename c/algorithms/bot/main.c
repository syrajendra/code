#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>

#define MAX_MATRIX 1024

struct matrix {
    int val;
    struct matrix *right;
    struct matrix *left;
    struct matrix *down;
};
typedef struct matrix matrix_t;

struct userInput {
    int starti;
    int startj;
    int endi;
    int endj;
    matrix_t *zrzc;
};
typedef struct userInput userInput_t;

matrix_t *createNode(int val)
{
    matrix_t *node = (matrix_t *) malloc(sizeof(matrix_t));
    node->val   = val;
    node->right = NULL;
    node->left  = NULL;
    node->down  = NULL;
    return node;
}

void constructMatrixList(int in_matrix[MAX_MATRIX][MAX_MATRIX], userInput_t *ui)
{
    int i=0, j=0;
    matrix_t *begin_node = NULL;
    matrix_t *curr_node = NULL;
    matrix_t *top_node  = NULL;
    for (int i=0; i<ui->endi+1; i++) {
        matrix_t *prev_node = NULL;
        for (int j=0; j<ui->endj+1; j++) {
            curr_node = createNode(in_matrix[i][j]);
            if (i==0 && j==0) {
                ui->zrzc  = curr_node;
            } else {
                if (prev_node) {
                    prev_node->right = curr_node;
                    curr_node->left  = prev_node;
                }
                if (top_node) {
                    top_node->down = curr_node;
                    //printf("top:%d top->down:%d\n", top_node->val, top_node->down->val);
                    top_node  = top_node->right;
                }
            }
            if (j==0) begin_node = curr_node;
            prev_node = curr_node;
        }
        top_node = begin_node;
    }
}

void displayMatrixList(matrix_t *head)
{
    matrix_t *node = head;
    int i=0, j=0;
    printf("Input matrix as list :\n");
    while(node) {
        printf("%d] ", i);
        matrix_t *i_node = node;
        while (node) {
            printf("%d ", node->val);
            //if(node->down) printf("(%d) ", node->down->val);
            //if(node->left) printf("<%d> ", node->left->val);
            node = node->right;
        }
        printf("\n");
        i++;
        node = i_node->down;
    }
}

void readInputFile(const char *filepath, userInput_t *ui)
{
    struct stat sbuf;
    int ret = stat(filepath, &sbuf);
    if (ret != 0) {
        printf("Error: Input filepath '%s' is not accessible\n", filepath);
        exit(1);
    }
    printf("Input file path : %s\n", filepath);
    FILE *fp = fopen(filepath, "r");
    if (fp) {
        fscanf(fp, "%d%d\n", &ui->starti, &ui->startj);
        fscanf(fp, "%d%d\n", &ui->endi, &ui->endj);
        printf("Start point : %d %d\n", ui->starti, ui->startj);
        printf("End point : %d %d\n", ui->endi, ui->endj);
        printf("Input matrix :\n");
        int in_matrix[MAX_MATRIX][MAX_MATRIX];
        for (int i=0; i<ui->endi+1; i++) {
            for (int j=0; j<ui->endj+1; j++) {
                fscanf(fp, "%d", &in_matrix[i][j]);
                printf("%d ", in_matrix[i][j]);
            }
            printf("\n");
        }
        constructMatrixList(in_matrix, ui);
    } else {
        printf("Error: Failed to open file '%s' error '%s'\n", filepath, strerror(errno));
        exit(1);
    }
}

void findTravelPath(userInput_t *ui)
{
    int i=ui->starti, j=ui->startj, cnt=0;
    matrix_t *node = ui->zrzc;
    
    const char *ofilepath = "out.txt";
    FILE *fp = fopen(ofilepath, "w");
    if (!fp) {
        printf("Error: Failed to create output file %s", ofilepath);
        exit(1);  
    }

    printf("\nFind path : \n");
    fprintf(fp, "Find path : \n");

    // start position
    printf("%d %d\n", i, j);
    fprintf(fp, "%d %d\n", i, j);

    // travel until right or down is available
    while(node) {
        if( (node == ui->zrzc) && 
            (node->right->val > 0) &&
            (node->down->val > 0) ) {
            printf("Error: Failed to find path to end point\n");
            break;
        }

        if ( (node->right && node->right->val > 0 && node->down && node->down->val > 0) ||
             (node->right && node->right->val > 0 && node->down == NULL) ||
             (node->down && node->down->val > 0   && node->right == NULL) ) {
            node->val = 2;
            // restart the algo from begining
            printf("Restarting finding path:\n");
            node = ui->zrzc;
            i = ui->starti;
            j = ui->startj;
            // start position
            printf("%d %d\n", i, j);
        }
        
        // this is when you have reached last node
        if (node->right == NULL && node->down == NULL) break;

        // first travel right
        if (node->right && node->right->val == 0) {
            node = node->right;
            j++;
            printf("%d %d\n", i, j);
            fprintf(fp, "%d %d\n", i, j);
            continue;
        }

        // next travel down
        if (node->down && node->down->val == 0) {
            node = node->down;
            i++;
            printf("%d %d\n", i, j);
            fprintf(fp, "%d %d\n", i, j);
            continue;
        }
    }
}

int main(int argc, char *argv[])
{
    if (argc > 1) {
        char *filepath = argv[1];
        userInput_t ui;
        readInputFile(filepath, &ui);
        displayMatrixList(ui.zrzc);
        findTravelPath(&ui);
    } else {
        printf("Error: Supply input file path\n");
        exit(1);
    }
    return 0;
}