/*
	If you find any mistakes in this file
	please let me know, so I can fix them and reupload
	fixed version for everyone.
*/

#include "SimulatedOS.h"
#include "Process.h"
#include "Memory.h"

int main()
{
	/*
	

	SimulatedOS osSim{3, 96, 32};

	osSim.NewProcess(4);
	osSim.NewProcess(2);
	osSim.NewProcess(7);

	osSim.PrintCPU();
	// CPU: 3

	osSim.PrintReadyQueue();
	// Ready-Queue: 1 2
	

	osSim.DiskReadRequested(0, "HW.txt");

	osSim.PrintCPU();
	// CPU: 1

	osSim.PrintDisk(0);
	// Disk 0: PID 3, "HW.txt"

	osSim.PrintDiskQueue(0);
	// Disk 0 I/O-queue: Empty

	osSim.PrintDiskQueue(5);
	// Instruction ignored: no disk with such number exists

	osSim.DiskJobCompleted(0);
	osSim.PrintCPU();
	// CPU: 3

	osSim.PrintDisk(0);
	// Disk 0: Idle

	osSim.FetchFrom(48);
	osSim.PrintRAM();
	// Frame	Page	PID
	// 0		0		1
	// 1		1		3
	// 2		0		3

	osSim.Exit();

	osSim.PrintCPU();
	// CPU: 1

	osSim.PrintRAM();
	// Frame	Page	PID
	// 0		0		1
	

std::cout << "TEST 2 ********************************************************" << std::endl;
*/
	SimulatedOS osSim2{ 1, 300, 100 };

	osSim2.NewProcess(10);//pid 1
		osSim2.PrintRAM();
	// Frame	Page	PID
	// 0		0		1++
	// 1		
	// 2		
	osSim2.NewProcess(5);//pid 2

	osSim2.PrintRAM();
	// Frame	Page	PID
	// 0		0		1++
	// 1		0		2
	// 2		

	osSim2.FetchFrom(120); //120/100 = 1 
	osSim2.PrintRAM(); //cpu-> pid1 pg1
	// Frame	Page	PID
	// 0		0		1++
	// 1		0		2
	// 2		1		1++

	osSim2.DiskReadRequested(0, "job.docx");//cpu->pid1 pg 1to disk queue
	osSim2.PrintRAM();//cpu-> pid2 pg0
	// Frame	Page	PID
	// 0		0		1++
	// 1		0		2
	// 2		1		1++

	osSim2.FetchFrom(777); //700/100 = 7
	osSim2.PrintRAM();//cpu->pid2 pg7
	// Frame	Page	PID
	// 0		7		2++
	// 1		0		2
	// 2		1		1

	osSim2.NewProcess(1);//pid3 pg0
	osSim2.PrintRAM();
	// Frame	Page	PID
	// 0		7		2
	// 1		0		2
	// 2		0		3++

	osSim2.DiskJobCompleted(0);
	// The following happens after this command:
	// The process 1 returns to ready-queue after the hard disk finished reading file for it.
	// Since the process 1 has the higher priority than the one currently running, the process 1 starts using the CPU immedeately
	// To use the CPU, the process one need its last used page (page 1) in memory (and thats what your simulation should provide)
	osSim2.PrintRAM(); //cpu->pid1 pg1
	// Frame	Page	PID
	// 0		7		2
	// 1		1		1
	// 2		0		3

	osSim2.Exit();
	osSim2.PrintRAM(); //pid1-pg1 deleted
	// Frame	Page	PID
	// 0		7		2
	// 1		
	// 2		0		3

	osSim2.FetchFrom(740);
	osSim2.PrintRAM();//cpu->pid2 since 7 is already in ram, stay the same
	// Frame	Page	PID
	// 0		7		2
	// 1		
	// 2		0		3

	osSim2.FetchFrom(3350);
	osSim2.PrintRAM();
	// Frame	Page	PID
	// 0		7		2
	// 1		33		2
	// 2		0		3

	osSim2.FetchFrom(740);
	// The page 7 of the process 2 is already in memory. RAM snapshot doesn't change but 
	// your simulation remembers that the page 7 is now freshly used.
	osSim2.PrintRAM();
	// Frame	Page	PID
	// 0		7		2
	// 1		33		2
	// 2		0		3

	osSim2.NewProcess(20);
	osSim2.FetchFrom(100);
	osSim2.PrintRAM();
	// Frame	Page	PID
	// 0		7		2
	// 1		1		4
	// 2		0		4

	return 0;
	
	
}