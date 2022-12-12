#include <iostream>
#include "simulatedOS.h"
#include "Process.h"

/**
 * @brief Construct a new simulation O S::simulation O S object
 The parameters specify number of hard disks in the simulated computer, amount of memory and page size.
 Disks and pages enumeration starts from 0. The amountOfRAM should always be divisible by pageSize. Feel 
 free to through any exception if it is not so. When testing, I promise to use only correct and greater 
 than zero constructor parameters.
 * 
 * @param m_numberOfDisks 
 * @param m_amountOfRAM 
 * @param m_pageSize 
 */
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

/**
 * @brief Creates a new process with the specified priority in the simulated system. The new process takes 
 place in the ready-queue or immediately starts using the CPU. Every process in the simulated system has a PID. 
 Your simulation assigns PIDs to new processes starting from 1 and increments it by one for each new process. 
 Do not reuse PIDs of the terminated processes. Every process has a program counter. PC starts from 0 for a new 
 process. This PC of ours doesn’t behave like a real PC and updates only when we issue a corresponding instruction.
 Page 0 that has the starting address of 0 should be loaded in RAM when a new process appears. It is considered 
 “freshly used”. No process can run until page with the current PC address is loaded in the memory.

 * 
 * @param m_priority 
 * @returns new process
 */
Process SimulatedOS::NewProcess(int m_priority){
    incrementPID();
    int new_pid = this->PID_Counter_;
    Process *new_process = new Process(m_priority, new_pid);
    addProcess(new_process);
    return *new_process;
}

/**
 * @brief Currently running process wants to terminate. 
 * Immediately remove the process from the system and free all the memory it was using.
 * 
 */
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
    //if queue is empty just add
    if(queue_.size() == 0){
        queue_.push_front(new_process);
    }
    else if(new_process->getPriority() >= queue_.front()->getPriority()){
        queue_.push_front(new_process); //if >= push to front
    }

    else if(new_process->getPriority() < queue_.front()->getPriority() && new_process->getPriority() > queue_.back()->getPriority()) { //while less than front and more than back
    while(new_process->getPriority() < queue_.front()->getPriority()){
        temp_stack_.push(queue_.front()); 
        queue_.pop_front();
            // all in queue will go into stack (LIFO)
        }      
            queue_.push_front(new_process);
        }
    
        
    else if(new_process->getPriority() <= queue_.back()->getPriority()) {
            queue_.push_back(new_process); //if => back, go ahead and push to back
    }  
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


