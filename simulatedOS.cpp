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
        std::deque<Memory*> *new_frame_deque = new std::deque<Memory*>;
        frame_vector_.push_back(*new_frame_deque);
    }
}

Process SimulatedOS::NewProcess(int m_priority){
    incrementPID();
    int new_pid = this->PID_Counter_;
    Process *new_process = new Process(m_priority, new_pid);
    addProcess(new_process);
    addToTable(new_process);
    
    return *new_process;
}

void SimulatedOS::Exit(){
      std::deque <Memory*> temp;
      for(long unsigned int i = 0; i < RAM_.size(); i++){
        if(queue_.front()->getPID() != RAM_[i]->getMemPID()){
            temp.push_back(RAM_[i]);
        }else{
            RAM_[i] = nullptr;
            delete RAM_[i];
        }
    }
    RAM_.clear();
    while(!temp.empty()){
        for(long unsigned int i = 0; i < temp.size();i++){
        RAM_.push_back(temp[i]);
        temp.pop_front();
    }
    }
    if(!queue_.empty()){
        delete queue_.front(); //deletes pointer of the front of cpu
        queue_.pop_front(); // or just this by itself?
    }else{
        throw std::invalid_argument("Nothing is using the CPU");
    }
}


void SimulatedOS::PrintCPU()const{
    std::cout << "CPU: " << queue_.front()->getPID() <<std::endl;
    std::cout << std::endl;
}

void SimulatedOS::PrintReadyQueue()const{
    std::cout << "Ready Queue: ";
    for(long unsigned int i = 1; i < queue_.size(); i++){
        std::cout << queue_[i]->getPID() << " ";
    }
    std::cout << std::endl;
    std::cout << std::endl;
}

void SimulatedOS::PrintRAM(){
    std::cout << "Frame" << '\t' << "Page" << '\t' << "PID" <<std::endl;
    for(long unsigned int i = 0; i < RAM_.size(); i++){    
            std::cout << i << '\t' << RAM_[i]->getMemPageNum() << '\t' << RAM_[i]->getMemPID() << std::endl;     
    }
    std::cout << std::endl;
}

void SimulatedOS::PrintDisk(int diskNumber)const{
    for(long unsigned int i = 0; i < diskQueue_vector_[diskNumber].size(); i++){
        if(diskQueue_vector_[diskNumber][i]->completeStatus() == false){
            std::cout << "Disk: " << diskNumber << " PID: " << diskQueue_vector_[diskNumber][i]->getPID() << " " << diskQueue_vector_[diskNumber][i]->getFileName()<<std::endl;
            std::cout << std::endl;
            break;
        }else{
            std::cout << "Disk " << diskNumber << " Idle" <<std::endl;
            std::cout << std::endl;
        }
    }
}

