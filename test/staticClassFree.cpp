
#include <iostream>

class staticClassFree
{
private:
    /* data */
    static int core;
public:
    staticClassFree(int val);
    ~staticClassFree();
};

staticClassFree::staticClassFree(int val)
{
    core += val;
}

staticClassFree::~staticClassFree()
{
    std::cout << core << std::endl;
}

int staticClassFree::core = 0;


class HighLevel
{
private:
    staticClassFree * in_;
public:
    HighLevel(int val);
    ~HighLevel();
};

HighLevel::HighLevel(int val)
{
    in_ = new staticClassFree(val);
}

HighLevel::~HighLevel()
{
    delete in_;
}


void example(){
    HighLevel B(2);
}

int main(){
    HighLevel A(1);
    example();
}