#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    int pipe_foo_to_bar[2]; // bar reading, foo writing
    int pipe_bar_to_foo[2]; // bar writing, foo reading

    // Create the pipes
    if (pipe(pipe_foo_to_bar) == -1) {
        perror("pipe");
        return 1;
    }

    if (pipe(pipe_bar_to_foo) == -1) {
        perror("pipe");
        return 1;
    }

    pid_t pid_foo = fork();
    if (pid_foo == -1) {
        perror("fork");
        return 1;
    }
    
    if (pid_foo == 0) {
        close(pipe_foo_to_bar[0]);
        close(pipe_bar_to_foo[1]);

        // Prepare arguments
        char read_fd[10], write_fd[10];
        snprintf(read_fd, sizeof(read_fd), "%d", pipe_bar_to_foo[0]);
        snprintf(write_fd, sizeof(write_fd), "%d", pipe_foo_to_bar[1]);

        // Execute foo
        if (execlp("./foo", "./foo", read_fd, write_fd, (char *)NULL) == -1) {
            perror("excelp");
            return 1;
        };
    }

    pid_t pid_bar = fork();
    if (pid_bar == -1) {
        perror("fork");
        return 1;
    }

    if (pid_bar == 0) {
        close(pipe_foo_to_bar[1]);
        close(pipe_bar_to_foo[0]);

        // Prepare arguments
        char read_fd[10], write_fd[10];
        snprintf(write_fd, sizeof(write_fd), "%d", pipe_bar_to_foo[1]);
        snprintf(read_fd, sizeof(read_fd), "%d", pipe_foo_to_bar[0]);

        // Execute bar
        if (execlp("./bar", "./bar", read_fd, write_fd, (char *)NULL) == -1) {
            perror("excelp");
            return 1; 
        }
    }

    close(pipe_foo_to_bar[0]);
    close(pipe_foo_to_bar[1]);
    close(pipe_bar_to_foo[0]);
    close(pipe_bar_to_foo[1]);

    // Wait for both children to terminate
    int status_foo;
    waitpid(pid_foo, &status_foo, 0);
    int status_bar;
    waitpid(pid_bar, &status_bar, 0);

    return 0;
}