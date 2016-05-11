/* 
 */

#include <LogCabin/Client.h>

namespace {


std::string
readStdin()
{
    std::cin >> std::noskipws;
    std::istream_iterator<char> it(std::cin);
    std::istream_iterator<char> end;
    std::string results(it, end);
    return results;
}

} // anonymous namespace

int
main(int argc, char** argv)
{
    try {
        OptionParser options(argc, argv);

        LogCabin::Client::Debug::setLogPolicy(
            LogCabin::Client::Debug::logPolicyFromString(
                options.logPolicy));

        Cluster cluster(options.cluster);
        Tree tree = cluster.getTree();

        if (options.timeout > 0) {
            tree.setTimeout(options.timeout);
        }

        if (!options.dir.empty()) {
            tree.setWorkingDirectoryEx(options.dir);
        }

        if (!options.condition.first.empty()) {
            tree.setConditionEx(options.condition.first,
                                options.condition.second);
        }

        std::string& path = options.path;
        switch (options.command) {
            case Command::MKDIR:
                tree.makeDirectoryEx(path);
                break;
            case Command::LIST: {
                std::vector<std::string> keys = tree.listDirectoryEx(path);
                for (auto it = keys.begin(); it != keys.end(); ++it)
                    std::cout << *it << std::endl;
                break;
            }
            case Command::DUMP: {
                if (path.empty() || path.at(path.size() - 1) != '/')
                    path.append("/");
                dumpTree(tree, path);
                break;
            }
            case Command::RMDIR:
                tree.removeDirectoryEx(path);
                break;
            case Command::WRITE:
                tree.writeEx(path, readStdin());
                break;
            case Command::READ: {
                std::string contents = tree.readEx(path);
                std::cout << contents;
                if (contents.empty() ||
                    contents.at(contents.size() - 1) != '\n') {
                    std::cout << std::endl;
                } else {
                    std::cout.flush();
                }
                break;
            }
            case Command::REMOVE:
                tree.removeFileEx(path);
                break;
        }
        return 0;

    } catch (const LogCabin::Client::Exception& e) {
        std::cerr << "Exiting due to LogCabin::Client::Exception: "
                  << e.what()
                  << std::endl;
        exit(1);
    }
}
