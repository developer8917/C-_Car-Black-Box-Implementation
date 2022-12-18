#include <iostream>
#include <stdio.h>
#include <thread>
#include <Windows.h>		// 화면 비우기용

using namespace std;

class mainVariable
{
private:
	int recodingTime = 60;	// 녹화 시간을 설정하는 변수
	bool onOff = false;		// 전역변수는 신이야
public:
	void setOnOff(bool pOnOff) { this->onOff = pOnOff; }
	void setRecodingTime(int pRecodingTime) { this->recodingTime = pRecodingTime; }

	bool getOnOff() { return onOff; }
	int getRecodingTime() { return recodingTime; }
};

mainVariable var;			// 전역 변수로 클래스 선언

void startPrint();			// 시작 화면 출력 함수
void Selectmenu();			// 메뉴 선택 함수
void recodingOnOff();		// 1번, 녹화 ON/OFF 함수
void recordingTimeCheck();	// 2번, 잔여 녹화 시간 확인 함수
void setRecodingTime();		// 3번, 녹화 시간 설정 함수
void* Thread_Func();		// 녹화할때 별도로 작업하게 될 쓰레드 함수

int main()
{
	// 맨 처음 화면 출력을 위해 함수 호출
	startPrint();

	// 메뉴 선택 함수
	Selectmenu();

	return 0;	// 메뉴에서 4번 입력 시 Selectmenu 함수를 탈출하여 종료됨
}

// 시작 화면 출력 함수
void startPrint()
{
	printf("===== BlackBox Simulator =====\n");
	// (1. 녹화 ON/OFF    [[ 현재상태 : 변수(On/Off) ]]
	// 여기부터 ~
	printf("1. 녹화 ON/OFF    [[ 현재상태 : ");
	var.getOnOff() ? printf("ON") : printf("OFF");
	printf(" ]]\n");
	// ~ 여기까지가 1번 텍스트 출력 소스

	// 2번, 3번, 4번 메뉴 출력
	printf("2. 잔여 녹화 시간 확인\n");
	printf("3. 녹화 시간 설정\n");
	printf("4. 프로그램 종료\n");
}

// 메뉴 선택 함수
void Selectmenu()
{
	// 4번 눌러서 프로그램 종료를 선택하기 전까지 무한 반복
	while (1)
	{
		int menuSelect = 0;			// 메뉴 선택 함수, 반복할때마다 초기화

		// 선택할 기능 선택
		printf("기능 선택 => ");
		scanf_s("%d", &menuSelect);

		printf("선택한 숫자 : %d\n", menuSelect);

		switch ((int)menuSelect)
		{
		case 1:						// 녹화 ON/OFF 선택
			system("cls");			// 화면 지움

			// 남은 시간이 0초가 아닐떄만 녹화 진행
			if (var.getRecodingTime() != 0)
			{
				recodingOnOff();
				startPrint();
			}
			// 남은 시간이 0초일 때는 메시지만 보내주고 메뉴 화면 다시 출력
			else
			{
				startPrint();
				printf("녹화 시간이 남아있지 않습니다.\n");
				printf("녹화 시간을 재설정하세요.\n");
			}
			break;
		case 2:						// 잔여 녹화 시간 확인 선택
			system("cls");			// 화면 지움, 메뉴 아래에 잔여 시간 표기해야되서 순서 바뀜
			startPrint();			// 메뉴 출력 후
			recordingTimeCheck();	// 그 아래에 잔여 시간 표기
			break;
		case 3:						// 녹화 시간 설정 선택
			setRecodingTime();
			system("cls");			// 화면 지움
			startPrint();
			break;
		case 4:						// 프로그램 종료 선택
			break;
		default:
			system("cls");			// 화면 지움, 메뉴 아래에 잘못된 숫자를 입력했다는 문구를 표기해야되서 순서 바뀜
			startPrint();			// 메뉴 출력 후
			printf("잘못된 숫자를 입력하셨습니다.\n");
			printf("다시 입력하세요\n");
			break;
		}

		if (menuSelect == 4)		// 프로그램 종료 선택 시 (4번)
			break;					// 반복문을 나감
	}
}

