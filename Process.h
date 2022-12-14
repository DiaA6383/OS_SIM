//Name: Alejandro Diaz
//Course: Operating Systems CSCI 340
//Professor: Shostak 
//Project: OS Simulator
#ifndef PROCESS_
#define PROCESS_
#include <iostream>
#include "Memory.h"

class Process{
    public:
    //constructor 
    Process(int m_priority, int m_PID);
   // ~Process();

    //getters and setters
    void setPC(int m_PC);
    void setPID(int m_PID);
    void setPriority(int m_priority);
    void setPage(int m_page);
    void setFileName(std::string m_FileName);
    void setDiskNumber(int m_disk_number_);
    void setCompleteStatus();
    
    int getPID()const;
    int getPC()const;
    bool completeStatus()const;
    int getPriority()const;
    int getPage()const;
    bool queue_isEmpty(int diskNumber)const;
    std::string getFileName()const;   
    int getDiskNumber()const;
    void printProcess()const;
    void incrementProcess();
   
    private:
    int PC_;
    int PID_;
    int priority_;
    int page_;
    std::string fileName_;
    int disk_number_;
    bool isCompleted = false;
};

#endif