#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include <string>

std::mutex locker;

void foo() {
   std::cout << "foo\n";
}

void bar(int x) {
   std::cout << "bar\n";
}

class AA {
   public:
      void a1() {
	 std::cout << "a1\n";
      }

      void a2(int n) {
	 std::cout << "a2 " << n << "\n";
      }

      void start() {
	 std::thread t1(&AA::a1, this);
	 std::thread t2(&AA::a2, this, 10);

	 t1.join();
	 t2.join();
      }
};

class ClassA
{
   public:
      void func1()
      {
	 std::lock_guard<std::recursive_mutex> lock(mMutex);
	 //std::lock_guard<std::mutex> lock(mMutex);	// dead lock
	 std::cout << "func1\n";
      }

      void func2()
      {
	 std::lock_guard<std::recursive_mutex> lock(mMutex);
	 //std::lock_guard<std::mutex> lock(mMutex);	// dead lock
	 std::cout << "func2\n";
	 func1();
      }
      void start() {
	 std::thread t1(&ClassA::func2, this);

	 t1.join();
      }
   private:
      std::recursive_mutex mMutex;
      //std::mutex mMutex;	// dead lock
};


struct bank_account
{
   explicit bank_account(std::string name, int money)
   {
      sName = name;
      iMoney = money;
   }

   std::string sName;
   int iMoney;
   std::mutex mMutex;
};

void transfer( bank_account &from, bank_account &to, int amount )
{
   // try to block lock

   // don't actually take the locks yet
   std::unique_lock<std::mutex> lock1( from.mMutex, std::defer_lock );
   std::unique_lock<std::mutex> lock2( to.mMutex, std::defer_lock );

   // lock both unique_locks without deadlock
   std::lock( lock1, lock2 );

   /*
      std::lock( from.mMutex, to.mMutex );
      std::lock_guard<std::mutex> lock1( from.mMutex, std::adopt_lock );
      std::lock_guard<std::mutex> lock2( to.mMutex, std::adopt_lock );   
      */

   from.iMoney -= amount;
   to.iMoney += amount;

   // output log
   std::cout << "Transfer " << amount << " from " 
      << from.sName << " to " << to.sName << std::endl;
   std::cout << from.sName << ": " << from.iMoney
      << " " << to.sName  << ": " << to.iMoney << std::endl;   
}

std::recursive_mutex once_mutex;
void once(){
   std::unique_lock<std::recursive_mutex> lock(once_mutex);
   std::cout << "once\n";
}

void func_once(int x){
   std::unique_lock<std::recursive_mutex> lock(once_mutex);
   static std::once_flag f;
   std::cout << "func_once: " << x << std::endl;
   std::call_once(f, once);
}

std::mutex m;
std::condition_variable cv;
std::string data;
bool ready = false;
bool processed = false;
void worker_thread()
{
   // Wait until main() sends data
   std::unique_lock<std::mutex> lk(m);
   cv.wait(lk, []{return ready;});

   // after the wait, we own the lock.
   std::cout << "Worker thread is processing data\n";
   data += " after processing";

   // Send data back to main()
   processed = true;
   std::cout << "Worker thread signals data processing completed\n";

   // Manual unlocking is done before notifying, to avoid waking up
   // the waiting thread only to block again (see notify_one for details)
   lk.unlock();
   cv.notify_one();
}

typedef struct{
   std::mutex m;
   int data;
}Node;
/* std::scoped_lock implementation */
void lock_impl(std::mutex &x, std::mutex &y) {
   while (true) {
      if (x.try_lock()) {
	 if (y.try_lock()) {
	    return;
	 }
	 x.unlock();
      }

      sched_yield();  // implementation-defined

      if (y.try_lock()) {
	 if (x.try_lock()) {
	    return;
	 }
	 y.unlock();
      }

      sched_yield();  // implementation-defined
   }
}
void my_swap(Node &lhs, Node &rhs) {
   if (&lhs == &rhs) return;

   //std::scoped_lock(lhs.m, rhs.m);

   lock_impl(lhs.m, rhs.m);

   /*
      std::unique_lock<std::mutex> lhs_lock( lhs.m, std::defer_lock );
      std::unique_lock<std::mutex> rhs_lock( rhs.m, std::defer_lock );  
      std::lock( lhs_lock, rhs_lock );
      */
   std::swap(lhs.data, rhs.data);
}
void func_swap()
{
   Node n1;
   Node n2;
   n1.data = 50;
   n2.data = 100;
   std::cout << "N1: " << n1.data << " N2: " << n2.data << std::endl; 
   std::thread t1(my_swap,std::ref(n1),std::ref(n2));
   t1.join();
   std::cout << "N1: " << n1.data << " N2: " << n2.data << std::endl;
}

int main() {
   std::thread t1( [](){
	 locker.lock();
	 std::cout << "foo\n";
	 locker.unlock();
	 } );

   std::thread t2( [x = 33](){
	 locker.lock();
	 std::cout << x << "bar\n";
	 locker.unlock();
	 } );

   std::thread t3( [](int x){
	 locker.lock();
	 std::cout << x << "bar\n";
	 locker.unlock();
	 } , 55 );

   std::thread t4( [](int x){
	 std::lock_guard<std::mutex> lg(locker);
	 std::cout << x << "bar\n";
	 } , 66 );    

   std::cout << "main, foo and bar now execute concurrently...\n"; // synchronize threads

   std::cout << "sleep 1s\n";
   //std::this_thread::sleep_for(std::chrono::seconds(1));
   std::this_thread::sleep_for(std::chrono::microseconds(1000000));

   std::cout << "join t1\n";
   t1.join();
   std::cout << "join t2\n";
   t2.join();
   std::cout << "join t3\n";
   t3.join();
   std::cout << "join t4\n";
   t4.join();

   std::cout << "foo and bar completed.\n";

   AA a;
   a.start();

   // recursive_mutex
   ClassA ca;
   ca.start();

   // unique_lock
   bank_account Account1( "User1", 100 );
   bank_account Account2( "User2", 50 );

   std::thread t11( [&](){ transfer( Account1, Account2, 10 ); } );
   std::thread t22( [&](){ transfer( Account2, Account1, 5 ); } );

   t11.join();
   t22.join();  

   // call_once
   for(int i = 0;i < 10;++i){
      std::thread(func_once,i+1).detach();
   }

   // condition_variable

   std::thread worker(worker_thread);

   data = "Example data";
   // send data to the worker thread
   {
      std::lock_guard<std::mutex> lk(m);
      ready = true;
      std::cout << "main() signals data ready for processing\n";
   }
   cv.notify_one();

   // wait for the worker
   {
      std::unique_lock<std::mutex> lk(m);
      cv.wait(lk, []{return processed;});
   }
   std::cout << "Back in main(), data = " << data << '\n';

   worker.join();


   //std::scoped_lock
   func_swap();

   return 0;
}
