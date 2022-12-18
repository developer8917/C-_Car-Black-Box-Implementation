#include <iostream>
#include <stdio.h>
#include <thread>
#include <Windows.h>		// ȭ�� �����

using namespace std;

class mainVariable
{
private:
	int recodingTime = 60;	// ��ȭ �ð��� �����ϴ� ����
	bool onOff = false;		// ���������� ���̾�
public:
	void setOnOff(bool pOnOff) { this->onOff = pOnOff; }
	void setRecodingTime(int pRecodingTime) { this->recodingTime = pRecodingTime; }

	bool getOnOff() { return onOff; }
	int getRecodingTime() { return recodingTime; }
};

mainVariable var;			// ���� ������ Ŭ���� ����

void startPrint();			// ���� ȭ�� ��� �Լ�
void Selectmenu();			// �޴� ���� �Լ�
void recodingOnOff();		// 1��, ��ȭ ON/OFF �Լ�
void recordingTimeCheck();	// 2��, �ܿ� ��ȭ �ð� Ȯ�� �Լ�
void setRecodingTime();		// 3��, ��ȭ �ð� ���� �Լ�
void* Thread_Func();		// ��ȭ�Ҷ� ������ �۾��ϰ� �� ������ �Լ�

int main()
{
	// �� ó�� ȭ�� ����� ���� �Լ� ȣ��
	startPrint();

	// �޴� ���� �Լ�
	Selectmenu();

	return 0;	// �޴����� 4�� �Է� �� Selectmenu �Լ��� Ż���Ͽ� �����
}

// ���� ȭ�� ��� �Լ�
void startPrint()
{
	printf("===== BlackBox Simulator =====\n");
	// (1. ��ȭ ON/OFF    [[ ������� : ����(On/Off) ]]
	// ������� ~
	printf("1. ��ȭ ON/OFF    [[ ������� : ");
	var.getOnOff() ? printf("ON") : printf("OFF");
	printf(" ]]\n");
	// ~ ��������� 1�� �ؽ�Ʈ ��� �ҽ�

	// 2��, 3��, 4�� �޴� ���
	printf("2. �ܿ� ��ȭ �ð� Ȯ��\n");
	printf("3. ��ȭ �ð� ����\n");
	printf("4. ���α׷� ����\n");
}

// �޴� ���� �Լ�
void Selectmenu()
{
	// 4�� ������ ���α׷� ���Ḧ �����ϱ� ������ ���� �ݺ�
	while (1)
	{
		int menuSelect = 0;			// �޴� ���� �Լ�, �ݺ��Ҷ����� �ʱ�ȭ

		// ������ ��� ����
		printf("��� ���� => ");
		scanf_s("%d", &menuSelect);

		printf("������ ���� : %d\n", menuSelect);

		switch ((int)menuSelect)
		{
		case 1:						// ��ȭ ON/OFF ����
			system("cls");			// ȭ�� ����

			// ���� �ð��� 0�ʰ� �ƴҋ��� ��ȭ ����
			if (var.getRecodingTime() != 0)
			{
				recodingOnOff();
				startPrint();
			}
			// ���� �ð��� 0���� ���� �޽����� �����ְ� �޴� ȭ�� �ٽ� ���
			else
			{
				startPrint();
				printf("��ȭ �ð��� �������� �ʽ��ϴ�.\n");
				printf("��ȭ �ð��� �缳���ϼ���.\n");
			}
			break;
		case 2:						// �ܿ� ��ȭ �ð� Ȯ�� ����
			system("cls");			// ȭ�� ����, �޴� �Ʒ��� �ܿ� �ð� ǥ���ؾߵǼ� ���� �ٲ�
			startPrint();			// �޴� ��� ��
			recordingTimeCheck();	// �� �Ʒ��� �ܿ� �ð� ǥ��
			break;
		case 3:						// ��ȭ �ð� ���� ����
			setRecodingTime();
			system("cls");			// ȭ�� ����
			startPrint();
			break;
		case 4:						// ���α׷� ���� ����
			break;
		default:
			system("cls");			// ȭ�� ����, �޴� �Ʒ��� �߸��� ���ڸ� �Է��ߴٴ� ������ ǥ���ؾߵǼ� ���� �ٲ�
			startPrint();			// �޴� ��� ��
			printf("�߸��� ���ڸ� �Է��ϼ̽��ϴ�.\n");
			printf("�ٽ� �Է��ϼ���\n");
			break;
		}

		if (menuSelect == 4)		// ���α׷� ���� ���� �� (4��)
			break;					// �ݺ����� ����
	}
}

// 1��, ��ȭ ON/OFF �Լ�
void recodingOnOff()
{
	switch (var.getOnOff())
	{
	case true:
	{
		var.setOnOff(false);				// ���� ���·� ����
		break;
	}
	case false:
	{
		var.setOnOff(true);					// ���� ���·� ����
		thread threadStart(Thread_Func);	// ��ȭ �۾��� ������ ���� �� ������ ����
		threadStart.detach();				// �����尡 ���� ������� �𸣹Ƿ� detach ���
		break;
	}
	}
}

