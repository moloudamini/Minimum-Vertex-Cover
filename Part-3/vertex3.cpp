#include<vector>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include <iostream>
using namespace std;



// we have considered 3 process A, B, and C for this program
int a3_loop_fun(void) {
    // Process C reading from both A and B
    while (!cin.eof()) {
        string line;
        getline(std::cin, line);
        if (line.size () > 0)
            cout << line << std::endl;
    }
    return 0;
}

int main (int argc, char **argv) {
    string s_val;
    int s_int = 10; 
    string n_val; 
    int n_int = 5; 
    string l_val;
    int l_int = 5;
    string c_val;
    int c_int = 20; 
    int cmd;

    opterr = 0;
    while ((cmd = getopt (argc, argv, "s:n:l:c:?")) != -1)
        switch (cmd)
        {
        case 's':
            s_val = optarg;
            s_int = atoi(s_val.c_str());
            if(s_int < 2) {
                cerr << "Error: the value of argument -s should be greater than 2" << endl;
                return 1;
            }
            break;

        case 'n':
            n_val = optarg;
            n_int = atoi(n_val.c_str());
            if(n_int < 1) {
                cerr << "Error: the value of argument -n should be greater than 1" << endl;
                return 1;
            }
            break;

        case 'l':
            l_val = optarg;
            l_int = atoi(l_val.c_str());
            if(l_int < 5) {
                cerr << "Error: the value of argument -l should be greater than 5" << endl;
                return 1;
            }
            break;

        case 'c':
            c_val = optarg;
            c_int = atoi(c_val.c_str());
            if(c_int < 1) {
                cerr << "Error: the value of argument -c should be greater than 1" << endl;
                return 1;
            }
            break;
        
        case '?':
            cerr << "Error: unknown input argument: " << optopt << endl;
            return 1;
        default:
            return 0;
        }

    std::vector<pid_t> kids;
    pid_t child_pid;    
    // create pipes
    int rgen_to_a1[2];
    pipe(rgen_to_a1);

    int a1_to_a2[2];
    pipe(a1_to_a2);
    
    
    // rgen will be run
    child_pid = fork();
    if (child_pid == 0) {
        // redirect stdout to the pipe
        dup2(rgen_to_a1[1], STDOUT_FILENO);
        close(rgen_to_a1[0]);
        close(rgen_to_a1[1]);
        execv("rgen", argv);
        perror ("Error: rgen can not be executed");
        return 1;
    } 
    else if (child_pid < 0) {
        cerr << "Error: rgen can not be forked" << endl;
        return 1;  
    }
    kids.push_back(child_pid);
    
    // a1ece650.py will be run  
    child_pid = fork();
    if (child_pid == 0) {
        // redirect stdin from the pipe
        dup2(rgen_to_a1[0], STDIN_FILENO);
        close(rgen_to_a1[1]);
        close(rgen_to_a1[0]);
        // redirect stdout from the pipe
        dup2(a1_to_a2[1], STDOUT_FILENO);
        close(a1_to_a2[0]);
        close(a1_to_a2[1]);
        execv("ece650-a1.py", argv);
        perror ("Error: ece650-a1.py can not be executed");
        return 1;
    } 
    else if (child_pid < 0) {
        cerr << "Error: ece650-a1.py can not be forked" << endl;
        return 1;  
    }
    kids.push_back(child_pid);
 
    // ece650-a2 will be run 
    child_pid = fork();
    if (child_pid == 0) {
        // redirect stdin from the pipe
        dup2(a1_to_a2[0], STDIN_FILENO);
        close(a1_to_a2[1]);
        close(a1_to_a2[0]);
        execv("ece650-a2", argv);
        perror ("Error: ece650-a2 can not be executed");
        return 1;
    } 
    else if (child_pid < 0) {
        cerr << "Error: ece650-a2 can not be forked" << endl;
        return 1;  
    }
    kids.push_back(child_pid);
   
    // ece650-a3 will be run 
    child_pid = fork();
    if (child_pid == 0) {
        // redirect stdout to the pipe
        dup2(a1_to_a2[1], STDOUT_FILENO);
        close(a1_to_a2[0]);
        close(a1_to_a2[1]);
        
        return a3_loop_fun();
    } 
    else if (child_pid < 0) {
        cerr << "Error: ece650-a3 can not be forked" << endl;
        return 1;  
    }
    kids.push_back(child_pid);

    int m;
    wait(&m);

    // send kill signal to all children
    for (pid_t k : kids) {
        int status;
        kill (k, SIGTERM);
        waitpid(k, &status, 0);
    }
    return 0;
}
