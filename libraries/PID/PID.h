struct PID_s
{
  int sp:24;    unsigned int dummy1:8;
  int d:6;      unsigned int dummy2:2;
  int i:6;      unsigned int dummy3:2;
  int p:6;      unsigned int dummy4:2;
  unsigned int dummy5:8;
  int out:12;   unsigned int dummy6:20;
  int pv:24;    unsigned int dummy7:8;
};

<<<<<<< HEAD
volatile PID_s *PID = (volatile struct PID_s *) 0xFFFFFD80;
=======
volatile PID_s *PID = (volatile struct PID_s *) 0xFFFFFD40;
>>>>>>> bbaddca15cb0e63f4c9f7bb8d88aed78ed559ee3
