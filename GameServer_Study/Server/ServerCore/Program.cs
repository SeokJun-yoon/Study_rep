using System;
using System.Threading;
using System.Threading.Tasks;

namespace ServerCore
{
    class Program
    {
        volatile static bool _stop = false;  // 전역으로 선언된 변수는, main과 ThreadMain 모두에서 접근이 가능
        // volatile을 앞에 붙이면, 최적화를 하지 말라는 의미

        // volatile는 C++과 C#에서 약간 다른 부분이 있는데,
        // 둘 다 최적화를 하지 말아달라는 의미는 같지만
        // C#는 조금 더 추가적인 내용이 있다.
        
        // 그래서 사실 volatile 보다는 추후의 memory barrier나 lock, atomic 등을 사용하는 것이 좋다.

        static void ThreadMain()
        {
            Console.WriteLine("쓰레드 시작!");

            while (_stop==false)
            {
                // 누군가가 stop 신호를 해주기를 기다린다.
            }

            Console.WriteLine("쓰레드 종료!");
        }

        static void Main(string[] args)
        {
            Task t = new Task(ThreadMain);
            t.Start();

            Thread.Sleep(1000);

            _stop = true;

            Console.WriteLine("Stop 호출");
            Console.WriteLine("종료 대기중");

            t.Wait();   // Thread 사용시에는 join과 같음
            // Release 모드로 실행 시, Wait 상태를 빠져나오지 못함.
            // 멀티쓰레드 환경에서는 이런 일이 발생할 수 있다.


            Console.WriteLine("종료 성공");
        }
    }
}



