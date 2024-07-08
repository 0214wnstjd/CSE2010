# How to run
- `gcc main.c -o main`
- `./main input.txt`

# Specification

## Goal
**Binary Search Tree 구현**

## Environment
MacOS     
C    
gcc

# Implementation

## Code 구조
**struct TreeNode** : Tree의 node 구조체   
- 멤버 변수:
  -	int data(key를 담음) 
  -	struct TreeNode *left, *right(child들에 대한 pointer)

**struct QueueNode** : Queue의 node 구조체
멤버 변수:
-	TreeNode *item(Tree의 node를 담는 포인터)
-	struct QueueNode *link(queue에서 다음 node에 대한 pointer)

**struct Queue** : QueueType 구조체
- 멤버 변수:
  -	QueueNode *front, *rear(queue의 앞, 뒤에 대한 pointer)

**void init(Queue \*q)** : Queue 초기화 함수
- front, rear pointer에 NULL을 넣어 초기화   

**int is_empty(Queue \*q)**: Queue 공백 체크 함수
- enqueue된게 없거나, dequeue가 다 되어 비어있다면, front가 NULL이 되었다는 뜻이므로, front가 NULL이면 1, 아니면 0 return함

**void enqueue(Queue \*q, TreeNode \*item)**: Queue 삽입 함수
- QueueNode를 동적할당하고, item이 TreeNode를 가리키게 해준 뒤, Queue의 rear의 link로 추가하거나 비어있는 queue라면 front, rear를 새로운 node로 바꿈

**TreeNode *dequeue(Queue *q)**: Queue 삭제 함수
- 빈 큐가 아니라면, front를 반환하기 위해 임시 포인터를 두고, front를 front의 다음(link)로 바꾼 뒤 메모리 해제 후 반환함

**TreeNode *search(TreeNode *node, int key)**: Tree 탐색 함수
- node부터 시작하여 key값과 node의 data를 비교하여 left 또는 right subtree로 내려감, 같을 때 즉시 return, 못찾으면 NULL return

**void insert_node(TreeNode \**root, int key)**: Tree 삽입 함수
- key와 현재 node의 data를 비교해가며 위치를 탐색함, 예외적으로 같을 때(중복)이면 left child로 고정함. 새로운 TreeNode 동적할당 후, 찾았던 위치에 삽입. empty tree일 땐 root에 삽입.

**int delete_node(TreeNode *root, int key)**: Tree 삭제 함수
- key를 가진 node를 탐색함, 삭제를 위해 부모 node 정보 또한 필요.
- 못 찾았을 경우 temp에 NULL이 들어가므로 해당 상황일 때 0을 return.
- 3가지 case로 분류하여 삭제 진행.
  1. 단말노드일 때, 부모와 해당 자식의 연결을 끊고 메모리 해제.
  2.  하나의 자식만 가질 때, 부모와 해당 자식의 연결 포인터에 자식의 자식을 연결 하고 메모리 해제.
  3. 두개의 자식을 다 가질 때, right subtree의 가장 왼쪽 node(data값이 제일 작음)를 찾아서 삭제 대상과 바꿔치기 하고 메모리 해제함.

**int get_height(TreeNode *node)**: Tree의 높이 계산 함수
- 재귀 호출하여 subtree 중 높이가 더 큰것에 1을 더하여 계산

**void print_level_order(TreeNode *root, FILE *fp)**: Tree를 레벨 순회하여 file write 함수
- get_height함수를 호출하여 높이를 계산하고, 높이를 통해 최대 개수를 계산함.
- root부터 enqueue하기 시작하여 dequeue후 file에 write해준 뒤, left child, right child 순으로 enqueue하는데, 자식이 없을 시 0을 data로 가지는 empty node를 enqueue한다.
- Tree의 최대 개수만큼 반복하여 각 레벨이 모두 출력되게 한다.

**void free_all_node(TreeNode *root)**: TreeNode를 모두 해제하는 함수
- LRV 순으로 free를 시킨다.

**int main(int argc, char \*argv[])**
- input.txt파일을 줄단위로 읽어와서 buffer에 저장.
- buffer[0]이 command가 되고 buffer[1]이 data가 됨.
- command i라면 삽입 후 파일에 Tree 출력.
- command s라면 탐색 후 결과 출력.
- command d라면 삭제 후 결과에 따라 Tree 또는 실패메시지 출력.
- 파일이 끝나면 free_all_node함수를 호출하여 Tree의 모든 Node 해제.

## Example
```
input.txt
-----------------------------------------
i5
i3
i6
i4
i7
s5
i9
d6
d2
i5
d5
s5
d5
s5
```
```
output.txt
-----------------------------------------
i5 : 5
i3 : 5, 3, 0
i6 : 5, 3, 6
i4 : 5, 3, 6, 0, 4, 0, 0
i7 : 5, 3, 6, 0, 4, 0, 7
s5 : Exist
i9 : 5, 3, 6, 0, 4, 0, 7, 0, 0, 0, 0, 0, 0, 0, 9
d6 : 5, 3, 7, 0, 4, 0, 9
d2 : Not Exist
i5 : 5, 5, 7, 3, 0, 0, 9, 0, 4, 0, 0, 0, 0, 0, 0
d5 : 7, 5, 9, 3, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0
s5 : Exist
d5 : 7, 3, 9, 0, 4, 0, 0
s5 : Not Exist
``` 

## 기타
동적할당 후 메모리 누수 방지를 위해 해제 해주는 것이 중요함. free_all_node()함수를 만들어 LRV 순으로 해제하게 하였음. 중위 순회 해제를 하는 이유는 자식을 해제하기 전에 부모를 해제해버리면 자식과의 연결이 끊겨 버려서 해제할 수 없게 되기 때문에 자식 둘을 먼저 해제하고 부모를 해제한다.

중복 삽입의 경우, 기존 노드의 left child에 추가하는 명세가 있어, left child에 추가하며 이진탐색 트리의 특성을 유지하기 위해 기존 노드의 left child를 추가된 노드의 left child로 가지게 하였음.

    

