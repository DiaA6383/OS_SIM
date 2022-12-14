#include <iostream>
#include "simulatedOS.h"
#include "vector"
#include "Process.h"


SimulatedOS::SimulatedOS(int m_numberOfDisks, int m_amountOfRAM, int m_pageSize){
     if(m_amountOfRAM % m_pageSize !=0){
        throw std::invalid_argument("amountOfRAM MUST be divisible to pageSize");
    }
    setNumberOfDisks(m_numberOfDisks);
    setPageSize(m_pageSize);
    setAmountOfRAM(m_amountOfRAM);
    PID_Counter_ = 0; //initialize PID after each OS instance
    for(int i = 0; i < m_numberOfDisks; i++){
        std::deque<Process*> *new_deque = new std::deque<Process*>;
        diskQueue_vector_.push_back(*new_deque);
    }
}

Process SimulatedOS::NewProcess(int m_priority){
    incrementPID();
    int new_pid = this->PID_Counter_;
    Process *new_process = new Process(m_priority, new_pid);
    addProcess(new_process);
    addToTable(new_pid, m_priority);
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

void SimulatedOS::FetchFrom(unsigned int memoryAddress){
   
    queue_.front()->setPC(memoryAddress);
    
}

void SimulatedOS::PrintCPU()const{
    std::cout << "CPU: " << queue_.front()->getPID() <<std::endl;
}

void SimulatedOS::PrintReadyQueue()const{
    std::cout << "Ready Queue: ";
    for(long unsigned int i = 1; i < queue_.size(); i++){
        std::cout << queue_[i]->getPID() << " ";
    }
    std::cout << std::endl;
}

void SimulatedOS::PrintRAM()const{
    std::cout << "Frame     Page     PID " << std::endl;
    for(long unsigned int i = 0; i < RAM_.size(); i++){    
    //std::cout << i << " " << RAM_[i]->getPageNum() << " " << RAM_[i]->getMemPID() << std::endl;
    }
}
void SimulatedOS::DiskReadRequested(int diskNumber, std::string fileName){
    if(diskNumber >= 0 && diskNumber <= this->getNumberOfDisks()){
        queue_.front()->setDiskNumber(diskNumber);
        queue_.front()->setFileName(fileName);
        diskQueue_vector_[diskNumber].push_back(queue_.front());//! doesnt exist 
        queue_.pop_front();
    }
}
void SimulatedOS::DiskJobCompleted(int diskNumber){
    if(diskNumber < 0 || diskNumber > this->getNumberOfDisks()-1){
        //throw std::invalid_argument("Disk Number does not exist.");
        std::cout << "Disk Number does not exist." <<std::endl;
    }else{
        for(long unsigned int i = 0; i < diskQueue_vector_[diskNumber].size(); i++){
            if(diskQueue_vector_[diskNumber][i]->completeStatus() != true){
                diskQueue_vector_[diskNumber][i]->setCompleteStatus();
                addProcess(diskQueue_vector_[diskNumber][i]);
                break;
            }
        }
        //addProcess(diskQueue_.front())
        //diskQueue_.pop_front();
    }
}
void SimulatedOS::PrintDisk(int diskNumber)const{
    for(long unsigned int i = 0; i < diskQueue_vector_[diskNumber].size(); i++){
        if(diskQueue_vector_[diskNumber][i]->completeStatus() == false){
            std::cout << "Disk: " << diskNumber << " PID: " << diskQueue_vector_[diskNumber][i]->getPID() << " " << diskQueue_vector_[diskNumber][i]->getFileName()<<std::endl;
            break;
        }else{
            //throw std::invalid_argument("Disk number does not exist.");
            std::cout << "Disk " << diskNumber << " Idle" <<std::endl;
        }
    }
}

void SimulatedOS::PrintDiskQueue(int diskNumber)const{
    if(diskNumber < 0 || diskNumber > this->getNumberOfDisks()){
        std::cout << "Instruction Ignored: no disk with such number exists" << std::endl;
    }
    else if(this->diskQueue_vector_[diskNumber].size() == 0||queue_isEmpty(diskNumber)==true){
        std::cout << "Disk " << diskNumber << " I/O queue: Empty" <<std::endl;

    }else{
        for(unsigned long int i = 0; i < this->diskQueue_vector_[diskNumber].size(); i++){
            if(this->diskQueue_vector_[diskNumber][i]->completeStatus() == false){
                std::cout << this->diskQueue_vector_[diskNumber][i]->getPID() << " ";
            }
        }
    }
}

//Helpers
void SimulatedOS::incrementPID(){
   PID_Counter_++;
}

bool SimulatedOS::addProcess(Process *new_process){ //if higher go 
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
    //5. make sure stack is empty in the end, should be in order 
    while (!temp_stack_.empty()) { 
                Process* tempPost = temp_stack_.top();
                temp_stack_.pop();
                queue_.push_front(tempPost);
            }
    return true;
}


void SimulatedOS::printQueue()const{
    std::cout << std::endl;
    for(long unsigned int i =0; i < getQueue().size(); i++){
    getQueue()[i]->printProcess();
    std::cout << std::endl;
    }
    std::cout << std::endl;
}

bool SimulatedOS::queue_isEmpty(int diskNumber)const{
    for(long unsigned int i = 0; i < this->diskQueue_vector_[diskNumber].size(); i++){
        if(this->diskQueue_vector_[diskNumber][i]->completeStatus() == false){
            return false;
            break;
        }
    }
    return true;
}
int SimulatedOS::findOldestInRAM()const{
    int oldest=0;
    int index=0;
    
    for(long unsigned int i = 0; i < RAM_.size();i++){
        if(RAM_[i]->getMemUse() > oldest){
            oldest = RAM_[i]->getMemUse();
            index = (int)i;
        }
        
    }
    return index;
}
void SimulatedOS::addToTable(int pid, int priority){
    Memory *new_memory = new Memory(0, pid, priority);
    
    if((int)RAM_.size() <= (getAmountOfRAM()/getPageSize())){
        new_memory->incrementUse();;
        RAM_.push_back(new_memory);
            
    }else{
        new_memory->incrementUse();
        RAM_[findOldestInRAM()] = new_memory;
        
    }
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
std::vector<Memory*>SimulatedOS::getRAM()const{
    return RAM_;
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



