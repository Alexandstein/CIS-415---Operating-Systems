/*

static void handleChild(int sig, siginfo_t* siginfo)
{
	Process* p;
	int status;
	printf("Child term. detected\n");
	wait(&status);
	pid_t pid = siginfo->si_pid;
	p = (Process*)ModHash_get(table, pid);
	printf("Completed Child %d: %s\n", p->pid, p->name);
	printf("%d's execution group is %d\n", pid, p->group);
	perror(0);
		
	//Iterate through table to see if any are running
	for(child = 0; child < table->size; child++){
		if((p = ((Process*)ModHash_get(table, child))) != 0){	//Check if process exists
			if(waitpid(-1, &status, WNOHANG) == -1){				//Check if running
																//If not, alert terminal
				printf("Completed Child %d: %s\n", p->pid, p->name); 	
			}else{
				printf("Child %d: %s still running\n", p->pid, p->name); 	
			}
		}
	}
	printf("Child Termination\n");
}
*/