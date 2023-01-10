#include <cstdint>
using namespace std;


//satp = SATP register with mode, asid, and ppn
//vaddr = virtual address
//paddr = physical address


//This program is based off of the 0xdeadbeef problem in the lecture

bool translate(uint64_t *satp, uint64_t vaddr, uint64_t &paddr){
	bool segfault = false;

	//MODE = 4 bits, ASID = 16 bits, PPN = 44 bits
	//ASID + PPN = 60 bits so you need to move the first four bits in the SATP to the end
	uint64_t mode = &satp >> 60;
	
	//If the first number of SATP is 8 (0x8000000000123456), MMU is ON
	//If the MMU is on, the function will read SATP register and translate vaddr to paddr
	if(mode == 8){	
		uint64_t vpn2 = (vaddr >> 30) & 0x1ff;
		uint64_t vpn1 = (vaddr >> 21) & 0x1ff;
		uint64_t vnp0 = (vaddr >> 12) & 0x1ff;
		uint64_t po = vaddr & 0xfff;
		paddr = (&satp & 0x1fffff) << 12;
		paddr = paddr + 8 * vpn2;
	}
	//If the first number of SATP is 0 (turned OFF), paddr = vaddr
	else if(mode == 0)
		paddr = vaddr;
	
	
	//Dereference paddr to get PTE at level 2 (this is where you use the MMU simulator
	uint64_t pte = 0x774101;
	
	uint64_t v = pte & 1;
	uint64_t rwx = (pte >> 1) & 0x7;

	if(v == 1 & rwx == 0){	//Branch
		ppn2 = (pte >> 28) & 0x3ffffff;
		ppn1 = (pte >> 19) & 0x1ff;
		ppn0 = (pte >> 10) & 0x1ff;
		paddr = (ppn2 << 30) | (ppn1 << 21) | (ppn0 << 12);
		paddr = paddr + vnp1 * 8;
	}	//remember it stops if it's not 0?

	pte = 0x2001; //again something happens here to get this numbr


	if((pte & 1) == 1 && ((pte >> 1) & 0x7) == 0){
		pte -= 1;
		pte = pte << 2;
		pte = pte + vpn0 * 8;
	}	//there's another choice here


	pte = 0x1700f; //what happense here? no one knows

	if((pte & 1) == 1 && ((pte >> 1) & 0x7) == 0)
		segfault = true;
	else{
		ppn2 = (pte >> 28) & 0x3ffffff;
		ppn1 = (pte >> 19) & 0x1ff;
		ppn0 = (pte >> 10) & 0x1ff;
		paddr = (ppn2 << 30) | (ppn1 << 21) | (ppn0 << 12);
		paddr = paddr | po;
	}


	return segfault;
}
