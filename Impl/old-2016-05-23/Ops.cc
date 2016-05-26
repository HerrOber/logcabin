
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
using LogCabin::Client::Configuration;
using LogCabin::Client::ConfigurationResult;
using LogCabin::Client::Result;
using LogCabin::Client::Server;
using LogCabin::Client::Status;

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

    void
    Ops::dumpTreeLocal(std::string path)
    {
        std::cout << path << std::endl;
        std::vector<std::string> children = tree->listDirectoryExLocal(path);
        for (auto it = children.begin(); it != children.end(); ++it) {
            std::string child = path + *it;
            if (*child.rbegin() == '/') { // directory
                dumpTreeLocal(child);
            } else { // file
                std::cout << child << ": " << std::endl;
                std::cout << "    " << tree->readExLocal(child) << std::endl;
            }
        }
    }

    /**
    Create a directory
    */
    void
    Ops::mkdir(std::string path){
        tree->makeDirectoryEx(path);
    }

    /**
    Remove a directory
    */
    void
    Ops::rmdir(std::string path){
        tree->removeDirectoryEx(path);
    }

    /**
    Read value of file
    */
    void
    Ops::read(std::string path){
        std::string contents = tree->readExLocal(path);
        std::cout << contents;
        if (contents.empty() ||
            contents.at(contents.size() - 1) != '\n') {
            std::cout << std::endl;
        } else {
            std::cout.flush();
        }
    }

    /**
    Read value of file local SM
    */
    void
    Ops::readLocal(std::string path){
        std::string contents = tree->readExLocal(path);
        std::cout << contents;
        if (contents.empty() ||
            contents.at(contents.size() - 1) != '\n') {
            std::cout << std::endl;
        } else {
            std::cout.flush();
        }
    }

    /**
    Write value to file
    */
    void
    Ops::write(std::string path, std::string data){
        tree->writeEx(path, data);//todo
    }

    /**
    Remove a file
    */
    void
    Ops::remove(std::string path){
        tree->removeFileEx(path);
    }

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
    Dump the entire tree of local SM, i.e. all K/V 
    */
    void
    Ops::dumpLocal(std::string path){
        if (path.empty() || path.at(path.size() - 1) != '/')
            path.append("/");
        dumpTreeLocal(path);
    }


    /**
    List all keys
    */
    void
    Ops::list(std::string path){
        std::vector<std::string> keys = tree->listDirectoryEx(path);
        for (auto it = keys.begin(); it != keys.end(); ++it)
            std::cout << *it << std::endl;
    }

    /**
    Make the local node leader, i.e. force election
    */
    void
    Ops::makeLeader(){

        std::string contents = tree->makeLeaderEx("/");
        std::cout << contents;
        if (contents.empty() ||
            contents.at(contents.size() - 1) != '\n') {
            std::cout << std::endl;
        } else {
            std::cout.flush();
        }
    }

    /**
    Get the current leader IP
    */
    void
    Ops::getLeader(){
        std::string contents = tree->getLeaderEx();
        std::cout << contents;
        if (contents.empty() ||
            contents.at(contents.size() - 1) != '\n') {
            std::cout << std::endl;
        } else {
            std::cout.flush();
        }
    }


    void
    printConfiguration(const std::pair<uint64_t, Configuration>& configuration)
    {
        std::cout << "Configuration " << configuration.first << ":" << std::endl;
        for (auto it = configuration.second.begin();
             it != configuration.second.end();
             ++it) {
            std::cout << "- " << it->serverId << ": " << it->addresses
                      << std::endl;
        }
        std::cout << std::endl;
    }

    int
    Ops::reconfigure(std::vector<std::string> serversInConfiguration){
        Cluster cluster(clusterAddr);
        std::pair<uint64_t, Configuration> configuration =
            cluster.getConfiguration();
        uint64_t id = configuration.first;
        std::cout << "Current configuration:" << std::endl;
        printConfiguration(configuration);

        std::cout << "Attempting to change cluster membership to the following:"
                  << std::endl;
        Configuration servers;
        for (auto it = serversInConfiguration.begin(); it != serversInConfiguration.end(); ++it) {
            Server info;
            Result result = cluster.getServerInfo(*it,
                                                  /* timeout = 2s */ 2000000000UL,
                                                  info);
            switch (result.status) {
                case Status::OK:
                    std::cout << info.serverId << ": "
                              << info.addresses
                              << " (given as " << *it << ")"
                              << std::endl;
                    servers.emplace_back(info.serverId, info.addresses);
                    break;
                case Status::TIMEOUT:
                    std::cout << "Could not fetch server info from "
                              << *it << " (" << result.error << "). Aborting."
                              << std::endl;
                    return 1;
                default:
                    std::cout << "Unknown error from "
                              << *it << " (" << result.error << "). Aborting."
                              << std::endl;
                    return 1;
            }
        }
        std::cout << std::endl;

        ConfigurationResult result = cluster.setConfiguration(id, servers);
        std::cout << "Membership change result: ";
        if (result.status == ConfigurationResult::OK) {
            std::cout << "OK" << std::endl;
        } else if (result.status == ConfigurationResult::CHANGED) {
            std::cout << "CHANGED (" << result.error << ")" << std::endl;
        } else if (result.status == ConfigurationResult::BAD) {
            std::cout << "BAD SERVERS (" << result.error << "):" << std::endl;
            for (auto it = result.badServers.begin();
                 it != result.badServers.end();
                 ++it) {
                std::cout << "- " << it->serverId << ": " << it->addresses
                          << std::endl;
            }
        }
        std::cout << std::endl;

        std::cout << "Current configuration:" << std::endl;
        printConfiguration(cluster.getConfiguration());

        if (result.status == ConfigurationResult::OK)
            return 0;
        else
            return 1;

    }





