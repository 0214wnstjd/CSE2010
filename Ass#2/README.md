# How to run
- `gcc main.c -o main`
- `./main input.txt`



# Specification

## Goal
**미로 출구 찾기 프로그램을 Stack으로 구현**

## Environment
MacOS     
C    
gcc

# Implementation

## Code 구조
**struct element** : 좌표 구조체
-	멤버 변수 : short r 행, shor c 열

**struct StackType** : stack 구조체
-	멤버 변수 : element stack[100] 스택 배열, int top 스택 top

**void init(StackType \*s)** : stack 초기화 함수

**int is_empty(StackType \*s)** : stack 공백 여부 반환 함수   

**int is_full(StackType \*s)** : stack 포화 여부 반환 함수    

**void push(StackType \*s, element item)** : stack에 item push 함수   

**void free_maze(void)** : 전역변수 maze 2차원 배열을 할당 해제   

**void print_maze(FILE \*fp)** : 파일에 maze를 출력하는 함수    

**element pop(StackType \*s)** : stack에서 top을 pop하여 반환하는 함수    

**void pushLoc(StackType \*s, int r, int c)** : (r,c)가 통로일 때 push하는 함수    

**int main(int argc, char \*argv[])**
-	실행 argument를 받아 read하여 parsing 후 maze 할당 및 채움
-	출구를 찾을 때까지 pushLoc을 통해 경로 탐색
-	콘솔에 경로 및 성공 여부 출력
-	Print_maze 함수를 통해 output.txt에 maze 출력

## 성공 Case
```
input.txt
-----------------------------------------
1 1 E 1 1 1
1 0 0 0 0 1
1 0 1 0 1 1
1 1 1 0 0 1
1 1 1 0 0 1
1 1 1 0 1 1
1 1 0 0 1 1
1 X 0 1 1 1
-----------------------------------------
의미: 
1 : 미로의 벽
0 : 미로의 통로
E : 미로의 입구
X : 미로의 출구 
```
```
output.txt
-----------------------------------------
```
  <img width="85" alt="image" src="https://github.com/0214wnstjd/CSE2010/assets/109850168/e2283617-79c5-45c6-9f9f-b089ccd183b5">

```
-----------------------------------------
의미: 
[] : 미로의 벽
공백 : 통로
. : 탐색 경로
E : 입구 
X : 출구 
```
```
터미널/콘솔
-----------------------------------------
출력형식:
시작 (0, 2) -> (1, 2) -> (1, 3) -> (1, 4) ->
 (2, 3) -> (3, 3) -> (3, 4) -> (4, 4) ->
 (4, 3) -> (5, 3) -> (6, 3) -> (6, 2) ->
 (7, 2) -> (7, 1) -> 도착
탐색 성공
```
## 실패 Case
```
input.txt
-----------------------------------------
1 1 E 1 1 1
1 0 0 0 0 1
1 0 1 0 1 1
1 1 1 0 0 1
1 1 1 0 0 1
1 1 1 0 1 1
1 1 1 0 1 1
1 X 0 1 1 1
-----------------------------------------
의미: 
1 : 미로의 벽
0 : 미로의 통로
E : 미로의 입구
X : 미로의 출구 
```
```
output.txt
-----------------------------------------
```
  <img width="85" alt="image" src="https://github.com/0214wnstjd/CSE2010/assets/109850168/2dd7ecce-d87c-45f4-b110-b9e2e446bc22">
  
```
-----------------------------------------
의미: 
[] : 미로의 벽
공백 : 통로
. : 탐색 경로
E : 입구 
X : 출구 
```
```
터미널/콘솔
-----------------------------------------
출력형식:
시작 (0, 2) -> (1, 2) -> (1, 3) -> (1, 4) ->
 (2, 3) -> (3, 3) -> (3, 4) -> (4, 4) ->
 (4, 3) -> (5, 3) -> (6, 3) -> (4, 3) ->
 (1, 1) -> (2, 1) -> 실패
```
## 기타
동적할당 후 메모리 누수 방지를 위해 해제 해주는 것이 중요함.   
free_maze()함수를 만들어 간단하게 해제하였음.



