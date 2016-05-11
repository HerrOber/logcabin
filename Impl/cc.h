
#include <string>

/*
  Class that represents the running Raft server instance
*/
class Raft{
  public:
    int m_pid, m_port;
    std::string m_address;
    Raft(int, int, std::string);
    ~Raft();
};

/*
  Class that represents the running Regional Control (RC) entity.
  This will run a 'regular' host at startup but may later run
  on a AP after a RC failure.
*/
class RC{
  public:
    int m_pid, m_port;
    std::string m_address;
    RC(int, int, std::string);
    ~RC();
};

/*
  The main class of the entire stack, it will check connectivity to all nodes
  and detect failures. It is also responsible for the running Raft intsance,
  APC and the RC. If this node becomes leader it is resposible for spawning the
  RC.
*/
class APC{
  int m_pid, m_port;
  public:
    APC(int, int);
    ~APC();
};
class Core{
  public:
    Core(bool, Raft, RC, int , char**);
  private:
    Raft m_raft;
    RC m_rc;
    void udpServer();
    void udpClient(int, char**);
};
