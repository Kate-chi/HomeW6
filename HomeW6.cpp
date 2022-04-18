

#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <algorithm>
#include <random>


using namespace std;

mutex m_cout;

class Pcout
{
private:
    lock_guard<mutex> lg;
public:
    Pcout() : lg(lock_guard<mutex>(m_cout))
    {

    }

    template<typename T>
    Pcout& operator << (const T& data)
    {
        std::cout << data;
        return *this;
    }

    Pcout& operator << (ostream& (*en)(ostream&))
    {
        std::cout << en;
        return *this;
    }
};

void doSomething(int number) {
    
    Pcout() << "start thread " << number << endl;
    Pcout() << "stop thread " << " " << number << endl;
}

//====================================================================

const int MAX = 1000000;

int getPrime(int n) {
    vector<bool> tmp(MAX, true);
    int count = 1;
    this_thread::sleep_for(4s);
    for (int i = 2; i < MAX; i++) {
        if (tmp[i]) {
            if (count == n)
                return i;
            count++;
            for (int j = 2 * i; j < MAX; j += i)
                tmp[j] = false;
        }
    }
    return 0;
}


void progress()
{
    int i = 0;
    int c = 5;
    while ( i != c) {
        ++i;
        this_thread::sleep_for(1s);
        cout << i << " ";
    }
}


//=======================================================================


mutex m;

void generate_things(vector<int>& v)
{
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dis(0, 1000);

    lock_guard lg(m);

    generate(v.begin(), v.end(), [&]()
        {
            return dis(gen);
        });
}

void pop_thing(vector<int>& v)
{
    lock_guard lg(m);

    cout << *max_element(v.begin(), v.end()) << endl;
}



int main()
{
    
    thread th1(doSomething, 1);
    thread th2(doSomething, 2);
    thread th3(doSomething, 3);
    th1.join();
    th2.join();
    th3.join();
    
    //=======================================================================

    thread gp1(getPrime, 5);
    thread pr1(progress);
    gp1.join();
    pr1.join();
   
    //=======================================================================
    
    vector<int> v(100);
    size_t count = 100;

    while (count--)
    {
        thread owner([&]()
            {
                generate_things(v);
            });
        thread thief([&]()
            {
                pop_thing(v);
            });

        owner.join();
        thief.join();
    }
    
    
    return 0;

}


