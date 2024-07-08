# How to run
- `gcc main.c -o main`
- `./main input.txt`



# Specification

## Goal
**Linked List로 다항식을 구현, 다항식의 덧셈과 곱셈 연산 구현**

## Environment
MacOS     
C    
gcc

# Implementation

## Code 구조

**ListNode 구조체**
  - 다항식의 한 항을 표현하기위한 계수 지수 및 다음 노드 링크포인터로 구성

**ListHeader 구조체**
  -	리스트 길이 및 헤드 테일 포인터로 구성

**init 함수**
  -	header 포인터를 인자로 받아 길이 및 헤드 테일 포인터 0과 NULL로 초기화

**insert_node_last 함수(void)**
  -	header 포인터, 계수, 지수를 인자로 받음
  -	node 동적할당 후 header 포인터의 tail에 추가

**free_all_node 함수(void)**
  -	header 포인터를 인자로 받음
  -	동적할당 메모리를 누수를 방지하기 위해 연결리스트의 모든 노드 해지

**poly_add 함수(ListHeader)**
  -	피연산 header 포인터 두개 인자로 받음
  -	지수를 비교하여 지수 순서대로 결과 header에 insert_node_last를 통해 추가
  -	덧셈 결과를 담는 리스트를 가리키는 header 리턴

**poly_mul 함수**
  -	피연산 header 포인터 두개와 연산 결과를 저장하기위한 header 포인터 한개를 인자로 받음
  -	a 리스트의 한 노드와 b 리스트와의 곱셈결과를 임시 저장
  -	해당 곱셈결과를 poly_add 함수를 통해 합침
  -	메모리 누수 방지를 위해 중간중간 더이상 안 쓰는 list들 free_all_node를 통해 메모리 해지

**main 함수**
  -	argc, argv를 통해 실행 인자 받아옴
  -	input 파일 오픈하여 fgets를 통해 한줄씩 받아옴
  -	string.h의 strtok함수 이용해서 parsing
  -	각각 a와 b에 저장한 후 덧셈 곱셈 연산 결과 c에 저장
  -	fprintf, fputs 함수를 통해 c 리스트 output 파일에 출력

## Example
```
input.txt
-----------------------------------------
6 4
3 3
9 2
5 0

8 5
2 4
4 3
2 1
-----------------------------------------
의미: 
(6x^4+3x^3+9x^2+5),(8x^5+2x^4+4x^3+2x)
```
```
output.txt
-----------------------------------------
Addition
8 5
8 4
7 3
9 2
2 1
5 0
Multiplication
48 9
36 8
102 7
30 6
88 5
16 4
38 3
10 1
-----------------------------------------
의미: 
(8x^5+8x^4+7x^3+9x^2+2x+5), 
(48x^9+36x^8+102x^7+30x^6+88x^5+16x^4+38x^3+10x)
``` 

## 기타
**poly_add**   
공간복잡도: O(m+n)   
시간복잡도: O(m+n)   

**poly_mul**   
공간복잡도: O(mn)   
시간복잡도: O(mn)    

