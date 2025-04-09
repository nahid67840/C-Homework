MD Mahabub Miah
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// 삼각형을 출력하는 함수
void print_triangle(int n) {
    // 직삼각형
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            printf(" ");
        }
        for (int j = 0; j <= i; j++) {
            printf("*");
        }
        printf("\n");
    }
}

void print_reverse_triangle(int n) {
    // 역직삼각형
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < i; j++) {
            printf(" ");
        }
        for (int j = 0; j < n - i; j++) {
            printf("*");
        }
        printf("\n");
    }
}

int main() {
    char name[100];
    char date[11];  // yyyy-mm-dd 포맷
    time_t t;
    struct tm tm_info;

    // 날짜와 이름 입력 받기
    printf("[현재 날짜를 \"yyyy-mm-dd\" 형식으로 입력하세요]: ");
    scanf("%10s", date);
    printf("[당신의 이름을 입력하세요]: ");
    scanf(" %[^\n]s", name);  // 이름 입력 시 공백 포함 가능하게

    // 정상 입력 처리 메시지
    printf("**입력이 정상적으로 처리되었습니다.**\n");

    // 3초 대기 후 화면 지우기
    sleep(3);
    system("clear");  // 윈도우에서는 "cls"로 대체 가능

    // 삼각형 출력
    int triangle_size = 10;
    print_triangle(triangle_size);
    print_reverse_triangle(triangle_size);

    // 스플래시 화면 출력
    printf("+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    printf("*                         [마그라테아 ver 0.1]                               *****\n");
    printf("**   풀 한 포기 없는 황무지에서 반짝이는 행성을 만들내는 곳 마그라테아,            ****\n");
    printf("*   사람들이 보지 못하는 잠재력을 찾고 전문가의 손길을 더해 보석을 빗는 곳,       ***\n");
    printf("**    마그라테아에 오신걸 환영합니다.                                         **\n");
    printf("***                                                                          *\n");
    printf("+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    printf("[사용자]: %s                                   [실행 시간]:%s\n", name, date);
    printf("============================================================================\n");

    return 0;
}
