void checkargs(FILE *&,int &,int,char *[]);
void dofuncs(int,char *,char *);
void endtrk(mtrk_hdr &,eot,track *&);
void newendtrk(mtrk_hdr &,eot,track *&);
int preparselin(char *&);
int parselin(char *,char,char *&,char *&);
void fatal(const char *);
void warning(const char *);
void writemidifil(int,struct mthd_hdr);
