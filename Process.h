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
    void setPage(int m_page);
    void setFileName(std::string m_FileName);
    void setDiskNumber(int m_disk_number_);
    
    int getPID()const;
    int getPC()const;
    int getPriority()const;
    int getPage()const;
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
};

#endif