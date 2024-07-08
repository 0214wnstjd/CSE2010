#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define max(a, b) (((a) > (b)) ? (a) : (b))

// Tree Node
typedef struct TreeNode
{
    int data;
    struct TreeNode *left, *right;
} TreeNode;

// Queue Node
typedef struct QueueNode
{
    TreeNode *item;
    struct QueueNode *link;
} QueueNode;

// Queue
typedef struct Queue
{
    QueueNode *front, *rear;
} Queue;

// Queue 초기화 함수
void init(Queue *q)
{
    q->front = q->rear = NULL;
}

// Queue 공백 체크 함수
int is_empty(Queue *q)
{
    // enqueue 된게 없음, 즉 front가 null
    return (q->front == NULL);
}

// Queue 삽입 함수
void enqueue(Queue *q, TreeNode *item)
{
    // 노드 동적할당
    QueueNode *temp = (QueueNode *)malloc(sizeof(QueueNode));
    if (temp == NULL)
    {
        printf("Malloc Error\n");
        return;
    }
    temp->item = item;
    temp->link = NULL;
    // empty queue에 처음 enqueue
    if (is_empty(q))
    {
        q->front = temp;
        q->rear = temp;
    }
    else
    {
        // rear -> temp 로 연결, rear를 temp로
        q->rear->link = temp;
        q->rear = temp;
    }
}

// Queue 삭제 함수
TreeNode *dequeue(Queue *q)
{
    QueueNode *temp = q->front;
    TreeNode *item;
    // 공백 체크
    if (is_empty(q))
    {
        printf("Empty Queue\n");
        return NULL;
    }
    else
    {
        // front 삭제 및 반환
        item = temp->item;
        q->front = q->front->link;
        // 공백이 될 때
        if (q->front == NULL)
            q->rear = NULL;
        // 메모리 해제
        free(temp);
        return item;
    }
}

// 탐색 함수
TreeNode *
search(TreeNode *node, int key)
{
    while (node != NULL)
    {
        if (key == node->data)
            return node;
        // key가 더 작으면 left subtree로 내려감
        else if (key < node->data)
            node = node->left;
        // key가 더 크면 right subtree로 내려감
        else
            node = node->right;
    }
    // 없으면 NULL 반환
    return NULL;
}

// 삽입 함수
void insert_node(TreeNode **root, int key)
{
    TreeNode *parent, *temp;
    TreeNode *new;
    temp = *root;
    parent = NULL;

    // 위치 탐색
    while (temp != NULL)
    {
        parent = temp;
        // 작으면 left child의 subtree로
        if (key < parent->data)
            temp = parent->left;
        // 같으면 left child로 고정
        else if (key == temp->data)
        {
            temp = parent->left;
            break;
        }
        // 크면 right child의 subtree로
        else
            temp = parent->right;
    }

    // 할당
    new = (TreeNode *)malloc(sizeof(TreeNode));
    if (new == NULL)
    {
        printf("Malloc Error\n");
        return;
    }
    new->data = key;
    new->left = new->right = NULL;

    // 삽입
    if (parent != NULL)
    {
        // 작으면 left child에 삽입
        if (key < parent->data)
            parent->left = new;
        // 중복시 left child 위치에 삽입
        else if (key == parent->data)
        {
            new->left = parent->left;
            parent->left = new;
        }
        // 크면 right child에 삽입
        else
            parent->right = new;
    }
    // 빈 Tree일 때
    else
        *root = new;
}

// 삭제 함수 (성공 시 1, 실패 시 0 반환)
int delete_node(TreeNode **root, int key)
{
    TreeNode *parent, *child, *successor, *successor_parent, *temp;
    parent = NULL;
    temp = *root;

    // key를 가진 노드 탐색
    while (temp != NULL && temp->data != key)
    {
        parent = temp;
        temp = (key < temp->data) ? temp->left : temp->right;
    }
    // 탐색 실패, 삭제 실패
    if (temp == NULL)
        return 0;

    // case 1: 단말노드인 경우
    if ((temp->left == NULL) && (temp->right == NULL))
    {
        if (parent != NULL)
        {
            // 부모와 연결 해제
            if (parent->left == temp)
                parent->left = NULL;
            else
                parent->right = NULL;
        }
        // 부모가 null, 즉 root를 삭제하는 경우
        else
            *root = NULL;
    }
    // case 2: 하나의 자식만 가지는 경우
    else if ((temp->left == NULL) || (temp->right == NULL))
    {
        // 자식이 left인지 right인지
        child = (temp->left != NULL) ? temp->left : temp->right;
        // 부모와 자식을 연결
        if (parent != NULL)
        {
            // left child에 연결
            if (parent->left == temp)
                parent->left = child;
            // right child에 연결
            else
                parent->right = child;
        }
        // root 삭제
        else
            *root = child;
    }
    // case 3: 두개의 자식을 가지는 경우
    else
    {
        // right subtree에서 탐색
        successor_parent = temp;
        successor = temp->right;
        // child에서 leftmost를 찾아감
        while (successor->left != NULL)
        {
            successor_parent = successor;
            successor = successor->left;
        }
        // successor 부모와 자식을 연결
        if (successor_parent->left == successor)
            successor_parent->left = successor->right;
        else
            successor_parent->right = successor->right;
        // 삭제 대상 successor로 바꿔치기
        temp->data = successor->data;
        temp = successor;
    }
    // 메모리 해제
    free(temp);
    return 1;
}

