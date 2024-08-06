#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pwd.h>
#include <time.h>
#include <string.h> // Add this line to include the necessary header

int main() {
    FILE *fp1, *fp2;
    struct stat proc_stat;
    struct passwd *pw;
    char proc_file[100];
    char line[100];
    int pid, ppid, nice_val, tty_nr;
    char tty_name[100];
    uid_t ruid, euid;
    time_t start_time, cpu_time;

    // Open files for writing
    fp1 = fopen("procatto.txt", "w");
    fp2 = fopen("procatt1.txt", "w");

    // Iterate through all processes
    for (pid = 1; pid <= getpid(); pid++) {
        sprintf(proc_file, "/proc/%d/stat", pid);

        // Open the stat file for the current process
        if (stat(proc_file, &proc_stat) == 0) {
            // Read process attributes from the stat file
            FILE *fp = fopen(proc_file, "r");
            if (fp != NULL) {
                fscanf(fp, "%d %*s %*c %d %*d %*d %d %*d %*u %*u %*u %*u %*u %ld %ld",
                       &pid, &ppid, &nice_val, &start_time, &cpu_time);
                fclose(fp);

                // Get the terminal name
                sprintf(proc_file, "/proc/%d/fd/0", pid);
                if (stat(proc_file, &proc_stat) == 0) {
                    readlink(proc_file, tty_name, sizeof(tty_name) - 1);
                    tty_nr = proc_stat.st_rdev;
                } else {
                    strcpy(tty_name, "?"); // Use strcpy() to assign the string
                    tty_nr = -1;
                }

                // Get the real and effective user IDs
                ruid = proc_stat.st_uid;
                euid = geteuid();
                pw = getpwuid(ruid);

                // Print process attributes
                fprintf(fp1, "PID: %d\nPPID: %d\nNice: %d\nTTY: %s\nRUID: %s\nEUID: %s\nStart Time: %ld\nCPU Time: %ld\n\n",
                        pid, ppid, nice_val, tty_name, pw->pw_name, pw->pw_name, start_time, cpu_time);
                fprintf(fp2, "PID: %d\nPPID: %d\nNice: %d\nTTY: %s\nRUID: %s\nEUID: %s\nStart Time: %ld\nCPU Time: %ld\n\n",
                        pid, ppid, nice_val, tty_name, pw->pw_name, pw->pw_name, start_time, cpu_time);
            }
        }
    }

    // Close the files
    fclose(fp1);
    fclose(fp2);

    return 0;
}
