void checkargs(FILE *&,int &,int,char *[]);
void dofuncs(int,char *,char *);
void endtrk(mtrk_hdr &,eot,track *&);
void newendtrk(mtrk_hdr &,eot,track *&);
int preparselin(char *&);
int parselin(char *,char,char *&,char *&);
void fatal(const char *);
void warning(const char *);
void writemidifil(int,struct mthd_hdr);
int writevarlen(unsigned char *,unsigned long);
void newtrack(track *&);
void initMTrk(mtrk_hdr &,mthd_hdr &,track *&);
void error(const char *);
void fatal(const char *); //this is already declared
void warning(const char *); //this too