// 트리 높이 반환 함수
int get_height(TreeNode *node)
{
    int height = 0;
    // 재귀 탈출 조건 : 단말 노드의 child가 node로 호출될 때
    if (node != NULL)
    {
        // left와 right 중 더 큰 높이에 1을 더함, 재귀 호출하여 계산
        height = 1 + max(get_height(node->left), get_height(node->right));
    }
    return height;
}

// 레벨 순회 트리 파일 프린트 함수
void print_level_order(TreeNode *root, FILE *fp)
{
    Queue q;
    TreeNode *temp;
    // 빈 곳에 0을 write하기 위해 높이로 최대 개수를 계산
    int height = get_height(root);
    // 최대 개수는 2^h - 1
    double count = pow(2, height) - 1;
    // 빈 곳에 넣어줄 node
    TreeNode *empty;
    empty = (TreeNode *)malloc(sizeof(TreeNode));
    empty->data = 0;
    empty->left = empty->right = NULL;
    init(&q);
    if (root == NULL)
    {
        printf("Empty tree\n");
        return;
    }

    // 큐에 추가
    enqueue(&q, root);
    temp = root;
    // 큐가 비지 않고, 노드 최대 개수 만큼 반복
    while (!is_empty(&q) && (count > 0))
    {
        // 큐에서 꺼냄
        temp = dequeue(&q);
        // 파일에 출력
        fprintf(fp, "%d", temp->data);

        // child도 큐에 추가, 없으면 empty node 추가
        if (temp->left != NULL)
            enqueue(&q, temp->left);
        else
            enqueue(&q, empty);

        if (temp->right != NULL)
            enqueue(&q, temp->right);
        else
            enqueue(&q, empty);

        // 더 있으면 ", "로 구분, 단말 노드의 rightmost의 경우는 출력 X
        if (!is_empty(&q) && ((--count) > 0))
            fprintf(fp, ", ");
    }
}
// 후위순회 해제 함수
void free_all_node(TreeNode *root)
{
    if (root)
    {
        // LRV 순으로 free
        free_all_node(root->left);
        free_all_node(root->right);
        free(root);
    }
}

int main(int argc, char *argv[])
{
    if (argc != 2) // 실행 커맨드 오류시
    {
        printf("Correct execution form : ./main.exe [filename]\n");
        return 1;
    }
    FILE *fp1 = fopen(argv[1], "r");      // input file
    FILE *fp2 = fopen("output.txt", "w"); // output file
    if (!fp1 || !fp2)
    {
        printf("File open error\n");
        return 1;
    }
    // command 저장용 buffer
    char buffer[10];
    // command parse해서 나온 data
    int data;

    // 트리 root node
    TreeNode *root = NULL;

    while (!feof(fp1))
    {
        // buffer 초기화
        buffer[0] = '\0';
        fgets(buffer, 10, fp1);
        // data를 char to int로 변환한것, '숫자'('숫자'의 ascii code) - '0'('0'의 ascii code) = 숫자
        data = buffer[1] - '0';
        // command 출력
        fprintf(fp2, "%c%c : ", buffer[0], buffer[1]);
        // command로 구분
        switch (buffer[0])
        {
        // i 커맨드
        case 'i':
            // insert_node 호출 후 출력
            insert_node(&root, data);
            print_level_order(root, fp2);
            break;
        // s 커맨드
        case 's':
            // search 호출, NULL return 받을 시 존재하지 않음, 그 외 존재함
            if (search(root, data) == NULL)
                fprintf(fp2, "Not Exist");
            else
                fprintf(fp2, "Exist");
            break;
        // d 커맨드
        case 'd':
            // delete_node 호출, 0을 return 받을 시 존재하지 않음, 1을 return 받으면 성공, 성공 후 출력
            if (delete_node(&root, data))
                print_level_order(root, fp2);
            else
                fprintf(fp2, "Not Exist");
            break;
        }
        // 파일 끝이면 개행 안함
        if (!feof(fp1))
            fprintf(fp2, "\n");
    }
    fclose(fp1);
    fclose(fp2);
    // 메모리 해제
    free_all_node(root);
    return 1;
}