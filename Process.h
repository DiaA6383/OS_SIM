#ifndef PROCESS_
#define PROCESS_
#include <iostream>

class Process{
    public:
    //constructor 
    Process(int m_priority, int m_PID);
    //~Process();

    //getters and setters
    void setPC(int m_PC);
    void setPID(int m_PID);
    void setPriority(int m_priority);
    int getPID()const;
    int getPC()const;
    int getPriority()const;
    void printProcess()const;
    void incrementProcess();

    //bool operator>(const Process& rhs);
    //bool operator<(const Process& rhs);
    struct ComparePriority{
    bool operator()(const Process& lhs, const Process& rhs){
        return lhs.getPriority() > rhs.getPriority();
    }
};


  

    private:
    int PC_;
    int PID_;
    int priority_;
    
    
};

#endif