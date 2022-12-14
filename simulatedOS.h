#ifndef SIMULATED_OS_ 
#define SIMULATED_OS_

#include <iostream>
#include "Process.h"
#include "Memory.h"
#include <queue>
#include <stack>

class SimulatedOS{
    public:
    struct RAM_Mem;
    //main methods
    SimulatedOS(int m_numberOfDisks, int m_amountOfRAM, int m_pageSize);
    Process NewProcess(int m_priority);
    void Exit();
    void DiskReadRequested(int diskNumber, std::string fileName);
    void FetchFrom(unsigned int memoryAddress);
    void PrintCPU()const;
    void PrintReadyQueue()const;
    void PrintRAM()const;
    void PrintDisk(int diskNumber)const;
    void DiskJobCompleted(int diskNumber);
    void PrintDiskQueue(int diskNumber)const;
    //void PrintDiskQueue(int diskQueue)const;
   
     //getters
    int getNumberOfDisks()const;
    int getAmountOfRAM()const;
    int getPageSize()const;
    int getPIDcounter()const;
    int getCurrentPage()const;
    int getCurrentDisk()const;
    std::vector<Memory*>getRAM()const;
    
    std::deque<Process*> getQueue()const;
   
    //setters
    void setNumberOfDisks(int m_numberOfDisks);
    void setAmountOfRAM(int m_amountOfRAM);
    void setPageSize(int m_pageSize);
    void setCurrentPage(int m_page);
    void setCurrentDisk(int m_disk);
    
    //helper functions
    void incrementPID();
    void printOS()const;
    bool addProcess(Process *new_process);
    void printQueue()const;
    bool queue_isEmpty(int diskNumber)const;
    int findOldestInRAM()const;
    void addToTable(int pid, int priority);
    


    private:
    int numberOfDisks_;
    int amountOfRAM_;
    int pageSize_;
    int PID_Counter_;
    int curr_disk_;
    int curr_page_;
    std::deque<Process*> queue_;
    std::stack<Process*> temp_stack_;

    std::vector<Memory*> RAM_;
    std::vector<std::deque<Process*>> diskQueue_vector_;

    
    
   
};

#endif