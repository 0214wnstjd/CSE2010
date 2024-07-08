#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 연결 리스트 노드
typedef struct ListNode
{
    int coef;              // 계수
    int expon;             // 지수
    struct ListNode *link; // 다음 노드를 가리키는 포인터
} ListNode;

// 연결 리스트 헤더
typedef struct ListHeader
{
    int length;     // 연결 리스트 길이(노드의 개수)
    ListNode *head; // 연결 리스트 시작
    ListNode *tail; // 연결 리스트 끝
} ListHeader;

// 초기화 함수
void init(ListHeader *plist)
{
    plist->length = 0;
    plist->head = plist->tail = NULL;
}

// node 추가 함수
void insert_node_last(ListHeader *plist, int coef, int expon)
{
    ListNode *temp = (ListNode *)malloc(sizeof(ListNode));
    if (temp == NULL) // malloc error 발생
    {
        printf("malloc 할당 에러\n");
        exit(0);
    }
    temp->coef = coef;       // coef 입력
    temp->expon = expon;     // expon 입력
    if (plist->tail == NULL) // tail이 NULl, 즉 plist가 empty
    {
        // head와 tail 모두 temp로 설정
        plist->head = plist->tail = temp;
    }
    else // tail이 NULL이 아님, 즉 plist에 node가 있음
    {
        // 기존 tail의 다음으로 temp 연결 후 tail을 temp로
        plist->tail->link = temp;
        plist->tail = temp;
        plist->length++;
    }
}
// free 함수
void free_all_node(ListHeader *plist)
{
    ListNode *temp; // node를 free전 임시 저장하기위한 포인터
    for (ListNode *node = plist->head; node != NULL;)
    {
        temp = node;
        node = node->link;
        // temp의 동적할당 메모리 해지
        free(temp);
    }
}

// 다항식 덧셈
ListHeader poly_add(ListHeader *plist1, ListHeader *plist2)
{
    ListNode *a = plist1->head; // plist1의 node를 가리키는 pointer
    ListNode *b = plist2->head; // plist2의 node를 가리키는 pointer
    ListHeader plist3;          // 결과가 저장될 곳의 header
    init(&plist3);
    int sum;
    while (a && b) // a와 b가 모두 null이 아니라면, 즉 항이 남아있다면 반복
    {
        if (a->expon == b->expon) // a와 b가 가리키는 항이 같은 차수
        {
            sum = a->coef + b->coef;
            if (sum) // 계수의 합이 0이 아닐때만 추가
                insert_node_last(&plist3, sum, a->expon);
            // a, b를 다음 항을 가리키게 이동
            a = a->link;
            b = b->link;
        }
        else if (a->expon > b->expon) // a 항이 지수가 더큼
        {
            // a 항 추가
            insert_node_last(&plist3, a->coef, a->expon);
            a = a->link;
        }
        else // b 항이 지수가 더큼
        {
            // b 항 추가
            insert_node_last(&plist3, b->coef, b->expon);
            b = b->link;
        }
    }
    // a의 남은 항들 추가
    for (; a != NULL; a = a->link)
        insert_node_last(&plist3, a->coef, a->expon);
    // b의 남은 항들 추가
    for (; b != NULL; b = b->link)
        insert_node_last(&plist3, b->coef, b->expon);

    // 결과 header 리턴
    return plist3;
}

// 다항식 곱셈
void poly_mul(ListHeader *plist1, ListHeader *plist2, ListHeader *plist3)
{
    ListNode *a = plist1->head; // plist1의 node를 가리키는 pointer
    ListNode *b = plist2->head; // plist2의 node를 가리키는 pointer
    ListHeader temp;            // a의 한 항과 b의 모든 항에 대한 곱셈의 결과를 저장할 임시 헤더
    ListHeader temp_plist3;     // add전 plist3를 저장할 임시 헤더
    init(&temp);
    while (a) // a가 null이 아니라면, 즉 항이 남아있다면 반복
    {
        // temp에 곱셈 결과 저장
        insert_node_last(&temp, a->coef * b->coef, a->expon + b->expon);
        // b가 다음 항을 가리키게 이동
        b = b->link;
        if (b == NULL) // b가 NULL이면, 즉 b의 모든 항과 곱셈 끝
        {
            // temp_plist3에 기존 plist3 header가 가리키는 곳 저장
            temp_plist3 = *plist3;
            // plist3에 temp로 다항식 덧셈을 해줌
            *plist3 = poly_add(plist3, &temp);
            // 메모리 누수 방지를 위해 기존 node들 해지
            free_all_node(&temp_plist3);
            // a가 다음 항을 가리키게 이동
            a = a->link;
            // b는 첫 항을 가리키게 이동
            b = plist2->head;
            // 메모리 누수 방지를 위해 temp node들 해지
            free_all_node(&temp);
            // temp 초기화
            init(&temp);
        }
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
    char buffer[100];
    char *token;
    ListHeader a, b;
    init(&a);
    init(&b);
    int i, coef, expon, turn = 0;
    while (!feof(fp1)) // fp1의 끝에 도달할때까지 반복
    {
        // input file parsing하여 a와 b에 입력
        fgets(buffer, 100, fp1);
        if (!strcmp(buffer, "\n")) // b에 입력할 차례가 될때
        {
            turn = 1;
            continue;
        }
        // 공백기준으로 descript
        token = strtok(buffer, " ");
        i = 0;
        coef = 0;
        expon = 0;
        while (token != NULL)
        {
            if (!i)
            {
                // 계수 입력
                coef = atoi(token);
            }
            else
            {
                // 지수 입력
                expon = atoi(token);
            }
            i++;
            // 다음 공백까지의 문자열
            token = strtok(NULL, " ");
        }
        if (!turn)
        {
            insert_node_last(&a, coef, expon);
        }
        else
        {
            insert_node_last(&b, coef, expon);
        }
    }
    fclose(fp1);
    ListHeader c;
    init(&c);
    c = poly_add(&a, &b);
    fputs("Addition\n", fp2);
    for (ListNode *node = c.head; node != NULL; node = node->link)
    {
        // c의 모든 노드 파일에 출력
        fprintf(fp2, "%d %d\n", node->coef, node->expon);
    }
    init(&c);
    poly_mul(&a, &b, &c);
    fputs("Multiplication\n", fp2);
    for (ListNode *node = c.head; node != NULL; node = node->link)
    {
        // c의 모든 노드 파일에 출력
        fprintf(fp2, "%d %d\n", node->coef, node->expon);
    }
    fclose(fp2);
    free_all_node(&a);
    free_all_node(&b);
    free_all_node(&c);
    return 0;
}