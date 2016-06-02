/* Copyright (c) 2012 Stanford University
 * Copyright (c) 2015 Diego Ongaro
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR(S) DISCLAIM ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL AUTHORS BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <cassert>
#include <getopt.h>
#include <iostream>
#include <iterator>
#include <sstream>

#include <LogCabin/Client.h>
#include <LogCabin/Debug.h>
#include <LogCabin/Util.h>
#include "Ops.h"

namespace {

using LogCabin::Client::Cluster;
using LogCabin::Client::Tree;
using LogCabin::Client::Util::parseNonNegativeDuration;



} // anonymous namespace

enum class Command {
    MKDIR,
    LIST,
    DUMP,
    DUMP_LOCAL,
    RMDIR,
    WRITE,
    READ,
    READ_LOCAL,
    REMOVE,
    MAKE_LEADER,
    GET_LEADER,
    RECONFIGURE,
};

std::string
readStdin()
{
    std::cin >> std::noskipws;
    std::istream_iterator<char> it(std::cin);
    std::istream_iterator<char> end;
    std::string results(it, end); 
    return results;
}

int
main(int argc, char** argv)
{
    Command command;
    std::string path;
    std::vector<std::string> serversInConfiguration;
    std::string node = argv[1];
    for(int i = 0; i < argc; i++){
        std::string cmdStr = argv[i];
        if ("mkdir" == cmdStr){
            command = Command::MKDIR;
            path = argv[i+1];
            break;
        }
        else if ("list" == cmdStr){
            command = Command::LIST;
            path = argv[i+1];
            break;
        }
        else if ("dump" == cmdStr){
            command = Command::DUMP;
            if ((i+1) < argc)
                path = argv[i+1];
            else
                path = "/";
            break;
        }
        else if ("dump-local" == cmdStr || "dumplocal" == cmdStr){
            command = Command::DUMP_LOCAL;
            if ((i+1) < argc)
                path = argv[i+1];
            else
                path = "/";
            break;
        }
        else if ("rmdir" == cmdStr){
            command = Command::RMDIR;
            path = argv[i+1];
            break;
        }
        else if ("write" == cmdStr){
            command = Command::WRITE;
            path = argv[i+1];
            break;
        }
        else if ("read" == cmdStr){
            command = Command::READ;
            path = argv[i+1];
            break;
        }
        else if ("read-local" == cmdStr || "readlocal" == cmdStr){
            command = Command::READ_LOCAL;
            path = argv[i+1];
            break;
        }
        else if ("remove" == cmdStr){
            command = Command::REMOVE;
            path = argv[i+1];
            break;
        }
        else if ("make-leader" == cmdStr || "makeleader" == cmdStr){
            command = Command::MAKE_LEADER;
            break;
        }
        else if ("get-leader" == cmdStr || "getleader" == cmdStr){
            command = Command::GET_LEADER;
            break;
        }
        else if ("reconfigure" == cmdStr || "configure" == cmdStr){
            command = Command::RECONFIGURE;

            int servers = argc - (i++ + 1);
            serversInConfiguration = std::vector<std::string>(servers);
            for (int j = 0; j < servers; j++, i++){
                serversInConfiguration[j] = argv[i];
            }
            break;
        }
    }
    if(0){
        std::cout << "No command" << std::endl;
    }

    //std::cout << "Ops test " << std::endl;
    try {
        //Ops opsVar = Ops("10.0.2.1:5254", 0, "");
        Ops opsVar = Ops(node, 0, "");
        std::string result;
        switch (command){
            case Command::MKDIR:
                /*std::cout << "mkdir:"+path << std::endl;*/
                opsVar.mkdir(path);
                break;

            case Command::LIST:
                /*std::cout << "list:"+path << std::endl;*/
                result = opsVar.listKeys(path);
                std::cout << result << std::endl;
                break;
            
            case Command::DUMP:
                /*std::cout << "dump:"+path << std::endl;*/
                result = opsVar.dump(path);
                std::cout << result << std::endl;
                break;
            
            case Command::DUMP_LOCAL:
                /*std::cout << "dumplocal:"+path << std::endl;*/
                result = opsVar.dumpLocal(path);
                std::cout << result << std::endl;
                break;
            
            case Command::RMDIR:
                /*std::cout << "rmdir:"+path << std::endl;*/
                opsVar.rmdir(path);
                break;
            
            case Command::WRITE:
                /*std::cout << "write:"+path + "-" + readStdin() << std::endl;*/
                opsVar.write(path, readStdin());
                break;
            
            // TODO!!! NOT WORKING!!!!
            case Command::READ:
                /*std::cout << "read:"+path << std::endl;*/
                result = opsVar.read(path);
                //std::cout << result << std::endl;
                break;
            
            case Command::READ_LOCAL:
                /*std::cout << "readlocal:"+path << std::endl;*/
                result = opsVar.readLocal(path);
                std::cout << result << std::endl;
                break;
            
            case Command::REMOVE:
                /*std::cout << "remove:"+path << std::endl;*/
                opsVar.remove(path);
                break;
            
            case Command::MAKE_LEADER:
                /*std::cout << "makeleader:"<< std::endl;*/
                opsVar.makeLeader();
                break;
            
            case Command::GET_LEADER:
                /*std::cout << "getleader:"<< std::endl;*/
                result = opsVar.getLeader();
                std::cout << result << std::endl;
                break;
            
            case Command::RECONFIGURE:
                /*std::cout << "reconfigure:"<< std::endl;
                int serv = (u_int) serversInConfiguration.size();
                for (int i = 0; i < serv; i++)
                    std::cout << "server:"+serversInConfiguration[i]<< std::endl;
                */
                int res = opsVar.reconfigure(serversInConfiguration);
                std::cout << res + "" << std::endl;
                break;   
        }

        return 0;

    } catch (const LogCabin::Client::Exception& e) {
        std::cout << "catch " << std::endl;

        std::cerr << "Exiting due to LogCabin::Client::Exception: "
                  << e.what()
                  << std::endl;
        exit(1);
    }
}



