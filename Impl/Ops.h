#include "include/LogCabin/Client.h"
#include "include/LogCabin/Debug.h"
#include "include/LogCabin/Util.h"

class Ops{
private:
    std::string clusterAddr;
    uint64_t timeout;	
	std::string logPolicy;
    std::shared_ptr<LogCabin::Client::Tree> tree;
    void dumpTree(std::string);
	int dumpTreeLocal();
public:
	Ops(std::string clusterAddr, uint64_t, std::string);
	~Ops();
	void mkdir();
    void rmdir();
    void read();
    void write();
    void remove();
    void dump(std::string);
    void list();
    void makeLeader();
    void getLeader();
};
