#include <iostream>
#include "process.h"
Process::Process(int m_priority, int m_PID){
    setPC(0);
    setPriority(m_priority);
    setPID(m_PID);
    setPage(0);
}
/**
 * @brief Destroy the Process:: Process object
 * set all Process attributes to null and delete object since we used NEW keyword
 */
//Process::~Process();

void Process::setPC(int m_PC){
    this->PC_ = m_PC;
}
void Process::setPID(int m_PID){
    this->PID_ = m_PID;
}
void Process::setPriority(int m_priority){
    this->priority_ = m_priority;
}
void Process::setPage(int m_page){
    this->page_ = m_page;
}

int Process::getPC()const{
    return PC_;
}
int Process::getPID()const{
    return  PID_;
}
int Process::getPriority()const{
    return priority_;
}
int Process::getPage()const{
    return page_;
}

void Process::printProcess()const{
    std::cout << "PID: " << this->getPID() << std::endl;
    std::cout << "PC: " << this->getPC() << std::endl;
    std::cout << "Priority: " << this->getPriority() << std::endl;
    std::cout << std::endl;
}