//std::string s;
        //std::vector<std::string> serversInConfiguration = {"10.0.2.1","10.0.2.2","10.0.2.3"};
        //if (opsVar.reconfigure(serversInConfiguration))
          //  std::cout << "FAIL" << std::endl;    
        //else 
          //  std::cout << "SUCCESS" << std::endl;    

/*
        s = opsVar.dumpLocal("");
        std::cout << s << std::endl;
        std::cout << "---------------" << std::endl;

        opsVar.mkdir("/apa/apa1");
        opsVar.mkdir("/apa/apa2");
        opsVar.write("/apa/apa1/file1", "contents of file 1111111");
        opsVar.write("/apa/apa1/file2", "contents of file 2222222");

        s = opsVar.dumpLocal("");
        std::cout << s << std::endl;
        std::cout << "---------------" << std::endl;

        opsVar.remove("/apa/apa1/file2");

        s = opsVar.dumpLocal("");
        std::cout << s << std::endl;
        std::cout << "---------------" << std::endl;

        opsVar.rmdir("/apa/apa1");

        s = opsVar.dumpLocal("");
        std::cout << s << std::endl;
        std::cout << "---------------" << std::endl;

        std::cout << "---------------" << std::endl;

        s = opsVar.read("/test/test2/file2");
        std::cout << s << std::endl;
        
        std::cout << "---------------" << std::endl;

        s = opsVar.readLocal("/test/file");
        std::cout << s << std::endl;


        std::cout << "---------------" << std::endl;

        s = opsVar.listKeys("/");
        std::cout << s << std::endl;

        //s = opsVar.getLeader();
        //std::cout << s << std::endl;

        //opsVar.makeLeader();
        
        //s = opsVar.getLeader();
        //std::cout << s << std::endl;

        //opsVar.dumpLocal("");
*/
