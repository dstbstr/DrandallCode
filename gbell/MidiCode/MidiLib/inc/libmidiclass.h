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
        mthd_hdr();
};

class mtrk_hdr
{
    public:
        char id[4];
        unsigned int length;
        mtrk_hdr();
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
    public:
        eot();
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
    public:
        tsig();
};

class tmpo
{
    unsigned char stat;
    char mnum;
    char msgbytes;
    public:
        char buf[3];
        tmpo(); 
};

class keysig
{
    unsigned char stat;
    char mnum;
    char msgbytes;
    public:
        unsigned char fltshrp;
        char majmin; 
        keysig();
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
        susmsg();
};

class chgprog
{
    public:
        unsigned char mnum;
        char instr;
};

#define HDRSIZ 8
