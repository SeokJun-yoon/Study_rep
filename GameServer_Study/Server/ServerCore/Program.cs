using System;
using System.Threading;
using System.Threading.Tasks;

namespace ServerCore
{
    class Program
    {
        static void Main(string[] args)
        {
            int[,] arr = new int[10000, 10000];
            {
                long now = DateTime.Now.Ticks;
                for (int y = 0; y < 10000; y++)
                    for (int x = 0; x < 10000; x++)
                        arr[y, x] = 1;
                long end = DateTime.Now.Ticks;
                Console.WriteLine($"(y, x) 순서 걸린 시간 {end - now}");

            }
            // 일반적인 생각으로는 위와 아래의 시간이 같아야 한다.
            {
                long now = DateTime.Now.Ticks;
                for (int y = 0; y < 10000; y++)
                    for (int x = 0; x < 10000; x++)
                        arr[x, y] = 1;
                long end = DateTime.Now.Ticks;
                Console.WriteLine($"(x, y) 순서 걸린 시간 {end - now}");
            }

            // 하지만 실행해보면 꽤나 큰 시간 차이가 난다.
            // 첫 번째 경우, 인접한 메모리 공간 때문에 캐시 히트가 일어나는데
            // 두 번재 경우, 인접하지 않은 부분에 대한 접근이므로 시간이 느려지는 것이다.
        }

    }
}