// ��ȭ�� ������ ������ �Լ�
void* Thread_Func()
{
	FILE* file = NULL;					// ���� ����

	fopen_s(&file, "data.rec", "a+");	// data.rec ���Ϸ� ����
	char str[] = "recoding...->";		// ���� �տ� �ؽ�Ʈ ǥ��� ����

	// �ؽ�Ʈ ����
	char recodeStart[] = "��ȭ�� �����մϴ�.";
	char timeOver[] = "��ȭ �ð��� ��� ����Ͽ����ϴ�.";
	char recodeEnd[] = "��ȭ�� ����˴ϴ�.";

	// ��ȭ�� ó�� ������ �� ���
	if (file != NULL && var.getRecodingTime() != 0)			// file�� 0�� �� �ִٰ� ���ͼ� �׳� �߰�
	{
		fprintf(file, "%s\n", recodeStart);					// ��ȭ ������ ���
	}

	while (0 < var.getRecodingTime())	// ��ȭ�ð��� �����ϱ� ������ ��� �ݺ�
	{
		if (file != NULL)	// ��ȭ ������ �ִٸ�
		{
			fprintf(file, "%s%d\n", str, var.getRecodingTime());	// ��ȭ�ǰ� �ִٴ� �ǹ̷� �ؽ�Ʈ ���
		}
		else				// ��ȭ ���� ������
		{
			printf("\n��ȭ�� ������ �������� �ʽ��ϴ�.\n");	// ������ �������� �ʴ´ٰ� ����ϱ�
			printf("3�� �� ȭ���� �ʱ�ȭ �մϴ�.\n");		// 3���� ȭ�� �����Ѵٰ� �˷��ֱ� (�ٸ��� ������ ����� �ǹ̷�)

			Sleep(3 * 1000);		// 3�� �� (���Ƿ� ����)
			system("cls");		// ȭ�� �ѹ� ������ ��
			startPrint();		// �޴� �ٽ� ���		
		}
		var.setRecodingTime(var.getRecodingTime() - 1);	// ���� ��ȭ�ð��� �����ͼ� -1 �� �ٽ� ����
		Sleep(1000);	// 1000 = 1�� ���� �ݺ��ض�

		if (var.getRecodingTime() == 0)			// �ܿ� ��ȭ �ð��� 0�ʰ� �Ǹ� ��ȭ�� �����
		{
			if (file != NULL)	// ��ȭ ������ �ִٸ�
			{
				fprintf(file, "%s\n", timeOver);	// ��ȭ�ð��� ���� �Ǿ��ٰ� ���
			}
			var.setOnOff(false);				// ��ȭ ���·� ���� ���·� ����
			system("cls");						// On ���¸� Off ���·� �ٲ�ߵǼ� ȭ�� ����
			startPrint();						// �޴� �ٽ� ���
		}

		if (var.getOnOff() == false)			// ��ȭ OFF ���·� ��ȯ�ϸ�
		{
			if (file != NULL)						// file�� 0�� �� �ִٰ� ���ͼ� �׳� �߰�
			{
				fprintf(file, "%s\n", recodeEnd);	// ��ȭ�� ���� �Ǿ��ٰ� ���
			}
			break;								// �ݺ��� Ż��
		}
	}

	// ��ȭ �� ���� �� (�ݺ��� ������ ��
	if (file != NULL)		// ������ ���°� �ƴ϶��
		fclose(file);		// fopen�Լ��� ������ ������ �ݾƶ�.

	return NULL;			// ��ȯ�Ұ� �����Ƿ� NULL ó�� (������ ��ȯ �ؾߵǴ��� �˾ҳ�)
}

// 2��, �ܿ� ��ȭ �ð� Ȯ�� �Լ�
void recordingTimeCheck()
{
	// ���� �ܿ� ��ȭ�ð� ���
	printf("���� �ܿ� ��ȭ�ð� : %d��\n", var.getRecodingTime());
}

// 3��, ��ȭ �ð� ���� �Լ�
void setRecodingTime()
{
	int setRecord = 0;						// �缳���� ��ȭ �ð��� ������ ���� ����

	printf("��ȭ �ð� �Է� (�ʴ���) -> ");
	scanf_s("%d", &setRecord);				// �缳���� ��ȭ �ð� �Է�

	var.setRecodingTime(setRecord);			// ��ȭ �ð� ����

	// ��ȭ ���ϰ� �ְ�, ���� �ð��� 0�ʰ� �ƴҶ��� (���� �ð��� 0���ε� �Լ��� �۵��ϴ°� ����)
	if (var.getOnOff() == false && var.getRecodingTime() != 0)
		recodingOnOff();					// ��ȭ ���� �Լ� �۵�
}