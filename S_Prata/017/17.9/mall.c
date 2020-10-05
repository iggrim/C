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
    Queue line;
    Item temp;                // new customer data
    int hours;                // hours of simulation
    int perhour;              // average # of arrivals per hour
    long cycle, cyclelimit;   // loop counter, limit
    long turnaways = 0;       // turned away by full queue
    long customers = 0;       // joined the queue
    long served = 0;          // served during the simulation // количество клиентов, обслуженных за время моделирования
    long sum_line = 0;        // cumulative line length
    int wait_time = 0;        // time until Sigmund is free
    double min_per_cust;      // average time between arrivals
    long line_wait = 0;       // cumulative time in line 
    
    InitializeQueue(&line);
    
	srand((unsigned int) time(0)); // random initializing of rand() // srand инициализирует  ("засевает" начальным значением)генератор случайных чисел (При вызове rand()). Если srand() никогда не вызывалось,  то подразумевается, что  вызывалась  srand(1). 
	// srand задает различные исходные числа. Кроме того, с помощью функции srand() можно многократно генерировать одну и ту же последовательность псевдослучайных чисел.
    
	puts("Case Study: Sigmund Lander's Advice Booth");
    puts("Enter the number of simulation hours:");
    scanf("%d", &hours);
    cyclelimit = MIN_PER_HR * hours; // часы в количество минут 
    
	puts("Enter the average number of customers per hour:"); // Введите среднее количество клиентов, прибывающих ======за час=====:   
	scanf("%d", &perhour);
	
    min_per_cust = MIN_PER_HR / perhour; // минимальное(или среднее) время затраченное на клиента в течении часа
    
    for (cycle = 0; cycle < cyclelimit; cycle++) // цикл по количеству минут
    {
        if (newcustomer(min_per_cust)) // если клиент появляется в течение данной минуты(cycle)
        {
            if (QueueIsFull(&line))
                turnaways++; // количество отказов (завернутых клиентов из очереди)
            else
            {
                customers++; // увеличиваем количество пришедших в цикле, если клиент появляется в течение данной минуты(cycle)
                temp = customertime(cycle); // пишем во временную структуру Item параметры клиента: на какой минуте прибыл, время обслуживания (от 1 мин. до 3 мин.) в
                EnQueue(temp, &line); // создаем связанный список из узлов
            }
        }
        if (wait_time <= 0 && !QueueIsEmpty(&line))
        {
            DeQueue (&temp, &line); // Удаление элемента из начала очереди // если пришедший клиент не ждет(wait_time <= 0) и сразу обслуживается 
            wait_time = temp.processtime; // время до освобождения Зигмунда т.е. желаемое количество минут консультации пришедшего клиента
			
            line_wait += cycle - temp.arrive; // накопительное значение времени в очереди       
			served++; // количество клиентов, обслуженных за время моделирования
        }
        if (wait_time > 0)
            wait_time--;
		
        sum_line += QueueItemCount(&line); // накопительное значение длины очереди // например, в процессе вычислений sum_line была равна 5, а в текущий момент очередь равна 3(три узла), тогда накопительное значение sum_line равно 8
    }
    
    if (customers > 0)
    {
        printf("customers accepted: %ld\n", customers);
        printf("  customers served: %ld\n", served);
        printf("       turnaways: %ld\n", turnaways);
        printf("average queue size: %.2f\n",
               (double) sum_line / cyclelimit);
        printf(" average wait time: %.2f minutes\n",
               (double) line_wait / served);
    }
    else
        puts("No customers!");
    EmptyTheQueue(&line);
    puts("Bye!");
    
    return 0;
}

// х - среднее время между прибытием клиентов в минутах 
// возвращает true, если клиент появляется в течение данной минуты // один цикл одна минута // количество минут получаем из введенного количества часов и запускаем цикл    
bool newcustomer(double x)
{
	//double r;
    //int m;
    //r = rand(); // 21853 // разные значения
    //m = RAND_MAX; // 32767
	
	// функция rand () генерирует целое число из диапазона от 0 до RAND_MAX; значение RAND_MAX определено в stdlib.h. Обычно это INT_MAX. 
	// srand инициализирует  ("засевает" начальным значением)генератор случайных чисел (При вызове rand())
	// srand задает различные исходные числа. Кроме того, с помощью функции srand() можно многократно генерировать одну и ту же последовательность псевдослучайных чисел
    
	//if (r * x / m < 1)
    if (rand() * x / RAND_MAX < 1)
        return true;
    else
        return false;
}

// when - время прибытия клиента
// функция возвращает структуру Item со временем прибытия, установленным в when,
// и временем обслуживания, установленным в случайное значение из диапазона от 1 до 3 
//
Item customertime(long when)
{
    Item cust;
    
    cust.processtime = rand() % 3 + 1; // желаемое количество минут консультации
                                      // определяется случайным образом,(одна, две или три минуты).
    cust.arrive = when; // на какой минуте цикла прибыл клиент
    
    return cust;
}
