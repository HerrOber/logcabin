%module Ops
%{
#include "Ops.h"
%}
%include "Ops.h"

class Ops{
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
