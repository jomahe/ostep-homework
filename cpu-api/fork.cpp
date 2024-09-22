#include <unistd.h>
#include <iostream>
#include <fcntl.h>
#include <sys/types.h>

using namespace std;


void q1(int& x) {
   // Q:
   // Write a question that calls fork(). Before calling fork(), have the main
   // process access a variable and set its value to something. What value is
   // the varable in the child process? What happens to the variable when both
   // the child and parent change the value of the variable?

   // A (before executing):
   // depending on the ordering of the processes, the value of x should
   // change to 85 or 115 and then back to 100.

   // A (corrected):
   // Actual behavior seen when running this program is that it
   // deterministically runs the parent process first, then runs the child
   // process. However, the child process acts as though the original value of x
   // was unchanged. In the call to main, the program exits twice and prints
   // x=115 then prints x=85.

   x = 100;
   pid_t forkStatus = fork();

   if (forkStatus < 0) {  // Fork failed
      cout << "Fork failed" << endl;
      return;
   } else if (forkStatus == 0) {  // Child process
      x -= 15;
      cout << "Child PID is: " << getpid() << endl;
      cout << "New value of x: " << x << endl;
   } else {
      x += 15;
      // pid_t waitReturn = waitpid();
      // cout << "return value of wait: " << waitReturn << endl;
      cout << "Parent PID is: " << getpid() << endl;
      cout << "New value of x is: " << x << endl;
   }
}

void q2() {
   // Q:
   // Write a program that opens a file (with the open() system call) and then
   // calls fork() to create a new process. Can both the child and parent access
   // the file descriptor returned by open()? What happens when they are writing
   // to the file concurrently?

   // A (before executing):
   // Both processes should be able to access the file descriptor, but because
   // the processes open the FD pre-fork, both processes have the initial state
   // of the file in their execution environment, and so only the changes of one
   // process will propagate in the end.

   // A (corrected):
   // Changes from both the parent and child process propagate to the file. This
   // is likely because of the way that we close the file descriptor in each
   // process after writing to it, which could acquire a lock or something
   // similar. It could also be because the file descriptor is a reference to an
   // object, and any changes will propagate no matter what.

   // From ChatGPT: the changes are appended rather than overwritten because of
   // the file offset attribute of file descriptors. When a process writes to a
   // file, the file offset is advanced and the child process sees this.

   const char* fileName = "q2.txt";
   int fd = open(fileName, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);

   if (fd < 0) {
      cout << "Bad file descriptor!" << endl;
      return;
   }

   pid_t forkStatus = fork();
   char* payload;

   if (forkStatus < 0) {
      cout << "Bad fork status" << endl;
      return;
   } else if (forkStatus == 0) {
      cout << "running child proc" << endl;
      payload = "Hello from the child process!\n";
   } else {
      cout << "running parent proc" << endl;
      sleep(1);  // This line is the key to question three
      payload = "Hello from the parent process!\n";
   }
   ssize_t bytesWritten = write(fd, payload, strlen(payload));

   if (bytesWritten < 0) {
      cerr << "Nothing written to the file!" << endl;
      close(fd);
   }

   if (close(fd) < 0) {
      cerr << "error closing file" << endl;
   }
}


void q4() {
   // Q:
   // Write a program that calls fork() ad then calls some form of exec() to run
   // the program /bin/ls. See if you can try all of the variants of exec():
   // execl(), execle(), execlp(), execv(), execvp(), execpve(). Why are there
   // so many variants of the same basic call?

   pid_t forkStatus = fork();
   if (forkStatus < 0) {
      cout << "Fork unsuccessful" << endl;
   } else if (forkStatus == 0) {  // child proc
      cout << "running child proc" << endl;
      // wait(NULL);
   } else {  // Parent proc
      cout << "running parent proc" << endl;
   }
   char* args[] = { (char*)"/bin/ls", nullptr };
   execv("/bin/ls", args);
}

void q7() {
   pid_t forkStatus = fork();

   if (forkStatus < 0) {
      cout << "bad fork" << endl;
      return;
   } else if (forkStatus == 0) {
      cout << "running child proc" << endl;
      close(STDOUT_FILENO);
      cout << "attempting to print" << endl;
   } else {
      wait(nullptr);
      cout << "running parent proc" << endl;
   }
}


void q8() {
   int pipefds[2];
   char buffer[5];
   // Writing from c1 to c2;
   if (pipe(pipefds) == -1) cout << "pipe error" << endl;

   pid_t c1_pid = fork();

   if (c1_pid < 0) {
      cout << "bad fork for c1" << endl;
      return;
   } else if (c1_pid == 0) {
      cout << "running c1 proc" << endl;
      // Close read end of the pipe
      close(pipefds[0]);
      char* message = "Hello";
      write(pipefds[1], message, strlen(message));
      cout << "child 1 sent: " << message << endl;
   } else {
      pid_t c2_pid = fork();
      if (c2_pid < 0) {
         cout << "bad fork for c1" << endl;
         return;
      } else if (c2_pid == 0) {
         cout << "running c2 proc" << endl;
         close(pipefds[1]);
         read(pipefds[0], buffer, 5);

         cout << "Message from process c1: " << buffer << endl;
      } else {
         cout << "running parent proc" << endl;
      }
   }

}

int main() {
   // int x;
   // q1(x);
   // cout << "Final value of x: " << x << endl;

   // q2();

   // q4();

   // q7();

   // q8();
   return 0;
}