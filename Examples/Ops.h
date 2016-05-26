//#include "../include/LogCabin/Client.h"
//#include "../include/LogCabin/Debug.h"
//#include "../include/LogCabin/Util.h"
#include <LogCabin/Client.h>
#include <LogCabin/Debug.h>
#include <LogCabin/Util.h>

//#include "Ops.h"
class Ops{
private:
    std::string clusterAddr;
    uint64_t timeout;	
	std::string logPolicy;
    std::shared_ptr<LogCabin::Client::Cluster> clstr;
    std::shared_ptr<LogCabin::Client::Tree> tree;
    std::string dumpTree(std::string);
	std::string dumpTreeLocal(std::string);
    void printConfiguration(const std::pair<uint64_t, LogCabin::Client::Configuration>&);
public:
	Ops(std::string, uint64_t, std::string);
	~Ops();
	void mkdir(std::string);
    void rmdir(std::string);
    std::string read(std::string);
    std::string readLocal(std::string);
    void write(std::string,std::string);
    void remove(std::string);
    std::string dump(std::string);
    std::string dumpLocal(std::string);
    std::string listKeys(std::string);
    void makeLeader();
    std::string getLeader();
    int reconfigure(std::vector<std::string>);
};
