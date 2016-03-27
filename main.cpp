//
//  main.cpp
//  memorymanagement
//
//  Created by 王 豪烨 on 15/6/9.
//  Copyright (c) 2015年 王 豪烨. All rights reserved.
//
//编写程序模拟存储器运行。分别以byte、short(2字节)、int(4字节)数组模拟内存，实现：lw、sw、lh、lhu、sw、lb、lbu、sb功能。同时考虑大小头两种情况。

#include <iostream>
#include <iomanip>
#include <stdlib.h>
using namespace std;
typedef unsigned char BYTE;

class mmu{
private:
    BYTE mem1[64];//地址递增
    short mem2[32];
    int mem3[16];
    bool flag;     //BigEndian==true,LittleEndian= false
public:
    mmu(bool fl=false);//默认小头
    void lw(int adr);//取字32bit
    void sw(int adr, int dat);//存字
    
    void lh(int adr);//取半字16bit
    void lhu(int adr);//取无符号半字
    void sh(int adr, short dat);//存半字
    
    void lb(int adr);//取字节8bit
    void lbu(int adr);//取无符号字节
    void sb(int adr,BYTE dat);//存字节
    
    void show();//显示整个内存
    //void init();//输入字节
};
mmu::mmu(bool fl){
    flag=fl;
    for (int i=0; i<64; i++) {
        mem1[i]=0;
    }
    for (int i=0; i<32; i++) {
        mem2[i]=0;
    }
    for (int i=0; i<16; i++) {
        mem3[i]=0;
    }
}

void mmu::lw(int adr){//取32位
    int temp1,temp2,temp3;
    if (flag==false) {     //little
        temp3=mem3[adr/4];    //int
        temp2=mem2[adr/2]+(mem2[adr/2+1]<<16);    //short
        temp1=mem1[adr]+(mem1[adr+1]<<8)+(mem1[adr+2]<<16)+(mem1[adr+3]<<24);//BYTE
    }
    else{//big
        temp3=mem3[adr/4];
        temp3=(temp3>>24)+(temp3<<8>>24<<8)+(temp3<<16>>24<<16)+(temp3<<24);
        temp2=(mem2[adr/2])+(mem2[adr/2+1]<<16);
        temp2=(temp2>>24)+(temp2<<8>>24<<8)+(temp2<<16>>24<<16)+(temp2<<24);
        temp1=(mem1[adr]<<24)+(mem1[adr+1]<<16)+(mem1[adr+2]<<8)+mem1[adr+3];
    }
    cout<<"int: 0x"<<setfill('0')<<setw(8)<<hex<<temp3<<endl;
    cout<<"short: 0x"<<setfill('0')<<setw(8)<<hex<<temp2<<endl;
    cout<<"BYTE: 0x"<<setfill('0')<<setw(8)<<hex<<temp1<<endl;
}

void mmu::sw(int adr, int dat){//存32位
    int temp;
    temp=(dat>>24)+(dat<<8>>24<<8)+(dat<<16>>24<<16)+(dat<<24);
    if (flag==false) {//little
        mem3[adr/4]=dat;
        mem2[adr/2+1]=dat>>16;
        mem2[adr/2]=(dat<<16)>>16;
        mem1[adr+3]=dat>>24;
        mem1[adr+2]=(dat<<8)>>24;
        mem1[adr+1]=(dat<<16)>>24;
        mem1[adr]=(dat<<24)>>24;
    }
    else {//big
        mem3[adr/4]=temp;
        mem2[adr/2]=temp<<16>>16;
        mem2[adr/2+1]=temp>>16;
        mem1[adr]=dat>>24;
        mem1[adr+1]=(dat<<8)>>24;
        mem1[adr+2]=(dat<<16)>>24;
        mem1[adr+3]=(dat<<24)>>24;
    }
}

void mmu::lh(int adr){
    int temp1,temp2,temp3;
    if (flag==false) {//little
        temp3=mem3[adr/4]<<16>>16;
        temp2=mem2[adr/2];
        temp1=mem1[adr]+(mem1[adr+1]<<8);
    }
    else {//big
        temp3=(mem3[adr/4]>>24)+(mem3[adr/4]<<8>>24<<8);
        temp2=(mem2[adr/2]<<24>>16)+(mem2[adr/2]>>8);
        temp1=(mem1[adr]<<8)+mem1[adr+1];
    }
    if (temp3<0) {
        cout<<"int: 0x"<<setfill('f')<<setw(8)<<hex<<temp3<<endl;
    }
    else {
        cout<<"int: 0x"<<setfill('0')<<setw(8)<<hex<<temp3<<endl;
    }
    if (temp2<0) {
        cout<<"short: 0x"<<setfill('f')<<setw(8)<<hex<<temp2<<endl;
    }
    else {
        cout<<"short: 0x"<<setfill('0')<<setw(8)<<hex<<temp2<<endl;
    }
    if (temp1<0) {
        cout<<"BYTE: 0x"<<setfill('f')<<setw(8)<<hex<<temp1<<endl;
    }
    else {
        cout<<"BYTE: 0x"<<setfill('0')<<setw(8)<<hex<<temp1<<endl;
    }
}

