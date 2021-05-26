#include <iostream>
#include <thread>
#include <assert.h>
#include <atomic>

struct PetersonsAlgorithm
{
  void entry(int thread_id)
  {
    flag[thread_id] = true;
    int otherThreadId = 1 ^ thread_id; // xor is 1 if 0 and 0 if 1
    turn = otherThreadId;
    //Possible reordering
    while (flag[otherThreadId] && turn == otherThreadId)
      ;
  }

  void exit(int thread_id)
  {
    flag[thread_id] = false;
  }

private:
  int turn = 0;
  bool flag[2];
};

void total_sum(PetersonsAlgorithm &petersons_algorithm, int &counter, int thread_id)
{
  for (int i = 0; i < 1000000; i++)
  {
    petersons_algorithm.entry(thread_id);
    counter++;
    petersons_algorithm.exit(thread_id);
  }
}

int main()
{
  PetersonsAlgorithm petersons_algorithm;
  int counter = 0;

  std::thread thread0(total_sum, std::ref(petersons_algorithm), std::ref(counter), 0);
  std::thread thread1(total_sum, std::ref(petersons_algorithm), std::ref(counter), 1);

  thread0.join();
  thread1.join();

  std::cout << "Counter value is : " << counter << std::endl;
  assert(counter == 2000000);
}