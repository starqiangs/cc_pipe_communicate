#include <iostream>
#include <cstring>
#include <cstdlib>

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

using namespace std;

/*
    fork()
    RETURN VALUE
       On success, the PID of the child process is returned in the parent, and 0 is returned  in
       the  child.   On  failure, -1 is returned in the parent, no child process is created, and
       errno is set appropriately.
*/

int main()
{
    pid_t pid;
    int fd1[2]; // 父进程给子进程的
    int fd2[2]; // 子进程给父进程的

    char buff[1024] = {0};
    int pipe_1 = pipe(fd1);
    int pipe_2 = pipe(fd2);
    if (pipe_1 < 0 || pipe_2 < 0)
    {
        cout << " pipe create failed!" << endl;
        exit(-1);
    }

    pid = fork();
    if (pid < 0)
    {
        cout << " fork failed!" << endl;
        exit(-2);
    }
    if (pid == 0)
    {
        // child 关闭fd1写端
        close(fd1[1]);
        int rd = read(fd1[0], buff, sizeof(buff));
        if (rd > 0)
        {
            cout << "读取：" << buff;
        }
        close(fd1[0]);
        exit(0);
    }
    else if (pid > 0)
    {
        // parent; 关闭fd1的独端
        memset(buff, 0, sizeof(buff));
        printf("Enter a string: ");
        fgets(buff, sizeof(buff), stdin);
        close(fd1[0]);
        int wr = write(fd1[1], buff, sizeof(buff));
        if (wr > 0)
        {
            std::cout << "写入：" << buff;
            waitpid(pid, nullptr, 0);
            exit(0);
        }
        close(fd1[1]);
    }
    return 0;
}