void mmu::lhu(int adr){
    unsigned int temp1,temp2,temp3;
    if (flag==false) {//little
        temp3=mem3[adr/4]<<16>>16;
        temp2=mem2[adr/2];
        temp1=mem1[adr]+(mem1[adr+1]<<8);
    }
    else {//big
        temp3=(mem3[adr/4]>>24)+(mem3[adr/4]<<8>>24<<8);
        temp2=(mem2[adr/2]<<24>>16)+(mem2[adr/2]>>8);
        temp1=(mem1[adr]<<8)+mem1[adr+1];
    }
    cout<<"int: 0x"<<setfill('0')<<setw(8)<<hex<<temp3<<endl;
    cout<<"short: 0x"<<setfill('0')<<setw(8)<<hex<<temp2<<endl;
    cout<<"BYTE: 0x"<<setfill('0')<<setw(8)<<hex<<temp1<<endl;
}

void mmu::sh(int adr , short dat){//存半字
    short temp;
    temp=(temp<<8)+(temp>>8);
    if (flag==false) {//little
        mem3[adr/4]=dat;
        mem2[adr/2]=dat;
        mem1[adr+1]=dat>>8;
        mem1[adr]=dat<<8>>8;
    }
    else {
        mem3[adr/4]=temp<<16;
        mem2[adr/2]=temp;
        mem1[adr]=temp<<8>>8;
        mem1[adr+1]=temp>>8;
    }
}

void mmu::lb(int adr){
    int temp1,temp2,temp3;
    if (flag==false) {
        temp3=mem3[adr/4]<<24>>24;
        temp2=mem2[adr/2]<<8>>8;
        temp1=mem1[adr];
    } else {
        temp3=mem3[adr/4]>>24;
        temp2=mem2[adr/2]>>8;
        temp1=mem1[adr];
    }
    if (temp3<0) {
        cout<<"int: 0x"<<setfill('f')<<setw(8)<<hex<<temp3<<endl;
    }
    else {
        cout<<"int: 0x"<<setfill('0')<<setw(8)<<hex<<temp3<<endl;
    }
    if (temp2<0) {
        cout<<"short: 0x"<<setfill('f')<<setw(8)<<hex<<temp2<<endl;
    }
    else {
        cout<<"short: 0x"<<setfill('0')<<setw(8)<<hex<<temp2<<endl;
    }
    if (temp1<0) {
        cout<<"BYTE: 0x"<<setfill('f')<<setw(8)<<hex<<temp1<<endl;
    }
    else {
        cout<<"BYTE: 0x"<<setfill('0')<<setw(8)<<hex<<temp1<<endl;
    }
}

void mmu::lbu(int adr){
    unsigned int temp1,temp2,temp3;
    if (flag==false) {
        temp3=mem3[adr/4]<<24>>24;
        temp2=mem2[adr/2]<<8>>8;
        temp1=mem1[adr];
    } else {
        temp3=mem3[adr/4]>>24;
        temp2=mem2[adr/2]>>8;
        temp1=mem1[adr];
    }
    cout<<"int: 0x"<<setfill('0')<<setw(8)<<hex<<temp3<<endl;
    cout<<"short: 0x"<<setfill('0')<<setw(8)<<hex<<temp2<<endl;
    cout<<"BYTE: 0x"<<setfill('0')<<setw(8)<<hex<<temp1<<endl;
}

void mmu::sb(int adr, BYTE dat){
    if (flag==false) {
        mem3[adr/4]=dat;
        mem2[adr/2]=dat;
        mem1[adr]=dat;
    } else {
        mem3[adr/4]=dat<<24;
        mem2[adr/2]=dat<<8;
        mem1[adr]=dat;
    }
}

void mmu::show(){
    int n;
    cout<<"int: ";
    for (int i=0; i<16; i++) {
        cout<<hex<<mem3[i]<<" ";
    }
    cout<<endl;
    cout<<"short: ";
    for (int i=0; i<32; i++) {
        cout<<hex<<mem2[i]<<" ";
    }
    cout<<endl;
    cout<<"BYTE: ";
    for (int i=0; i<64; i++) {
        n=mem1[i];
        cout<<hex<<n<<" ";
    }
    cout<<endl;
}

int main(int argc, const char * argv[]) {
    mmu mem;//默认为小头
   // mem=new mmu(true);
    mem.sb(12, 0x12);
    // mem.lbu(4);
    mem.sw(4,0x12345678);
    mem.lw(4);
    mem.lh(6);
    mem.show();
    return 0;
}
