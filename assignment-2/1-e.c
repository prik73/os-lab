

#include <stdio.h>
#include <unistd.h>
#include <pwd.h>

int main() {
    // Get the real user ID and effective user ID
    uid_t ruid = getuid();
    uid_t euid = geteuid();

    // Get the user information for RUID
    struct passwd *ruid_info = getpwuid(ruid);
    // Get the user information for EUID
    struct passwd *euid_info = getpwuid(euid);

    // Check if user information was retrieved successfully
    if (ruid_info != NULL && euid_info != NULL) {
        printf("Real User ID (RUID): %d, Username: %s\n", ruid, ruid_info->pw_name);
        printf("Effective User ID (EUID): %d, Username: %s\n", euid, euid_info->pw_name);
    } else {
        printf("Error retrieving user information.\n");
    }

    return 0;
}
