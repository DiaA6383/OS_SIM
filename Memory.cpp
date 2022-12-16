//Name: Alejandro Diaz
//Course: Operating Systems CSCI 340
//Professor: Shostak 
//Project: OS Simulator
#include "Memory.h"
#include <iostream>

Memory::Memory(int m_pageNum_, int m_pid_f, int m_priority_f){
    setMemPageNum(m_pageNum_);
    setMemPID(m_pid_f);
    setMemPriority(m_priority_f);
    use_ = 0;
}
int Memory::getMemPageNum()const{
    return pageNum_;
}
int Memory::getMemPID()const{
    return pid_f;
}
int Memory::getMemUse()const{
    return use_;
}
int Memory::getMemPriority()const{
    return priority_f;
}
void Memory::setMemPageNum(int m_pageNum){
    pageNum_ = m_pageNum;
}
void Memory::setMemPID(int m_pid_f){
    pid_f = m_pid_f;
}
void Memory::setMemPriority(int m_priority){
    priority_f = m_priority;
}
void Memory::makeFreshlyUsed(){
    use_ = 0;
}
void Memory::incrementUse(){
    use_++;
}
void Memory::setMemCompleteStatus(){
    this->isCompleted = true;
}



