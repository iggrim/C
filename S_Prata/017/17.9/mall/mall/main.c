// mall.c -- use the Queue interface
// compile with queue.c
#include <stdio.h>
#include <stdlib.h>    // for rand() and srand()
#include <time.h>      // for time()
#include "queue.h"     // change Item typedef
#define MIN_PER_HR 60.0

bool newcustomer(double x);   // is there a new customer?
Item customertime(long when); // set customer parameters

int main(void)
{
    SetConsoleCP(1251); // <Windows.h> ������� SetConsoleCP ������������� ������� �������� �����
    SetConsoleOutputCP(1251);// <Windows.h>

    Queue line;
    Item temp;                // new customer data
    int hours;                // hours of simulation
    int perhour;              // average # of arrivals per hour
    long cycle, cyclelimit;   // loop counter, limit
    long turnaways = 0;       // turned away by full queue
    long customers = 0;       // joined the queue
    long served = 0;          // served during the simulation
    long sum_line = 0;        // cumulative line length // ������������� �������� ����� �������
    int wait_time = 0;        // time until Sigmund is free
    double min_per_cust;      // average time between arrivals
    long line_wait = 0;       // cumulative time in line

    InitializeQueue(&line);
    srand((unsigned int) time(0)); // random initializing of rand() // srand ��������������  ("��������" ��������� ���������)��������� ��������� ����� (rand()). ���� srand() ������� �� ����������, �� ���������������, ��� ����������  srand(1).
    puts("Case Study: Sigmund Lander's Advice Booth");
    puts("Enter the number of simulation hours:");
    scanf("%d", &hours);
    cyclelimit = MIN_PER_HR * hours;

    puts("Enter the average number of customers per hour:"); // ������� ������� ���������� ��������, ����������� �� ���:
    scanf("%d", &perhour);

    min_per_cust = MIN_PER_HR / perhour; // �����������(��� �������) ����� ����������� �� ������� � ������� ����

    for (cycle = 0; cycle < cyclelimit; cycle++)
    {
        if (newcustomer(min_per_cust))
        {
            if (QueueIsFull(&line))
                {
                    turnaways++;
                    printf("��������: %ld\n", turnaways);
                }
            else
            {
                customers++;
                printf("���-�� ��������� �������� : %ld\n", customers);
                temp = customertime(cycle);
                EnQueue(temp, &line);
            }
        }
        if (wait_time <= 0 && !QueueIsEmpty(&line))
        {
            DeQueue (&temp, &line);
            wait_time = temp.processtime; // ����� �� ������������ �������� // �������� ���������� ����� ������������
            line_wait += cycle - temp.arrive; // ������������� �������� ������� � �������

            //printf("cycle: %ld, temp.arrive: %ld,line_wait: %ld\n", cycle, temp.arrive, line_wait );
            printf("������. ������ � ����� ����������� �������� �� ����. ������: %d, ����� ��������: %ld, �����. ����. �������: %ld\n", wait_time, temp.arrive, line_wait );

            served++; // ���������� ��������, ����������� �� ����� �������������
            printf("���������� ����������� ��������: %ld\n", served);
        }
        if (wait_time > 0)
            {
              wait_time--;
              printf("��������� ����� �������� �� ���� ���.: %d\n", wait_time);
            }

        sum_line += QueueItemCount(&line);// ��������, � �������� ���������� sum_line ���� ����� 5,
        //� � ������� ������ ������� ����� 3(��� ����), ����� ������������� �������� sum_line ����� 8
        printf("������������� �������� ����� �������: %ld\n\n", sum_line);
    }

    if (customers > 0)
    {
        //printf("customers accepted: %ld\n", customers);
        printf("�������� ��������: %ld\n", customers);
        printf("  ����������� ��������: %ld\n", served);
        printf("       �������: %ld\n", turnaways);
        printf("������� ����� �������: %.2f\n",
               (double) sum_line / cyclelimit);
        printf(" cpe��ee ����� ��������: %.2f minutes\n",
               (double) line_wait / served);
    }
    else
        puts("No customers!");
    EmptyTheQueue(&line);
    puts("Bye!");

    return 0;
}

// � - ������� ����� ����� ��������� �������� � �������
// ���������� true, ���� ������ ���������� � ������� ������ ������
// ���� ���� ���� ������
// ���������� ����� �������� �� ���������� ���������� ����� � ��������� ����
bool newcustomer(double x)
{
    double r;
    int m;
    r = rand(); // 21853 // ������ ��������
    m = RAND_MAX; // 32767
    static int count = 0;

    if (r * x / m < 1)
        {//if (rand() * x / RAND_MAX < 1)
        //printf("����� �����: %d, true %.4f, rand: %.4f\n", ++count,(r * x / m), r );
        printf("����� �����: %d, ������ � ��� ������\n", count++);
        return true;
        }
    else{
        //printf("����� �����: %d, false %.4f, rand: %.4f\n", ++count,(r * x / m), r );
        printf("����� �����: %d, � ��� ������ ����� �� ������\n", count++ );
        return false;
        }
}

// when is the time at which the customer arrives
// function returns an Item structure with the arrival time
// set to when and the processing time set to a random value
// in the range 1 - 3
Item customertime(long when)
{
    Item cust;

    cust.processtime = rand() % 3 + 1; // �������� ���������� ����� ������������
                                       // ������������ �������� �������,(����, ��� ��� ��� ������).
    cust.arrive = when;

    printf("����� ��������: %ld, ����������� ����� ����.:%d\n", cust.arrive, cust.processtime);
    return cust;
}
