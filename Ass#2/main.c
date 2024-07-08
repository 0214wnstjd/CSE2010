#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_STACK_SIZE 100

// 좌표 구조체
typedef struct
{
    short r;
    short c;
} element;

// Stack 구조체
typedef struct
{
    element stack[MAX_STACK_SIZE]; // array로 구현한 stack
    int top;                       // stack top
} StackType;

// 전역 변수
char **maze = NULL; // maze : 2 dimentional array
int row = 0;        // maze's row
int column = 0;     // maze's column

// stack init 함수
void init(StackType *s)
{
    s->top = -1;
}

// stack 비었는지 체크하는 함수
int is_empty(StackType *s)
{
    // stack이 empty라면, 즉 top이 -1이면 return 1
    // else return 0
    return (s->top == -1);
}

// stack 포화인지 체크하는 함수
int is_full(StackType *s)
{
    // stack 꽉찼다면, 즉 top이 99라면 return 1
    // else return 0
    return (s->top == (MAX_STACK_SIZE - 1));
}

// stack에 push하는 함수
void push(StackType *s, element item)
{
    // stack 포화인지 체크
    if (is_full(s))
    {
        printf("스택 포화 에러\n");
        return;
    }
    else
        // top에 item 추가
        s->stack[++(s->top)] = item;
}

// maze 2차원 배열 동적할당 해제 함수
void free_maze(void)
{
    // char * 해제
    for (int i = 0; i < row; i++)
    {
        free(maze[i]);
    }
    // char ** 해제
    free(maze);
}

// maze를 file에 출력하는 함수
void print_maze(FILE *fp)
{
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < column; j++)
        {
            // 막힌 곳
            if (maze[i][j] == '1')
                fprintf(fp, "[]");
            // 입구
            else if (maze[i][j] == 'E')
                fprintf(fp, "E ");
            // 도착점
            else if (maze[i][j] == 'X')
                fprintf(fp, "X ");
            // 경로
            else if (maze[i][j] == '.')
                fprintf(fp, ". ");
            // 뚫린 곳
            else
                fprintf(fp, "  ");
        }
        // 마지막 행이 아닐 때만 개행
        if (i != row - 1)
            fprintf(fp, "\n");
    }
}

// stack에서 pop하는 함수
element pop(StackType *s)
{
    // 스택 비어있는지 체크
    if (is_empty(s))
    {
        printf("스택 공백 에러\n");
        exit(1);
    }
    else
        // stack top에서 element 꺼내고 top을 1 감소시킴
        return s->stack[(s->top)--];
}

// 가능한 경로일 때 push하는 함수
void pushLoc(StackType *s, int r, int c)
{
    // 외곽 바깥 주변이 호출되었을때
    if (r < 0 || c < 0)
        return;
    if (r >= row || c >= column)
        return;

    // 막혔거나 이미 지난곳이 아닐때만 push
    if (maze[r][c] != '1' && maze[r][c] != '.')
    {
        // tmp element 만들고 push
        element tmp;
        tmp.r = r;
        tmp.c = c;
        push(s, tmp);
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

    // 행과 열 계산
    while (!feof(fp1))
    {
        fgetc(fp1);

        // 첫 행에서 열 계산
        if (!row)
            column++;

        // '\n'이나 파일의 끝일때, 즉 다음 행이 나오거나 마지막일 때 row 증가
        if (fgetc(fp1) == '\n' || feof(fp1))
        {
            row++;
        }
    }

    // maze 크기 동적 할당
    maze = (char **)malloc(row * sizeof(char *));
    for (int i = 0; i < row; i++)
    {
        maze[i] = (char *)malloc(column * sizeof(char));
    }

    // input 파일 maze에 저장
    fseek(fp1, 0, SEEK_SET);
    element entry = {0, 0}; // 입구 좌표
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < column; j++)
        {
            // maze에 문자 저장
            maze[i][j] = fgetc(fp1);

            // 입구 지정
            if (maze[i][j] == 'E')
            {
                entry.r = i;
                entry.c = j;
            }

            // ' '와 '\n' 문자 넘기는 용도
            fgetc(fp1);
        }
    }

    fclose(fp1);

    // 미로를 output.txt에 출력
    print_maze(fp2);
    fprintf(fp2, "\n\n탐색 경로\n");

    int r, c;    // 좌표
    StackType s; // stack

    // stack 초기화
    init(&s);

    element here = {0, 0}; // 시작 좌표

    // 시작 좌표 입구로 설정
    here = entry;

    // 콘솔에 출력
    printf("시작 (%d, %d) ", here.r, here.c);

    // 출구를 찾을 때까지
    while (maze[here.r][here.c] != 'X')
    {
        r = here.r;
        c = here.c;

        // 경로 표시
        maze[r][c] = '.';

        // 현재 위치의 상하 좌우 좌표에 대해 pushLoc 호출
        pushLoc(&s, r - 1, c);
        pushLoc(&s, r + 1, c);
        pushLoc(&s, r, c - 1);
        pushLoc(&s, r, c + 1);

        // 더이상 갈 수 있는 곳이 없을 때
        if (is_empty(&s))
        {
            // 탐색 경로 출력
            print_maze(fp2);
            printf("-> 실패\n");

            // maze 해제
            free_maze();
            return 0;
        }
        else
        {
            // stack에서 다음 경로 pop
            here = pop(&s);
            printf("-> (%d, %d) ", here.r, here.c);
        }
    }

    // 탐색 경로 출력
    print_maze(fp2);
    printf("-> 도착\n탐색 성공\n");

    fclose(fp2);

    // maze 해제
    free_maze();

    return 1;
}