// 1번, 녹화 ON/OFF 함수
void recodingOnOff()
{
	switch (var.getOnOff())
	{
	case true:
	{
		var.setOnOff(false);				// 종료 상태로 변경
		break;
	}
	case false:
	{
		var.setOnOff(true);					// 실행 상태로 변경
		thread threadStart(Thread_Func);	// 녹화 작업을 별도로 진행 할 스레드 실행
		threadStart.detach();				// 스레드가 언제 종료될지 모르므로 detach 사용
		break;
	}
	}
}

// 녹화를 진행할 스레드 함수
void* Thread_Func()
{
	FILE* file = NULL;					// 파일 선언

	fopen_s(&file, "data.rec", "a+");	// data.rec 파일로 저장
	char str[] = "recoding...->";		// 숫자 앞에 텍스트 표기용 변수

	// 텍스트 변수
	char recodeStart[] = "녹화를 시작합니다.";
	char timeOver[] = "녹화 시간을 모두 사용하였습니다.";
	char recodeEnd[] = "녹화가 종료됩니다.";

	// 녹화를 처음 시작할 때 기록
	if (file != NULL && var.getRecodingTime() != 0)			// file은 0일 수 있다고 나와서 그냥 추가
	{
		fprintf(file, "%s\n", recodeStart);					// 녹화 시작을 기록
	}

	while (0 < var.getRecodingTime())	// 녹화시간에 도달하기 전까지 계속 반복
	{
		if (file != NULL)	// 녹화 파일이 있다면
		{
			fprintf(file, "%s%d\n", str, var.getRecodingTime());	// 녹화되고 있다는 의미로 텍스트 기록
		}
		else				// 녹화 파일 없으면
		{
			printf("\n녹화할 파일이 존재하지 않습니다.\n");	// 파일이 존재하지 않는다고 출력하기
			printf("3초 후 화면을 초기화 합니다.\n");		// 3초후 화면 리셋한다고 알려주기 (다른거 누르지 말라는 의미로)

			Sleep(3 * 1000);		// 3초 후 (임의로 지정)
			system("cls");		// 화면 한번 지워준 후
			startPrint();		// 메뉴 다시 출력		
		}
		var.setRecodingTime(var.getRecodingTime() - 1);	// 남은 녹화시간을 가져와서 -1 후 다시 대입
		Sleep(1000);	// 1000 = 1초 쉬고 반복해라

		if (var.getRecodingTime() == 0)			// 잔여 녹화 시간이 0초가 되면 녹화가 종료됨
		{
			if (file != NULL)	// 녹화 파일이 있다면
			{
				fprintf(file, "%s\n", timeOver);	// 녹화시간이 만료 되었다고 기록
			}
			var.setOnOff(false);				// 녹화 상태롤 종료 상태로 변경
			system("cls");						// On 상태를 Off 상태로 바꿔야되서 화면 지움
			startPrint();						// 메뉴 다시 출력
		}

		if (var.getOnOff() == false)			// 녹화 OFF 상태로 전환하면
		{
			if (file != NULL)						// file은 0일 수 있다고 나와서 그냥 추가
			{
				fprintf(file, "%s\n", recodeEnd);	// 녹화가 종료 되었다고 기록
			}
			break;								// 반복문 탈출
		}
	}

	// 녹화 다 했을 때 (반복문 나왔을 때
	if (file != NULL)		// 파일이 없는게 아니라면
		fclose(file);		// fopen함수로 개방한 파일을 닫아라.

	return NULL;			// 반환할게 없으므로 NULL 처리 (무조건 반환 해야되는지 알았네)
}

// 2번, 잔여 녹화 시간 확인 함수
void recordingTimeCheck()
{
	// 현재 잔여 녹화시간 출력
	printf("현재 잔여 녹화시간 : %d초\n", var.getRecodingTime());
}

// 3번, 녹화 시간 설정 함수
void setRecodingTime()
{
	int setRecord = 0;						// 재설정한 녹화 시간을 대입할 변수 선언

	printf("녹화 시간 입력 (초단위) -> ");
	scanf_s("%d", &setRecord);				// 재설정할 녹화 시간 입력

	var.setRecodingTime(setRecord);			// 녹화 시간 변경

	// 녹화 안하고 있고, 남은 시간이 0초가 아닐때만 (남은 시간이 0초인데 함수가 작동하는걸 방지)
	if (var.getOnOff() == false && var.getRecodingTime() != 0)
		recodingOnOff();					// 녹화 시작 함수 작동
}