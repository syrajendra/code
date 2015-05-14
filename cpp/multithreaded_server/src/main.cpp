#include "common.hpp"
#include <sys/resource.h>
#include <stdlib.h>
#include <pwd.h>
#include <string>

using namespace std;

static int  			g_sd;
static circular_queue_t g_queue;

int open_file_descriptor_limit(unsigned int fd_limit)
{
	struct rlimit rlim;
	if(-1 == getrlimit(RLIMIT_NOFILE, &rlim)) {
		printf("ERROR: Failed to run getrlimit (%s)\n", strerror(errno));
		exit(1);
	}
	printf("Default file descriptor limit soft: %d hard: %d\n", (int)rlim.rlim_cur, (int)rlim.rlim_max);
	if(fd_limit > 0) {
		if(rlim.rlim_cur < fd_limit || rlim.rlim_max < fd_limit) {
			rlim.rlim_cur = rlim.rlim_max = fd_limit;
			if (-1 == setrlimit(RLIMIT_NOFILE, &rlim)) {
				printf("WARNING: Failed to run setrlimit (%s) for %d, Try as sudo\n", strerror(errno), fd_limit);
				return -1;
			}
			printf("Increased file descriptor limit to soft & hard : %d\n", fd_limit);
			return 0;
		} else {
			printf("Default file descriptor limit is greater than %d\n", fd_limit);
			return 0;
		}
	}
	return 0;
}

void cleanup()
{
	TRACE("Cleaning up");
	fclose(g_stream);
	close(g_sd);
	cleanup_circular_queue(&g_queue);
}

void show_credentials(const char *user)
{
	uid_t real_user_id 			= 0;
	uid_t effective_user_id 	= 0;
	uid_t saved_user_id 		= 0;
	uid_t g_real_user_id 		= 0;
	uid_t g_effective_user_id 	= 0;
	uid_t g_saved_user_id 		= 0;
	getresuid(&real_user_id, &effective_user_id, &saved_user_id);
	getresgid(&g_real_user_id, &g_effective_user_id, &g_saved_user_id);
	printf("========================= %s ========================\n", user);
	printf("Real user ID 		: %d\n", (int)real_user_id);
	printf("Effective user ID 	: %d\n", (int)effective_user_id);
	printf("Saved user ID 		: %d\n", (int)saved_user_id);
	printf("Real group ID 		: %d\n", (int)g_real_user_id);
	printf("Effective group ID 	: %d\n", (int)g_effective_user_id);
	printf("Saved group ID 		: %d\n", (int)g_saved_user_id);
	printf("======================================================\n");
}

void actions_as_root()
{
	string cur_user = string(getenv("USER"));
	printf("Execution credentials: \n");
	show_credentials(cur_user.c_str());
	open_file_descriptor_limit(MAX_QUEUE_LENGTH);
	if (getuid() == 0) { // if root
		string login_user = string(getenv("SUDO_USER"));
		printf("Login user: %s\n", login_user.c_str());
		if (cur_user != login_user) {
			long buflen = 0;
			if(-1 != (buflen=sysconf(_SC_GETPW_R_SIZE_MAX))) {
				char buf[buflen];
				struct passwd pwbuf, *pwbufp;
				if ((0 == getpwnam_r(login_user.c_str(), &pwbuf, buf, buflen, &pwbufp)) && pwbufp) {
					if (getuid() == 0) {
						if(-1 == setresgid(pwbufp->pw_gid, pwbufp->pw_gid, pwbufp->pw_gid)) {
							printf("ERROR: Failed to set effective user id to real user id [%s]\n", strerror(errno));
						}
						if(-1 == setresuid(pwbufp->pw_uid, pwbufp->pw_uid, pwbufp->pw_uid)) {
							printf("ERROR: Failed to set effective user id to real user id [%s]\n", strerror(errno));
						}
						printf("Execution credentials changed:\n");
						show_credentials(login_user.c_str());
					} else {
						printf("WARNING: Failed to change user to '%s'\n", login_user.c_str());
					}
				} else {
					printf("ERROR: Failed to get password database\n");
				}
			}
		}
	}
}

int main(int argc, char *argv[])
{
	actions_as_root();
	create_log_file();
	TRACE("Starting server program");
	g_sd = create_server();
	init_circular_queue(&g_queue, MAX_QUEUE_LENGTH);
	create_threads(g_sd, &g_queue);
	atexit(cleanup);
	return 0;
}
