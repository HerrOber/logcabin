
#include <cassert>
#include <getopt.h>
#include <iostream>
#include <iterator>
#include <sstream>

//env['STATIC_AND_SHARED_OBJECTS_ARE_THE_SAME']=1

//#include <LogCabin/Client.h>
//#include <LogCabin/Debug.h>
//#include <LogCabin/Util.h>

#include "Ops.h"

using namespace std;

using LogCabin::Client::Cluster;
using LogCabin::Client::Tree;
using LogCabin::Client::Util::parseNonNegativeDuration;

    Ops::Ops(std::string clusterAddr = "127.0.0.1:5254", uint64_t timeout = 0, std::string logPolicy = "")
    :clusterAddr (clusterAddr)
    ,timeout (timeout)
    ,logPolicy (logPolicy)
    ,tree()
    {
        LogCabin::Client::Debug::setLogPolicy(
            LogCabin::Client::Debug::logPolicyFromString(
                logPolicy));
        Cluster cluster(clusterAddr);
        *tree = cluster.getTree();

        if (timeout > 0) {
            tree->setTimeout(timeout);
        }


    }

    Ops::~Ops(){

    }

        /**
         * Depth-first search tree traversal, dumping out contents of all files
         */
    void
    Ops::dumpTree(std::string path)
    {
        std::cout << path << std::endl;
        std::vector<std::string> children = tree->listDirectoryEx(path);
        for (auto it = children.begin(); it != children.end(); ++it) {
            std::string child = path + *it;
            if (*child.rbegin() == '/') { // directory
                dumpTree(child);
            } else { // file
                std::cout << child << ": " << std::endl;
                std::cout << "    " << tree->readEx(child) << std::endl;
            }
        }   
    }

    int
    Ops::dumpTreeLocal()
    {
       return 4711; 
    }

    /**
    Create a directory
    */
    void
    Ops::mkdir(){}

    /**
    Remove a directory
    */
    void
    Ops::rmdir(){}

    /**
    Read value of file
    */
    void
    Ops::read(){}

    /**
    Write value to file
    */
    void
    Ops::write(){}

    /**
    Remove a file
    */
    void
    Ops::remove(){}

    /**
    Dump the entire tree, i.e. all K/V
    */
    void
    Ops::dump(std::string path){
        if (path.empty() || path.at(path.size() - 1) != '/')
            path.append("/");
        
        dumpTree(path);
    }

    /**
    List all keys
    */
    void
    Ops::list(){}

    /**
    Make the local node leader, i.e. force election
    */
    void
    Ops::makeLeader(){}

    /**
    Get the current leader IP
    */
    void
    Ops::getLeader(){}





