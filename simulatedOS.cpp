#include <iostream>
#include "simulatedOS.h"
#include "Process.h"


SimulatedOS::SimulatedOS(int m_numberOfDisks, int m_amountOfRAM, int m_pageSize){
    setNumberOfDisks(m_numberOfDisks);
    setPageSize(m_pageSize);
    setCurrentPage(0);
    setCurrentDisk(0);
    if(m_amountOfRAM % m_pageSize !=0){
        throw std::invalid_argument("amountOfRAM MUST be divisible to pageSize");
    }
    setAmountOfRAM(m_amountOfRAM);
    PID_Counter_ = 0; //initialize PID after each OS instance
    
   
}

Process SimulatedOS::NewProcess(int m_priority){
    incrementPID();
    int new_pid = this->PID_Counter_;
    Process *new_process = new Process(m_priority, new_pid);
    addProcess(new_process);
    return *new_process;
}

void SimulatedOS::Exit(){
    if(!queue_.empty()){
        delete queue_.front(); //deletes pointer of the front of cpu
        queue_.pop_front(); // or just this by itself?
    }else{
        throw std::invalid_argument("Nothing is using the CPU");
    }
}
void SimulatedOS::PrintCPU()const{
    std::cout << "CPU: " << queue_.front()->getPID() <<std::endl;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//setters and getters for encapsulation purposes
int SimulatedOS::getNumberOfDisks()const{
    return numberOfDisks_;
}
int SimulatedOS::getAmountOfRAM()const{
    return amountOfRAM_;
}
int SimulatedOS::getPageSize()const{
    return pageSize_;
}
int SimulatedOS::getCurrentPage()const{
    return curr_page_;
}
int SimulatedOS::getCurrentDisk()const{
    return curr_disk_;
}
std::deque<Process*> SimulatedOS::getQueue()const{
    return queue_;
}

void SimulatedOS::setNumberOfDisks(int m_numberOfDisks){
    numberOfDisks_ = m_numberOfDisks;
}
void SimulatedOS::setAmountOfRAM(int m_amountOfRAM){
    amountOfRAM_ = m_amountOfRAM;
}
void SimulatedOS::setPageSize(int m_pageSize){
    pageSize_ = m_pageSize;
}
void SimulatedOS::setCurrentPage(int m_page){
    curr_page_ = m_page;
}
void SimulatedOS::setCurrentDisk(int m_disk){
    curr_disk_ = m_disk;
}

//Helpers
void SimulatedOS::incrementPID(){
   PID_Counter_++;
}

void SimulatedOS::printOS()const{
    std::cout << "~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
    std::cout << "Your OS:" << std::endl;
    std::cout << std::endl;
    std::cout << "Number of Disks: " << getNumberOfDisks() << std::endl;
    std::cout << "Amount of RAM: " << getAmountOfRAM() << std::endl;
    std::cout << "Size of Page: " << getPageSize() << std::endl;
    std::cout << std::endl;
    std::cout << "Current Page: " << getCurrentPage() << std::endl;
    std::cout << "Current Disk: " << getCurrentDisk() <<std::endl;
     std::cout << "~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
}

bool SimulatedOS::addProcess(Process *new_process){
    //1. if queue is empty
    if(queue_.size() == 0){
        queue_.push_front(new_process);
    }
    //2. if new process is higher priority than front of queue
    else if(new_process->getPriority() >= queue_.front()->getPriority()){
        queue_.push_front(new_process); //if >= push to front
    }
    //3. if new process is less(or equal) priority than back of queue
    else if(new_process->getPriority() <= queue_.back()->getPriority()) {
        queue_.push_back(new_process); 
    }  
    //4 if new process is of lower priority than front BUT lower priority than back of queue
    else if(new_process->getPriority() < queue_.front()->getPriority() 
         && new_process->getPriority() > queue_.back()->getPriority()) { 
      while(new_process->getPriority() < queue_.front()->getPriority()){
                temp_stack_.push(queue_.front()); 
                queue_.pop_front(); // all in queue will go into stack (LIFO)
            }      
                queue_.push_front(new_process);
            }
    //make sure stack is empty in the end
    while (!temp_stack_.empty()) { //at the end, empty stack, since its LIFO, it goes back in order.
                Process* tempPost = temp_stack_.top();
                temp_stack_.pop();
                queue_.push_front(tempPost);
            }
    return true;
}
void SimulatedOS::printQueue()const{
    std::cout << "CPU-> <-";

    for(long unsigned int i =0; i < getQueue().size(); i++){
    std::cout << getQueue()[i]->getPID() << "|";
    }
    
    std::cout << std::endl;
}


