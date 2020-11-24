#ifndef __LIBMIDICLASS_H__
#define __LIBMIDICLASS_H__

extern long ppqn; //this doesn't appear to be used in this file.

//would consider adding a header guard to this header file
class mthd_hdr
{
    char id[4];
    public:
        unsigned int length;
    private:
        unsigned short format;
    public:
        unsigned short numtrks;
        unsigned short division;
};

class mtrk_hdr
{
    public:
        char id[4];
        unsigned int length;
};

struct track
{
    int trck;
    char* datap;
    int datalen;
    int vel;
    int notelen;
    int ticks;
    int nu;
    int chnl;
    track *np;
};

class eot
{
    unsigned char stat;
    unsigned char eotm;
    unsigned char eots;
};

class tsig
{
    unsigned char stat;
    char mnum;
    char msgbytes;
    public:
        char nn;
        char dd;
    private:
        char cc;
        char bb;
};

class tmpo
{
    unsigned char stat;
    char mnum;
    char msgbytes;
    public:
        char buf[3];
};

class keysig
{
    unsigned char stat;
    char mnum;
    char msgbytes;
    public:
        unsigned char fltshrp;
        char majmin; 
};

class notemsg
{
    public:
        unsigned char mnum;
        char note;
        char vel;
};

class susmsg
{
    public:
        unsigned char mnum;
    private:
        char sus;
    public:
        char onoff;
};

class chgprog
{
    public:
        unsigned char mnum;
        char instr;
};

#endif // __LIBMIDICLASS_H__