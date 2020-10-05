#include <stdio.h>
#include <stdlib.h>
#include <Windows.h> // SetConsoleCP, SetConsoleOutputCP
#include <time.h>

void filecpy(FILE *stream_from, FILE *stream_to);

// ������ ��� ������

char buf1[BUFSIZ * 10];
char buf2[BUFSIZ * 10];


void main(int argc, char *argv[]) {
	// =======���� ������ ���� �������� � ��������� ANSI=======
	SetConsoleCP(1251); // <Windows.h> ������� SetConsoleCP ������������� ������� �������� �����
    SetConsoleOutputCP(1251);// <Windows.h>

         time_t start, end;
         FILE *stream_from, *stream_to;


         if(argc < 3) {
                  printf("������� ����� ������!\n");
                  exit(-1);
         }

// ��������� ����� � ���������� ��� �����������
// ����� ������������ �������

         if((stream_from = fopen(argv[1], "rt")) == NULL) {
                  printf("������� ��� �������� �����!\n"); // �������������
                  exit(-1);
         }
         stream_to = fopen(argv[2], "wt+");

// ���������� ����� ������ �����������

         start = clock();

// ��������� ����������� �����

         filecpy(stream_from,stream_to);

// ���������� ����� ���������� �����������

         end = clock();

// ������� ����� ����������� ��� �������������
// ������� ������������ �������

         printf("����� �����������: %5.1f ������ ������, ������: %d\n",
                 ((float)end - start) / CLK_TCK, BUFSIZ);


// ������ ���� ����� �������� �������

         if((stream_from = fopen(argv[1], "rt")) == NULL) exit(-1);
         stream_to = fopen(argv[2], "wt+");

// ������������� ������ ��� ��� ��������,
// ��� � ��� ��������� ������

         setvbuf(stream_from, buf1, _IOFBF, sizeof(buf1));
         setvbuf(stream_to,   buf2, _IOFBF, sizeof(buf2));

// �������� ���� � �������� �����������������
// �����������

         start = clock();
         filecpy(stream_from,stream_to);
         end = clock();

         printf("����� �����������: %5.10f ������ ������: %d\n",
                 ((float)end - start) / CLK_TCK, BUFSIZ * 10);

// �������� ��� ������������� �����������

         if((stream_from = fopen(argv[1], "rt")) == NULL) exit(-1);
         stream_to = fopen(argv[2], "wt+");

         setvbuf(stream_from, NULL, _IONBF, 0);
         setvbuf(stream_to,   NULL, _IONBF, 0);

         start = clock();
         filecpy(stream_from,stream_to);
         end = clock();

         printf("����� �����������: %5.1f ����������� �� ������������\n",
                 ((float)end - start) / CLK_TCK);

         exit(0);
}

// ������� ��� ����������� ������

void filecpy(FILE *stream_from, FILE *stream_to) {

         char linebuf[256];

// ���� �����������. ������� ������ �� ����� -
// ����� �������� �����
// ������� feof () ���������� ��������� ��������, ���� ��� ��������� ������ ������� ����� � � � ��������� ����� �����, � ���� � ��������� ������. 
         while(!feof(stream_from)) {

// ������ � ����� linebuf ���� ������

                  if(fgets(linebuf, 255, stream_from) == NULL) break;

// ���������� ���������� ������ linebuf
// � �������� ����

                  if(fputs(linebuf, stream_to) != 0) break;
         }

// ��������� ������� � �������� �����

         fclose(stream_from);
         fclose(stream_to);
}

/* F:\Temp\C_lang\!_S_Prata\013\13.5.2>add.exe wordy dest
����� �����������:   0.0 ������ ������, ������: 512
����� �����������: 0.0010000000 ������ ������: 5120
����� �����������:   0.0 ����������� �� ������������ */