#ifndef MEMORY_
#define MEMORY_
#include <iostream>
#include <vector>
class Memory{
    public:
    Memory(int m_pageNum, int m_pid_f,int m_priority_f);
    int getMemPageNum()const;
    int getMemPID()const;
    int getMemUse()const;
    int getMemPriority()const;


    void setMemPageNum(int m_pageNum);
    void setMemPriority(int m_priority);
    void setMemPID(int m_pid_f);
    void setMemUse();
    void incrementUse();
    void makeFreshlyUsed();

    


    private:
    int pageNum_;
    int pid_f;
    int use_;
    int priority_f;
   
    

};

#endif