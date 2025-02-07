using System;
using System.Threading;
using System.Threading.Tasks;

namespace ServerCore
{
    class Program
    {
        static void MainThread(object state)
        {
            for (int i=0; i<5; i++)
                Console.WriteLine("Hello Thread!");
        }

        static void Main(string[] args)
        {

            ThreadPool.SetMinThreads(1, 1);
            ThreadPool.SetMaxThreads(5, 5);

            for (int i=0;i<5;i++)
            {
                // 오래 걸리는 작업일 경우, 아래와 같이 task를 활용하여 .LongRunning을 통해 오래 걸림을 알려주어 
                // 모든 쓰레드가 이 task를 하는데 투입되지 않도록 한다.
                Task t = new Task(() => { while (true) { } }, TaskCreationOptions.LongRunning);
                t.Start();
            }

            //for (int i = 0; i < 4; i++)
            //    ThreadPool.QueueUserWorkItem((obj) => { while (true) { } });

            ThreadPool.QueueUserWorkItem(MainThread);
            // Thread Pool?
            // 임시로 알바를 쓰듯이, 대기하고있는 쓰레드를 갖다 쓴다.

            // * 쓰레드 풀링?


            // 아래와 같이 쓰레드를 원하는 갯수(아래에선 1000개) 만큼 생성하고 실행하게끔도 가능은 함.
            // 하지만, CPU 코어가 쓰레드를 바꾸는 과정에서의 비용이 더 크기 때문에 비효율적이 됨.
            // 그래서 무조건 쓰레드를 많이 만드는 것만이 좋은 방법은 절대 아니다.

            //for (int i=0;i<1000;i++)
            //{
            //    Thread t = new Thread(MainThread);
            //    //t.Name = "Test Thread"; // Thread 이름 지정 가능
            //    t.IsBackground = true;
            //    t.Start();
            //}

            //Console.WriteLine("Waiting for Thread!");

            //t.Join();   // C++ 도 같은 함수를 사용함
            //Console.WriteLine("Hello World!");

            while (true)
            {

            }
        }
    }
}


// C#에서는 사실상 직접적으로 쓰레드를 관리할 일은 적을 것. 쓰레드 풀을 활용하는 것이 좋음.
