/**/
%module Ops

%{
#include "Ops.h"
%}

class Ops{
private:
    std::string clusterAddr;
    uint64_t timeout;	
	std::string logPolicy;
    std::shared_ptr<LogCabin::Client::Tree> tree;
    void dumpTree(std::string);
	void dumpTreeLocal(std::string);
public:
	Ops(std::string, uint64_t, std::string);
	~Ops();
	void mkdir(std::string);
	void rmdir(std::string);
	void read(std::string);
	void readLocal(std::string);
	void write(std::string,std::string);
	void remove(std::string);
	void dump(std::string);
	void dumpLocal(std::string);
	void listKeys(std::string);
	void makeLeader();
	void getLeader();
	int reconfigure(std::vector<std::string>);
};

