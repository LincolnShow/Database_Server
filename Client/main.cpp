#include "client.h"
#include <iostream>
#include <fstream>
#include <cstring>

std::string DeleteSeparators(std::string where, char what);

/// @brief Main Client Module
/// @param argv[1] - Port. 12345 by default.
/// @param argv[2] - Arguments (t - testing).
/// @param argv[3] - Filename with tests.
int main(int argc, char *argv[])
{
    int port=DEF_PORT;
    bool testing = false;
    int filePathIndx = 0;

    if (argc > 1)
    {
        int tmpPort = atoi(argv[1]);
        if (tmpPort > 0)
        {
            port = tmpPort;
            std::cout << "Port set to: " << port <<std::endl;
        }
    }

    if (argc == 3)
        if (argv[1] == "t")
        {
            ///Working with test file
            testing = true;
            filePathIndx = 2;
            std::cout << "Configured for testing"<<std::endl;
        }
        else
        {
            perror("Wrong 2nd argument");
            exit(1);
        }

    if (argc == 4)
    {
        int tmpPort = atoi(argv[1]);
        if (tmpPort < 0)
        {
            perror("Wrong port argument");
            exit(1);
        }
        if (std::string(argv[2]) == std::string("t"))
        {
            ///Working with test file
            testing = true;
            std::cout << "Configured for testing"<<std::endl;
            filePathIndx = 3;
        }
        else
        {
            perror("Wrong 2nd argument");
            exit(1);
        }
    }

    Client* client = new Client(port);

    ///Testing part
    if (testing)
    {
        std::cout << "Openning file "<< argv[filePathIndx] <<std::endl;
        std::ifstream testfile (argv[filePathIndx], std::ios::in);
        if (!testfile.is_open())
        {
            perror("file can't be opened");
            exit(2);
        }
        else
        {
            std::cout << "OK" << std::endl;
            //Parsing
            char testName [BUF_SIZE];
            std::string resultExp;
            std::string result;
            std::string cmd;
            std::string inData;

            ///Reading from file
            while (!testfile.eof())
            {
                memset(testName, 0, BUF_SIZE);
                resultExp.clear();
                result.clear();
                ///Reading name of Test
                do
                {
                    testfile.getline(testName, BUF_SIZE);
                    if (testfile.eof()) return 0;
                }
                while (strlen(testName)<=0);
                std::cout << "Test name is " << testName << std::endl;

                ///Reading expected Resault
                while (1)
                {
                    testfile >> inData;
                    if (inData == "{") break;
                    resultExp += inData;
                }

                ///Deleting (' ', '\\n') from expected Resault
                resultExp=DeleteSeparators(resultExp, ' ');
                resultExp=DeleteSeparators(resultExp, '\n');

                std::cout << "Expected Result is " << resultExp << std::endl;

                ///Reading commands
                while(1)
                {
                    testfile >> cmd;
                    if (cmd == "}") break;
                    result = result + client->SendMsg(cmd);
                    cmd.clear();
                }
                result=DeleteSeparators(result, ' ');
                result=DeleteSeparators(result, '\n');
                std::cout << "Result" << result <<std::endl;
                std::cout << "Test name:" << testName;
                if (resultExp == result)
                {
                    std::cout << " passed" << std::endl;
                }
                else
                {
                    std::cout << " failed" << std::endl;
                }
                std::cout << std::endl;
            }

            testfile.close();
        }
    }


    ///User working part
    std::string msg;
    while(1)
    {
        std::cin>>msg;
        if( (msg[0]=='E' || msg[0]=='e') &&
            (msg[1]=='X' || msg[1]=='x') &&
            (msg[2]=='I' || msg[2]=='i') &&
            (msg[3]=='T' || msg[3]=='t')   )
        {
            std::cout<<"Good bye"<<std::endl;
            break;
        }
        msg = client->SendMsg(msg);
        std::cout << msg << std::endl;
    }
    delete client;
    return 0;
}

std::string DeleteSeparators(std::__cxx11::string where, char what)
{
    int pos;
    while ((pos=where.find(what)) != std::string::npos)
    {
        where.erase(pos, 1);
    }
    return where;
}
