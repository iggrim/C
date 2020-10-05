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
    SetConsoleCP(1251); // <Windows.h> Функция SetConsoleCP устанавливает кодовую страницу ввода
    SetConsoleOutputCP(1251);// <Windows.h>

    Queue line;
    Item temp;                // new customer data
    int hours;                // hours of simulation
    int perhour;              // average # of arrivals per hour
    long cycle, cyclelimit;   // loop counter, limit
    long turnaways = 0;       // turned away by full queue
    long customers = 0;       // joined the queue
    long served = 0;          // served during the simulation
    long sum_line = 0;        // cumulative line length // накопительное значение длины очереди
    int wait_time = 0;        // time until Sigmund is free
    double min_per_cust;      // average time between arrivals
    long line_wait = 0;       // cumulative time in line

    InitializeQueue(&line);
    srand((unsigned int) time(0)); // random initializing of rand() // srand инициализирует  ("засевает" начальным значением)генератор случайных чисел (rand()). Если srand() никогда не вызывалось, то подразумевается, что вызывалась  srand(1).
    puts("Case Study: Sigmund Lander's Advice Booth");
    puts("Enter the number of simulation hours:");
    scanf("%d", &hours);
    cyclelimit = MIN_PER_HR * hours;

    puts("Enter the average number of customers per hour:"); // Введите среднее количество клиентов, прибывающих за час:
    scanf("%d", &perhour);

    min_per_cust = MIN_PER_HR / perhour; // минимальное(или среднее) время затраченное на клиента в течении часа

    for (cycle = 0; cycle < cyclelimit; cycle++)
    {
        if (newcustomer(min_per_cust))
        {
            if (QueueIsFull(&line))
                {
                    turnaways++;
                    printf("отказано: %ld\n", turnaways);
                }
            else
            {
                customers++;
                printf("кол-во прибывших клиентов : %ld\n", customers);
                temp = customertime(cycle);
                EnQueue(temp, &line);
            }
        }
        if (wait_time <= 0 && !QueueIsEmpty(&line))
        {
            DeQueue (&temp, &line);
            wait_time = temp.processtime; // время до освобождения Зигмунда // желаемое количество минут консультации
            line_wait += cycle - temp.arrive; // накопительное значение времени в очереди

            //printf("cycle: %ld, temp.arrive: %ld,line_wait: %ld\n", cycle, temp.arrive, line_wait );
            printf("обслуж. клиент с ранее запрошенным временем на конс. равным: %d, время прибытия: %ld, накоп. знач. времени: %ld\n", wait_time, temp.arrive, line_wait );

            served++; // количество клиентов, обслуженных за время моделирования
            printf("количество обслуженных клиентов: %ld\n", served);
        }
        if (wait_time > 0)
            {
              wait_time--;
              printf("уменьшаем время ожидания на одну мин.: %d\n", wait_time);
            }

        sum_line += QueueItemCount(&line);// например, в процессе вычислений sum_line была равна 5,
        //а в текущий момент очередь равна 3(три узла), тогда накопительное значение sum_line равно 8
        printf("накопительное значение длины очереди: %ld\n\n", sum_line);
    }

    if (customers > 0)
    {
        //printf("customers accepted: %ld\n", customers);
        printf("принятых клиентов: %ld\n", customers);
        printf("  обслуженных клиентов: %ld\n", served);
        printf("       отказов: %ld\n", turnaways);
        printf("средняя длина очереди: %.2f\n",
               (double) sum_line / cyclelimit);
        printf(" cpeднee время ожидания: %.2f minutes\n",
               (double) line_wait / served);
    }
    else
        puts("No customers!");
    EmptyTheQueue(&line);
    puts("Bye!");

    return 0;
}

// х - среднее время между прибытием клиентов в минутах
// возвращает true, если клиент появляется в течение данной минуты
// один цикл одна минута
// количество минут получаем из введенного количества часов и запускаем цикл
bool newcustomer(double x)
{
    double r;
    int m;
    r = rand(); // 21853 // разные значения
    m = RAND_MAX; // 32767
    static int count = 0;

    if (r * x / m < 1)
        {//if (rand() * x / RAND_MAX < 1)
        //printf("номер цикла: %d, true %.4f, rand: %.4f\n", ++count,(r * x / m), r );
        printf("номер цикла: %d, прибыл в эту минуту\n", count++);
        return true;
        }
    else{
        //printf("номер цикла: %d, false %.4f, rand: %.4f\n", ++count,(r * x / m), r );
        printf("номер цикла: %d, в эту минуту никто не прибыл\n", count++ );
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

    cust.processtime = rand() % 3 + 1; // желаемое количество минут консультации
                                       // определяется лучайным образом,(одна, две или три минуты).
    cust.arrive = when;

    printf("время прибытия: %ld, запрошенное время обсл.:%d\n", cust.arrive, cust.processtime);
    return cust;
}