void SimulatedOS::PrintDiskQueue(int diskNumber)const{
    if(diskNumber < 0 || diskNumber > this->getNumberOfDisks()){
        std::cout << "Instruction Ignored: no disk with such number exists" << std::endl;
        std::cout << std::endl;
    }
    else if(this->diskQueue_vector_[diskNumber].size() == 0||queue_isEmpty(diskNumber)==true){
        std::cout << "Disk " << diskNumber << " I/O queue: Empty" <<std::endl;
        std::cout << std::endl;

    }else{
        for(unsigned long int i = 0; i < this->diskQueue_vector_[diskNumber].size(); i++){
            if(this->diskQueue_vector_[diskNumber][i]->completeStatus() == false){
                std::cout << this->diskQueue_vector_[diskNumber][i]->getPID() << " "<<std::endl;
            }
        }
    }
}
void SimulatedOS::FetchFrom(unsigned int memoryAddress){
    makeRamOlder();
    //bool alreadyExists = false;
    queue_.front()->setPC(memoryAddress);
    queue_.front()->setPage(memoryAddress/this->getPageSize());
    addToTable(queue_.front());
    /*
    for(long unsigned int i = 0; i < RAM_.size(); i++){
        if(RAM_[i]->getMemPID() == queue_.front()->getPID() && RAM_[i]->getMemPageNum() == queue_.front()->getPage()){
            alreadyExists =true;
            //makeRamOlder();
            RAM_[i]->makeFreshlyUsed();
        }
    }
    if(alreadyExists == false){
        
    }
    */
    //if statement if already in RAM, dont push anything and just increment everyhting by one (make ram older)
    //if same pid with same page already exists-- just increment and make the mem thing new 
}
void SimulatedOS::DiskReadRequested(int diskNumber, std::string fileName){
    makeRamOlder();
    if(diskNumber >= 0 && diskNumber <= this->getNumberOfDisks()){
        frame_vector_[diskNumber].push_back(RAM_[findCPUuser()]);

        queue_.front()->setDiskNumber(diskNumber);
        queue_.front()->setFileName(fileName);
        diskQueue_vector_[diskNumber].push_front(queue_.front());//! doesnt exist 
        queue_.pop_front();
        }
        //for all in ram if pid == curr using cpu && newest 
        
        //findCPUuser()->makeFreshlyUsed();

        //addToTable(queue_.front());
        /*
        
        for(long unsigned int i = 0; i < RAM_.size();i++){
            if(queue_.front()->getPID()== RAM_[i]->getMemPID()&&findCPUuser(queue_.front())->getMemPageNum() !=RAM_[i]->getMemPageNum()){
                addToTable(queue_.front()->getPage(), RAM_[i]->getMemPID(), queue_.front()->getPriority());
            }
        }
        */
        
        
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
                if(queue_.front()->getPID() == diskQueue_vector_[diskNumber][i]->getPID()){ //if disk jon completed is using the CPU, then you need to load it into memory if its not already there 

                }
                break;
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

void SimulatedOS::addToTable(Process* process){
    makeRamOlder();
    Memory *new_memory = new Memory(process->getPage(), process->getPID(), process->getPriority());
    //if there is space in the RAM, just add it 
    if((int)RAM_.size() < (getAmountOfRAM()/getPageSize())){
        new_memory->incrementUse();;
        RAM_.push_back(new_memory);
    //if not, decide which spot it should take (frame last used)
    }else{
        new_memory->incrementUse();
        RAM_[findOldestInRAM()] = new_memory;
    }
}

void SimulatedOS::makeRamOlder(){
    for(long unsigned int i = 0; i < RAM_.size();i++){
        RAM_[i]->incrementUse();
    }
}

void SimulatedOS::printQueue()const{
    std::cout << std::endl;
    for(long unsigned int i =0; i < getQueue().size(); i++){
    getQueue()[i]->printProcess();
    std::cout << std::endl;
    }
    std::cout << std::endl;
}
int SimulatedOS::findOldestInRAM(){
      std::vector<Memory*> results;
    int newest = 100;
    long unsigned int answer;

    for(long unsigned int i = 0; i< RAM_.size(); i++){
        if(queue_.front()->getPID() == RAM_[i]->getMemPID()){
            results.push_back(RAM_[i]);
            }
        }
         if(results.size() > 1){
            for(long unsigned int i = 0; i < results.size(); i++){
                 if(results[i]->getMemUse() < newest){
                    newest = results[i]->getMemUse();
                    answer = i;
                }
            } 
        }

    int oldest=0;
    int oldest_index;
    for(long unsigned int j = 0; j < RAM_.size();j++){
            if(RAM_[j]->getMemUse() >= oldest && j != answer){
            oldest = RAM_[j]->getMemUse();
            oldest_index = j;
         }
    }
    return oldest_index;
}
int SimulatedOS::findCPUuser(){
    std::vector<Memory*> results;
    int newest = 100;
    int answer= 0;
    int usingCPU = 0;
    for(long unsigned int i = 0; i< RAM_.size(); i++){
        if(queue_.front()->getPID() == RAM_[i]->getMemPID()){
            results.push_back(RAM_[i]);
         }
    }
        for(long unsigned int i = 0; i < results.size(); i++){
        if(results[i]->getMemUse() < newest){
        newest = results[i]->getMemUse();
        answer = i; //this!!!!
             }   
          } 
          for(long unsigned int j = 0; j < RAM_.size(); j++){
            if(results[answer]->getMemPID() == RAM_[j]->getMemPID() && results[answer]->getMemPageNum() == RAM_[j]->getMemPageNum()){
                usingCPU = j;
            }
          }
        return usingCPU; 
    }
    
        
   

bool SimulatedOS::queue_isEmpty(int diskNumber)const{
    for(long unsigned int i = 1; i < this->diskQueue_vector_[diskNumber].size(); i++){
        if(this->diskQueue_vector_[diskNumber][i]->completeStatus() == false){
            return false;
            break;
        }
    }
    return true;
}
bool SimulatedOS::isUsingCPU(int index){
    if(RAM_[index] == RAM_[findCPUuser()]){
        return true;
     }else{
        return false;
     }
}

bool SimulatedOS::isUnique(Memory* mem){
    for(long unsigned int i = 0; i < RAM_.size(); i++){
        if(mem->getMemPageNum() == RAM_[i]->getMemPageNum() && mem->getMemPID() == RAM_[i]->getMemPageNum()){
            return false;
            break;
        }
    }
    return true;
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