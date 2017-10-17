typedef void (* FuncSig)(void);

void Start_OS(void);

void Schedule(void);

void Reschedule(void);

extern unsigned int SystemStack;